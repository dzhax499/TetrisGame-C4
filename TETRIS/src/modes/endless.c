// Nama Program : endless.c
// Deskripsi    : Program ini digunakan untuk mode endless dimana permainan akan terus berlanjut 
//                sampai pemain memilih untuk berhenti / mengalami kekalahan.
// Pembuat      : [Rizky Satria Gunawan, 241511089, 24 February 2024, 8:32]


#include "endless.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define BOARD_WIDTH 10
#define BOARD_HEIGHT 20

int board[BOARD_HEIGHT][BOARD_WIDTH];
Tetromino currentTetromino;

void initEndlessMode() {
    // Mengatur seed untuk angka acak
    srand(time(NULL));
    
    // Mengosongkan papan permainan
    for (int i = 0; i < BOARD_HEIGHT; i++) {
        for (int j = 0; j < BOARD_WIDTH; j++) {
            board[i][j] = 0;
        }
    }
    
    // Memunculkan tetromino pertama
    spawnTetromino();
}

void spawnTetromino() {
    // Memilih bentuk tetromino secara acak
    currentTetromino.shape = rand() % 7; // Misal ada 7 jenis tetromino
    currentTetromino.x = BOARD_WIDTH / 2;
    currentTetromino.y = 0;
}

void updateGame() {
    // Cek jika tetromino bisa turun
    if (canMoveDown()) {
        currentTetromino.y++;
    } else {
        placeTetromino();
        clearFullRows();
        spawnTetromino();
    }
    
    if (checkGameOver()) {
        printf("Game Over!\n");
        initEndlessMode(); // Restart game
    }
}

int canMoveDown() {
    // Cek apakah tetromino bisa turun tanpa keluar batas atau menabrak blok lain
    return (currentTetromino.y + 1 < BOARD_HEIGHT);
}

void placeTetromino() {
    // Simpan tetromino di papan
    board[currentTetromino.y][currentTetromino.x] = 1; // Sederhana, harus diperbaiki untuk bentuk yang lebih kompleks
}

void clearFullRows() {
    for (int i = 0; i < BOARD_HEIGHT; i++) {
        int full = 1;
        for (int j = 0; j < BOARD_WIDTH; j++) {
            if (board[i][j] == 0) {
                full = 0;
                break;
            }
        }
        if (full) {
            // Geser semua baris di atas ke bawah
            for (int k = i; k > 0; k--) {
                for (int j = 0; j < BOARD_WIDTH; j++) {
                    board[k][j] = board[k - 1][j];
                }
            }
            // Kosongkan baris teratas
            for (int j = 0; j < BOARD_WIDTH; j++) {
                board[0][j] = 0;
            }
        }
    }
}

int checkGameOver() {
    // Jika ada blok di bagian atas setelah menempatkan tetromino, game over
    for (int j = 0; j < BOARD_WIDTH; j++) {
        if (board[0][j] != 0) {
            return 1;
        }
    }
    return 0;
}
