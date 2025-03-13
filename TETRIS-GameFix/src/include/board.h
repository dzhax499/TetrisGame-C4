// Nama file : board.h
// Deskripsi : Header untuk logika papan permainan Tetris (Array 2D)
// Oleh      : Ibnu Hilmi 241511079

#ifndef BOARD_H
#define BOARD_H

#include <stdbool.h>

// Konstanta ukuran papan
#define BOARD_WIDTH 10
#define BOARD_HEIGHT 20

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

// Struktur untuk menyimpan informasi papan permainan
typedef struct {
    BlockType grid[BOARD_HEIGHT][BOARD_WIDTH];
    int current_score;
    int current_level;
    int lines_cleared;
} TetrisBoard;

// Fungsi inisialisasi papan permainan
void InitBoard(TetrisBoard* board);

// Fungsi untuk memeriksa apakah posisi valid
bool IsValidPosition(TetrisBoard* board, BlockType block, int row, int col, int rotation);

// Fungsi untuk menempatkan blok di papan
void PlaceBlock(TetrisBoard* board, BlockType block, int row, int col, int rotation);

// Fungsi untuk menghapus baris yang penuh
int ClearFullLines(TetrisBoard* board);

// Fungsi untuk mengecek apakah permainan berakhir
bool IsGameOver(TetrisBoard* board);

// Fungsi untuk mendapatkan warna blok
Color GetBlockColor(BlockType block);

#endif // BOARD_H