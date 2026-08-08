#include "nx_pause_sound.h"

#include <math.h>
#include <stdint.h>

#define PAUSE_SINE_SIZE 256
#define PAUSE_PI 3.14159265358979323846

typedef struct nx_pause_tone
{
	int active;
	int remaining;
	int total;
	uint32_t phase_a;
	uint32_t phase_b;
	uint32_t step_a;
	uint32_t step_b;
	int gain;
	int forced_release;
	int forced_release_total;
} nx_pause_tone;

static nx_pause_tone pause_tone;
static volatile int pending_tone = -1;
static int queued_tone = -1;
static int pause_sample_rate = 48000;
static INT16 pause_sine[PAUSE_SINE_SIZE];

static int pause_wave(uint32_t phase)
{
	return pause_sine[(phase >> 8) & (PAUSE_SINE_SIZE - 1)];
}

static INT16 pause_clamp(int value)
{
	if (value > 32767)
		return 32767;
	if (value < -32768)
		return -32768;
	return (INT16)value;
}

static uint32_t pause_step(int frequency)
{
	return (uint32_t)(((uint64_t)frequency << 16) / (uint32_t)pause_sample_rate);
}

static void pause_tone_spec(int kind, int *frequency_a, int *frequency_b,
	int *milliseconds, int *gain)
{
	switch (kind)
	{
		case NX_PAUSE_SOUND_CONFIRM:
			*frequency_a = 523;
			*frequency_b = 784;
			*milliseconds = 76;
			*gain = 5600;
			break;
		case NX_PAUSE_SOUND_BACK:
			*frequency_a = 392;
			*frequency_b = 294;
			*milliseconds = 58;
			*gain = 4800;
			break;
		case NX_PAUSE_SOUND_ERROR:
			*frequency_a = 220;
			*frequency_b = 165;
			*milliseconds = 105;
			*gain = 5200;
			break;
		default:
			*frequency_a = 659;
			*frequency_b = 988;
			*milliseconds = 36;
			*gain = 3800;
			break;
	}
}

static void pause_begin_tone(int kind)
{
	int frequency_a;
	int frequency_b;
	int milliseconds;

	pause_tone_spec(kind, &frequency_a, &frequency_b, &milliseconds,
		&pause_tone.gain);

	pause_tone.total = pause_sample_rate * milliseconds / 1000;
	if (pause_tone.total < 1)
		pause_tone.total = 1;
	pause_tone.remaining = pause_tone.total;
	pause_tone.phase_a = 0;
	pause_tone.phase_b = 0;
	pause_tone.step_a = pause_step(frequency_a);
	pause_tone.step_b = pause_step(frequency_b);
	pause_tone.forced_release = 0;
	pause_tone.forced_release_total = 0;
	pause_tone.active = 1;
}

void nx_pause_sound_init(int sample_rate)
{
	int sample;

	if (sample_rate > 0)
		pause_sample_rate = sample_rate;
	for (sample = 0; sample < PAUSE_SINE_SIZE; ++sample)
		pause_sine[sample] = (INT16)(sin((2.0 * PAUSE_PI * sample) /
			PAUSE_SINE_SIZE) * 32767.0);
	nx_pause_sound_reset();
}

void nx_pause_sound_play(int kind)
{
	if (kind < NX_PAUSE_SOUND_NAVIGATE || kind > NX_PAUSE_SOUND_ERROR)
		kind = NX_PAUSE_SOUND_ERROR;
	pending_tone = kind;
}

