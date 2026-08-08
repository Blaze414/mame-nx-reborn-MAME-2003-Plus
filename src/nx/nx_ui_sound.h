#pragma once

#ifdef __cplusplus
extern "C" {
#endif

enum nx_ui_sound_kind
{
	NX_UI_SOUND_NAVIGATE = 0,
	NX_UI_SOUND_CONFIRM,
	NX_UI_SOUND_BACK,
	NX_UI_SOUND_ERROR
};

/* Launcher-only audio. Failures are silent so UI and game launch remain usable. */
int nx_ui_sound_init(void);

int nx_ui_sound_play(int kind);
void nx_ui_sound_exit(void);

#ifdef __cplusplus
}
#endif
