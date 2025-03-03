#ifndef TIMER_H
#define TIMER_H

#include "game_state.h"

void initTimer(GameState *state);
void updateTimer(GameState *state, float deltaTime);
void displayFormattedTime(float time);

#endif
