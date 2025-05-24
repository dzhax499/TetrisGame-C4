#ifndef ROTASI_DATA_H
#define ROTASI_DATA_H

#include <stdlib.h>
#include <stdbool.h>

// Definisi bentuk-bentuk blok Tetris terpisah
extern const int I_TETROMINO[2][4][4];  // Blok I punya 2 posisi unik
extern const int J_TETROMINO[4][4][4];  // Blok J punya 4 posisi
extern const int L_TETROMINO[4][4][4];  // Blok L punya 4 posisi
extern const int O_TETROMINO[1][4][4];  // Blok O punya 1 posisi (tidak berubah)
extern const int S_TETROMINO[2][4][4];  // Blok S punya 2 posisi unik
extern const int T_TETROMINO[4][4][4];  // Blok T punya 4 posisi
extern const int Z_TETROMINO[2][4][4];  // Blok Z punya 2 posisi unik

extern const int ROTATION_COUNT[7];

#endif