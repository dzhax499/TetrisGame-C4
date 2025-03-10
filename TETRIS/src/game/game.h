// Nama program : game.h
// Deskripsi    : header untuk pengelolaan logika utama permainan 
// Pembuat      : [Rizky Satria Gunawan, 241511089, 24 February 2025, 11:01]

#ifndef GAME_H
#define GAME_H

#include "../modes/level.h"
#include "../modes/endless.h"

#define BOARD_WIDTH 10
#define BOARD_HEIGHT 20

extern int board[BOARD_HEIGHT][BOARD_WIDTH];

void init_board();
void display_board();
void update_game(Settings *settings);
void run_game();

#endif // GAME_H