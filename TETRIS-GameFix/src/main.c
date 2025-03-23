#include "include/tetris.h"
#include "include/board.h"
#include "include/blocks.h"
#include "include/rendering.h"
#include "include/scoring.h"
#include "include/main_menu.h"
#include "include/game_sound.h"  // Add this include
#include "raylib.h"
#include <time.h>

int main(void)
{
    // Initialize window ONCE
    InitWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "Tetris Game");
    SetTargetFPS(60);
    
    // Initialize audio system
    InitAudioDevice();

    // Seed random
    srand(time(NULL));

    // Initialization
    InitBlocks0();
    InitMainMenu();
    InitGameSound();  // Initialize game sounds

    TetrisBoard board;
    InitBoard1(&board);

    ScoreData scoreData;
    InitScoring(&scoreData);

    // Game state variables
    bool inGame = false;
    bool gameOver = false;
    bool wasPreviouslyInGame = false;

    // Timer for automatic block fall
    float fallTimer = 0.0f;
    float fallDelay = 1.0f; // Waktu jatuh awal

    // Start with menu music
    PlayBackgroundMusic(MUSIC_MENU);

    while (!WindowShouldClose())
    {
        BeginDrawing();
        ClearBackground(DARKGRAY);

        // Update game sounds
        UpdateGameSound();

        MenuState currentMenuState = GetCurrentMenuState();
        if (currentMenuState == MENU_STATE_PLAY)
        {
            if (!inGame) {
                // Start gameplay music when entering game
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
            // Track if we just entered the game
            if (!wasPreviouslyInGame) {
                wasPreviouslyInGame = true;
            }
            
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
                        StopBackgroundMusic();  // Switch back to menu music
                        PlaySoundEffect(SOUND_GAME_OVER); // Play game over sound
                    }
                }
                fallTimer = 0; // Reset timer
            }

            // Player controls
            if (IsKeyPressed(KEY_LEFT))
                MoveBlockHorizontal(&board.current_block, &board, -1);

            if (IsKeyPressed(KEY_RIGHT))
                MoveBlockHorizontal(&board.current_block, &board, 1);

            if (IsKeyPressed(KEY_UP)) {
                RotateBlock(&board.current_block, &board);
                PlaySoundEffect(SOUND_CLICK); // Play click sound on rotation
            }

            if (IsKeyPressed(KEY_DOWN))
                MoveBlockDown(&board.current_block, &board);

            if (IsKeyPressed(KEY_SPACE))
            {
                HardDropBlock(&board.current_block, &board);
                board.current_block = board.next_block;
                board.next_block = GenerateRandomBlock();
                PlaySoundEffect(SOUND_CLICK); // Play click sound on hard drop
            }
            
            // Tambahkan kontrol hold
            if (IsKeyPressed(KEY_C))
            {
                HoldCurrentBlock(&board);
                PlaySoundEffect(SOUND_CLICK); // Play click sound on hold
            }

            // Mute/unmute background music with M key
            if (IsKeyPressed(KEY_M))
            {
                ToggleBackgroundMusic();
            }

            // Clear full lines
            int linesCleared = ClearFullLines(&board);
            if (linesCleared > 0)
            {
                AddLineClearScore(&scoreData, linesCleared);
                PlaySoundEffect(SOUND_CLICK); // Play sound on line clear
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
                wasPreviouslyInGame = false;
                fallTimer = 0;
                PlaySoundEffect(SOUND_CLICK); // Play click sound on restart
                PlayBackgroundMusic(MUSIC_MENU); // Return to menu music
            }
        }
        else
        {
            if (wasPreviouslyInGame) {
                // We just returned to the menu from gameplay
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
    UnloadGameSound();  // Unload game sounds
    CloseAudioDevice();  // Close audio device
    CloseWindow();

    return 0;
}