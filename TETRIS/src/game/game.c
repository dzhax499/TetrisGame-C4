// Nama Program : game.c
// Deskripsi    : Implementasi logika utama permainan,
//                bertujuan mengelola siklus dan aturan dasar permainan
// Pembuat      : [Rizky Satria Gunawan, 241511089, 24 February 2025, 10:52]


#include "game.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int board[BOARD_HEIGHT][BOARD_WIDTH] = {0};

void init_board() {
    for (int i = 0; i < BOARD_HEIGHT; i++) {
        for (int j = 0; j < BOARD_WIDTH; j++) {
            board[i][j] = 0;
        }
    }
}

void display_board(SDL_Renderer *renderer) {
    // Bersihkan layar dengan warna hitam
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);

    // Gambar blok-blok di papan
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255); // Warna putih
    for (int i = 0; i < BOARD_HEIGHT; i++) {
        for (int j = 0; j < BOARD_WIDTH; j++) {
            if (board[i][j] == 1) {
                SDL_FRect block = {j * BLOCK_SIZE, i * BLOCK_SIZE, BLOCK_SIZE, BLOCK_SIZE};
                SDL_RenderFillRect(renderer, &block);
            }
        }
    }

    // Tampilkan ke layar
    SDL_RenderPresent(renderer);
}

void update_game(GameState *game) {
    // Logika untuk mengupdate game state
    // Misalnya, menurunkan blok, mengecek garis yang terisi, dll.
}

void run_game() {
    GameState game;
    init_settings(&game.settings);
    init_board();

    // Inisialisasi SDL
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        fprintf(stderr, "SDL could not initialize! SDL_Error: %s\n", SDL_GetError());
        return;
    }

    // Buat window
    game.window = SDL_CreateWindow("Tetris Game", BOARD_WIDTH * BLOCK_SIZE, BOARD_HEIGHT * BLOCK_SIZE, 0);
    if (!game.window) {
        fprintf(stderr, "Window could not be created! SDL_Error: %s\n", SDL_GetError());
        SDL_Quit();
        return;
    }

    game.renderer = SDL_CreateRenderer(game.window, NULL); // Sesuai dengan SDL3
    if (!game.renderer) {
        fprintf(stderr, "Renderer could not be created! SDL_Error: %s\n", SDL_GetError());
        SDL_DestroyWindow(game.window);
        SDL_Quit();
        return;
    }

    // Game loop
    int running = 1;
    while (running) {
        SDL_Event event;
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_EVENT_QUIT) {
                running = 0;
            }
            // Handle input (misalnya, tombol keyboard)
        }

        // Update game state
        update_game(&game);

        // Render papan
        display_board(game.renderer);

        // Tunggu sesuai kecepatan permainan
        SDL_Delay(game.settings.speed / 1000); // Konversi mikrodetik ke milidetik
    }

    // Bersihkan SDL
    SDL_DestroyRenderer(game.renderer);
    SDL_DestroyWindow(game.window);
    SDL_Quit();
}

int main() {
    srand(time(NULL));
    run_game();
    return 0;
}