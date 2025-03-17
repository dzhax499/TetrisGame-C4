// Nama file : main.c
// Deskripsi : File utama program game Tetris (management game loop)
// Oleh      : Dzakit Tsabit 241511071

// Nama file : main.c
// Deskripsi : Implementasi utama game Tetris
// Oleh      : Dzakit Tsabit 241511071

#include "raylib.h"
#include "include/tetris.h"
#include "include/blocks.h"
#include "include/board.h"
#include "include/rendering.h"
#include "include/scoring.h"
#include "include/main_menu.h"
#include "include/menu_music.h"
#include <stdlib.h>
#include <stdio.h>
#include <time.h>

// Game state
TetrisBoard board;
TetrisBlock currentBlock;
bool gameOver = false;
bool paused = false;

void UpdateGame(void) {
    if (IsKeyPressed(KEY_P)) {
        paused = !paused;
    }
    if (paused || gameOver) return;
    
    if (IsKeyPressed(KEY_LEFT)) {
        MoveBlockHorizontal(&currentBlock, &board, -1);
    }
    if (IsKeyPressed(KEY_RIGHT)) {
        MoveBlockHorizontal(&currentBlock, &board, 1);
    }
    if (IsKeyPressed(KEY_DOWN)) {
        MoveBlockDown(&currentBlock, &board);
    }
    if (IsKeyPressed(KEY_UP)) {
        RotateBlock(&currentBlock, &board);
    }
    if (IsKeyPressed(KEY_SPACE)) {
        HardDropBlock(&currentBlock, &board);
    }
    
    if (IsGameOver(&currentBlock, &board)) {
        gameOver = true;
    }
}

int main(void) {
    InitGame();
    InitMainMenu();
    InitMusic();
    while (!WindowShouldClose()) {
        if (currentState == MENU_STATE_MAIN) {
            UpdateMainMenu();
            DrawMainMenu();
        } else if (currentState == MENU_STATE_PLAY) {
        UpdateGame();
        DrawGame(&board);
    }
    CloseWindow();
    return 0;
}
