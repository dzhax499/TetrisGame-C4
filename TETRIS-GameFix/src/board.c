// Nama file : board.c
// Deskripsi : Implementasi logika papan permainan Tetris (Array 2D)
// Oleh      : Ibnu Hilmi 241511079
//             Rizky Satria Gunawan 241511089

#include "include/board.h"
#include "include/blocks.h"  // Mengimpor definisi blok dari blocks.h
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

    if (block >= BLOCK_EMPTY && block <= BLOCK_Z) {
        return BLOCK_COLORS[block];
    }
    return GRAY; // Jika tidak valid, kasih warna default
}

// Inisialisasi papan permainan
void InitBoard1(TetrisBoard *board) {
    memset(board->grid, BLOCK_EMPTY, sizeof(board->grid));
    board->current_score = 0;
    board->current_level = 1;
    board->lines_cleared = 0;
    board->game_over = false;
    board->hold_block.hasHeld = false;
    board->next_blocks = NULL;

    // Isi 5 blok pertama
    for (int i = 0; i < 5; i++) {
        AddNextBlock(board, GenerateRandomBlock());
    }

    board->current_block = PopNextBlock(board);
}

// Hapus baris yang penuh dan update skor
int ClearFullLines(TetrisBoard* board) {
    int linesCleared = 0;
    
    // Iterasi dari bawah ke atas
    for (int y = BOARD_HEIGHT - 1; y >= 0; y--) {
        bool isFullLine = true;
        
        // Periksa apakah baris penuh
        for (int x = 0; x < BOARD_WIDTH; x++) {
            if (board->grid[y][x] == BLOCK_EMPTY) {
                isFullLine = false;
                break;
            }
        }
        
        // Jika baris penuh, hapus dan geser baris di atasnya ke bawah
        if (isFullLine) {
            linesCleared++;
            
            for (int moveY = y; moveY > 0; moveY--) {
                memcpy(board->grid[moveY], board->grid[moveY - 1], sizeof(board->grid[moveY]));
            }
            
            // Bersihkan baris teratas
            memset(board->grid[0], BLOCK_EMPTY, sizeof(board->grid[0]));
            y++; // Periksa baris yang sama lagi setelah pergeseran
        }
    }
    
    // Update skor berdasarkan jumlah baris yang dihapus
    switch (linesCleared) {
        case 1: board->current_score += 100; break;
        case 2: board->current_score += 300; break;
        case 3: board->current_score += 500; break;
        case 4: board->current_score += 800; break;
    }
    
    // Update level
    board->lines_cleared += linesCleared;
    board->current_level = 1 + (board->lines_cleared / 10);
    
    return linesCleared;
}


// Fungsi debug: Cetak papan ke konsol
void PrintBoard(TetrisBoard* board) {
    for (int y = 0; y < BOARD_HEIGHT; y++) {
        for (int x = 0; x < BOARD_WIDTH; x++) {
            printf("%d ", board->grid[y][x]);
        }
        printf("\n");
    }
    printf("\n");
}

// Periksa apakah permainan berakhir
bool IsGameOver(TetrisBlock *block, TetrisBoard *board) {
    (void)block; // Tidak digunakan saat ini
    // Periksa baris paling atas
    for (int x = 0; x < BOARD_WIDTH; x++) {
        if (board->grid[0][x] != BLOCK_EMPTY) {
            board->game_over = true;
            return true;
        }
    }
    return false;
}

// Tambah blok ke circular linked list
void AddNextBlock(TetrisBoard* board, TetrisBlock newBlock) {
    BlockNode* newNode = (BlockNode*)malloc(sizeof(BlockNode));
    newNode->block = newBlock;

    if (!board->next_blocks) {
        newNode->next = newNode;
        board->next_blocks = newNode;
    } else {
        BlockNode* temp = board->next_blocks;
        while (temp->next != board->next_blocks) {
            temp = temp->next;
        }
        temp->next = newNode;
        newNode->next = board->next_blocks;
    }
}

// Ambil dan hapus blok dari circular list
TetrisBlock PopNextBlock(TetrisBoard* board) {
    if (!board->next_blocks) return GenerateRandomBlock();

    BlockNode* head = board->next_blocks;
    TetrisBlock result = head->block;

    if (head->next == head) {
        free(head);
        board->next_blocks = NULL;
    } else {
        BlockNode* temp = head;
        while (temp->next != head) temp = temp->next;
        temp->next = head->next;
        board->next_blocks = head->next;
        free(head);
    }

    return result;
}

void PrintNextBlocks(TetrisBoard* board) {
    if (!board->next_blocks) {
        printf("Antrian kosong\n");
        return;
    }

    BlockNode* current = board->next_blocks;
    do {
        printf("Blok type: %d\n", current->block.type);
        current = current->next;
    } while (current != board->next_blocks);
}
