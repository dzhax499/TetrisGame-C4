// Nama file : board.c
// Deskripsi : Implementasi logika papan permainan Tetris (Array 2D)
// Oleh      : Ibnu Hilmi 241511079
//             Rizky Satria Gunawan 241511089

#include "include/board.h"
#include "include/blocks.h"
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "raylib.h"

TetrisBoard *board = NULL;

Color GetBlockColor(BlockType block) {
    static const Color BLOCK_COLORS[] = {
        BLANK, SKYBLUE, DARKBLUE, ORANGE, YELLOW, LIME, PURPLE, RED
    };
    return (block >= BLOCK_EMPTY && block <= BLOCK_Z) ? BLOCK_COLORS[block] : GRAY;
}

// Inisialisasi papan dengan penanganan error
void InitBoard(TetrisBoard *board) {
    if (!board) return;

    memset(board->grid, BLOCK_EMPTY, sizeof(board->grid));
    board->current_score = 0;
    board->current_level = 1;
    board->lines_cleared = 0;
    board->game_over = false;
    board->hold_block.hasHeld = false;
    board->next_blocks = NULL;

    for (int i = 0; i < 5; i++) {
        TetrisBlock newBlock = GenerateRandomBlock();
        AddNextBlock(board, newBlock);
    }

    board->current_block = PopNextBlock(board);
}

// Clear lines dengan optimasi
int ClearFullLines(TetrisBoard* board) {
    if (!board) return 0;

    int linesCleared = 0;
    for (int y = BOARD_HEIGHT - 1; y >= 0; y--) {
        bool isFull = true;
        for (int x = 0; x < BOARD_WIDTH; x++) {
            if (board->grid[y][x] == BLOCK_EMPTY) {
                isFull = false;
                break;
            }
        }

        if (isFull) {
            linesCleared++;
            for (int moveY = y; moveY > 0; moveY--) {
                memcpy(board->grid[moveY], board->grid[moveY - 1], BOARD_WIDTH * sizeof(BlockType));
            }
            memset(board->grid[0], BLOCK_EMPTY, BOARD_WIDTH * sizeof(BlockType));
            y++; // Re-check current row after shift
        }
    }

    // Update score
    if (linesCleared > 0) {
        const int SCORE_TABLE[] = {0, 100, 300, 500, 800};
        board->current_score += SCORE_TABLE[linesCleared];
        board->lines_cleared += linesCleared;
        board->current_level = 1 + (board->lines_cleared / 10);
    }

    return linesCleared;
}

// Tambah blok dengan pengecekan memori
void AddNextBlock(TetrisBoard* board, TetrisBlock newBlock) {
    if (!board) return;

    BlockNode* newNode = (BlockNode*)malloc(sizeof(BlockNode));
    if (!newNode) {
        TraceLog(LOG_ERROR, "Failed to allocate memory for next block!");
        return;
    }

    newNode->block = newBlock;

    if (!board->next_blocks) {
        newNode->next = newNode; // Circular ke diri sendiri
        board->next_blocks = newNode;
    } else {
        BlockNode* tail = board->next_blocks;
        while (tail->next != board->next_blocks) tail = tail->next;
        tail->next = newNode;
        newNode->next = board->next_blocks;
    }
}

// Ambil blok dengan penanganan antrian kosong
TetrisBlock PopNextBlock(TetrisBoard* board) {
    if (!board || !board->next_blocks) {
        TetrisBlock defaultBlock = {BLOCK_I, 0, 0, 0}; // Fallback
        return defaultBlock;
    }

    BlockNode* head = board->next_blocks;
    TetrisBlock result = head->block;

    if (head->next == head) {
        free(head);
        board->next_blocks = NULL;
    } else {
        BlockNode* tail = head;
        while (tail->next != head) tail = tail->next;
        tail->next = head->next;
        board->next_blocks = head->next;
        free(head);
    }

    // Tambahkan blok baru ke antrian untuk menjaga panjang
    AddNextBlock(board, GenerateRandomBlock());
    return result;
}

// Fungsi bantu untuk debug
void PrintNextBlocks(TetrisBoard* board) {
    if (!board || !board->next_blocks) {
        printf("Antrian kosong atau board NULL\n");
        return;
    }

    BlockNode* current = board->next_blocks;
    int count = 0;
    do {
        printf("Blok #%d: Tipe %d\n", ++count, current->block.type);
        current = current->next;
    } while (current != board->next_blocks && count < 10); // Prevent infinite loop
}