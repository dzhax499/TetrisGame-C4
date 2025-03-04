// audio_manager.c
#include "audio_manager.h"
#include <stdio.h>

static int is_muted = 0;

void init_audio() {
    if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0) {
        printf("SDL_mixer error: %s\n", Mix_GetError());
    }
}

void set_volume(int volume) {
    if (!is_muted) {
        Mix_VolumeMusic(volume);
        Mix_Volume(-1, volume);
    }
}

void mute_audio(int mute) {
    is_muted = mute;
    Mix_VolumeMusic(mute ? 0 : MIX_MAX_VOLUME);
}

void close_audio() {
    Mix_CloseAudio();
}