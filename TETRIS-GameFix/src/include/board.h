#ifndef BOARD_H
#define BOARD_H

#include "raylib.h"
#include "tetris.h"
#include "scoring.h"
#include <stdbool.h>

// Konstanta ukuran papan
#define BOARD_WIDTH 10
#define BOARD_HEIGHT 20
#define BLOCK_SIZE 30
#define BOARD_OFFSET_X 250
#define BOARD_OFFSET_Y 50
#define NEXT_BLOCKS_COUNT 3 // Jumlah blok berikutnya yang ditampilkan

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

// Struktur node untuk next_blocks (doubly linked list)
typedef struct NextBlockNode {
    BlockType type;
    struct NextBlockNode* next;
    struct NextBlockNode* prev;
} NextBlockNode;

// Struktur node untuk riwayat hold (doubly linked list)
typedef struct HoldHistoryNode {
    BlockType type;
    struct HoldHistoryNode* next;
    struct HoldHistoryNode* prev;
} HoldHistoryNode;

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
    TetrisBlock current_block;
    NextBlockNode* next_blocks; // Linked list untuk blok berikutnya
    HoldHistoryNode* hold_history; // Riwayat blok yang di-hold
    bool hasHeld;
    int current_score;
    int current_level;
    int lines_cleared;
    bool game_over;
    ScoreData score_data;
} TetrisBoard;

extern TetrisBoard *board;

// Fungsi inisialisasi papan permainan
void InitBoard(TetrisBoard* board);

// Fungsi untuk menghapus baris yang penuh
int ClearFullLines(TetrisBoard* board);

// Fungsi untuk memeriksa game over
bool IsGameOver(TetrisBlock *block, TetrisBoard *board);

// Fungsi utilitas
Color GetBlockColor(BlockType block);

// Fungsi untuk mengelola next_blocks (linked list)
void InitNextBlocks(TetrisBoard* board, int count);
BlockType GetNextBlock(TetrisBoard* board);
void AddNextBlock(TetrisBoard* board, BlockType type);

// Fungsi untuk mengelola hold_history (linked list)
void HoldCurrentBlock(TetrisBoard* board);
BlockType GetLastHoldBlock(TetrisBoard* board);

// Fungsi debug
void PrintBoard(TetrisBoard* board);

#endif // BOARD_H