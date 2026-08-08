#include <stdio.h>
#include <string.h>
#include <sys/stat.h>

#include "nx_paths.h"

/* Where this build keeps everything that is not the executable. Resolved once,
   from the directory the .nro was launched out of.

   It used to be the fixed string "sdmc:/switch/mame-nx", which meant two builds
   of this app - a stable one and one being tested - shared a settings file, a
   library index, an artwork cache and a save-state folder, however far apart
   their .nro files were kept. The only way to try the other one was to swap
   .nro files in place, and then the two disagreed about what the settings file
   meant.

   The ROM list has always resolved "roms/" relative to the working directory,
   which hbmenu sets to the folder holding the .nro - so ROMs were already
   per-folder while everything else was not. This just applies the same rule to
   the rest, which makes a build a folder: copy the whole thing, rename it, and
   the copy has its own settings, its own library, its own states, and shares
   nothing. */

static char s_base[256];

static const char *kLegacyBase = "sdmc:/switch/mame-nx";

void nx_paths_init(const char *argv0)
{
	const char *cut;

	s_base[0] = 0;

	/* argv[0] is the full path to the .nro when hbmenu launched us. Under a
	   forwarder or a title takeover there may be no argv at all, which is why
	   the fallback below exists rather than an assert. */
	if (argv0 && *argv0)
	{
		cut = strrchr(argv0, '/');
		if (cut && cut != argv0)
		{
			size_t len = (size_t)(cut - argv0);

			if (len < sizeof(s_base))
			{
				memcpy(s_base, argv0, len);
				s_base[len] = 0;
			}
		}
	}

	/* Has to name a mount, or every path built from it lands somewhere the
	   filesystem will not follow. */
	if (!s_base[0] || !strchr(s_base, ':'))
		snprintf(s_base, sizeof(s_base), "%s", kLegacyBase);

	mkdir(s_base, 0777);
}

const char *nx_base_dir(void)
{
	if (!s_base[0])
		snprintf(s_base, sizeof(s_base), "%s", kLegacyBase);
	return s_base;
}

const char *nx_path(char *out, size_t len, const char *leaf)
{
	if (!out || len == 0) return "";

	if (leaf && *leaf) snprintf(out, len, "%s/%s", nx_base_dir(), leaf);
	else               snprintf(out, len, "%s", nx_base_dir());

	return out;
}

const char *nx_path_mkdir(char *out, size_t len, const char *leaf)
{
	nx_path(out, len, leaf);
	mkdir(out, 0777);
	return out;
}
