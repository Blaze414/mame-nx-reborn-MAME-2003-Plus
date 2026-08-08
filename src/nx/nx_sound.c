
 
#include "osdepend.h"
#include "osd_cpu.h"
#include "mame2003.h"
#include "driver.h"

#include "log.h"
#include "nx_mame.h"
#include "nx_pause_sound.h"
#include <switch.h>
  
int samples_per_frame = 0;
  
#define SAMPLERATE 48000
#define CHANNELCOUNT 2
#define BYTESPERSAMPLE sizeof(uint16_t)
/* One stereo sample as the device takes it: two 16-bit channels. */
#define BYTES_PER_FRAME (CHANNELCOUNT * BYTESPERSAMPLE)

/* Buffer capacity is set by the SLOWEST driver, not 60Hz: samples per frame is
   sample_rate/fps, so a 30Hz game needs 1600 where a 60Hz one needs 800. Size
   for 30Hz or the clamp below silently drops a third of every frame. (This was
   SAMPLERATE/(1000/60); 1000/60 truncates to 16, which was wrong twice over.) */
#define MAX_SAMPLES_PER_FRAME ((SAMPLERATE + 29) / 30)

/* Silence queued at startup, one 60Hz frame per buffer - enough cushion to
   cover the first few frames without adding audible latency. */
#define PREFILL_SAMPLES ((SAMPLERATE + 59) / 60)

/* Frames of audio queued to the device, and effectively the whole of the audio
   latency: one buffer is produced and one consumed per frame, so the queue
   settles at however many buffers exist, and the delay from a sound being
   emulated to being heard is that many frames. Four is ~66ms at 60Hz.

   Two used to leave barely a frame of slack, so any late frame drained the
   queue and crackled - but much of that was buffers falling out of circulation
   entirely (see free_list above), which is now fixed. Whether three or even two
   holds depends on how close to full speed a given driver runs on a given unit,
   which is not something this file can know. So it is a setting with the safe
   value as its default, rather than a guess baked in here. */
#define AUDIO_BUFFER_MAX 4

/* Set from the frontend's Settings; read when a stream starts, clamped here. */
int nx_audio_buffers = AUDIO_BUFFER_MAX;

static int audio_buffer_count(void)
{
	if (nx_audio_buffers < 2) return 2;
	if (nx_audio_buffers > AUDIO_BUFFER_MAX) return AUDIO_BUFFER_MAX;
	return nx_audio_buffers;
}

typedef struct nxAudioData
{
    AudioOutBuffer source_buffer[AUDIO_BUFFER_MAX];
    int count;                          /* how many are actually in use */
    AudioOutBuffer *released_buffer;    /* the one being filled right now */
    /* Released by audout and not yet refilled. audout hands back a linked list,
       not a single buffer, and it hands back everything that finished since the
       last poll - so on any frame that overran there are two or three. Without
       somewhere to keep the tail of that list, every buffer past the head was
       simply dropped: never re-appended, never seen again. They are not leaked
       memory (they live in source_buffer[] and are freed with it), they are
       leaked *queue depth*, which is worse, because it is silent and it only
       ever goes one way. Four buffers of cover becomes three, then two, then
       one, and at one there is no slack left at all - every long frame from
       then on is an underrun. That is the intermittent crackle on a heavy
       driver, and why it sets in after a while rather than immediately. */
    AudioOutBuffer *free_list;
    uint32_t released_count;

} nxAudioData;
 
static uint32_t audio_data_size()
{
      return (PREFILL_SAMPLES * BYTES_PER_FRAME);
}

static size_t audio_buffer_size(void *data)
{
      return ((MAX_SAMPLES_PER_FRAME * BYTES_PER_FRAME) + 0xfff) & ~0xfff;
}
 
nxAudioData *audioPtr = NULL;

/* Set from the flag mixer.c passes to osd_start_audio_stream(). */
static int s_is_stereo = 1;

