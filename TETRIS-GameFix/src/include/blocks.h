// Nama file : blocks.h
// Deskripsi : Header untuk blok-blok game Tetris  
// Oleh      : Dzakit Tsabit 241511071

#ifndef BLOCKS_H
#define BLOCKS_H

#include "tetris.h"
#include <stdbool.h>

// init sistem blok
void initBlocks(void);

TetrisBlock GenerateRandomBlock(void);
bool IsValidBlockPosition(TetrisBlock *block, TetrisBoard *board, int testX, int testY, int testRotation);
bool RotateBlock(TetrisBlock *block, TetrisBoard *board);
bool MoveBlockHorizontal(TetrisBlock *block, TetrisBoard *board, int dx);
bool MoveBlockDown(TetrisBlock *block, TetrisBoard *board);
void HardDropBlock(TetrisBlock *block, TetrisBoard *board);
void PlaceBlock(TetrisBlock *block, TetrisBoard *board);
bool IsGameOver(TetrisBlock *block, TetrisBoard *board);


#endif