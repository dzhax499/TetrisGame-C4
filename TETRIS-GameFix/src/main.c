// Nama file : main.c
// Deskripsi : File utama program game Tetris (management game loop)
// Oleh      : Dzakit Tsabit 241511071

// Nama file : main.c
// Deskripsi : Implementasi utama game Tetris
// Oleh      : Dzakit Tsabit 241511071

#include "raylib.h"
#include "include/tetris.h"
#include "include/blocks.h"
#include <stdlib.h>
#include <stdio.h>
#include <time.h>

// Game state
TetrisBoard board;
TetrisBlock currentBlock;
bool gameOver = false;
bool paused = false;

void InitGame(void) {
    InitWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "Tetris Game");
    SetTargetFPS(60);
    
    for (int i = 0; i < BOARD_HEIGHT; i++) {
        for (int j = 0; j < BOARD_WIDTH; j++) {
            board.grid[i][j] = 0;
        }
    }
    
    currentBlock = GenerateRandomBlock();
}

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

void DrawGame(void) {
    BeginDrawing();
    ClearBackground(RAYWHITE);
    
    if (gameOver) {
        DrawGameOverScreen(0);
    } else if (paused) {
        DrawPauseOverlay();
    } else {
        for (int i = 0; i < BOARD_HEIGHT; i++) {
            for (int j = 0; j < BOARD_WIDTH; j++) {
                if (board.grid[i][j] != 0) {
                    DrawRectangle(j * 30, i * 30, 30, 30, BLACK);
                }
            }
        }
    }
    EndDrawing();
}

int main(void) {
    InitGame();
    while (!WindowShouldClose()) {
        UpdateGame();
        DrawGame();
    }
    CloseWindow();
    return 0;
}
