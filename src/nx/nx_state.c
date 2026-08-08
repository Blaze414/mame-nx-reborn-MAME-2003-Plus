#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>
#include <time.h>
#include <sys/stat.h>

#include "driver.h"
#include "state.h"
#include "nx_state.h"
#include "nx_paths.h"

/* Defined in state.c as a libretro serialize helper and never declared in
   state.h. It walks the registry assigning offsets and returns the total,
   which is both the buffer size and the answer to "can this driver be saved
   at all" - it returns zero for a driver flagged GAME_DOESNT_SERIALIZE and for
   one whose registered entries point at memory that was never allocated. */
extern size_t state_get_dump_size(void);



/* Byte offsets inside the dump, from state_save_save_finish(). The game name
   is written but never checked on load - the signature check catches a state
   from a driver with a different variable layout, but two sets that share a
   driver have the same layout and the same signature, so a Street Fighter II
   state would load into a clone of it and only misbehave later. Cheaper to
   compare the name. */
enum { SS_HDR_NAME = 0x0a, SS_HDR_NAME_MAX = 10, SS_HDR_BYTES = 0x18 };

static char s_error[160];

static void fail(const char *fmt, ...)
{
	va_list ap;

	va_start(ap, fmt);
	vsnprintf(s_error, sizeof(s_error), fmt, ap);
	va_end(ap);
}

const char *nx_state_error(void)
{
	return s_error[0] ? s_error : "";
}

/* Under this build's own folder, so two copies of the app do not write into
   each other's slots - see nx_paths.h. */
static const char *state_dir(char *out, size_t len)
{
	return nx_path(out, len, "states");
}

static void slot_path(char *out, size_t len, int slot)
{
	char dir[288];

	state_dir(dir, sizeof(dir));
	snprintf(out, len, "%s/%s-%d.sta", dir, Machine->gamedrv->name, slot);
}

size_t nx_state_size(void)
{
	if (!Machine || !Machine->gamedrv) return 0;
	return state_get_dump_size();
}

int nx_state_available(void)
{
	size_t size = nx_state_size();

	/* Sets the reason on the way out. A gate that answers only yes or no
	   leaves the menu with nothing to say beyond hiding itself, and a row
	   that is missing looks exactly like a row that was never written. */
	if (!Machine || !Machine->gamedrv)
	{
		fail("No game is running.");
		return 0;
	}

	if (Machine->gamedrv->flags & GAME_DOESNT_SERIALIZE)
	{
		fail("%s is marked as unable to save its state.",
			 Machine->gamedrv->description);
		return 0;
	}

	if (size <= SS_HDR_BYTES)
	{
		/* state_get_dump_size() bails at the first registered entry whose
		   pointer is null - a driver that registers memory it never allocated.
		   Nothing here can fix that; naming it is the useful part. */
		fail("%s does not register enough of itself to be saved. "
			 "This is a limitation of the driver, not of the app.",
			 Machine->gamedrv->description);
		return 0;
	}

	s_error[0] = 0;
	return 1;
}

/*-------------------------------------------------------------------
	Slot listing
-------------------------------------------------------------------*/
static struct
{
	int  used;
	char info[48];
} s_slot[NX_STATE_SLOTS];

void nx_state_refresh(void)
{
	int i;

	for (i = 0; i < NX_STATE_SLOTS; i++)
	{
		char path[256];
		struct stat st;

		slot_path(path, sizeof(path), i);

		if (stat(path, &st) != 0 || st.st_size <= (off_t)SS_HDR_BYTES)
		{
			s_slot[i].used = 0;
			snprintf(s_slot[i].info, sizeof(s_slot[i].info), "Empty");
			continue;
		}

		{
			struct tm tmv;
			time_t when = st.st_mtime;
			char stamp[32];

			localtime_r(&when, &tmv);
			strftime(stamp, sizeof(stamp), "%d %b  %H:%M", &tmv);
			s_slot[i].used = 1;
			snprintf(s_slot[i].info, sizeof(s_slot[i].info), "%s   %lu KB",
					 stamp, (unsigned long)(st.st_size + 1023) / 1024);
		}
	}
}

int nx_state_slot_used(int slot)
{
	if (slot < 0 || slot >= NX_STATE_SLOTS) return 0;
	return s_slot[slot].used;
}

const char *nx_state_slot_info(int slot)
{
	if (slot < 0 || slot >= NX_STATE_SLOTS) return "";
	return s_slot[slot].info[0] ? s_slot[slot].info : "Empty";
}

/*-------------------------------------------------------------------
	Serialize

	Tag 0 is everything that is not a CPU; tag n+1 is CPU n. Each CPU's
	registrations are read through its own context, and its banking has to be
	reset first or the pointers the entries were registered against belong to
	whichever bank the previous context left selected.
-------------------------------------------------------------------*/
static int walk_tags(int loading)
{
	int cpunum;

	state_save_set_current_tag(0);
	if (loading ? state_save_load_continue() : state_save_save_continue())
		return -1;

	for (cpunum = 0; cpunum < cpu_gettotalcpu(); cpunum++)
	{
		int bad;

		cpuintrf_push_context(cpunum);
		activecpu_reset_banking();

		state_save_set_current_tag(cpunum + 1);
		bad = loading ? state_save_load_continue() : state_save_save_continue();

		cpuintrf_pop_context();

		/* Popped first: leaving a context pushed because a tag failed would
		   leave the machine running as the wrong CPU. */
		if (bad) return -1;
	}
	return 0;
}

