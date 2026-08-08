 #include<switch.h>
#include <stdarg.h>
#include <stdio.h>
#include <string.h>
// Include the only file
#include "CustomUI.h"
#include "nx_RomList.h"



extern "C" {
#include "osd_cpu.h"
#include "driver.h"
#include "mame.h"
#include "nx_mame.h"
#include "nx_perf.h"
#include "nx_pause_sound.h"
#include "nx_paths.h"

}

CRomList romList;

/* dedicated pad for the exit-to-browser hotkey below - separate from the
   per-player pads nx_joystick.c drives for MAME's own input, since this
   check runs regardless of what MAME's input system is doing. */
static PadState s_exitPad;

int main(int argc, char **argv)
{
	/* First, before anything opens a file: every data path in the app is built
	   from the folder this .nro was launched out of. */
	nx_paths_init(argc > 0 ? argv[0] : 0);

	//socketInitializeDefault();
	//pcvInitialize();

	/* Once, for the largest number of players anything in this app will ask
	   for - the four that nx_joystick.c drives. It used to be called three
	   times with three different counts (1 here, 2 in UI::Init, 4 when a game
	   started), so the console's supported-controller set was rewritten twice
	   more after the pads had already been opened against the first one. */
	padConfigureInput( 4, HidNpadStyleSet_NpadStandard );
	padInitializeDefault( &s_exitPad );

	nx_fileio_init();

	romList.InitRomList();
	romList.RefreshRomList();
	 
	UI::Init();
 
	options.tate_mode = 1;
	options.samplerate = 48000;

	options.brightness = 1.0f;
	options.pause_bright = 0.65f;
	options.gamma = 1.0f;
	options.color_depth = 0;
	
	/* Artwork is off by default on V1: every enabled layer is another
	   full-screen alpha blit per frame plus its own decoded bitmap in a
	   heap we are already short of. Re-enable from the pause menu. */
	options.use_artwork = ARTWORK_USE_NONE;
	options.artwork_res = 0;
	options.artwork_crop = false;

	nx_perf_init();

    while(appletMainLoop())
    {
		if (UI::gameRunning)
		{
			/* core no longer has a "return to frontend" concept of its own
			   (that was libretro/RA's job), so the pause chord below is read
			   outside MAME entirely and drives the frontend's own overlay. */
			padUpdate( &s_exitPad );
			u64 kHeld = padGetButtons( &s_exitPad );
			u64 kDown = padGetButtonsDown( &s_exitPad );

			/* The frontend overlay owns the frame while it is open: no CPU
			   timeslice runs, and UI::Loop() paints itself over the last game
			   frame that nx_display left on screen. */
			if (UI::OverlayOpen())
			{
				UI::Loop();
				/* mame_frame() is not running, so nothing else is refilling
				   audout - without this the overlay's tones are queued and
				   never played. */
				nx_pause_sound_update();
				Gfx::flushOverlay();
				continue;
			}

			/* Pause is Plus+Minus, edge-triggered on whichever of the two
			   completes the chord. The previous stick gestures are gone: a
			   click registers on the frame it goes down, so pressing L Stick
			   then R Stick opened the overlay on the way into the two-stick
			   chord - one gesture doing two things. A chord of two buttons
			   nobody holds together during play has no such halfway state. */
			if ((kHeld & HidNpadButton_Plus) && (kHeld & HidNpadButton_Minus) &&
				(kDown & (HidNpadButton_Plus | HidNpadButton_Minus)))
			{
				UI::OpenOverlay();
				Gfx::flushOverlay();
				continue;
			}

			/* Motors are level-triggered: a pulse started in the overlay keeps
			   running until something sends the stop packet, and UI::Loop -
			   which used to be the only caller - stops running the moment the
			   overlay closes. Tick it here too so a buzz can never outlive the
			   menu that started it. */
			UI::TickHaptics();

			/* Leaving is the overlay's "Return to Game Library" row, which can
			   only raise a flag because it runs outside the teardown point. */
			if (nx_exit_to_menu)
			{
				nx_exit_to_menu = 0;
				mame_done();
				nx_perf_game_stop();
				UI::gameRunning = false;
				romList.RefreshRomList();
				/* Refresh(), not Draw(): the library view holds indices into
				   m_vecAvailRomList, which RefreshRomList() just rebuilt. */
				UI::Refresh();
				/* Refresh() leaves the library drawn but unpresented, which is
				   exactly what the fade needs - it snapshots that and walks the
				   darkness off it. Presents every frame, so no flush here. */
				UI::TransitionIn();
			}
			else
			{
				/* Decide the frameskip for this frame before running it -
				   mame_frame() reads that decision back through
				   osd_skip_this_frame() while it works. */
				nx_perf_frame_begin();
				mame_frame();
				nx_perf_frame_end();
			}
		}
		else
		{
			UI::Loop();
			Gfx::flush();
		}

		/* Gfx::flush() presents the browser; osd_update_video_and_audio()
		   presents the game. Exactly one of them may run per iteration - when
		   both did, the display alternated between the two images every frame
		   and strobed the whole screen at 60Hz. */
    }
 
    nx_perf_exit();
    UI::Exit();
	
    return 0;
}

/* Filled by nx_audit_printf() while VerifyRomSet() runs; read straight after,
   on the same thread, so no locking is needed. */
extern "C" { char g_auditDetail[256]; }

extern "C" void CLIB_DECL nx_audit_printf(const char *fmt, ...)
{
	va_list ap;
	size_t used = strlen(g_auditDetail);

	/* Keep the first lines and drop the rest - a bad set can report dozens of
	   files, and only the first few fit on screen anyway. */
	if (used + 1 >= sizeof(g_auditDetail))
		return;

	va_start(ap, fmt);
	vsnprintf(&g_auditDetail[used], sizeof(g_auditDetail) - used, fmt, ap);
	va_end(ap);
}

extern "C" void RenderMessage(char *name)
{
	/* On a glass plate rather than straight onto the splash. The message is
	   usually bad news - a missing BIOS, a set that will not fit - and it used
	   to be white text laid directly over whatever the bundled logo happens to
	   be, which is legible only by luck. */
	Gfx::drawBgImage();
	UI::Glass(120, 300, 1040, 120, 20, UI::kPanel, UI::kPanelBot, UI::kEdge);
	Gfx::drawTextWrapped(160, 330, name, UI::kText, 20, 960, 3);
	Gfx::flush();
}


extern "C" void RenderProgress(const char *name, struct rom_load_data *romdata)
{
 
	char title[128];

	Gfx::drawBgImage();

	if( name )
	{
		sprintf( title, "Loading \"%s\"",name );		 
		sprintf( &title[strlen(title)], " (%d/ %d)", romdata->romsloaded, romdata->romstotal );
	}
	else
		strcpy( title, "Loading complete!" );
 	
	
	Gfx::drawText(420,360, title,  { 255, 255, 255, 255 }, 20);
	Gfx::flush();
 	
}