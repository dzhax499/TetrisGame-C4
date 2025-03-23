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

// Game states
typedef enum {
    GAME_STATE_MENU,
    GAME_STATE_GAMEPLAY,
    GAME_STATE_OPTIONS,
    GAME_STATE_HIGHSCORE,
    GAME_STATE_CREDITS
} GameState;

// Function to reset to main menu
void ResetToMainMenu(GameState* currentGameState) {
    *currentGameState = GAME_STATE_MENU;
    
    // Reset menu state
    InitMainMenu();
    
    // Potentially stop any ongoing game processes
    // Add any necessary cleanup or reset logic for specific game states here
}

int main(void)
{
    // Inisialisasi window
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Raylib Game Menu Demo");
    SetTargetFPS(60);
    
    // Inisialisasi audio
    InitAudioDevice();
    
    // Inisialisasi game state
    GameState currentGameState = GAME_STATE_MENU;
    
    // Inisialisasi menu
    InitMainMenu();

    // Inisialisasi background music
    // InitMenuMusic();

    // Inisialisasi click sound
    // InitClickSound();
    
    // Game loop
    while (!WindowShouldClose())
    {        
        // Update
        switch (currentGameState)
        {
            case GAME_STATE_MENU:
                UpdateMainMenu();
                MenuState menuState = GetCurrentMenuState();
                
                // Handle menu state changes
                // UpdateClickSound();
                switch (menuState)
                {
                    case MENU_STATE_PLAY:
                        currentGameState = GAME_STATE_GAMEPLAY;
                        break;
                    case MENU_STATE_OPTIONS:
                        currentGameState = GAME_STATE_OPTIONS;
                        break;
                    case MENU_STATE_CREDITS:
                        currentGameState = GAME_STATE_CREDITS;
                        break;
                    case MENU_STATE_HIGHSCORE:
                        currentGameState = GAME_STATE_HIGHSCORE;
                        break;
                    case MENU_STATE_EXIT:
                        CloseWindow();
                        break;
                    default:
                        break;
                }
                break;
                
            case GAME_STATE_GAMEPLAY:
                // Check for ESC key to return to menu
                if (IsKeyPressed(KEY_ESCAPE))
                {
                    ResetToMainMenu(&currentGameState);
                }
                break;
                
            case GAME_STATE_OPTIONS:
                // Check for ESC key to return to menu
                if (IsKeyPressed(KEY_ESCAPE))
                {
                    ResetToMainMenu(&currentGameState);
                }
                break;
                
            case GAME_STATE_CREDITS:
                // Check for ESC key to return to menu
                if (IsKeyPressed(KEY_ESCAPE))
                {
                    ResetToMainMenu(&currentGameState);
                }
                break;
            case GAME_STATE_HIGHSCORE:
                // Check for ESC key to return to menu
                if (IsKeyPressed(KEY_ESCAPE))
                {
                    ResetToMainMenu(&currentGameState);
                }
                break;
        }
        
        // Draw
        BeginDrawing();
        ClearBackground(RAYWHITE);
        
        switch (currentGameState)
        {
            case GAME_STATE_MENU:
                DrawMainMenu();
                break;
                
            case GAME_STATE_GAMEPLAY:
                // Check for ESC key to return to menu
                if (IsKeyPressed(KEY_ESCAPE))
                {
                    ResetToMainMenu(&currentGameState);
                }
                
                // Initialize game variables (these should be moved to proper initialization)
                static bool inGame = true;
                static bool gameOver = false;
                static float fallTimer = 0.0f;
                static float fallDelay = 1.0f;
                static TetrisBoard board;
                static ScoreData scoreData;
                
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
                    ClearBackground(SKYBLUE);
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
                    ClearBackground(RED);
                    DrawText("GAME OVER", 300, 250, 40, RED);
                    DrawText("Press R to Restart", 280, 300, 20, WHITE);
                    DrawText(TextFormat("Final Score: %d", scoreData.score), 280, 350, 20, WHITE);

                    if (IsKeyPressed(KEY_R))
                    {
                        SaveHighScore(&scoreData); // Save high score
                        InitBoard1(&board);  // Changed from InitBoard1 to match your includes
                        InitScoring(&scoreData);
                        gameOver = false;
                        inGame = true;
                        fallTimer = 0;
                    }
                }
                break;
                
            case GAME_STATE_OPTIONS:
                ClearBackground(BEIGE);
                DrawText("OPTIONS SCREEN", SCREEN_WIDTH/2 - MeasureText("OPTIONS SCREEN", 40)/2, 
                        SCREEN_HEIGHT/2 - 40, 40, BLACK);
                DrawText("Press ESC to return to menu", SCREEN_WIDTH/2 - MeasureText("Press ESC to return to menu", 20)/2, 
                        SCREEN_HEIGHT/2 + 20, 20, BLACK);
                break;
                
            case GAME_STATE_CREDITS:
                ClearBackground(LIGHTGRAY);
                //backgroundTexture = LoadTexture("bg.png");
                DrawText("CREDITS", SCREEN_WIDTH/2 - MeasureText("CREDITS", 40)/2, 
                    SCREEN_HEIGHT/2 - 150, 40, BLACK);
    
                // Draw names and IDs
                DrawText("Dzakir Tsabit \t 241511071", SCREEN_WIDTH/2 - MeasureText("Dzakir Tsabit 241511071", 20)/2, 
                        SCREEN_HEIGHT/2 - 50, 20, BLACK);
                DrawText("Ibnu Hilmi \t\t 241511079", SCREEN_WIDTH/2 - MeasureText("Ibnu Hilmi 241511079", 20)/2, 
                        SCREEN_HEIGHT/2 - 20, 20, BLACK);
                DrawText("Rizky Satria \t\t 241511089", SCREEN_WIDTH/2 - MeasureText("Rizky Satria 241511089", 20)/2, 
                        SCREEN_HEIGHT/2 + 10, 20, BLACK);
                DrawText("Fatimah Hawwa \t 241511074", SCREEN_WIDTH/2 - MeasureText("Fatimah Hawwa 241511074", 20)/2, 
                        SCREEN_HEIGHT/2 + 40, 20, BLACK);
                DrawText("Varian Abi \t\t 241511091", SCREEN_WIDTH/2 - MeasureText("Varian Abi 241511091", 20)/2, 
                        SCREEN_HEIGHT/2 + 70, 20, BLACK);
                
                DrawText("Press ESC to return to menu", SCREEN_WIDTH/2 - MeasureText("Press ESC to return to menu", 20)/2, 
                        SCREEN_HEIGHT/2 + 120, 20, BLACK);
                break;

            case GAME_STATE_HIGHSCORE:
                ClearBackground(PURPLE);
                DrawText("HIGHSCORE SCREEN", SCREEN_WIDTH/2 - MeasureText("HIGHSCORE", 40)/2, 
                         SCREEN_HEIGHT/2 - 40, 40, BLACK);
                DrawText("Press ESC to return to menu", SCREEN_WIDTH/2 - MeasureText("Press ESC to return to menu", 20)/2, 
                         SCREEN_HEIGHT/2 + 20, 20, BLACK);
                break;
        }
        
        EndDrawing();
    }
    
    // Clean up
    UnloadMainMenu();
    CloseAudioDevice();
    CloseWindow();
    
    return 0;
}