// Nama file : blocks.h
// Deskripsi : Header untuk blok-blok game Tetris  
// Oleh      : Dzakit Tsabit 241511071 (github : dzhax4499)
#ifndef BLOCKS_H
#define BLOCKS_H

#include "tetris.h"
#include "board.h"
#include <stdlib.h>
#include <stdbool.h>

// Inisialisasi sistem blok
void InitBlocks(void);


// Fungsi untuk menghasilkan blok acak
TetrisBlock GenerateRandomBlock(void);


// Fungsi untuk memeriksa posisi blok
bool IsValidBlockPosition(TetrisBlock *block, TetrisBoard *board, int testX, int testY, int testRotation);

// Fungsi untuk memutar blok
bool RotateBlock(TetrisBlock *block, TetrisBoard *board);

// Fungsi untuk menggerakkan blok secara horizontal
bool MoveBlockHorizontal(TetrisBlock *block, TetrisBoard *board, int dx);

// Fungsi untuk menggerakkan blok ke bawah
bool MoveBlockDown(TetrisBlock *block, TetrisBoard *board);

// Fungsi untuk hard drop (jatuh cepat)
void HardDropBlock(TetrisBlock *block, TetrisBoard *board);

void HoldCurrentBlock(TetrisBoard* board);

// Fungsi untuk menempatkan blok di papan
void PlaceBlock(TetrisBlock *block, TetrisBoard *board);

//harddrop hitung
int CalculateDropDistance(TetrisBlock *block, TetrisBoard *board);

bool RotateBlockWithWallKick(TetrisBlock *block, TetrisBoard *board);

void PrintBlockShape(TetrisBlock *block);
void ResetHoldCount(TetrisBoard *board);
int GetRemainingHolds(TetrisBoard *board);
bool CanUseHold(TetrisBoard *board);

#endif