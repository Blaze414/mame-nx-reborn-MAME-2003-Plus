#ifndef OSDEPEND_H
#define OSDEPEND_H

#include "osd_cpu.h"
#include "inptport.h"

#ifdef __cplusplus
extern "C" {
#endif

/* The Win32 port requires this constant for variable arg routines. */
#ifndef CLIB_DECL
#define CLIB_DECL
#endif

#ifdef __LP64__
#define FPTR unsigned long   /* 64bit: sizeof(void *) is sizeof(long)  */
#else
#define FPTR unsigned int
#endif


int osd_init(void);
void osd_exit(void);


/******************************************************************************

	Display / Sound / Keyboard / Joystick / Timing / Utility

	Superseded by src/mame2003.h - MAME2003-Plus's own vendored core now owns
	the whole osd_create_display/osd_update_video_and_audio/osd_*_audio_stream/
	osd_get_key_list/osd_get_joy_list/osd_customize_inputport_defaults/osd_cycles
	contract there (struct osd_create_params included). Keeping a second,
	independent copy here would conflict wherever a file includes both headers
	(duplicate struct body, and the X_AXIS/Y_AXIS/Z_AXIS/PEDAL_AXIS macros this
	file used to define here now clash with mame2003.h's enum of the same names).

******************************************************************************/

/* inp header */
typedef struct
{
	char name[9];      /* 8 bytes for game->name + NUL */
	char version[3];   /* byte[0] = 0, byte[1] = version byte[2] = beta_version */
	char reserved[20]; /* for future use, possible store game options? */
} INP_HEADER;



/******************************************************************************

	Miscellaneous

	osd_pause/osd_display_loading_rom_message are no longer called by core
	(that moved to mame2003.h's mame_pause / the RenderProgress hook), but
	nx_misc.c still implements them as the NX-side pause plumbing (see
	mame_pause() in nx_misc.c), so the declarations stay.

******************************************************************************/

/* forward-declared here (not just implicitly by use below) so this tag
   is file-scope, not prototype-scope - otherwise it's a distinct type
   from mame2003.h's own "struct rom_load_data;" forward decl, and
   nx_misc.c's definition (which sees mame2003.h's) conflicts with this
   prototype even though they look textually identical. */
struct rom_load_data;
int osd_display_loading_rom_message(const char *name,struct rom_load_data *romdata);
void osd_pause(int paused);



#if defined MAME_NET || defined XMAME_NET
/* network */
int osd_net_init(void);
#ifdef XMAME_NET
int osd_net_active(void);
#endif
int osd_net_send(int player, unsigned char buf[], int *size);
int osd_net_recv(int player, unsigned char buf[], int *size);
#ifdef MAME_NET
int osd_net_sync(void);
#elif defined XMAME_NET
void osd_net_sync(unsigned short input_port_values[MAX_INPUT_PORTS],
		unsigned short input_port_defaults[MAX_INPUT_PORTS]);
#endif
int osd_net_input_sync(void);
int osd_net_exit(void);
int osd_net_add_player(void);
int osd_net_remove_player(int player);
int osd_net_game_init(void);
int osd_net_game_exit(void);
#endif /* MAME_NET */

#ifdef MESS
/* this is here to follow the current mame file hierarchy style */
#include "osd_mess.h"
#endif

#ifdef __GNUC__
void CLIB_DECL logerror(const char *text,...)
      __attribute__ ((format (printf, 1, 2)));
#else
void CLIB_DECL logerror(const char *text,...);
#endif

#ifdef __cplusplus
}
#endif

#endif
