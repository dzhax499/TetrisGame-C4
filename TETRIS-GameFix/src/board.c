// Nama file : board.c
// Deskripsi : Implementasi logika papan permainan Tetris (Array 2D)
// Oleh      : Ibnu Hilmi 241511079

#include "raylib.h"
#include "include/board.h"
#include <string.h>
#include <stdlib.h>

// Definisi rotasi blok (4 rotasi untuk setiap blok)
const int BLOCK_SHAPES[7][4][4][4] = {
    // I-Block (Cyan)
    {
        {{0,0,0,0}, {1,1,1,1}, {0,0,0,0}, {0,0,0,0}},
        {{0,1,0,0}, {0,1,0,0}, {0,1,0,0}, {0,1,0,0}},
        {{0,0,0,0}, {1,1,1,1}, {0,0,0,0}, {0,0,0,0}},
        {{0,1,0,0}, {0,1,0,0}, {0,1,0,0}, {0,1,0,0}}
    },
    // J-Block (Blue)
    {
        {{1,0,0,0}, {1,1,1,0}, {0,0,0,0}, {0,0,0,0}},
        {{0,1,1,0}, {0,1,0,0}, {0,1,0,0}, {0,0,0,0}},
        {{0,0,0,0}, {1,1,1,0}, {0,0,1,0}, {0,0,0,0}},
        {{0,1,0,0}, {0,1,0,0}, {1,1,0,0}, {0,0,0,0}}
    },
    // L-Block (Orange)
    {
        {{0,0,1,0}, {1,1,1,0}, {0,0,0,0}, {0,0,0,0}},
        {{0,1,0,0}, {0,1,0,0}, {0,1,1,0}, {0,0,0,0}},
        {{0,0,0,0}, {1,1,1,0}, {1,0,0,0}, {0,0,0,0}},
        {{1,1,0,0}, {0,1,0,0}, {0,1,0,0}, {0,0,0,0}}
    },
    // O-Block (Yellow)
    {
        {{0,1,1,0}, {0,1,1,0}, {0,0,0,0}, {0,0,0,0}},
        {{0,1,1,0}, {0,1,1,0}, {0,0,0,0}, {0,0,0,0}},
        {{0,1,1,0}, {0,1,1,0}, {0,0,0,0}, {0,0,0,0}},
        {{0,1,1,0}, {0,1,1,0}, {0,0,0,0}, {0,0,0,0}}
    },
    // S-Block (Green)
    {
        {{0,1,1,0}, {1,1,0,0}, {0,0,0,0}, {0,0,0,0}},
        {{0,1,0,0}, {0,1,1,0}, {0,0,1,0}, {0,0,0,0}},
        {{0,1,1,0}, {1,1,0,0}, {0,0,0,0}, {0,0,0,0}},
        {{0,1,0,0}, {0,1,1,0}, {0,0,1,0}, {0,0,0,0}}
    },
    // T-Block (Purple)
    {
        {{0,1,0,0}, {1,1,1,0}, {0,0,0,0}, {0,0,0,0}},
        {{0,1,0,0}, {0,1,1,0}, {0,1,0,0}, {0,0,0,0}},
        {{0,0,0,0}, {1,1,1,0}, {0,1,0,0}, {0,0,0,0}},
        {{0,1,0,0}, {1,1,0,0}, {0,1,0,0}, {0,0,0,0}}
    },
    // Z-Block (Red)
    {
        {{1,1,0,0}, {0,1,1,0}, {0,0,0,0}, {0,0,0,0}},
        {{0,0,1,0}, {0,1,1,0}, {0,1,0,0}, {0,0,0,0}},
        {{1,1,0,0}, {0,1,1,0}, {0,0,0,0}, {0,0,0,0}},
        {{0,0,1,0}, {0,1,1,0}, {0,1,0,0}, {0,0,0,0}}
    }
};

// Fungsi untuk mendapatkan warna blok
Color GetBlockColor(BlockType block) {
    switch (block) {
        case BLOCK_I: return SKYBLUE;    // Cyan
        case BLOCK_J: return BLUE;       // Blue
        case BLOCK_L: return ORANGE;     // Orange
        case BLOCK_O: return YELLOW;     // Yellow
        case BLOCK_S: return GREEN;      // Green
        case BLOCK_T: return PURPLE;     // Purple
        case BLOCK_Z: return RED;        // Red
        default: return LIGHTGRAY;       // Default untuk blok kosong
    }
}

// Inisialisasi papan permainan
void InitBoard(TetrisBoard* board) {
    // Bersihkan grid
    memset(board->grid, BLOCK_EMPTY, sizeof(board->grid));
    
    // Reset skor dan level
    board->current_score = 0;
    board->current_level = 1;
    board->lines_cleared = 0;
}

// Periksa apakah posisi blok valid
bool IsValidPosition(TetrisBoard* board, BlockType block, int row, int col, int rotation) {
    // Periksa setiap sel dalam blok
    for (int y = 0; y < 4; y++) {
        for (int x = 0; x < 4; x++) {
            // Lewati sel kosong
            if (BLOCK_SHAPES[block][rotation][y][x] == 0) continue;
            
            // Koordinat aktual di papan
            int boardX = col + x;
            int boardY = row + y;
            
            // Periksa batas papan
            if (boardX < 0 || boardX >= BOARD_WIDTH || 
                boardY < 0 || boardY >= BOARD_HEIGHT) {
                return false;
            }
            
            // Periksa tabrakan dengan blok lain
            if (board->grid[boardY][boardX] != BLOCK_EMPTY) {
                return false;
            }
        }
    }
    return true;
}

// Tempatkan blok di papan
void PlaceBlock(TetrisBoard* board, BlockType block, int row, int col, int rotation) {
    for (int y = 0; y < 4; y++) {
        for (int x = 0; x < 4; x++) {
            // Lewati sel kosong
            if (BLOCK_SHAPES[block][rotation][y][x] == 0) continue;
            
            // Koordinat aktual di papan
            int boardX = col + x;
            int boardY = row + y;
            
            // Tempatkan blok
            board->grid[boardY][boardX] = block;
        }
    }
}

// Hapus baris yang penuh
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
        
        // Jika baris penuh, hapus dan geser turun
        if (isFullLine) {
            linesCleared++;
            
            // Geser baris di atasnya ke bawah
            for (int moveY = y; moveY > 0; moveY--) {
                memcpy(board->grid[moveY], board->grid[moveY - 1], 
                       sizeof(board->grid[moveY]));
            }
            
            // Bersihkan baris teratas
            memset(board->grid[0], BLOCK_EMPTY, sizeof(board->grid[0]));
            
            // Kembalikan y untuk memeriksa baris yang baru saja diturunkan
            y++;
        }
    }
    
    // Update skor
    switch (linesCleared) {
        case 1: board->current_score += 100; break;
        case 2: board->current_score += 300; break;
        case 3: board->current_score += 500; break;
        case 4: board->current_score += 800; break;
    }
    
    // Update level dan baris yang dihapus
    board->lines_cleared += linesCleared;
    board->current_level = 1 + (board->lines_cleared / 10);
    
    return linesCleared;
}

// Periksa apakah permainan berakhir
bool IsGameOver(TetrisBoard* board) {
    // Periksa baris paling atas
    for (int x = 0; x < BOARD_WIDTH; x++) {
        if (board->grid[0][x] != BLOCK_EMPTY) {
            return true;
        }
    }
    return false;
}