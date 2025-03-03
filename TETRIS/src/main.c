// Nama Program : main.c
// Deskripsi    : 
// Pembuat      : [Rizky Satria Gunawan, 241511089, 3 Maret 2025, 8:52]

#include "game/game.h"

int main() {
    GameState game;
    game_init(&game); // Inisialisasi game
    game_run(&game);  // Jalankan game loop
    return 0;
}