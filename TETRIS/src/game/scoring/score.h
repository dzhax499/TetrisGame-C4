#ifndef SCORE_H
#define SCORE_H

#include "game_state.h"

void initScore(GameState *state);
void updateScore(GameState *state, int linesCleared);
void applyComboBonus(GameState *state);

#endif 
