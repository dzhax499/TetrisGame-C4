//Nama file : blocks.c
//Deskripsi : Implementasi blok-blok game Tetris
//Oleh      : Dzakit Tsabit 241511071
#include <string.h>
#include "include/blocks.h"
#include <stdlib.h>
#include <stdio.h>
#include "raylib.h"

// Warna untuk setiap jenis blok Tetris
// static const Color TETROMINO_COLORS[7] = {
//     {0, 255, 255, 255},   // Cyan (I)
//     {0, 0, 255, 255},     // Blue (J)
//     {255, 165, 0, 255},   // Orange (L)
//     {255, 255, 0, 255},   // Yellow (O)
//     {0, 255, 0, 255},     // Green (S)
//     {128, 0, 128, 255},   // Purple (T)
//     {255, 0, 0, 255}      // Red (Z)
// };

static const int WallKickTests[4][5][2] = {
    // 0 -> R (Dari posisi awal ke kanan)
    {{0,0}, {-1,0}, {-1,1}, {0,-2}, {-1,-2}},
    // R -> 2 (Dari kanan ke posisi terbalik)
    {{0,0}, {1,0}, {1,-1}, {0,2}, {1,2}},
    // 2 -> L (Dari posisi terbalik ke kiri)
    {{0,0}, {1,0}, {1,1}, {0,-2}, {1,-2}},
    // L -> 0 (Dari kiri kembali ke posisi awal)
    {{0,0}, {-1,0}, {-1,-1}, {0,2}, {-1,2}}
};


void InitBlocks0(void) {
    // Inisialisasi random seed
    srand(time(NULL));
}

TetrisBlock GenerateRandomBlock(void) {
    TetrisBlock block;
    block.type = rand() % 7;
    block.rotation = 0;
    
    // Sesuaikan posisi awal agar blok muncul di bagian atas papan
    block.x = BOARD_WIDTH / 2 - 2;
    block.y = -1;  // Mulai di atas papan
    
    // Salin bentuk blok dari template
    for (int y = 0; y < 4; y++) {
        for (int x = 0; x < 4; x++) {
            block.shape[y][x] = TETROMINO_SHAPES[block.type][0][y][x];
        }
    }
    
    // Tetapkan warna
    static const Color TETROMINO_COLORS[7] = {
        SKYBLUE,    // I
        DARKBLUE,   // J
        ORANGE,     // L
        YELLOW,     // O
        LIME,       // S
        PURPLE,     // T
        RED         // Z
    };
    block.color = TETROMINO_COLORS[block.type];
    
    return block;
}

bool RotateBlockWithWallKick(TetrisBlock *block, TetrisBoard *board) {
    int initialRotation = block->rotation;
    int newRotation = (block->rotation + 1) % 4;
    
    // Try standard rotation first
    if (IsValidBlockPosition(block, board, block->x, block->y, newRotation)) {
        block->rotation = newRotation;
        return true;
    }
    
    // Try wall kicks
    for (int testIndex = 0; testIndex < 5; testIndex++) {
        int kickX = WallKickTests[initialRotation][testIndex][0];
        int kickY = WallKickTests[initialRotation][testIndex][1];
        
        if (IsValidBlockPosition(block, board, block->x + kickX, block->y + kickY, newRotation)) {
            block->x += kickX;
            block->y += kickY;
            block->rotation = newRotation;
            return true;
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
            // Lewati sel kosong
            if (TETROMINO_SHAPES[block->type][testRotation][y][x] == 0) continue;

            // Hitung koordinat absolut di papan
            int boardX = testX + x;
            int boardY = testY + y;

            // Cek batas horizontal
            if (boardX < 0 || boardX >= BOARD_WIDTH) {
                return false;
            }

            // Cek batas vertikal (bawah papan)
            if (boardY >= BOARD_HEIGHT) {
                return false;
            }

            // Untuk sel di atas papan (sebelum mencapai baris 0)
            // Izinkan pergerakan, tetapi jangan validasi
            if (boardY < 0) continue;

            // Cek tabrakan dengan blok yang sudah ada
            if (board->grid[boardY][boardX] != BLOCK_EMPTY) {
                return false;
            }
        }
    }

    return true;
}

bool RotateBlock(TetrisBlock *block, TetrisBoard *board) {
    int newRotation = (block->rotation + 1) % 4;
    
    // Cek validitas rotasi
    if (IsValidBlockPosition(block, board, block->x, block->y, newRotation)) {
        // Update rotasi
        block->rotation = newRotation;
        
        // Salin bentuk baru
        for (int y = 0; y < 4; y++) {
            for (int x = 0; x < 4; x++) {
                block->shape[y][x] = TETROMINO_SHAPES[block->type][newRotation][y][x];
            }
        }
        
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

void HoldCurrentBlock(TetrisBoard* board) {
    // Jika belum pernah memegang blok
    if (!board->hold_block.hasHeld) {
        board->hold_block.block = board->current_block;
        board->hold_block.hasHeld = true;
        
        // Ganti blok saat ini dengan blok berikutnya
        board->current_block = board->next_block;
        board->next_block = GenerateRandomBlock();
    } 
    // Jika sudah pernah memegang blok
    else {
        // Tukar blok yang sedang aktif dengan blok yang di-hold
        TetrisBlock tempBlock = board->current_block;
        board->current_block = board->hold_block.block;
        board->hold_block.block = tempBlock;
        
        // Reset posisi blok
        board->current_block.x = BOARD_WIDTH / 2 - 2;
        board->current_block.y = 0;
    }
}

void PlaceBlock(TetrisBlock *block, TetrisBoard *board) {
    for (int y = 0; y < 4; y++) {
        for (int x = 0; x < 4; x++) {
            // Hanya tempatkan sel yang terisi
            if (TETROMINO_SHAPES[block->type][block->rotation][y][x] != 0) {
                int boardX = block->x + x;
                int boardY = block->y + y;
                
                // Pastikan dalam batas papan dan di bawah baris 0
                if (boardY >= 0 && boardY < BOARD_HEIGHT && 
                    boardX >= 0 && boardX < BOARD_WIDTH) {
                    board->grid[boardY][boardX] = block->type + 1;
                }
            }
        }
    }
}