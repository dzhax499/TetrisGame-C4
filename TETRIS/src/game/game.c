// Nama Program : game.c
// Deskripsi    : Implementasi logika utama permainan,
//                bertujuan mengelola siklus dan aturan dasar permainan
// Pembuat      : [Rizky Satria Gunawan, 241511089, 24 February 2025, 10:52]

#include "game.h"
#include <stdio.h>
#include <unistd.h> // Untuk usleep

// Fungsi untuk menjalankan permainan Tetris
void run_game() {
    Settings settings;
    settings.mode = 1; // Mulai dari level 1
    settings.speed = SPEEDS[1]; // Kecepatan awal

    initEndlessMode(); // Inisialisasi mode endless

    while (1) {
        // Update game state
        updateGame();

        // Tampilkan papan permainan
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

        // Tampilkan informasi level dan kecepatan
        printf("Level: %d\n", settings.mode);
        printf("Speed: %d microseconds\n", settings.speed);

        // Tunggu sesuai kecepatan permainan
        usleep(settings.speed);

        // Bersihkan layar (untuk terminal yang mendukung)
        printf("\033[H\033[J");

        // Cek jika level perlu ditingkatkan
        // Misalnya, setiap 10 baris yang dihapus, naikkan level
        // Ini hanya contoh sederhana, Anda bisa menyesuaikan logikanya
        static int lines_cleared = 0;
        lines_cleared++;
        if (lines_cleared % 10 == 0) {
            settings.mode++;
            update_settings(&settings, settings.mode, SPEEDS[1] - (settings.mode - 1) * 20000);
        }
    }
}

int main() {
    run_game();
    return 0;
}