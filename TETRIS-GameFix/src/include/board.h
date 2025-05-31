// Nama file : board.h
// Deskripsi : Header untuk logika papan permainan Tetris (Doubly Linked List of Rows)
// Oleh      : Ibnu Hilmi 241511079
//             Rizky Satria Gunawan
// Modifikasi: Menggunakan Doubly Linked List untuk representasi baris papan

#ifndef BOARD_H
#define BOARD_H

#include "raylib.h"
#include "tetris.h" // Assuming tetris.h contains TetrisBlock and HoldBlock
#include "scoring.h"
#include <stdbool.h>

// Konstanta ukuran papan
#define BOARD_WIDTH 10
#define BOARD_HEIGHT 20 // This now represents the number of rows (nodes) in the linked list
#define BLOCK_SIZE 30
#define BOARD_OFFSET_X 250  // Geser ke tengah
#define BOARD_OFFSET_Y 50   // Tetap sama


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

// Struktur untuk satu baris dalam papan permainan (node dalam linked list)
typedef struct RowNode {
    BlockType columns[BOARD_WIDTH]; // Array untuk menyimpan blok dalam satu baris
    struct RowNode* prev;           // Pointer ke baris sebelumnya (atas)
    struct RowNode* next;           // Pointer ke baris berikutnya (bawah)
} RowNode;

// Struktur untuk menyimpan informasi papan permainan
typedef struct {
    RowNode* head_row;      // Pointer ke baris paling atas (indeks 0)
    RowNode* tail_row;      // Pointer ke baris paling bawah (indeks BOARD_HEIGHT - 1)
    // int      current_row_count; // Bisa ditambahkan jika tinggi papan dinamis, tapi kita jaga BOARD_HEIGHT
    TetrisBlock current_block;
    TetrisBlock next_block;
    HoldBlock hold_block;
    int current_score;
    int current_level;
    int lines_cleared;
    bool game_over;
    ScoreData score_data;
} TetrisBoard;


// Fungsi inisialisasi papan permainan
void InitBoard1(TetrisBoard* board);

// Fungsi untuk membebaskan memori yang dialokasikan untuk papan (linked list)
void FreeBoard(TetrisBoard* board);

// Fungsi untuk mendapatkan tipe blok pada posisi (row, col) tertentu
BlockType GetBlockAt(TetrisBoard* board, int r, int c);

// Fungsi untuk mengatur tipe blok pada posisi (row, col) tertentu
void SetBlockAt(TetrisBoard* board, int r, int c, BlockType type);

// Fungsi untuk menghapus baris yang penuh
int ClearFullLines(TetrisBoard* board);

// Fungsi untuk memeriksa game over
bool IsGameOver(TetrisBlock *block, TetrisBoard *board); // Signature might need adjustment based on block interaction

// Fungsi utilitas
Color GetBlockColor(BlockType block);

// Fungsi debug
void PrintBoard(TetrisBoard* board);

#endif // BOARD_H