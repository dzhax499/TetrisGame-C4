// Nama file : timer.c
// Deskripsi : Implementasi pengelolaan timer dalam permainan Tetris
// Oleh      : Ibnu Hilmi 241511079

#include "include/timer.h"
#include <stdlib.h>

static float startTime = 0.0f;
static float elapsedTime = 0.0f;
static float pauseStartTime = 0.0f;
static float totalPausedDuration = 0.0f;
static bool isPaused = false;

void InitGameTimer(void) {
    startTime = GetTime();
    elapsedTime = 0.0f;
    pauseStartTime = 0.0f;
    totalPausedDuration = 0.0f;
    isPaused = false;
}

void UpdateGameTimer(void) {
    if (!isPaused) {
        elapsedTime = GetTime() - startTime - totalPausedDuration;
    }
}

float GetElapsedGameTime(void) {
    return elapsedTime;
}

void PauseGameTimer(void) {
    if (!isPaused) {
        pauseStartTime = GetTime();
        isPaused = true;
    }
}

void ResumeGameTimer(void) {
    if (isPaused) {
        totalPausedDuration += GetTime() - pauseStartTime;
        isPaused = false;
    }
}

bool IsTimerPaused(void) {
    return isPaused;
}
