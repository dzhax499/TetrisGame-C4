#include "include/tetris.h" // Pastikan TetrisBlock didefinisikan di sini
#include "include/board.h"  // Menggunakan TetrisBlock
#include "include/blocks.h" // Menggunakan TetrisBlock
#include "include/rendering.h"
#include "include/scoring.h"
#include "include/main_menu.h"
#include "include/menu_music.h"
#include "include/click_sound.h"
#include "raylib.h"
#include <time.h>

int main(void)
{
    // Inisialisasi window dan rendering
    InitWindow(800, 600, "Tetris Game");
    SetTargetFPS(60);

    // Inisialisasi menu utama
    InitMainMenu();

    // Inisialisasi background music
    InitMenuMusic();

    // Inisialisasi click sound
    InitClickSound();

    // Inisialisasi papan permainan
    TetrisBoard board;
    InitBoard1(&board);

    // Inisialisasi sistem skor
    ScoreData scoreData;
    InitScoring(&scoreData);

    // Inisialisasi rendering
    InitRendering();

    // Variabel untuk mengontrol state game
    bool inGame = false;
    bool gameOver = false;

    // Loop utama game
    while (!WindowShouldClose())
    {
        BeginDrawing();
        ClearBackground(DARKGRAY);
        DrawText("DEBUG: Drawing frame", 10, 10, 20, WHITE);
        EndDrawing();
        printf("Loop berjalan\n");
        // Update state game berdasarkan menu
        MenuState currentMenuState = GetCurrentMenuState();
        if (currentMenuState == MENU_STATE_PLAY)
        {
            printf("Switching to Game Mode\n");
            inGame = true;
        }
        else if (currentMenuState == MENU_STATE_EXIT)
        {
            break;
        }

        if (inGame)
        {
            // Update game logic
            if (!gameOver)
            {
                printf("Game logic berjalan\n");
                // Gerakkan blok ke bawah secara otomatis
                if (IsKeyPressed(KEY_DOWN))
                {
                    if (!MoveBlockDown(&board.current_block, &board))
                    {
                        PlaceBlock(&board.current_block, &board);
                        board.current_block = board.next_block;
                        board.next_block = GenerateRandomBlock();
                        if (IsGameOver(&board.current_block, &board))
                        {
                            gameOver = true;
                        }
                    }
                }

                // Gerakkan blok ke kiri atau kanan
                if (IsKeyPressed(KEY_LEFT))
                {
                    MoveBlockHorizontal(&board.current_block, &board, -1);
                }
                if (IsKeyPressed(KEY_RIGHT))
                {
                    MoveBlockHorizontal(&board.current_block, &board, 1);
                }

                // Rotasi blok
                if (IsKeyPressed(KEY_UP))
                {
                    RotateBlock(&board.current_block, &board);
                }

                // Hard drop
                if (IsKeyPressed(KEY_SPACE))
                {
                    int dropDistance = CalculateDropDistance(&board.current_block, &board); // Hitung jarak jatuh
                    HardDropBlock(&board.current_block, &board);
                    AddDropScore(&scoreData, dropDistance);
                    PlaceBlock(&board.current_block, &board);
                    board.current_block = board.next_block;
                    board.next_block = GenerateRandomBlock();
                    if (IsGameOver(&board.current_block, &board))
                    {
                        gameOver = true;
                    }
                }

                // Hapus baris yang penuh
                int linesCleared = ClearFullLines(&board);
                if (linesCleared > 0)
                {
                    AddLineClearScore(&scoreData, linesCleared);
                }
            }

            // // Render game
            // BeginDrawing();
            // ClearBackground(DARKGRAY);

            if (gameOver)
            {
                DrawText("GAME OVER", 300, 250, 40, RED);
                DrawText("Press R to Restart", 280, 300, 20, WHITE);
                if (IsKeyPressed(KEY_R))
                {
                    InitBoard1(&board);
                    InitScoring(&scoreData); // Reset skor
                    gameOver = false;
                    inGame = false; // Kembali ke menu setelah restart
                }
            }
            else
            {
                DrawBoard(&board);
                DrawActiveTetromino(&board.current_block);
                DrawNextBlock(&board);
                DrawScore(&board, &scoreData); // Tampilkan skor dan level
            }

            EndDrawing();
        }
        else
        {
            printf("Main menu aktif\n");
            // // Render menu utama
            // BeginDrawing();
            // ClearBackground(DARKGRAY);
            // DrawText("DEBUG: Drawing frame", 10, 10, 20, WHITE);
            // EndDrawing();

            UpdateMainMenu();
            UpdateMenuMusic();
            UpdateClickSound();
            DrawMainMenu();
        }
    }

    // Unload resources
    UnloadMainMenu();
    UnloadMenuMusic(); 
    UnloadClickSound();
    CloseRendering();
    CloseWindow();

    return 0;
}