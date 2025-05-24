// Nama file : timer.c
// Deskripsi : Implementasi pengelolaan timer dalam permainan Tetris
// Oleh      : Ibnu Hilmi 241511079

#include "include/timer.h"
#include <stdlib.h>

static float startTime = 0.0f;
static float elapsedTime = 0.0f;

void InitGameTimer(void) {
    startTime = GetTime();
    elapsedTime = 0.0f;
}

void UpdateGameTimer(void) {
    elapsedTime = GetTime() - startTime;
}

float GetElapsedGameTime(void) {
    return elapsedTime;
}
