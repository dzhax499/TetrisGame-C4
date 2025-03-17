// Nama program : game.h
// Deskripsi    : header untuk pengelolaan logika utama permainan 
// Pembuat      : [Rizky Satria Gunawan, 241511089, 24 February 2025, 11:01]

#ifndef GAME_H
#define GAME_H

#include <SDL3/SDL.h> // Mengimpor SDL3
#include "../modes/level.h"
#include "../modes/endless.h"

#define BOARD_WIDTH 10
#define BOARD_HEIGHT 20
#define BLOCK_SIZE 30 // Ukuran blok dalam piksel

extern int board[BOARD_HEIGHT][BOARD_WIDTH];

// Struktur untuk menyimpan state game
typedef struct {
    SDL_Window *window;
    SDL_Renderer *renderer;
    Settings settings;
} GameState;

void init_board();
void display_board(SDL_Renderer *renderer);
void update_game(GameState *game);
void run_game();

#endif // GAME_H