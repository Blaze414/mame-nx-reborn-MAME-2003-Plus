/***************************************************************************

	nx_perf.c

	See nx_perf.h. Everything here is V1-shaped: the thresholds below were
	picked against a 1020MHz A57 with a 1785MHz boost ceiling.

***************************************************************************/

#include <switch.h>
#include <string.h>
#include <stdlib.h>

#include "osd_cpu.h"
#include "driver.h"
#include "nx_perf.h"

/*-------------------------------------------------
	CPU cost weighting
-------------------------------------------------*/

/* Emulated MHz alone is a poor proxy for host cost: a 28MHz SH-2 costs far
   more per cycle to interpret than a 28MHz 68000, which in turn costs more
   than a Z80. So each CPU contributes clock * weight, where weight is a
   rough "host cycles per emulated cycle" multiplier.

   The enum members only exist when their HAS_ macro is on, hence the
   guards - a driver whose CPU was compiled out never reaches the switch. */
static int cpu_weight(int cpu_type)
{
	switch (cpu_type)
	{
#if (HAS_SH2)
		case CPU_SH2:		return 12;	/* ST-V, Model 2 sound - the worst offenders */
#endif
#if (HAS_PSXCPU)
		case CPU_PSXCPU:	return 10;	/* ZN, namcos11/12 */
#endif
#if (HAS_R3000)
		case CPU_R3000BE:	case CPU_R3000LE:	return 10;
#endif
#if (HAS_R4600)
		case CPU_R4600BE:	case CPU_R4600LE:	return 11;
#endif
#if (HAS_R5000)
		case CPU_R5000BE:	case CPU_R5000LE:	return 11;
#endif
#if (HAS_TMS34010)
		case CPU_TMS34010:	return 8;	/* midway y/t/w units */
#endif
#if (HAS_TMS34020)
		case CPU_TMS34020:	return 9;
#endif
#if (HAS_DSP32C)
		case CPU_DSP32C:	return 9;
#endif
#if (HAS_ASAP)
		case CPU_ASAP:		return 8;
#endif
#if (HAS_JAGUAR)
		case CPU_JAGUARGPU:	case CPU_JAGUARDSP:	return 9;
#endif
#if (HAS_ADSP2100)
		case CPU_ADSP2100:	return 6;
#endif
#if (HAS_ADSP2105)
		case CPU_ADSP2105:	return 6;
#endif
#if (HAS_V60)
		case CPU_V60:		return 7;
#endif
#if (HAS_V70)
		case CPU_V70:		return 8;
#endif
#if (HAS_M68020)
		case CPU_M68020:	return 5;
#endif
#if (HAS_M68EC020)
		case CPU_M68EC020:	return 5;
#endif
#if (HAS_G65816)
		case CPU_G65816:	return 4;	/* nss / snes */
#endif
#if (HAS_ARM)
		case CPU_ARM:		return 4;
#endif
#if (HAS_M68000)
		case CPU_M68000:	return 3;
#endif
		default:			return 2;	/* Z80, 6502, 6809, sound CPUs */
	}
}

/* Weighted score, in "A57-equivalent MHz". Thresholds are where a V1
   actually falls over, measured against the boost ceiling rather than the
   stock clock, since we boost automatically when a driver needs it. */
#define TIER_MEDIUM_SCORE	60
#define TIER_HEAVY_SCORE	200
#define TIER_EXTREME_SCORE	600

static int driver_score(const struct GameDriver *gamedrv)
{
	struct InternalMachineDriver drv;
	int total = 0;
	int i;

	if (!gamedrv || !gamedrv->drv)
		return 0;

	expand_machine_driver(gamedrv->drv, &drv);

	for (i = 0; i < MAX_CPU; i++)
	{
		int clock = drv.cpu[i].cpu_clock;
		if (drv.cpu[i].cpu_type == 0 || clock <= 0)
			continue;
		total += (clock / 1000000) * cpu_weight(drv.cpu[i].cpu_type);
	}

	return total;
}

/* Browser redraws call this per visible row, and expand_machine_driver()
   walks the whole constructor each time, so keep a small direct-mapped
   cache. ponytail: fixed 256 entries, no eviction policy - the browser only
   ever looks at a window of rows, so collisions just recompute. */
#define TIER_CACHE_SIZE 256
static struct { const struct GameDriver *drv; nx_tier_t tier; } s_tier_cache[TIER_CACHE_SIZE];

nx_tier_t nx_perf_driver_tier(const struct GameDriver *gamedrv)
{
	unsigned slot;
	int score;
	nx_tier_t tier;

	if (!gamedrv)
		return NX_TIER_LIGHT;

	slot = ((unsigned long)gamedrv >> 4) & (TIER_CACHE_SIZE - 1);
	if (s_tier_cache[slot].drv == gamedrv)
		return s_tier_cache[slot].tier;

	score = driver_score(gamedrv);
	if (score >= TIER_EXTREME_SCORE)     tier = NX_TIER_EXTREME;
	else if (score >= TIER_HEAVY_SCORE)  tier = NX_TIER_HEAVY;
	else if (score >= TIER_MEDIUM_SCORE) tier = NX_TIER_MEDIUM;
	else                                 tier = NX_TIER_LIGHT;

	s_tier_cache[slot].drv = gamedrv;
	s_tier_cache[slot].tier = tier;
	return tier;
}

