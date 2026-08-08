#pragma once

#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif

/* Save states for the running game.

   src/state.c already carries the whole serializer - every driver registers its
   variables through state_save_register_* on load, and the registry is live the
   moment a game starts. What it has never had in this tree is a caller: the
   only ones were libretro's retro_serialize()/retro_unserialize(), which left
   with RetroArch. This is that caller, plus the file on the SD card.

   Nothing here schedules or defers. Upstream has to, because its save menu runs
   inside MAME's own UI loop with the CPUs still scheduled; our pause overlay
   runs outside mame_frame() entirely, so by the time any of this is called the
   machine is already stopped at a frame boundary and a save is just a copy. */

enum { NX_STATE_SLOTS = 10 };

/* Zero when this driver cannot be serialized - either it declares
   GAME_DOESNT_SERIALIZE, or one of its registered entries points at memory
   that was never allocated. Sets nx_state_error() to the reason on the way
   out: the menu shows the rows either way and explains itself, because a row
   that hides when it cannot work is indistinguishable from one that was never
   built. */
int nx_state_available(void);

/* Bytes one state occupies, for the size readout. Zero when unavailable. */
size_t nx_state_size(void);

/* 0 on success, non-zero on failure; nx_state_error() then describes it.
   MAME's own usrintf_showmessage() goes to a renderer this frontend never
   runs, so anything state.c reports that way is invisible and the checks that
   matter are repeated here. */
int nx_state_save(int slot);
int nx_state_load(int slot);

const char *nx_state_error(void);

/* Cheap enough to call per repaint: one stat() per slot, and only when the
   slot page is on screen. Refreshed explicitly after a save so the row the
   player just wrote to stops saying "Empty". */
void        nx_state_refresh(void);
int         nx_state_slot_used(int slot);
/* "Empty", or the date and size of what is in the slot. */
const char *nx_state_slot_info(int slot);

#ifdef __cplusplus
}
#endif
