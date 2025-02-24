// Nama Program : game.c
// Deskripsi    : Implementasi logika utama permainan,
//                bertujuan mengelola siklus dan aturan dasar permainan
// Pembuat      : [Rizky Satria Gunawan, 241511089, 24 February 2025, 10:52]

// game.c
#include "game.h"
#include "modes/endless.h"
#include "modes/level.h"
#include <SDL2/SDL.h>

GameState gameState = MENU;

void handleGameState() {
    switch (gameState) {
        case MENU:
            // Tampilkan menu utama
            break;
        case PLAYING:
            // Jalankan game mode
            break;
        case PAUSED:
            // Pause menu
            break;
    }
}