const char *nx_perf_tier_name(nx_tier_t tier)
{
	switch (tier)
	{
		case NX_TIER_LIGHT:   return "Full speed";
		case NX_TIER_MEDIUM:  return "Playable";
		case NX_TIER_HEAVY:   return "Slow";
		case NX_TIER_EXTREME: return "Unplayable";
	}
	return "";
}

/*-------------------------------------------------
	Memory budget
-------------------------------------------------*/

/* Margin over the ROM footprint for the frontend, the framebuffer and MAME's
   own working allocations - gfx decode dwarfs the ROMs on some drivers, so it
   is deliberately generous. */
#define NX_HEAP_MARGIN	(96u * 1024u * 1024u)

int nx_perf_is_applet(void)
{
	AppletType type = appletGetAppletType();
	return (type != AppletType_Application && type != AppletType_SystemApplication);
}

/* Ask the allocator, not the kernel.

   svcGetInfo(InfoType_UsedMemorySize) counts memory mapped into the process,
   and libnx's __libnx_initheap claims nearly all of it as the malloc heap
   before main() runs. So total-used is a few MB in every launch mode - it
   measures the slack *outside* the heap, while every byte MAME will actually
   ask for comes from inside it. A title takeover reserves more, so it reports
   even less free. Probing the allocator asks the question that matters: can
   this many bytes be handed out right now. */
static int heap_can_serve(unsigned long bytes)
{
	void *p = malloc(bytes);

	if (!p)
		return 0;
	free(p);
	return 1;
}

/* Only used to fill in the "x MB free" half of the message - a report, not a
   decision. Halving down from the request costs at most ~20 probes. */
static unsigned long heap_probe_free(unsigned long start)
{
	unsigned long b = start;

	while (b >= 1024ul * 1024ul && !heap_can_serve(b))
		b /= 2;

	return heap_can_serve(b) ? b : 0;
}

static unsigned long driver_rom_bytes(const struct GameDriver *gamedrv)
{
	const struct RomModule *region;
	unsigned long total = 0;

	for (region = rom_first_region(gamedrv); region; region = rom_next_region(region))
		total += ROMREGION_GETLENGTH(region);

	return total;
}

nx_fit_t nx_perf_driver_fits(const struct GameDriver *gamedrv,
							 unsigned long *needed, unsigned long *avail)
{
	unsigned long rom_bytes, want;

	if (!gamedrv)
		return NX_FIT_OK;

	rom_bytes = driver_rom_bytes(gamedrv);
	want      = rom_bytes + NX_HEAP_MARGIN;

	/* Report the full request, not just the ROMs - otherwise the message can
	   read "needs 10MB, only 64MB free", which is nonsense to a user. */
	if (needed) *needed = want;

	if (heap_can_serve(want))
	{
		if (avail) *avail = want;
		return NX_FIT_OK;
	}

	/* Only now is the slow probe worth it - the message needs a number. */
	if (avail) *avail = heap_probe_free(want);

	/* Doesn't fit here. Distinguish "wrong launch mode" from "hopeless" so
	   the browser can tell the user to relaunch over a game rather than
	   just refusing. */
	return nx_perf_is_applet() ? NX_FIT_NEEDS_FULL_RAM : NX_FIT_TOO_BIG;
}

/*-------------------------------------------------
	Frame pacing and adaptive boost
-------------------------------------------------*/

static int   s_boosted;
static int   s_max_skip;			/* frameskip budget for the running driver */
static int   s_consecutive_skips;
static int   s_skip_this_frame;
static u64   s_frame_target;		/* ticks per emulated frame */
static u64   s_frame_start;
static s64   s_debt;				/* ticks we are behind by */
static u64   s_review_at;			/* next boost re-evaluation */
static int   s_late_frames;
static int   s_total_frames;
static int   s_ts_up;				/* temperature sensor opened */
static int   s_soc_temp = -1;		/* last SoC reading in whole degrees C */
static int   s_thermal_hold;		/* boost withheld because the SoC is hot */

/* TMP451 external channel is the SoC die. A V1 idles around the mid-40s and
   sits in the 60s under a boosted emulation load; the system's own throttle
   starts north of 80. Backing off at 72 and only re-allowing at 65 keeps us
   clear of that without oscillating - and it is a wide enough band that a
   warm room does not flip the governor every review. */
#define THERMAL_BACKOFF_C	72
#define THERMAL_RESUME_C	65

static void thermal_sample(void)
{
	s32 celsius = 0;

	if (!s_ts_up)
		return;

	if (R_SUCCEEDED(tsGetTemperature(TsLocation_External, &celsius)))
		s_soc_temp = (int)celsius;
}

int nx_perf_soc_temp(void)
{
	return s_soc_temp;
}

int nx_perf_thermal_limited(void)
{
	return s_thermal_hold;
}

