#pragma once

#ifdef __cplusplus
extern "C" {
#endif

/* Read/write access to the running driver's dip switches, so the frontend's
   pause overlay can edit them itself instead of handing the player over to
   MAME's own menu renderer for one screen. The InputPort walk is the same one
   usrintrf.c's setdipswitches() does; only the presentation differs.

   Indices are positions in the filtered list and stay valid for as long as the
   game is running - Machine->input_ports is built once at load. */

int         nx_dip_count(void);
const char *nx_dip_name(int index);
/* Label of the setting currently selected, or "-" if the driver's default
   value matches no declared setting (a real case in some sets). */
const char *nx_dip_value(int index);
/* direction > 0 next setting, < 0 previous. Clamps at both ends rather than
   wrapping: a dip is a physical switch, and running off one end back to the
   other is not how one behaves. */
void        nx_dip_change(int index, int direction);

#ifdef __cplusplus
}
#endif
