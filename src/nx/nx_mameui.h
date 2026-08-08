#pragma once

#ifdef __cplusplus
extern "C" {
#endif

/* Read/write bridges onto the data behind MAME's own pause-menu pages, so the
   frontend overlay can render them in its own visual language instead of
   handing the player over to usrintrf.c's bitmap-font renderer for one screen.

   Every walk below is the same one the matching usrintrf.c / cheat.c function
   does. Only the presentation differs - the state these touch is MAME's, and
   changes take effect through MAME's own paths.

   All const char * returns point at either driver-owned strings or per-call
   static buffers. Copy or draw them before the next call; none survive one. */

/*---------------------------------------------------------- game history */
/* Fills buffer with history.dat's entry for the running driver. Returns 0 on
   success, non-zero when no entry exists (the common case - history.dat is an
   optional download). */
int nx_history_load(char *buffer, int size);

/*-------------------------------------------------------- analog controls */
/* Each analog port exposes three editable fields; ports whose driver marks
   them digital-only expose the first two. */
enum { NX_ANALOG_DELTA = 0, NX_ANALOG_REVERSE, NX_ANALOG_SENSITIVITY,
       NX_ANALOG_FIELDS };

int         nx_analog_count(void);
const char *nx_analog_name(int index);
const char *nx_analog_field_name(int field);
const char *nx_analog_field_value(int index, int field);
void        nx_analog_change(int index, int field, int direction);

/*----------------------------------------------------------- input remap */
/* general != 0 walks inputport_defaults (every driver); general == 0 walks
   Machine->input_ports (this game only). The two lists are independent and
   indices are not interchangeable. */
int         nx_input_count(int general);
const char *nx_input_name(int index, int general);
const char *nx_input_binding(int index, int general);

/* Capture runs across frames: start once, then poll every frame until it
   returns non-zero. The overlay must ignore its own buttons while a capture is
   open, or the press being recorded also drives the menu. */
void        nx_input_capture_start(void);
int         nx_input_capture_poll(int index, int general);
void        nx_input_capture_clear(int index, int general);

/*------------------------------------------------------------- memory card */
/* Neo Geo only; nx_memcard_available() is 0 everywhere else. */
enum { NX_MEMCARD_LOAD = 0, NX_MEMCARD_SAVE, NX_MEMCARD_EJECT, NX_MEMCARD_CREATE };

int         nx_memcard_available(void);
int         nx_memcard_inserted(void);
int         nx_memcard_slot(void);
/* Returns 0 on success. Load and create take the slot number; save and eject
   act on whatever is inserted. */
int         nx_memcard_action(int action, int slot);

/*------------------------------------------------------------------ cheats */
/* Implemented in cheat.c - the cheat list and its activate/deactivate paths
   are static there, and reaching them from outside would mean duplicating the
   flag logic that decides what a given row even means. */
int         nx_cheat_count(void);
const char *nx_cheat_name(int index);
/* 0 for comment rows, which are labels rather than settings. */
const char *nx_cheat_value(int index);
int         nx_cheat_selectable(int index);
/* One-shot cheats fire; on/off cheats toggle; multi-option cheats step. */
void        nx_cheat_change(int index, int direction);
void        nx_cheat_activate(int index);

#ifdef __cplusplus
}
#endif
