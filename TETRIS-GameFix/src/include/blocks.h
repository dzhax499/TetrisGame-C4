// Nama file : blocks.h
// Deskripsi : Header untuk blok-blok game Tetris  
// Oleh      : Dzakit Tsabit 241511071

#ifndef BLOCKS_H
#define BLOCKS_H

#include "tetris.h"
#include "board.h"
#include <stdlib.h>
#include <stdbool.h>

// Inisialisasi sistem blok
void InitBlocks0(void);
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
    },
    // Add the missing shapes to your array:

// S Tetromino (Green)
{
    // Rotasi 0°
    {{0,1,1,0}, 
     {1,1,0,0}, 
     {0,0,0,0}, 
     {0,0,0,0}},

    // Rotasi 90°
    {{0,1,0,0}, 
     {0,1,1,0}, 
     {0,0,1,0}, 
     {0,0,0,0}},

    // Rotasi 180° (same as 0°)
    {{0,0,0,0}, 
     {0,1,1,0}, 
     {1,1,0,0}, 
     {0,0,0,0}},

    // Rotasi 270° (same as 90°)
    {{1,0,0,0}, 
     {1,1,0,0}, 
     {0,1,0,0}, 
     {0,0,0,0}}
},

// T Tetromino (Purple)
{
    // Rotasi 0°
    {{0,1,0,0}, 
     {1,1,1,0}, 
     {0,0,0,0}, 
     {0,0,0,0}},

    // Rotasi 90°
    {{0,1,0,0}, 
     {0,1,1,0}, 
     {0,1,0,0}, 
     {0,0,0,0}},

    // Rotasi 180°
    {{0,0,0,0}, 
     {1,1,1,0}, 
     {0,1,0,0}, 
     {0,0,0,0}},

    // Rotasi 270°
    {{0,1,0,0}, 
     {1,1,0,0}, 
     {0,1,0,0}, 
     {0,0,0,0}}
},

// Z Tetromino (Red)
{
    // Rotasi 0°
    {{1,1,0,0}, 
     {0,1,1,0}, 
     {0,0,0,0}, 
     {0,0,0,0}},

    // Rotasi 90°
    {{0,0,1,0}, 
     {0,1,1,0}, 
     {0,1,0,0}, 
     {0,0,0,0}},

    // Rotasi 180° (same as 0°)
    {{0,0,0,0}, 
     {1,1,0,0}, 
     {0,1,1,0}, 
     {0,0,0,0}},

    // Rotasi 270° (same as 90°)
    {{0,1,0,0}, 
     {1,1,0,0}, 
     {1,0,0,0}, 
     {0,0,0,0}}
}
    
};


// Fungsi untuk menghasilkan blok acak
TetrisBlock GenerateRandomBlock(void);


// Fungsi untuk memeriksa posisi blok
bool IsValidBlockPosition(TetrisBlock *block, TetrisBoard *board, int testX, int testY, int testRotation);

// Fungsi untuk memutar blok
bool RotateBlock(TetrisBlock *block, TetrisBoard *board);

// Fungsi untuk menggerakkan blok secara horizontal
bool MoveBlockHorizontal(TetrisBlock *block, TetrisBoard *board, int dx);

// Fungsi untuk menggerakkan blok ke bawah
bool MoveBlockDown(TetrisBlock *block, TetrisBoard *board);

// Fungsi untuk hard drop (jatuh cepat)
void HardDropBlock(TetrisBlock *block, TetrisBoard *board);

void HoldCurrentBlock(TetrisBoard* board);

// Fungsi untuk menempatkan blok di papan
void PlaceBlock(TetrisBlock *block, TetrisBoard *board);

//harddrop hitung
int CalculateDropDistance(TetrisBlock *block, TetrisBoard *board);

bool RotateBlockWithWallKick(TetrisBlock *block, TetrisBoard *board);

#endif