/* How many samples mixer.c will have written into mix_buffer on the next call.
   mixer_sh_update() fills exactly as many as we last returned (mixer.c:763 and
   :782 loop to samples_this_frame, assigned from our return at :806), and
   mix_buffer is a persistent static that is never cleared - so re-deriving the
   count instead of remembering it would replay stale samples from two frames
   back any time we returned a short count. */
static int s_pending_samples = 0;

/* Exact samples per frame, and the fraction of one left owing. Driver rates are
   not whole numbers - MK2 is 53.204950fps, so 48000/fps is 902.17 - and handing
   back the truncated 902 every frame runs the device 9 samples/sec short until
   the queue drains and clicks. Carry the remainder instead. */
static double s_samples_per_frame_f = 0.0;
static double s_sample_debt = 0.0;

/* How many samples to ask mixer.c for next frame: the running average tracks
   the driver's true rate, so the count alternates (902, 902, 902, 902, 902,
   903, ...) instead of drifting. */
static int next_frame_samples(void)
{
	int n;

	s_sample_debt += s_samples_per_frame_f;
	n = (int)s_sample_debt;
	s_sample_debt -= n;

	if (n > MAX_SAMPLES_PER_FRAME)
		n = MAX_SAMPLES_PER_FRAME;

	return n;
}

/* Moves a whole released chain onto the free list. `count` is authoritative:
   the last node's next is not guaranteed to be NULL. */
static void audio_push_released(AudioOutBuffer *list, u32 count)
{
	while (list && count-- > 0)
	{
		AudioOutBuffer *next = list->next;

		list->next = audioPtr->free_list;
		audioPtr->free_list = list;
		list = next;
	}
}

/* Everything audout has finished with since the last call. */
static void audio_collect_released(void)
{
	AudioOutBuffer *list = NULL;
	u32 count = 0;

	if (R_FAILED(audoutGetReleasedAudioOutBuffer(&list, &count)))
		return;
	audio_push_released(list, count);
}

static AudioOutBuffer *audio_take_free(void)
{
	AudioOutBuffer *buf = audioPtr->free_list;

	if (buf)
	{
		audioPtr->free_list = buf->next;
		buf->next = NULL;
	}
	return buf;
}

int osd_start_audio_stream(int stereo)
{

	if (Machine->sample_rate == 0) return 0;

	/* determine the number of samples per frame */
	samples_per_frame = Machine->sample_rate / Machine->drv->frames_per_second;
	s_samples_per_frame_f = (double)Machine->sample_rate / Machine->drv->frames_per_second;
	s_sample_debt = 0.0;
	s_pending_samples = samples_per_frame;
	nx_pause_sound_init(Machine->sample_rate);

	/* sound_start() (sndintrf.c:1241-1260) can fail AFTER mixer_sh_start() has
	   already brought us up, and it returns without calling mixer_sh_stop() -
	   its own comment admits the leak. audioPtr would then still be set on the
	   next launch, the buffer-append below would be skipped, and the new audout
	   session would play from an empty queue forever. Tear down first so this
	   is idempotent. */
	if (audioPtr)
		osd_stop_audio_stream();

	audoutInitialize();
	audoutStartAudioOut();

	/* mixer.c hands us one INT16 per sample for a mono game and two for a
	   stereo one - see the is_stereo branch in mixer_update_audio(). audout
	   is always 2ch, so a mono game has to be expanded on the way out. */
	s_is_stereo = stereo;

	/* audout is fixed 48kHz/2ch/PCM16 on every Switch. Machine->sample_rate
	   comes from options.samplerate, which nx_maintest.cpp sets to 48000 - if
	   those ever diverge, everything downstream plays at the wrong pitch. */
	if (audoutGetSampleRate() != (u32)Machine->sample_rate)
		log_cb(RETRO_LOG_WARN, LOGPRE "audout is %luHz but MAME is running at %dHz - pitch will be wrong\n",
			(unsigned long)audoutGetSampleRate(), Machine->sample_rate);

	if (!audioPtr)
	{
		audioPtr = (struct nxAudioData *)calloc(1,sizeof(*audioPtr));
	
		audioPtr->count = audio_buffer_count();

		for (int i = 0; i < audioPtr->count; i++) {
			  
			audioPtr->source_buffer[i].next = NULL;
			audioPtr->source_buffer[i].buffer = aligned_alloc(0x1000, audio_buffer_size(NULL));
			audioPtr->source_buffer[i].buffer_size = audio_buffer_size(NULL);
			audioPtr->source_buffer[i].data_size = audio_data_size();
			audioPtr->source_buffer[i].data_offset = 0;
			memset(audioPtr->source_buffer[i].buffer,0x00,audio_buffer_size(NULL));
			audoutAppendAudioOutBuffer(&audioPtr->source_buffer[i]);
		}	
	
		audioPtr->released_buffer = NULL;
	}
	
	return samples_per_frame;


}

