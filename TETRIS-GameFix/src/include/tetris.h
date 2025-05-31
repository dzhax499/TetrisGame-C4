// Nama file : tetris.h
// Deskripsi : Header untuk game Tetris
// Oleh      : Dzakit Tsabit 241511071

#ifndef TETRIS_H
#define TETRIS_H

#include <stdbool.h>
#include <stdlib.h>
#include "linkedlist_block.h"
#include <time.h>
#include <stdio.h>
#include "raylib.h"

// Definisi ukuran papan permainan
#define BOARD_WIDTH 10
#define BOARD_HEIGHT 20

// Definisi ukuran window
#define WINDOW_WIDTH 1024 // Lebih lebar
#define WINDOW_HEIGHT 768 // Lebih tinggi

typedef struct
{
    int type;        // Tipe blok (0-6 untuk I, J, L, O, S, T, Z)
    int rotation;    // Rotasi saat ini (0-3)
    int shape[4][4]; // Bentuk blok
    int x, y;        // Posisi pada papan
    int ukuranblok;  // Ukuran blok (4x4)
    Color color;     // Warna blok

    RotationNode *rotationNode;
} TetrisBlock;

// Tambahkan struktur untuk menyimpan hold block
typedef struct
{
    TetrisBlock block;
    bool hasHeld;
    int holdCount;
} HoldBlock;


void DrawPauseOverlay(void);
#endif