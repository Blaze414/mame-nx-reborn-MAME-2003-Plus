#pragma once

#include "osd_cpu.h"

#ifdef __cplusplus
extern "C" {
#endif

enum nx_pause_sound_kind
{
	NX_PAUSE_SOUND_NAVIGATE = 0,
	NX_PAUSE_SOUND_CONFIRM,
	NX_PAUSE_SOUND_BACK,
	NX_PAUSE_SOUND_ERROR
};

/* In-game UI audio. Mixed into MAME PCM; never owns audout. */
void nx_pause_sound_init(int sample_rate);
void nx_pause_sound_play(int kind);
void nx_pause_sound_mix(INT16 *stereo_samples, int frames);
/* Render one complete queued tone so pause-loop jitter cannot split it. */
int nx_pause_sound_render(INT16 *stereo_samples, int max_frames);
/* Submit queued tones while MAME's paused nested UI is not running sound_update(). */
void nx_pause_sound_update(void);
void nx_pause_sound_reset(void);

#ifdef __cplusplus
}
#endif
