// sfx.c
#include "sfx.h"
#include <stdio.h>

void play_sfx(const char *file) {
    Mix_Chunk *sfx = Mix_LoadWAV(file);
    if (!sfx) {
        printf("Failed to load SFX: %s\n", Mix_GetError());
        return;
    }
    Mix_PlayChannel(-1, sfx, 0);
    Mix_FreeChunk(sfx);
}