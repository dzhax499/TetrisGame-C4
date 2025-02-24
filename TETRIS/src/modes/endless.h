// Nama program : scoretetris.h
// Deskripsi    : header untuk pengelolaan mode endless (tanpa batas), yang akan berhenti bila kondisi
//                tertentu terpenuhi.
// Pembuat      : [Rizky Satria Gunawan, 241511089, 24 February 2025, 8:40]

#ifndef ENDLESS_H
#define ENDLESS_H

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define BOARD_WIDTH 10
#define BOARD_HEIGHT 20

typedef struct {
    int shape;
    int x, y;
} Tetromino;

extern int board[BOARD_HEIGHT][BOARD_WIDTH];
extern Tetromino currentTetromino;

void initEndlessMode();
void spawnTetromino();
void updateGame();
int canMoveDown();
void placeTetromino();
void clearFullRows();
int checkGameOver();

#endif // ENDLESS_H