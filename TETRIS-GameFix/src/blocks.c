//Nama file : blocks.c
//Deskripsi : Implementasi blok-blok game Tetris
//Oleh      : Dzakit Tsabit 241511071

#include "include/blocks.h"
#include <stdlib.h>
#include <stdio.h>
#include "raylib.h"

// Definisi bentuk-bentuk blok Tetris (bentuk I, J, L, O, S, T, Z)
// Setiap bentuk memiliki 4 rotasi
static const int TETROMINO_SHAPES[7][4][4][4] = {
    // I Tetromino (Cyan)
    {
        // Rotasi 0° (Horizontal)
        {{0,0,0,0}, 
         {1,1,1,1}, 
         {0,0,0,0}, 
         {0,0,0,0}},

        // Rotasi 90° (Vertical)
        {{0,0,1,0}, 
         {0,0,1,0}, 
         {0,0,1,0}, 
         {0,0,1,0}},

        // Rotasi 180° (Horizontal, sama seperti 0°)
        {{0,0,0,0}, 
         {0,0,0,0}, 
         {1,1,1,1}, 
         {0,0,0,0}},

        // Rotasi 270° (Vertical, sama seperti 90° tapi geser ke kiri)
        {{0,1,0,0}, 
         {0,1,0,0}, 
         {0,1,0,0}, 
         {0,1,0,0}}
    },
    
    // J Tetromino (Blue)
    {
        // Rotasi 0°
        {{1,0,0,0}, 
         {1,1,1,0}, 
         {0,0,0,0}, 
         {0,0,0,0}},

        // Rotasi 90°
        {{0,1,1,0}, 
         {0,1,0,0}, 
         {0,1,0,0}, 
         {0,0,0,0}},

        // Rotasi 180°
        {{0,0,0,0}, 
         {1,1,1,0}, 
         {0,0,1,0}, 
         {0,0,0,0}},

        // Rotasi 270°
        {{0,1,0,0}, 
         {0,1,0,0}, 
         {1,1,0,0}, 
         {0,0,0,0}}
    },

    // L Tetromino (Orange)
    {
        // Rotasi 0°
        {{0,0,1,0}, 
         {1,1,1,0}, 
         {0,0,0,0}, 
         {0,0,0,0}},

        // Rotasi 90°
        {{0,1,0,0}, 
         {0,1,0,0}, 
         {0,1,1,0}, 
         {0,0,0,0}},

        // Rotasi 180°
        {{0,0,0,0}, 
         {1,1,1,0}, 
         {1,0,0,0}, 
         {0,0,0,0}},

        // Rotasi 270°
        {{1,1,0,0}, 
         {0,1,0,0}, 
         {0,1,0,0}, 
         {0,0,0,0}}
    },

    // O Tetromino (Yellow)
    {
        // Tetromino O tidak berubah bentuk dalam rotasi
        {{0,1,1,0}, 
         {0,1,1,0}, 
         {0,0,0,0}, 
         {0,0,0,0}},

        {{0,1,1,0}, 
         {0,1,1,0}, 
         {0,0,0,0}, 
         {0,0,0,0}},

        {{0,1,1,0}, 
         {0,1,1,0}, 
         {0,0,0,0}, 
         {0,0,0,0}},

        {{0,1,1,0}, 
         {0,1,1,0}, 
         {0,0,0,0}, 
         {0,0,0,0}}
    }
};


// Warna untuk setiap jenis blok Tetris
static const Color TETROMINO_COLORS[7] = {
    {0, 255, 255, 255},   // Cyan (I)
    {0, 0, 255, 255},     // Blue (J)
    {255, 165, 0, 255},   // Orange (L)
    {255, 255, 0, 255},   // Yellow (O)
    {0, 255, 0, 255},     // Green (S)
    {128, 0, 128, 255},   // Purple (T)
    {255, 0, 0, 255}      // Red (Z)
};

void InitBlocks(void) {
    //  konsistensi API
}

/**
 * @brief Menghasilkan blok Tetris acak baru
 * @return TetrisBlock Blok Tetris baru
 */
TetrisBlock GenerateRandomBlock(void) {
    TetrisBlock block;
    
    // Pilih bentuk blok acak
    block.type = rand() % 7;
    
    // Mulai dengan rotasi pertama
    block.rotation = 0;
    
    // Posisi awal (di tengah atas)
    block.x = BOARD_WIDTH / 2 - 2;
    block.y = 0;
    
    // Atur warna blok sesuai tipe
    block.color = TETROMINO_COLORS[block.type];
    
    return block;
}

/**
 * @brief Menghasilkan blok Tetris acak baru
 * @return TetrisBlock Blok Tetris baru
 */ 
void RotateBock(TetrisBlock* block) {
    // Simpan rotasi saat ini
    int current_rotation = block->rotation;
    
    // Perbarui rotasi
    block->rotation = (block->rotation + 1) % 4;
    
    // Jika tabrakan terjadi, kembalikan rotasi ke nilai sebelumnya
    if (CheckCollision(block, block->x, block->y)) {
        block->rotation = current_rotation;
    }
}
