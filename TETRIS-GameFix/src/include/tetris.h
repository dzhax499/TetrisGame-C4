// Nama file : tetris.h
// Deskripsi : Header untuk game Tetris
// Oleh      : Dzakit Tsabit 241511071

#ifndef TETRIS_H
#define TETRIS_H

#include <stdbool.h>
#include <stdlib.h>
#include <time.h>
#include <stdio.h>
#include "raylib.h"

// Definisi ukuran papan permainan
#define BOARD_WIDTH  10
#define BOARD_HEIGHT 20

// Definisi ukuran window
#define WINDOW_WIDTH  1024  // Lebih lebar
#define WINDOW_HEIGHT 768   // Lebih tinggi

// typedef struct {
//     int grid[BOARD_HEIGHT][BOARD_WIDTH]; // 0 kosong, 1-7 warna blok
// } TetrisBoard0;

typedef struct {
    int type;     // Tipe blok (0-6 untuk I, J, L, O, S, T, Z)
    int rotation; // Rotasi saat ini (0-3)
    int shape[4][4]; // Bentuk blok
    int x, y;     // Posisi pada papan
    int ukuranblok; // Ukuran blok (4x4)
    Color color;  // Warna blok

    bool efekmeledak;    // efek ledakan aktif
    int waktumeledak;  // waktu animasi

    RotationNode *rotationNode;
} TetrisBlock;

// Tambahkan struktur untuk menyimpan hold block
typedef struct {
    TetrisBlock block;
    bool hasHeld;
} HoldBlock;

// Fungsi load dan unload asset permainan
void LoadGameAssets(void);
void UnloadGameAssets(void);

// Fungsi drawing tambahan
void DrawPauseOverlay(void);
void DrawGameOverScreen(int score);


#endif