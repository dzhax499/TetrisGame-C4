#include "timer.h"
#include <SDL2/SDL.h>  // Menggunakan SDL untuk waktu

void initTimer(GameTimer *timer) {
    timer->startTime = SDL_GetTicks();
    timer->elapsedTime = 0;
    timer->isPaused = false;
}

void updateTimer(GameTimer *timer, uint32_t currentTime) {
    if (!timer->isPaused) {
        timer->elapsedTime = currentTime - timer->startTime;
    }
}

void pauseTimer(GameTimer *timer) {
    timer->isPaused = true;
}

void resumeTimer(GameTimer *timer) {
    timer->isPaused = false;
    timer->startTime = SDL_GetTicks() - timer->elapsedTime;
}

uint32_t getElapsedTime(GameTimer *timer) {
    return timer->elapsedTime;
}
