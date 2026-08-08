#ifndef MAME2003_H
#define MAME2003_H

#include <stdio.h>
#include "osd_cpu.h"
#include "inptport.h"

#ifdef __cplusplus
extern "C" {
#endif

/* The Win32 port requires this constant for variable arg routines. */
#ifndef CLIB_DECL
#define CLIB_DECL
#endif

#define FPTR uintptr_t


/******************************************************************************

	Parameters (real core limits - referenced by struct RunningMachine etc,
	not RA-specific despite living in the old libretro glue header)

******************************************************************************/

#define APPNAME             "mame-nx"
#define MAX_GFX_ELEMENTS    32
#define MAX_MEMORY_REGIONS  32
#define MAX_PLAYER_COUNT    8

#define PATH_MAX_LENGTH     4096
#define PATH_DEFAULT_SLASH_C() '/'


/******************************************************************************

	Analog axes

******************************************************************************/

enum
{
  X_AXIS = 0,
  Y_AXIS,
  Z_AXIS,
  PEDAL_AXIS,
  MAX_ANALOG_AXES
};


/******************************************************************************

	Display

******************************************************************************/

/* mame_bitmap used to be declared here, but has moved to common.c */
/* sadly, the include order requires that at least this forward declaration is here */
struct mame_bitmap;
struct mame_display;
struct performance_info;
struct rectangle;
struct rom_load_data;


/* these are the parameters passed into osd_create_display */
struct osd_create_params
{
	int width, height;			/* width and height */
	int aspect_x, aspect_y;		/* aspect ratio X:Y */
	int depth;					/* depth, either 16(palette), 15(RGB) or 32(RGB) */
	int colors;					/* colors in the palette (including UI) */
	float fps;					/* frame rate */
	int video_attributes;		/* video flags from driver */
	int orientation;			/* orientation requested by the user */
};

int osd_create_display(const struct osd_create_params *params, UINT32 *rgb_components);
void osd_close_display(void);

/* mirrors the params passed to osd_create_display - a few drivers (e.g.
   segas32.c) read this directly instead of going through the OSD layer. */
extern struct osd_create_params video_config;

/* vector-graphics drivers (avgdvg.c, namcos22.c, sega.c, mystwarr.c, ...)
   read this bare global instead of options.translucency directly. */
extern int translucency;

int osd_skip_this_frame(void);
void osd_update_video_and_audio(struct mame_display *display);

/* Pause or resume mame, true->pause, false->resume. */
extern void mame_pause(bool pause);

/* Set by the pause menu's "Exit Game" row, cleared by the main loop once it has
   torn the machine down. A flag rather than a direct call because the menu runs
   inside a frame and mame_done() may only run between frames. */
extern int nx_exit_to_menu;

/* on-screen warning/status toast (missing ROMs/samples, XML dat progress, ...) */
extern void frontend_message_cb(const char *message_string, unsigned frames_to_display);


/******************************************************************************

	Sound

******************************************************************************/

int osd_start_audio_stream(int stereo);
int osd_update_audio_stream(INT16 *buffer);
void osd_update_silent_stream(void);
void osd_stop_audio_stream(void);

void osd_set_mastervolume(int attenuation);
int osd_get_mastervolume(void);
void osd_sound_enable(int enable);


/******************************************************************************

	Keyboard

******************************************************************************/

const struct KeyboardInfo *osd_get_key_list(void);
int osd_is_key_pressed(int keycode);
int osd_readkey_unicode(int flush);


/******************************************************************************

	Joystick

******************************************************************************/

const struct JoystickInfo *osd_get_joy_list(void);
int osd_is_joy_pressed(int joycode);
int osd_is_joystick_axis_code(int joycode);

void osd_analogjoy_read(int player, int analog_axis[MAX_ANALOG_AXES], InputCode analogjoy_input[MAX_ANALOG_AXES]);

/* Legacy joystick calibration - not driven as long as needs_calibration returns 0. */
int osd_joystick_needs_calibration(void);
void osd_joystick_start_calibration(void);
const char *osd_joystick_calibrate_next(void);
void osd_joystick_calibrate(void);
void osd_joystick_end_calibration(void);


/******************************************************************************

	Trackball, Spinner, Mouse, Pointer, Lightgun

******************************************************************************/

/* Returns relative or absolute positions for various X-Y coordinate devices */
void osd_xy_device_read(int player, int *deltax, int *deltay, const char* type);


/******************************************************************************

	Utility functions

******************************************************************************/

void osd_customize_inputport_defaults(struct ipd *defaults);


/******************************************************************************

	Timing

******************************************************************************/

typedef INT64 cycles_t;

cycles_t osd_cycles(void);
cycles_t osd_cycles_per_second(void);
cycles_t osd_profiling_ticks(void);


/******************************************************************************

	Logging - real implementation in nx_misc.c. Declared here (in the header
	practically every core/driver/sound file reaches via driver.h/mame.h),
	not just in osdepend.h, since logerror() is called all over the core
	and most of those files never include osdepend.h directly.

******************************************************************************/

#ifdef __GNUC__
void CLIB_DECL logerror(const char *text,...) __attribute__ ((format (printf, 1, 2)));
#else
void CLIB_DECL logerror(const char *text,...);
#endif

#ifdef __cplusplus
}
#endif

#endif /* MAME2003_H */
