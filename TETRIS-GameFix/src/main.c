#include "include/tetris.h"
#include "include/board.h"
#include "include/blocks.h"
#include "include/rendering.h"
#include "include/scoring.h"
#include "include/main_menu.h"
#include "include/game_sound.h"
#include "raylib.h"
#include <stdio.h>
#include <time.h>

#define WINDOW_WIDTH 1024
#define WINDOW_HEIGHT 768

int main(void)
{
    // Inisialisasi jendela dan sistem audio
    InitWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "Tetris Game");
    SetTargetFPS(60);
    InitAudioDevice();

    // Seed random
    srand(time(NULL));

    // Inisialisasi komponen game
    InitBlocks0();
    InitMainMenu();
    InitGameSound();

    TetrisBoard board;
    InitBoard1(&board);

    ScoreData scoreData;
    InitScoring(&scoreData);

    // Variabel status permainan
    bool inGame = false;
    bool gameOver = false;
    bool wasPreviouslyInGame = false;
    bool showLineClearNotification = false;
    float levelUpTimer = 0.0f;
    float fallTimer = 0.0f;
    float fallDelay = 1.0f;

    // Mulai dengan musik menu
    PlayBackgroundMusic(MUSIC_MENU);

    while (!WindowShouldClose())
    {
        BeginDrawing();
        ClearBackground(DARKGRAY);
        
        // Update suara permainan
        UpdateGameSound();
        
        MenuState currentMenuState = GetCurrentMenuState();
        
        if (currentMenuState == MENU_STATE_PLAY)
        {
            if (!inGame) {
                PlayBackgroundMusic(MUSIC_GAMEPLAY);
                PlaySoundEffect(SOUND_CLICK);
            }
            inGame = true;
        }
        else if (currentMenuState == MENU_STATE_EXIT)
        {
            break;
        }
        
        if (inGame && !gameOver)
        {
            if (!wasPreviouslyInGame) wasPreviouslyInGame = true;
            fallDelay = UpdateFallSpeed(&scoreData);
            fallTimer += GetFrameTime();

            if (fallTimer >= fallDelay)
            {
                if (!MoveBlockDown(&board.current_block, &board))
                {
                    PlaceBlock(&board.current_block, &board);
                    board.current_block = board.next_block;
                    board.next_block = GenerateRandomBlock();

                    if (IsGameOver(&board.current_block, &board))
                    {
                        gameOver = true;
                        StopBackgroundMusic();
                        PlaySoundEffect(SOUND_GAME_OVER);
                    }
                }
                fallTimer = 0;
            }
            
            // Kontrol pemain
            if (IsKeyPressed(KEY_LEFT)) 
                MoveBlockHorizontal(&board.current_block, &board, -1);
            if (IsKeyPressed(KEY_RIGHT)) 
                MoveBlockHorizontal(&board.current_block, &board, 1);
            if (IsKeyPressed(KEY_UP)) 
                RotateBlock(&board.current_block, &board);
            if (IsKeyPressed(KEY_DOWN)) 
                MoveBlockDown(&board.current_block, &board);
            if (IsKeyPressed(KEY_SPACE)) 
                HardDropBlock(&board.current_block, &board);
            if (IsKeyPressed(KEY_C)) 
                HoldCurrentBlock(&board);
            if (IsKeyPressed(KEY_M)) 
                ToggleBackgroundMusic();

            int linesCleared = ClearFullLines(&board);
            if (linesCleared > 0)
            {
                AddLineClearScore(&scoreData, linesCleared);

                PlaySoundEffect(SOUND_LINE_CLEAR);
                // Contoh logika level up tanpa CheckLevelUp

                //int previousLevel = scoreData.level;
                scoreData.level = scoreData.score / 1000;  // Contoh perhitungan level
                
                if (CheckLevelUp(&scoreData)) {
                    showLineClearNotification = true;
                    levelUpTimer = 2.0f;
                }
            }
            
            // Draw level-up notification
            if (showLineClearNotification) {
                levelUpTimer -= GetFrameTime();
                if (levelUpTimer <= 0) {
                    showLineClearNotification = false;
                }
            }

            // Gambar elemen permainan
            DrawBoard(&board);
            DrawActiveTetromino(&board.current_block);
            DrawBlockShadow(&board.current_block, &board);
            DrawHoldBlock(&board);
            DrawNextBlock(&board);
            DrawScore(&board, &scoreData);
            if (showLineClearNotification) 
                DrawText("LINE CLEAR!", 275, 250, 40, WHITE);
        }
        else if (gameOver)
        {
            DrawText("GAME OVER", 300, 250, 40, RED);
            DrawText("Press R to Restart", 280, 300, 20, WHITE);
            DrawText(TextFormat("Final Score: %d", scoreData.score), 280, 350, 20, WHITE);

            SaveHighScore(&scoreData);

            if (IsKeyPressed(KEY_R))
            {                
                InitBoard1(&board);
                InitScoring(&scoreData);
                gameOver = false;
                inGame = false;
                wasPreviouslyInGame = false;
                fallTimer = 0;
                PlaySoundEffect(SOUND_CLICK);
                PlayBackgroundMusic(MUSIC_MENU);
            }
        }
        else
        {
            if (wasPreviouslyInGame) {
                PlayBackgroundMusic(MUSIC_MENU);
                wasPreviouslyInGame = false;
            }
            UpdateMainMenu();
            DrawMainMenu();
        }

        EndDrawing();
    }
    
    // Cleanup
    UnloadMainMenu();
    UnloadGameSound();
    CloseAudioDevice();
    CloseWindow();

    return 0;
}