static void set_boost(int on)
{
	/* A thermal hold outranks every other reason to boost. Boost is what
	   generates the heat, so continuing to ask for it while hot is how a
	   handheld ends up throttled by the system into something far slower than
	   stock - worse than never having boosted. */
	if (on && s_thermal_hold)
		on = 0;

	if (on == s_boosted)
		return;

	/* FastLoad pins the CPU at its ceiling and trades away GPU clock. That
	   is exactly the right trade here: MAME is a software rasteriser, so
	   the GPU is idle apart from the final blit. */
	appletSetCpuBoostMode(on ? ApmCpuBoostMode_FastLoad : ApmCpuBoostMode_Normal);
	s_boosted = on;
}

void nx_perf_init(void)
{
	memset(s_tier_cache, 0, sizeof(s_tier_cache));
	s_boosted = 0;
	s_thermal_hold = 0;
	s_ts_up = R_SUCCEEDED(tsInitialize());
	thermal_sample();
}

void nx_perf_exit(void)
{
	set_boost(0);
	if (s_ts_up) { tsExit(); s_ts_up = 0; }
}

void nx_perf_game_start(const struct GameDriver *gamedrv)
{
	struct InternalMachineDriver drv;
	float fps = 60.0f;
	nx_tier_t tier = nx_perf_driver_tier(gamedrv);

	if (gamedrv && gamedrv->drv)
	{
		expand_machine_driver(gamedrv->drv, &drv);
		if (drv.frames_per_second > 1.0f)
			fps = drv.frames_per_second;
	}

	s_frame_target = (u64)((double)armGetSystemTickFreq() / fps);
	s_debt = 0;
	s_consecutive_skips = 0;
	s_skip_this_frame = 0;
	s_late_frames = 0;
	s_total_frames = 0;
	s_review_at = armGetSystemTick() + armGetSystemTickFreq();

	/* Frameskip budget by tier. Light drivers never skip - dropping frames
	   on a game that already fits is pure visual damage. */
	switch (tier)
	{
		case NX_TIER_LIGHT:   s_max_skip = 0; break;
		case NX_TIER_MEDIUM:  s_max_skip = 1; break;
		case NX_TIER_HEAVY:   s_max_skip = 3; break;
		case NX_TIER_EXTREME: s_max_skip = 5; break;
	}

	/* Anything above medium is known to need the headroom, so start boosted
	   instead of making the user watch it stutter for a second first. */
	set_boost(tier >= NX_TIER_HEAVY);

	s_frame_start = armGetSystemTick();
}

void nx_perf_game_stop(void)
{
	set_boost(0);
}

int nx_perf_frame_begin(void)
{
	s_frame_start = armGetSystemTick();

	/* Only skip once we owe more than a whole frame, and never more than
	   the budget in a row, so a driver that simply cannot keep up still
	   draws something instead of going black. */
	if (s_debt > (s64)s_frame_target && s_consecutive_skips < s_max_skip)
		s_consecutive_skips++;
	else
		s_consecutive_skips = 0;

	/* Latch it: mame_frame() asks osd_skip_this_frame() several times while
	   running a frame and must get the same answer every time. */
	s_skip_this_frame = (s_consecutive_skips > 0);
	return s_skip_this_frame;
}

int nx_perf_skip_this_frame(void)
{
	return s_skip_this_frame;
}

void nx_perf_frame_end(void)
{
	u64 now = armGetSystemTick();
	s64 elapsed = (s64)(now - s_frame_start);

	s_debt += elapsed - (s64)s_frame_target;
	if (s_debt < 0)
		s_debt = 0;				/* running ahead: no credit, we are vsync-bound */

	s_total_frames++;
	if (elapsed > (s64)s_frame_target)
		s_late_frames++;

	/* Re-evaluate boost about once a second. Hysteresis is deliberately
	   wide: toggling the clock every review would thrash, and on a V1 the
	   thermal cost of holding boost is real. */
	if (now >= s_review_at)
	{
		thermal_sample();

		/* Governor first, so the frame-rate rule below is deciding within
		   whatever headroom is actually left. */
		if (s_soc_temp >= 0)
		{
			if (!s_thermal_hold && s_soc_temp >= THERMAL_BACKOFF_C)
			{
				s_thermal_hold = 1;
				set_boost(0);

				/* Buy back the work another way: a hot SoC that is also
				   behind will otherwise spin at stock clocks and stay hot.
				   One extra skip is cheaper than sustained throttling. */
				if (s_max_skip < 5)
					s_max_skip++;
			}
			else if (s_thermal_hold && s_soc_temp <= THERMAL_RESUME_C)
			{
				s_thermal_hold = 0;
			}
		}

		if (s_total_frames > 0)
		{
			int late_pct = (s_late_frames * 100) / s_total_frames;

			if (!s_boosted && late_pct > 10)
				set_boost(1);
			else if (s_boosted && late_pct < 2)
				set_boost(0);
		}

		s_late_frames = 0;
		s_total_frames = 0;
		s_review_at = now + armGetSystemTickFreq();
	}
}
