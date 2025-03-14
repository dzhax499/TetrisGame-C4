// Nama file : blocks.h
// Deskripsi : Header untuk blok-blok game Tetris  
// Oleh      : Dzakit Tsabit 241511071

#ifndef BLOCKS_H
#define BLOCKS_H

#include "tetris.h"
#include <stdbool.h>

// init sistem blok
void initBlocks(void);

// Fungsi management block
TetrisBlock GenerateBlock(void);
void RotateBlock(TetrisBlock* block);
void MoveBlock(TetrisBlock* block, int x, int y);
bool CheckCollision(TetrisBlock* block, int x, int y);
void LockBlock(TetrisBlock* block);
void ClearLines(void);
void DrawBlock(TetrisBlock* block);


#endif