int nx_state_save(int slot)
{
	size_t size;
	unsigned char *buf;
	char path[256];
	FILE *f;

	s_error[0] = 0;

	if (slot < 0 || slot >= NX_STATE_SLOTS)
	{
		fail("Bad slot number.");
		return -1;
	}

	size = state_get_dump_size();
	if (size <= SS_HDR_BYTES)
	{
		fail("%s cannot be saved: the driver does not support save states.",
			 Machine->gamedrv->description);
		return -1;
	}

	/* One allocation the size of the whole machine state, on top of the ROM
	   regions already resident. In applet mode that is the difference between
	   a save and a crash, so it is checked rather than assumed. */
	buf = (unsigned char *)malloc(size);
	if (!buf)
	{
		fail("Not enough memory for a %lu KB save state.",
			 (unsigned long)(size + 1023) / 1024);
		return -1;
	}

	state_save_save_begin(buf);
	if (walk_tags(0) != 0)
	{
		/* state.c has already cleared its own pointers by this point. */
		free(buf);
		fail("This game's state could not be collected.");
		return -1;
	}
	state_save_save_finish();

	{
		char dir[288];
		mkdir(nx_base_dir(), 0777);
		mkdir(state_dir(dir, sizeof(dir)), 0777);
	}
	slot_path(path, sizeof(path), slot);

	f = fopen(path, "wb");
	if (!f)
	{
		free(buf);
		fail("Could not write to %s.", path);
		return -1;
	}

	if (fwrite(buf, 1, size, f) != size)
	{
		/* A short write leaves a file that looks like a state and is not one,
		   and it would sit in the slot list claiming to be loadable. */
		fclose(f);
		remove(path);
		free(buf);
		fail("Ran out of room on the SD card.");
		return -1;
	}

	fclose(f);
	free(buf);
	nx_state_refresh();
	return 0;
}

int nx_state_load(int slot)
{
	size_t expect, size;
	unsigned char *buf;
	char path[256];
	FILE *f;
	long len;

	s_error[0] = 0;

	if (slot < 0 || slot >= NX_STATE_SLOTS)
	{
		fail("Bad slot number.");
		return -1;
	}

	expect = state_get_dump_size();
	if (expect <= SS_HDR_BYTES)
	{
		fail("%s cannot be loaded: the driver does not support save states.",
			 Machine->gamedrv->description);
		return -1;
	}

	slot_path(path, sizeof(path), slot);
	f = fopen(path, "rb");
	if (!f)
	{
		fail("Slot %d is empty.", slot + 1);
		return -1;
	}

	fseek(f, 0, SEEK_END);
	len = ftell(f);
	fseek(f, 0, SEEK_SET);

	if (len <= (long)SS_HDR_BYTES)
	{
		fclose(f);
		fail("Slot %d holds a damaged save.", slot + 1);
		return -1;
	}

	size = (size_t)len;
	buf = (unsigned char *)malloc(size);
	if (!buf)
	{
		fclose(f);
		fail("Not enough memory to read a %lu KB save state.",
			 (unsigned long)(size + 1023) / 1024);
		return -1;
	}

	if (fread(buf, 1, size, f) != size)
	{
		fclose(f);
		free(buf);
		fail("Slot %d could not be read.", slot + 1);
		return -1;
	}
	fclose(f);

	/* Checked before state_save_load_begin(), which reports its own failures
	   through usrintf_showmessage() - a renderer this frontend never runs, so
	   its diagnosis would be silent and the player would see a state simply
	   not load. */
	if (memcmp(buf, "MAMESAVE", 8) != 0)
	{
		free(buf);
		fail("Slot %d does not hold a save state.", slot + 1);
		return -1;
	}

	if (strncmp((const char *)buf + SS_HDR_NAME, Machine->gamedrv->name,
				SS_HDR_NAME_MAX) != 0)
	{
		char other[SS_HDR_NAME_MAX + 1];

		memcpy(other, buf + SS_HDR_NAME, SS_HDR_NAME_MAX);
		other[SS_HDR_NAME_MAX] = 0;
		free(buf);
		fail("Slot %d was saved from %s, not %s.", slot + 1,
			 other, Machine->gamedrv->name);
		return -1;
	}

	if (size != expect)
	{
		free(buf);
		fail("Slot %d was saved by a different build and cannot be loaded.",
			 slot + 1);
		return -1;
	}

	if (state_save_load_begin(buf, size) != 0)
	{
		free(buf);
		fail("Slot %d does not match this version of the driver.", slot + 1);
		return -1;
	}

	if (walk_tags(1) != 0)
	{
		free(buf);
		/* Half a state is in the machine and there is no putting the old one
		   back - state.c overwrites in place and keeps no copy. Say so plainly
		   rather than let the game misbehave unexplained. */
		fail("Slot %d failed while loading. Reset the game.", slot + 1);
		return -1;
	}

	state_save_load_finish();
	free(buf);
	return 0;
}
