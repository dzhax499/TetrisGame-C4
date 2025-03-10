// music.c
#include "music.h"
#include <SDL3/SDL_mixer.h>
#include <stdio.h>

// Variabel statis untuk menyimpan musik latar
static Mix_Music *bgMusic = NULL;

// Memuat dan memainkan musik latar dari file
void PlayMusic(const char *filename) {
    if (bgMusic) {
        Mix_FreeMusic(bgMusic);
    }
    bgMusic = Mix_LoadMUS(filename);
    if (!bgMusic) {
        printf("Failed to load music: %s\n", Mix_GetError());
        return;
    }
    Mix_PlayMusic(bgMusic, -1); // Loop musik tanpa henti
}

// Menjeda musik yang sedang dimainkan
void PauseMusic() {
    Mix_PauseMusic();
}

// Menghentikan dan membebaskan musik yang sedang dimainkan
void StopMusic() {
    Mix_HaltMusic();
    if (bgMusic) {
        Mix_FreeMusic(bgMusic);
        bgMusic = NULL;
    }
}