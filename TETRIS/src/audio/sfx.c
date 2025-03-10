// sfx.c
#include "sfx.h"
#include <SDL3/SDL_mixer.h>
#include <stdio.h>

// Variabel statis untuk menyimpan efek suara
static Mix_Chunk *soundEffect = NULL;

// Memuat efek suara dari file
void LoadSFX(const char *filename) {
    if (soundEffect) {
        Mix_FreeChunk(soundEffect);
    }
    soundEffect = Mix_LoadWAV(filename);
    if (!soundEffect) {
        printf("Failed to load sound effect: %s\n", Mix_GetError());
    }
}

// Memainkan efek suara yang telah dimuat
void PlaySFX() {
    if (soundEffect) {
        Mix_PlayChannel(-1, soundEffect, 0);
    }
}

// Membersihkan efek suara sebelum keluar
void FreeSFX() {
    if (soundEffect) {
        Mix_FreeChunk(soundEffect);
        soundEffect = NULL;
    }
}