int osd_update_audio_stream(INT16 *buffer)
{	 
	 
	/* Only this many samples of `buffer` are fresh - see s_pending_samples. */
	int samplerate_buffer_size = s_pending_samples ? s_pending_samples : samples_per_frame;



	if (!audioPtr->released_buffer)
    {
		audio_collect_released();
		audioPtr->released_buffer = audio_take_free();

		if (!audioPtr->released_buffer)
		{
			/* Bounded: audoutWaitPlayFinish() failing repeatedly used to spin
			   here forever with an empty error branch, hanging the emulator
			   with no way out. Give up and drop this frame's audio instead. */
			int tries = 0;

			while (audioPtr->released_buffer == NULL && tries++ < 16)
			{
				AudioOutBuffer *list = NULL;
				u32 count = 0;

				if (R_FAILED(audoutWaitPlayFinish(&list, &count, UINT64_MAX)))
					break;
				audio_push_released(list, count);
				audioPtr->released_buffer = audio_take_free();
			}

			/* Dropping this frame's audio still has to answer with the next
			   frame's request, or mixer.c refills a different count than the
			   one we read back at the top. */
			if (audioPtr->released_buffer == NULL)
			{
				s_pending_samples = next_frame_samples();
				return s_pending_samples;
			}


		}

		audioPtr->released_buffer->data_size = 0;

	}
	
	
	/* samplerate_buffer_size counts stereo samples; data_size and the buffer
	   size count bytes. This compared the two directly, so the clamp was
	   meaningless - it only ever held because a frame happens to fit. */
	{
		size_t free_bytes = audio_buffer_size(NULL) - audioPtr->released_buffer->data_size;

		if ((size_t)samplerate_buffer_size * BYTES_PER_FRAME > free_bytes)
			samplerate_buffer_size = (int)(free_bytes / BYTES_PER_FRAME);
	}

	{
		uint8_t *dst = (uint8_t *)(audioPtr->released_buffer->buffer) +
					   audioPtr->released_buffer->data_size;

		if (s_is_stereo)
		{
			memcpy(dst, buffer, samplerate_buffer_size * BYTES_PER_FRAME);
		}
		else
		{
			/* Mono source: mixer.c wrote one INT16 per sample, so copying
			   BYTES_PER_FRAME per sample would read twice the buffer and
			   interleave consecutive samples as L/R. Duplicate instead. */
			INT16 *out = (INT16 *)dst;
			int i;

			for (i = 0; i < samplerate_buffer_size; i++)
			{
				out[i * 2 + 0] = buffer[i];
				out[i * 2 + 1] = buffer[i];
			}
		}
	}
	/* Frontend tones are mixed into the game's own PCM rather than submitted
	   through a second audout session. Only one thing ever owns the stream, so
	   there is no buffer-ownership question at all - which two separate
	   sessions could not avoid. dst is interleaved stereo on both the mono and
	   stereo paths above, which is what mix() expects. */
	nx_pause_sound_mix((INT16 *)((uint8_t *)(audioPtr->released_buffer->buffer) +
	                             audioPtr->released_buffer->data_size),
	                   samplerate_buffer_size);

	audioPtr->released_buffer->data_size += samplerate_buffer_size * BYTES_PER_FRAME;
	audioPtr->released_buffer->buffer_size = audio_buffer_size(NULL);

	/* Submit once a full frame is buffered. The old threshold was 48000/1000
	   = 48 bytes, i.e. 12 samples - it queued sub-millisecond fragments. */
	if (audioPtr->released_buffer->data_size >= (size_t)samples_per_frame * BYTES_PER_FRAME)
    {
		audoutAppendAudioOutBuffer(audioPtr->released_buffer);
		audioPtr->released_buffer = NULL;
	}

	/* Whatever we return is what mixer.c fills next frame (mixer.c:806), so it
	   is the request for the NEXT frame, not a report of what we just took. */
	s_pending_samples = next_frame_samples();
	return s_pending_samples;
}