int nx_pause_sound_render(INT16 *stereo_samples, int max_frames)
{
	int kind = pending_tone;
	int frequency_a;
	int frequency_b;
	int milliseconds;
	int gain;
	int total;
	int attack_frames;
	int frame;
	uint32_t phase_a = 0;
	uint32_t phase_b = 0;
	uint32_t step_a;
	uint32_t step_b;

	if (!stereo_samples || max_frames <= 0 || kind < 0)
		return 0;
	pending_tone = -1;
	queued_tone = -1;
	pause_tone.active = 0;

	pause_tone_spec(kind, &frequency_a, &frequency_b, &milliseconds, &gain);
	total = pause_sample_rate * milliseconds / 1000;
	if (total > max_frames)
		total = max_frames;
	if (total < 1)
		return 0;
	attack_frames = pause_sample_rate * 4 / 1000;
	step_a = pause_step(frequency_a);
	step_b = pause_step(frequency_b);

	for (frame = 0; frame < total; ++frame)
	{
		int remaining = total - frame;
		int envelope = (int)(((int64_t)remaining * remaining * 256) /
			((int64_t)total * total));
		int wave;
		int sample;

		if (attack_frames > 0 && frame < attack_frames)
		{
			int attack = frame * 256 / attack_frames;
			if (attack < envelope)
				envelope = attack;
		}
		wave = (pause_wave(phase_a) * 3 + pause_wave(phase_b)) / 4;
		sample = wave * gain / 32768;
		sample = sample * envelope / 256;
		stereo_samples[frame * 2] = pause_clamp(sample);
		stereo_samples[frame * 2 + 1] = pause_clamp(sample);
		phase_a += step_a;
		phase_b += step_b;
	}

	return total;
}

void nx_pause_sound_mix(INT16 *stereo_samples, int frames)
{
	int frame;
	int requested = pending_tone;

	if (!stereo_samples || frames <= 0)
		return;
	if (requested >= 0)
	{
		pending_tone = -1;
		if (pause_tone.active)
		{
			/* Never hard-cut a waveform: that discontinuity becomes a click. */
			queued_tone = requested;
			if (pause_tone.forced_release <= 0)
			{
				pause_tone.forced_release_total = pause_sample_rate * 3 / 1000;
				if (pause_tone.forced_release_total < 1)
					pause_tone.forced_release_total = 1;
				pause_tone.forced_release = pause_tone.forced_release_total;
			}
		}
		else
			pause_begin_tone(requested);
	}

	for (frame = 0; frame < frames; ++frame)
	{
		int elapsed;
		int attack_frames;
		int release_frames;
		int envelope = 256;
		int wave;
		int sample;

		if (!pause_tone.active)
		{
			if (queued_tone < 0)
				continue;
			requested = queued_tone;
			queued_tone = -1;
			pause_begin_tone(requested);
		}

		elapsed = pause_tone.total - pause_tone.remaining;
		attack_frames = pause_sample_rate * 4 / 1000;
		release_frames = pause_sample_rate * 6 / 1000;
		if (attack_frames > 0 && elapsed < attack_frames)
			envelope = elapsed * 256 / attack_frames;
		if (release_frames > 0 && pause_tone.remaining < release_frames)
		{
			int release_envelope = pause_tone.remaining * 256 / release_frames;
			if (release_envelope < envelope)
				envelope = release_envelope;
		}
		if (pause_tone.forced_release > 0)
		{
			int forced_envelope = pause_tone.forced_release * 256 /
				pause_tone.forced_release_total;
			if (forced_envelope < envelope)
				envelope = forced_envelope;
		}

		wave = (pause_wave(pause_tone.phase_a) * 3 +
			pause_wave(pause_tone.phase_b)) / 4;
		sample = wave * pause_tone.gain / 32768;
		sample = sample * envelope / 256;

		stereo_samples[frame * 2] = pause_clamp(
			(int)stereo_samples[frame * 2] + sample);
		stereo_samples[frame * 2 + 1] = pause_clamp(
			(int)stereo_samples[frame * 2 + 1] + sample);

		pause_tone.phase_a += pause_tone.step_a;
		pause_tone.phase_b += pause_tone.step_b;
		--pause_tone.remaining;
		if (pause_tone.forced_release > 0)
			--pause_tone.forced_release;
		if (pause_tone.remaining <= 0 ||
			(pause_tone.forced_release_total > 0 && pause_tone.forced_release <= 0))
			pause_tone.active = 0;
	}
}

void nx_pause_sound_reset(void)
{
	pause_tone.active = 0;
	pause_tone.remaining = 0;
	pause_tone.forced_release = 0;
	pause_tone.forced_release_total = 0;
	pending_tone = -1;
	queued_tone = -1;
}
