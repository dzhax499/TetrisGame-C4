#ifndef TIMER_H
#define TIMER_H

#include <stdint.h>

// Struktur data timer
typedef struct {
    uint32_t startTime;
    uint32_t elapsedTime;
    bool isPaused;
} GameTimer;

// Inisialisasi timer
void initTimer(GameTimer *timer);

// Update waktu saat game berjalan
void updateTimer(GameTimer *timer, uint32_t currentTime);

// Pause dan resume timer
void pauseTimer(GameTimer *timer);
void resumeTimer(GameTimer *timer);

// Mendapatkan waktu yang sudah berjalan
uint32_t getElapsedTime(GameTimer *timer);

#endif
