// Nama file : board.h
// Deskripsi : Header untuk logika papan permainan Tetris (Array 2D)
// Oleh      : Ibnu Hilmi 241511079

#ifndef BOARD_H
#define BOARD_H

#include "raylib.h"
#include <stdbool.h>

// Konstanta ukuran papan
#define BOARD_WIDTH 10
#define BOARD_HEIGHT 20
#define BLOCK_SIZE 30
#define BOARD_OFFSET_X 100
#define BOARD_OFFSET_Y 50

// Tipe data untuk blok
typedef enum {
    BLOCK_EMPTY = 0,
    BLOCK_I,     // Cyan
    BLOCK_J,     // Blue
    BLOCK_L,     // Orange
    BLOCK_O,     // Yellow
    BLOCK_S,     // Green
    BLOCK_T,     // Purple
    BLOCK_Z      // Red
} BlockType;

// Struktur untuk menyimpan informasi blok yang sedang jatuh
typedef struct {
    BlockType type;
    int x;
    int y;
    int rotation;
} CurrentBlock;

// Struktur untuk menyimpan informasi papan permainan
typedef struct {
    BlockType grid[BOARD_HEIGHT][BOARD_WIDTH];
    CurrentBlock current_block;
    int current_score;
    int current_level;
    int lines_cleared;
    bool game_over;
} TetrisBoard;

// Fungsi inisialisasi papan permainan
void InitBoard(TetrisBoard* board);

// Fungsi untuk membuat blok baru
void GenerateNewBlock(TetrisBoard* board);

// Fungsi untuk menggerakkan blok
bool MoveBlockDown(TetrisBoard* board);
bool MoveBlockLeft(TetrisBoard* board);
bool MoveBlockRight(TetrisBoard* board);
bool RotateBlock(TetrisBoard* board);

// Fungsi untuk memeriksa apakah posisi valid
bool IsValidPosition(TetrisBoard* board, BlockType block, int row, int col, int rotation);

// Fungsi untuk menghapus baris yang penuh
int ClearFullLines(TetrisBoard* board);

// Fungsi rendering
void DrawBoard(TetrisBoard* board);
void DrawNextBlock(TetrisBoard* board);
void DrawScore(TetrisBoard* board);

// Fungsi utilitas
Color GetBlockColor(BlockType block);
BlockType GetRandomBlock();

// Fungsi debug
void PrintBoard(TetrisBoard* board);

#endif // BOARD_H