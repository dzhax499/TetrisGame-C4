//Nama file : blocks.c
//Deskripsi : Implementasi blok-blok game Tetris
//Oleh      : Dzakit Tsabit 241511071

#include "include/blocks.h"
#include <stdlib.h>
#include <stdio.h>
#include "raylib.h"

// Warna untuk setiap jenis blok Tetris
static const Color TETROMINO_COLORS[7] = {
    {0, 255, 255, 255},   // Cyan (I)
    {0, 0, 255, 255},     // Blue (J)
    {255, 165, 0, 255},   // Orange (L)
    {255, 255, 0, 255},   // Yellow (O)
    {0, 255, 0, 255},     // Green (S)
    {128, 0, 128, 255},   // Purple (T)
    {255, 0, 0, 255}      // Red (Z)
};

void InitBlocks0(void) {
    // Inisialisasi random seed
    srand(time(NULL));
}

TetrisBlock GenerateRandomBlock(void) {
    TetrisBlock block;
    block.type = rand() % 7;
    block.rotation = 0;
    block.x = BOARD_WIDTH / 2 - 2;
    block.y = 0;
    block.color = TETROMINO_COLORS[block.type];
    return block;
}

bool IsBlockCollision(TetrisBlock *block, TetrisBoard *board, int testX, int testY, int testRotation) {
    for (int y = 0; y < 4; y++) {
        for (int x = 0; x < 4; x++) {
            if (TETROMINO_SHAPES[block->type][testRotation][y][x] == 0) continue;
            int boardX = testX + x;
            int boardY = testY + y;
            if (boardX < 0 || boardX >= BOARD_WIDTH || boardY >= BOARD_HEIGHT || 
                (boardY >= 0 && board->grid[boardY][boardX] != BLOCK_EMPTY)) {
                return true;
            }
        }
    }
    return false;
}

int CalculateDropDistance(TetrisBlock *block, TetrisBoard *board) {
    int dropDistance = 0;

    // Simulasikan pergerakan blok ke bawah satu per satu
    while (IsValidBlockPosition(block, board, block->x, block->y + dropDistance + 1, block->rotation)) {
        dropDistance++;
    }

    return dropDistance;
}

bool IsValidBlockPosition(TetrisBlock *block, TetrisBoard *board, int testX, int testY, int testRotation) {
    for (int y = 0; y < 4; y++) {
        for (int x = 0; x < 4; x++) {
            if (TETROMINO_SHAPES[block->type][testRotation][y][x] == 0) continue;
            int boardX = testX + x;
            int boardY = testY + y;
            if (boardX < 0 || boardX >= BOARD_WIDTH || boardY >= BOARD_HEIGHT || 
                (boardY >= 0 && board->grid[boardY][boardX] != BLOCK_EMPTY)) {
                return false;
            }
        }
    }
    return true;
}

bool RotateBlock(TetrisBlock *block, TetrisBoard *board) {
    int newRotation = (block->rotation + 1) % 4;
    if (IsValidBlockPosition(block, board, block->x, block->y, newRotation)) {
        block->rotation = newRotation;
        return true;
    }
    return false;
}

bool MoveBlockHorizontal(TetrisBlock *block, TetrisBoard *board, int dx) {
    int newX = block->x + dx;
    if (IsValidBlockPosition(block, board, newX, block->y, block->rotation)) {
        block->x = newX;
        return true;
    }
    return false;
}

bool MoveBlockDown(TetrisBlock *block, TetrisBoard *board) {
    int newY = block->y + 1;
    if (IsValidBlockPosition(block, board, block->x, newY, block->rotation)) {
        block->y = newY;
        return true;
    }
    return false;
}

void HardDropBlock(TetrisBlock *block, TetrisBoard *board) {
    while (MoveBlockDown(block, board)) {}
    PlaceBlock(block, board);
}

void PlaceBlock(TetrisBlock *block, TetrisBoard *board) {
    for (int y = 0; y < 4; y++) {
        for (int x = 0; x < 4; x++) {
            if (TETROMINO_SHAPES[block->type][block->rotation][y][x] != 0) {
                int boardX = block->x + x;
                int boardY = block->y + y;
                if (boardY >= 0 && boardY < BOARD_HEIGHT && boardX >= 0 && boardX < BOARD_WIDTH) {
                    board->grid[boardY][boardX] = block->type;
                }
            }
        }
    }
}