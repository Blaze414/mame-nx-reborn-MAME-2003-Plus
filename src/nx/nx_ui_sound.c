#include "nx_ui_sound.h"

#include <switch.h>

#include <math.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#define UI_SOUND_BUFFER_COUNT 4
#define UI_SOUND_RATE 48000
#define UI_SOUND_ALIGNMENT 0x1000
#define UI_SOUND_MAX_FRAMES 4800
#define UI_SOUND_TAU 6.28318530717958647692f

typedef struct nx_ui_sound_slot
{
	AudioOutBuffer output;
	int queued;
} nx_ui_sound_slot;

static nx_ui_sound_slot ui_sound_slots[UI_SOUND_BUFFER_COUNT];
static int ui_sound_ready;
static size_t ui_sound_buffer_size;



static size_t ui_sound_align(size_t value)
{
	return (value + UI_SOUND_ALIGNMENT - 1) & ~(UI_SOUND_ALIGNMENT - 1);
}

static void ui_sound_release_finished(void)
{
	AudioOutBuffer *released = NULL;
	uint32_t released_count = 0;
	uint32_t released_index;

	if (!ui_sound_ready || R_FAILED(audoutGetReleasedAudioOutBuffer(
		&released, &released_count)))
		return;

	for (released_index = 0; released && released_index < released_count;
		released_index++, released = released->next)
	{
		int slot;
		for (slot = 0; slot < UI_SOUND_BUFFER_COUNT; ++slot)
		{
			if (&ui_sound_slots[slot].output == released)
			{
				ui_sound_slots[slot].queued = 0;
				break;
			}
		}
	}
}

static void ui_sound_tone(int kind, float *frequency_a, float *frequency_b,
	float *duration, float *gain)
{
	switch (kind)
	{
		case NX_UI_SOUND_CONFIRM:
			*frequency_a = 523.25f;
			*frequency_b = 783.99f;
			*duration = 0.075f;
			*gain = 0.19f;
			break;
		case NX_UI_SOUND_BACK:
			*frequency_a = 392.00f;
			*frequency_b = 293.66f;
			*duration = 0.052f;
			*gain = 0.16f;
			break;
		case NX_UI_SOUND_ERROR:
			*frequency_a = 220.00f;
			*frequency_b = 164.81f;
			*duration = 0.100f;
			*gain = 0.17f;
			break;
		default:
			*frequency_a = 659.25f;
			*frequency_b = 987.77f;
			*duration = 0.034f;
			*gain = 0.13f;
			break;
	}
}

int nx_ui_sound_init(void)
{
	int slot;

	if (ui_sound_ready)
		return 1;

	memset(ui_sound_slots, 0, sizeof(ui_sound_slots));
	ui_sound_buffer_size = ui_sound_align(
		UI_SOUND_MAX_FRAMES * 2 * sizeof(int16_t));

	if (R_FAILED(audoutInitialize()))
		return 0;
	if (R_FAILED(audoutStartAudioOut()))
	{
		audoutExit();
		return 0;
	}
	ui_sound_ready = 1;

	for (slot = 0; slot < UI_SOUND_BUFFER_COUNT; ++slot)
	{
		AudioOutBuffer *output = &ui_sound_slots[slot].output;
		output->buffer = aligned_alloc(UI_SOUND_ALIGNMENT, ui_sound_buffer_size);
		if (!output->buffer)
		{
			nx_ui_sound_exit();
			return 0;
		}
		memset(output->buffer, 0, ui_sound_buffer_size);
		output->buffer_size = ui_sound_buffer_size;
		output->data_offset = 0;
		output->data_size = 0;
		output->next = NULL;
	}

	return 1;
}

int nx_ui_sound_play(int kind)
{
	float frequency_a;
	float frequency_b;
	float duration;
	float gain;
	int frames;
	int slot;
	int frame;
	AudioOutBuffer *output;
	int16_t *samples;

	if (!ui_sound_ready)
		return 0;

	ui_sound_release_finished();
	for (slot = 0; slot < UI_SOUND_BUFFER_COUNT; ++slot)
		if (!ui_sound_slots[slot].queued)
			break;
	if (slot == UI_SOUND_BUFFER_COUNT)
		return 0;

	ui_sound_tone(kind, &frequency_a, &frequency_b, &duration, &gain);
	frames = (int)(duration * UI_SOUND_RATE);
	if (frames > UI_SOUND_MAX_FRAMES)
		frames = UI_SOUND_MAX_FRAMES;

	output = &ui_sound_slots[slot].output;
	samples = (int16_t *)output->buffer;
	for (frame = 0; frame < frames; ++frame)
	{
		float progress = (float)frame / (float)frames;
		float attack = frame < 144 ? (float)frame / 144.0f : 1.0f;
		float decay = 1.0f - progress;
		float envelope = attack * decay * decay;
		float time = (float)frame / (float)UI_SOUND_RATE;
		float wave = sinf(UI_SOUND_TAU * frequency_a * time) * 0.72f +
			sinf(UI_SOUND_TAU * frequency_b * time) * 0.28f;
		int16_t sample = (int16_t)(wave * envelope * gain * 32767.0f);
		samples[frame * 2] = sample;
		samples[frame * 2 + 1] = sample;
	}

	output->next = NULL;
	output->data_offset = 0;
	output->data_size = (size_t)frames * 2 * sizeof(int16_t);
	output->buffer_size = ui_sound_buffer_size;
	if (R_FAILED(audoutAppendAudioOutBuffer(output)))
		return 0;

	ui_sound_slots[slot].queued = 1;
	return 1;
}

void nx_ui_sound_exit(void)
{
	int slot;

	if (ui_sound_ready)
	{
		audoutStopAudioOut();
		audoutExit();
	}

	for (slot = 0; slot < UI_SOUND_BUFFER_COUNT; ++slot)
	{
		free(ui_sound_slots[slot].output.buffer);
		ui_sound_slots[slot].output.buffer = NULL;
		ui_sound_slots[slot].queued = 0;
	}
	ui_sound_ready = 0;
}
