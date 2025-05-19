// Nama file : board.h
// Deskripsi : Header untuk logika papan permainan Tetris (Array 2D)
// Oleh      : Ibnu Hilmi 241511079
//             Rizky Satria Gunawan

#ifndef BOARD_H
#define BOARD_H

#include "raylib.h"
#include "tetris.h"
#include "scoring.h"
#include <stdbool.h>

#define BOARD_WIDTH 10
#define BOARD_HEIGHT 20
#define BLOCK_SIZE 30
#define BOARD_OFFSET_X 250
#define BOARD_OFFSET_Y 50

typedef enum {
    BLOCK_EMPTY = 0,
    BLOCK_I, BLOCK_J, BLOCK_L, BLOCK_O, BLOCK_S, BLOCK_T, BLOCK_Z
} BlockType;

typedef struct {
    BlockType type;
    int x;
    int y;
    int rotation;
} CurrentBlock;

// Node untuk circular linked list
typedef struct BlockNode {
    TetrisBlock block;
    struct BlockNode* next;
} BlockNode;

// Satu definisi konsisten untuk TetrisBoard
typedef struct {
    BlockType grid[BOARD_HEIGHT][BOARD_WIDTH];
    TetrisBlock current_block;
    BlockNode* next_blocks;  // Circular linked list untuk next blocks
    TetrisBlock next_block;
    HoldBlock hold_block;
    int current_score;
    int current_level;
    int lines_cleared;
    bool game_over;
    ScoreData score_data;
} TetrisBoard;

extern TetrisBoard *board;

// Fungsi
void InitBoard(TetrisBoard* board);
int ClearFullLines(TetrisBoard* board);
bool IsGameOver(TetrisBlock *block, TetrisBoard *board);
Color GetBlockColor(BlockType block);
void PrintBoard(TetrisBoard* board);
void AddNextBlock(TetrisBoard* board, TetrisBlock newBlock);
TetrisBlock PopNextBlock(TetrisBoard* board);
void PrintNextBlocks(TetrisBoard* board);

#endif // BOARD_H