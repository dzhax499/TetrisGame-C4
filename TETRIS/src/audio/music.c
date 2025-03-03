// music.c
#include "music.h"
#include <stdio.h>

static Mix_Music *bg_music = NULL;

void play_music(const char *file) {
    if (bg_music) {
        Mix_FreeMusic(bg_music);
    }
    bg_music = Mix_LoadMUS(file);
    if (!bg_music) {
        printf("Failed to load music: %s\n", Mix_GetError());
        return;
    }
    Mix_PlayMusic(bg_music, -1);
}

void stop_music() {
    Mix_HaltMusic();
}