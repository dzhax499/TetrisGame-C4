#include "include/tetris.h"
#include "include/board.h"
#include "include/blocks.h"
#include "include/rendering.h"
#include "include/scoring.h"
#include "include/main_menu.h"
#include "include/game_sound.h"
#include "raylib.h"
#include <time.h>

#define WINDOW_WIDTH 1024
#define WINDOW_HEIGHT 768

int main(void)
{
    // Initialize window with consistent constants
    InitWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "Tetris Game");
    SetTargetFPS(60);
    
    // Initialize audio system
    InitAudioDevice();

    // Seed random
    srand(time(NULL));

    // Initialization
    InitBlocks0();
    InitMainMenu();
    InitGameSound();

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
    float fallDelay = 1.0f; // Initial fall time

    // Start with menu music
    PlayBackgroundMusic(MUSIC_MENU);

    while (!WindowShouldClose())
    {
        BeginDrawing();
        ClearBackground(DARKGRAY);

        // Update game sounds
        UpdateGameSound();

        MenuState currentMenuState = GetCurrentMenuState();
        
        // Handle menu state transitions
        if (currentMenuState == MENU_STATE_PLAY)
        {
            if (!inGame) {
                // Start gameplay music when entering game
                PlayBackgroundMusic(MUSIC_GAMEPLAY);
                PlaySoundEffect(SOUND_CLICK);
            }
            inGame = true;
        }
        else if (currentMenuState == MENU_STATE_CREDITS)
        {
            // Fix: Don't break the game loop, just display credits
            inGame = false; // Make sure we're not in game
            
            ClearBackground(LIGHTGRAY);
            DrawText("CREDITS", WINDOW_WIDTH/2 - MeasureText("CREDITS", 40)/2, 
                WINDOW_HEIGHT/2 - 150, 40, BLACK);

            // Draw names and IDs
            DrawText("Dzakir Tsabit \t 241511071", WINDOW_WIDTH/2 - MeasureText("Dzakir Tsabit 241511071", 20)/2, 
                WINDOW_HEIGHT/2 - 50, 20, BLACK);
            // Add other team member names here
            DrawText("Ibnu Hilmi \t 241511079", WINDOW_WIDTH/2 - MeasureText("Ibnu Hilmi 241511079", 20)/2, 
                WINDOW_HEIGHT/2 - 20, 20, BLACK);

            DrawText("Press ESC to return to menu", WINDOW_WIDTH/2 - MeasureText("Press ESC to return to menu", 20)/2, 
                WINDOW_HEIGHT/2 + 120, 20, BLACK);

            // Menu navigation is now handled in UpdateMainMenu()
        }
        else if (currentMenuState == MENU_STATE_HIGHSCORE)
        {
            // Fix: Don't break the game loop, just display highscores
            inGame = false; // Make sure we're not in game
            
            ClearBackground(LIGHTGRAY);
            DrawText("HIGHSCORES", WINDOW_WIDTH/2 - MeasureText("HIGHSCORES", 40)/2, 
                WINDOW_HEIGHT/2 - 150, 40, BLACK);
                
            // Add code to display high scores here
            DrawText("Coming Soon!", WINDOW_WIDTH/2 - MeasureText("Coming Soon!", 30)/2, 
                WINDOW_HEIGHT/2 - 30, 30, BLACK);
                
            DrawText("Press ESC to return to menu", WINDOW_WIDTH/2 - MeasureText("Press ESC to return to menu", 20)/2, 
                WINDOW_HEIGHT/2 + 120, 20, BLACK);
                
            // Menu navigation is now handled in UpdateMainMenu()
        }
        else if (currentMenuState == MENU_STATE_EXIT)
        {
            // Exit the game
            break;
        }
        else if (currentMenuState == MENU_STATE_MAIN)
        {
            // We're in the main menu
            inGame = false;
            
            if (wasPreviouslyInGame) {
                // We just returned to the menu from gameplay
                PlayBackgroundMusic(MUSIC_MENU);
                wasPreviouslyInGame = false;
            }
            
            UpdateMainMenu();
            DrawMainMenu();
        }

        // Game logic - only run if we're in game
        if (inGame && !gameOver)
        {
            // Track if we just entered the game
            if (!wasPreviouslyInGame) {
                wasPreviouslyInGame = true;
            }
            
            // Update fall speed based on current level
            #ifdef HAVE_UPDATE_FALL_SPEED
                fallDelay = UpdateFallSpeed(&scoreData);
            #else
                fallDelay = 1.0f - ((scoreData.level - 1) * 0.15f);
                if (fallDelay < 0.1f)
                    fallDelay = 0.1f;
            #endif
                
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
                        StopBackgroundMusic();
                        PlaySoundEffect(SOUND_GAME_OVER);
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
                PlaySoundEffect(SOUND_CLICK);
            }

            if (IsKeyPressed(KEY_DOWN))
                MoveBlockDown(&board.current_block, &board);

            if (IsKeyPressed(KEY_SPACE))
            {
                HardDropBlock(&board.current_block, &board);
                board.current_block = board.next_block;
                board.next_block = GenerateRandomBlock();
                PlaySoundEffect(SOUND_CLICK);
            }
            
            if (IsKeyPressed(KEY_C))
            {
                HoldCurrentBlock(&board);
                PlaySoundEffect(SOUND_CLICK);
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
                
                #ifdef SOUND_LINE_CLEAR
                    PlaySoundEffect(SOUND_LINE_CLEAR);
                #else
                    PlaySoundEffect(SOUND_CLICK);
                #endif
            }

            // Drawing
            
            DrawBlockShadow(&board.current_block, &board);
            
            DrawBoard(&board);
            DrawActiveTetromino(&board.current_block);
            
            DrawHoldBlock(&board);
            
            DrawNextBlock(&board);
            DrawScore(&board, &scoreData);
        }
        else if (gameOver)
        {
            DrawText("GAME OVER", WINDOW_WIDTH/2 - MeasureText("GAME OVER", 40)/2, 
                WINDOW_HEIGHT/2 - 50, 40, RED);
            DrawText("Press R to Restart", WINDOW_WIDTH/2 - MeasureText("Press R to Restart", 20)/2, 
                WINDOW_HEIGHT/2, 20, WHITE);
            DrawText(TextFormat("Final Score: %d", scoreData.score), 
                WINDOW_WIDTH/2 - MeasureText(TextFormat("Final Score: %d", scoreData.score), 20)/2, 
                WINDOW_HEIGHT/2 + 50, 20, WHITE);

            if (IsKeyPressed(KEY_R))
            {
                SaveHighScore(&scoreData);
                InitBoard1(&board);
                InitScoring(&scoreData);
                gameOver = false;
                inGame = false;
                wasPreviouslyInGame = false;
                fallTimer = 0;
                PlaySoundEffect(SOUND_CLICK);
                PlayBackgroundMusic(MUSIC_MENU);
                
                // Reset to main menu
                SetWindowState(MENU_STATE_MAIN);
            }
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