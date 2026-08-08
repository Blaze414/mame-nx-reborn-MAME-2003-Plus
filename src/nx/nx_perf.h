/***************************************************************************

	nx_perf.h

	Switch V1 performance budgeting: adaptive CPU boost, frame pacing with
	catch-up frameskip, per-driver cost tiers, and an up-front memory check.

	The Tegra X1 in a V1 runs four A57s at 1020MHz (1785MHz boosted) and,
	when launched as an applet from hbmenu, gives homebrew only a few
	hundred MB of heap. Both ceilings are low enough that a 5000-driver set
	will walk into them, so the browser needs to say so before a game loads
	rather than dying inside the allocator.

***************************************************************************/

#ifndef NX_PERF_H
#define NX_PERF_H

#ifdef __cplusplus
extern "C" {
#endif

struct GameDriver;

/* Rough cost class of a driver on V1 silicon. Used to set the frameskip
   budget and to label entries in the ROM browser. */
typedef enum
{
	NX_TIER_LIGHT = 0,	/* full speed at stock clocks */
	NX_TIER_MEDIUM,		/* full speed, may dip; boost covers it */
	NX_TIER_HEAVY,		/* needs boost and will drop frames */
	NX_TIER_EXTREME		/* not realistically playable on V1 */
} nx_tier_t;

/* Why a driver cannot be launched right now, if it cannot. */
typedef enum
{
	NX_FIT_OK = 0,
	NX_FIT_NEEDS_FULL_RAM,	/* would fit, but not in applet mode */
	NX_FIT_TOO_BIG			/* will not fit even with the full heap */
} nx_fit_t;

void nx_perf_init(void);
void nx_perf_exit(void);

/* Cost tier, computed from the machine driver's CPU list. Cheap enough to
   call while drawing the browser; results are cached per driver. */
nx_tier_t   nx_perf_driver_tier(const struct GameDriver *gamedrv);
const char *nx_perf_tier_name(nx_tier_t tier);

/* Can this driver's ROM regions be allocated in the heap we actually have?
   'needed' and 'avail' are filled in with byte counts when non-NULL. */
nx_fit_t nx_perf_driver_fits(const struct GameDriver *gamedrv,
							 unsigned long *needed, unsigned long *avail);

/* True when running as an hbmenu applet, where the heap is a fraction of
   what a full title-takeover launch would give us. */
int nx_perf_is_applet(void);

/* Last SoC temperature in whole degrees C, or -1 if the sensor is not open.
   Sampled about once a second while a game runs. */
int nx_perf_soc_temp(void);

/* Non-zero while CPU boost is being withheld to keep the SoC out of the
   system's own throttling range. */
int nx_perf_thermal_limited(void);

/* Call once per emulated frame, around mame_frame(). frame_begin() decides
   whether this frame's video can be dropped to catch up, and raises or
   drops the CPU boost based on how the last second actually went. */
void nx_perf_game_start(const struct GameDriver *gamedrv);
void nx_perf_game_stop(void);
int  nx_perf_frame_begin(void);	/* returns 1 if video should be skipped */
int  nx_perf_skip_this_frame(void);	/* latched answer for osd_skip_this_frame() */
void nx_perf_frame_end(void);

#ifdef __cplusplus
}
#endif

#endif	/* NX_PERF_H */
