#include "include/board.h"
#include "include/blocks.h"
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "raylib.h"

TetrisBoard *board = NULL;

Color GetBlockColor(BlockType block) {
    static const Color BLOCK_COLORS[] = {
        BLANK,      // 0 = Kosong
        SKYBLUE,    // 1 = I
        DARKBLUE,   // 2 = J
        ORANGE,     // 3 = L
        YELLOW,     // 4 = O
        LIME,       // 5 = S
        PURPLE,     // 6 = T
        RED         // 7 = Z
    };
    return (block >= BLOCK_EMPTY && block <= BLOCK_Z) ? BLOCK_COLORS[block] : GRAY;
}

// Inisialisasi papan permainan + next_blocks (linked list)
void InitBoard(TetrisBoard* board) {
    memset(board->grid, BLOCK_EMPTY, sizeof(board->grid));
    board->current_score = 0;
    board->current_level = 1;
    board->lines_cleared = 0;
    board->game_over = false;
    board->hasHeld = false;
    board->hold_history = NULL;
    
    // Inisialisasi next_blocks dengan 3 blok acak
    InitNextBlocks(board, NEXT_BLOCKS_COUNT);
    board->current_block = GenerateRandomBlock();
}

// Inisialisasi linked list next_blocks
void InitNextBlocks(TetrisBoard* board, int count) {
    NextBlockNode* head = NULL;
    NextBlockNode* tail = NULL;

    for (int i = 0; i < count; i++) {
        NextBlockNode* newNode = (NextBlockNode*)malloc(sizeof(NextBlockNode));
        newNode->type = GenerateRandomBlock().type;
        newNode->next = NULL;
        newNode->prev = tail;

        if (tail == NULL) {
            head = newNode;
        } else {
            tail->next = newNode;
        }
        tail = newNode;
    }
    board->next_blocks = head;
}

// Ambil blok berikutnya dari linked list
BlockType GetNextBlock(TetrisBoard* board) {
    if (board->next_blocks == NULL) return BLOCK_EMPTY;

    BlockType nextType = board->next_blocks->type;
    NextBlockNode* toRemove = board->next_blocks;
    board->next_blocks = board->next_blocks->next;

    // Tambahkan blok baru di akhir
    NextBlockNode* newNode = (NextBlockNode*)malloc(sizeof(NextBlockNode));
    newNode->type = GenerateRandomBlock().type;
    newNode->next = NULL;
    newNode->prev = NULL;

    if (board->next_blocks == NULL) {
        board->next_blocks = newNode;
    } else {
        NextBlockNode* current = board->next_blocks;
        while (current->next != NULL) {
            current = current->next;
        }
        current->next = newNode;
        newNode->prev = current;
    }

    free(toRemove);
    return nextType;
}

// Fungsi hold block dengan riwayat (doubly linked list)
void HoldCurrentBlock(TetrisBoard* board) {
    if (board->hasHeld) return;

    HoldHistoryNode* newNode = (HoldHistoryNode*)malloc(sizeof(HoldHistoryNode));
    newNode->type = board->current_block.type;
    newNode->next = board->hold_history;
    newNode->prev = NULL;

    if (board->hold_history != NULL) {
        board->hold_history->prev = newNode;
    }
    board->hold_history = newNode;
    board->hasHeld = true;

    // Ganti current_block dengan blok berikutnya
    board->current_block.type = GetNextBlock(board);
}

// Hapus baris yang penuh (tidak berubah)
int ClearFullLines(TetrisBoard* board) {
    // ... (implementasi sama seperti sebelumnya)
}

// Debug: Cetak papan
void PrintBoard(TetrisBoard* board) {
    // ... (implementasi sama seperti sebelumnya)
}

// Periksa game over
bool IsGameOver(TetrisBlock *block, TetrisBoard *board) {
    // ... (implementasi sama seperti sebelumnya)
}