//---------------------------------------------------------------------
//	osd_update_silent_stream - updatescreen() calls this instead of
//	sound_update() while paused, to keep the audio ring buffer fed
//	(same path as osd_update_audio_stream, just silence).
//---------------------------------------------------------------------
void osd_update_silent_stream(void)
{
	static INT16 silence[MAX_SAMPLES_PER_FRAME * CHANNELCOUNT];

	if (!audioPtr) return;

	osd_update_audio_stream(silence);
}

/* While the pause overlay is up, mame_frame() is not running, so nothing calls
   osd_update_audio_stream() and a queued tone would never reach the speakers.
   Refills whatever audout has handed back, using MAME's own buffers - the
   frontend never owns one. */
void nx_pause_sound_update(void)
{
	AudioOutBuffer *out;

	if (!audioPtr)
		return;

	/* Through the same free list the game path uses. Polling audout directly
	   here would step past anything already collected and left waiting, and
	   those buffers would sit out the whole time the overlay is open. */
	audio_collect_released();

	while ((out = audio_take_free()) != NULL)
	{
		INT16 *dest = (INT16 *)out->buffer;
		int max_frames = (int)(audio_buffer_size(NULL) / BYTES_PER_FRAME);
		int frames;

		memset(dest, 0, audio_buffer_size(NULL));
		frames = nx_pause_sound_render(dest, max_frames);
		if (frames <= 0)
			frames = samples_per_frame;

		out->next = NULL;
		out->data_offset = 0;
		out->data_size = (size_t)frames * BYTES_PER_FRAME;
		out->buffer_size = audio_buffer_size(NULL);

		if (R_FAILED(audoutAppendAudioOutBuffer(out)))
		{
			/* Back on the list rather than dropped, or a failed append would
			   cost a buffer permanently - the exact thing this list exists to
			   stop. */
			out->next = audioPtr->free_list;
			audioPtr->free_list = out;
			break;
		}
	}
}

void osd_stop_audio_stream(void)
{
	nx_pause_sound_reset();

	if (audioPtr)
	{
		 
		audoutStopAudioOut();
		audoutExit();

		for (int i = 0; i < audioPtr->count; i++)
			  free(audioPtr->source_buffer[i].buffer);

		free(audioPtr);
		audioPtr = NULL;
	}		
}

//---------------------------------------------------------------------
//	osd_set_mastervolume
//---------------------------------------------------------------------
void osd_set_mastervolume( INT32 attenuation )
{
  
}

//---------------------------------------------------------------------
//	osd_get_mastervolume
//---------------------------------------------------------------------
INT32 osd_get_mastervolume( void )
{
	return 0;
}

//---------------------------------------------------------------------
//	osd_sound_enable
//---------------------------------------------------------------------
void osd_sound_enable( INT32 enable )
{
 
}
 