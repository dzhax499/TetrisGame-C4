// Nama file : tetris.h
// Deskripsi : Header untuk game Tetris
// Oleh      : Dzakit Tsabit 241511071

#ifdef TETRIS_H
#define TETRIS_H

#include <stdbool.h>
#include "raylib.h"

// Definisi ukuran papan permainan
#define BOARD_WIDTH  10
#define BOARD_HEIGHT 20

// Definisi ukuran window
#define WINDOW_WIDTH  800
#define WINDOW_HEIGHT 600

typedef struct {
    int grid[BOARD_HEIGHT][BOARD_WIDTH]; // 0 kosong, 1-7 warna blok
} TetrisBoard;

typedef struct {
    int x, y;
    int rotation;
    int type;
} Tetromino;


#endif