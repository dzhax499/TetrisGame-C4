#include "include/tetris.h"
#include "include/board.h"
#include "include/blocks.h"
#include "include/rendering.h"
#include "include/scoring.h"
#include "include/main_menu.h"
#include "raylib.h"
#include <time.h>

#define SCREEN_WIDTH 1024
#define SCREEN_HEIGHT 768

int main(void)
{
    // Initialize window ONCE
    InitWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "Tetris Game");
    SetTargetFPS(60);

    // Seed random
    srand(time(NULL));

    // Initialization
    InitBlocks0();
    InitMainMenu();

    TetrisBoard board;
    InitBoard1(&board);

    ScoreData scoreData;
    InitScoring(&scoreData);

    // Game state variables
    bool inGame = false;
    bool gameOver = false;

    // Timer for automatic block fall
    float fallTimer = 0.0f;
    float fallDelay = 1.0f; // Waktu jatuh awal

    while (!WindowShouldClose())
    {
        BeginDrawing();
        ClearBackground(DARKGRAY);

        MenuState currentMenuState = GetCurrentMenuState();
        if (currentMenuState == MENU_STATE_PLAY)
        {
            inGame = true;
        }
        else if (currentMenuState == MENU_STATE_EXIT)
        {
            break;
        }

        if (inGame && !gameOver)
        {
            // Update fall speed based on current level
            fallDelay = UpdateFallSpeed(&scoreData);
            
            fallDelay = 1.0f - ((scoreData.level - 1) * 0.15f);
            if (fallDelay < 0.1f)
                fallDelay = 0.1f;
            // Update fall timer
            fallTimer += GetFrameTime();

            // Automatic block fall
            if (fallTimer >= fallDelay)
            {
                if (!MoveBlockDown(&board.current_block, &board))
                {
                    PlaceBlock(&board.current_block, &board);
                    board.current_block = board.next_block;
                    board.next_block = GenerateRandomBlock();

                    // Check game over
                    if (IsGameOver(&board.current_block, &board))
                    {
                        gameOver = true;
                    }
                }
                fallTimer = 0; // Reset timer
            }

            // Player controls
            if (IsKeyPressed(KEY_LEFT))
                MoveBlockHorizontal(&board.current_block, &board, -1);

            if (IsKeyPressed(KEY_RIGHT))
                MoveBlockHorizontal(&board.current_block, &board, 1);

            if (IsKeyPressed(KEY_UP))
                RotateBlock(&board.current_block, &board);

            if (IsKeyPressed(KEY_DOWN))
                MoveBlockDown(&board.current_block, &board);

            if (IsKeyPressed(KEY_SPACE))
            {
                HardDropBlock(&board.current_block, &board);
                board.current_block = board.next_block;
                board.next_block = GenerateRandomBlock();
            }
            // Tambahkan kontrol hold
            if (IsKeyPressed(KEY_C))
            {
                HoldCurrentBlock(&board);
            }

            // Clear full lines
            int linesCleared = ClearFullLines(&board);
            if (linesCleared > 0)
            {
                AddLineClearScore(&scoreData, linesCleared);
            }

            // Drawing
            DrawBoard(&board);
            DrawActiveTetromino(&board.current_block);
            // Sebelum menggambar
            DrawBlockShadow(&board.current_block, &board);
            DrawHoldBlock(&board);
            DrawNextBlock(&board);
            DrawScore(&board, &scoreData);
        }
        else if (gameOver)
        {
            DrawText("GAME OVER", 300, 250, 40, RED);
            DrawText("Press R to Restart", 280, 300, 20, WHITE);
            DrawText(TextFormat("Final Score: %d", scoreData.score), 280, 350, 20, WHITE);

            if (IsKeyPressed(KEY_R))
            {
                SaveHighScore(&scoreData); // Save high score
                InitBoard1(&board);
                InitScoring(&scoreData);
                gameOver = false;
                inGame = false;
                fallTimer = 0;
            }
        }
        else
        {
            UpdateMainMenu();
            DrawMainMenu();
        }

        EndDrawing();
    }

    // Cleanup
    UnloadMainMenu();
    CloseWindow();

    return 0;
}