// Nama file : board.h
// Deskripsi : Header untuk logika papan permainan Tetris (Array 2D)
// Oleh      : Ibnu Hilmi 241511079
//             Rizky Satria Gunawan

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
    TetrisBlock current_block; // Ganti dari CurrentBlock ke TetrisBlock
    TetrisBlock next_block;    // Tambahkan untuk blok berikutnya
    int current_score;
    int current_level;
    int lines_cleared;
    bool game_over;
} TetrisBoard;


TetrisBoard *board;

// Fungsi inisialisasi papan permainan
void InitBoard1(TetrisBoard* board);

// Fungsi untuk menghapus baris yang penuh
int ClearFullLines(TetrisBoard* board);

// Fungsi untuk memeriksa game over
bool IsGameOver(TetrisBlock *block, TetrisBoard *board);

// Fungsi utilitas
Color GetBlockColor(BlockType block);
BlockType GetRandomBlock();

// Fungsi debug
void PrintBoard(TetrisBoard* board);

#endif // BOARD_H