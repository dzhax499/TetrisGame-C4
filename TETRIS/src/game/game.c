// Nama Program : game.c
// Deskripsi    : Implementasi logika utama permainan,
//                bertujuan mengelola siklus dan aturan dasar permainan
// Pembuat      : [Rizky Satria Gunawan, 241511089, 24 February 2025, 10:52]


#include "game.h"
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <time.h>

#define BOARD_WIDTH 10
#define BOARD_HEIGHT 20

int board[BOARD_HEIGHT][BOARD_WIDTH] = {0};

void init_board() {
    for (int i = 0; i < BOARD_HEIGHT; i++) {
        for (int j = 0; j < BOARD_WIDTH; j++) {
            board[i][j] = 0;
        }
    }
}

void display_board() {
    for (int i = 0; i < BOARD_HEIGHT; i++) {
        for (int j = 0; j < BOARD_WIDTH; j++) {
            if (board[i][j] == 1) {
                printf("[]");
            } else {
                printf(" .");
            }
        }
        printf("\n");
    }
}

void update_game(Settings *settings) {
    // Logika untuk mengupdate game state
    // Misalnya, menurunkan blok, mengecek garis yang terisi, dll.
}

void run_game() {
    Settings settings;
    init_settings(&settings);
    init_board();

    while (1) {
        display_board();
        printf("Level: %d\n", settings.mode);
        printf("Speed: %d microseconds\n", settings.speed);

        update_game(&settings);

        usleep(settings.speed);
        printf("\033[H\033[J");
    }
}

int main() {
    srand(time(NULL));
    run_game();
    return 0;
}