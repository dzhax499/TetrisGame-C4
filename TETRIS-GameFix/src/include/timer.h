// Nama file : timer.h
// Deskripsi : Header untuk pengelolaan timer dalam permainan Tetris
// Oleh      : Ibnu Hilmi 241511079

#ifndef TIMER_H
#define TIMER_H

#include <raylib.h>
#include <stdbool.h>

void InitGameTimer(void);
void UpdateGameTimer(void);
float GetElapsedGameTime(void);
void PauseGameTimer(void);
void ResumeGameTimer(void);
bool IsTimerPaused(void);

#endif // TIMER_H
