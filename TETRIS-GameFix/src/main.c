// Nama file : main.c
// Deskripsi : File utama yang menggabungkan semua komponen, menginisialisasi permainan, dan mengatur loop utama permainan.
// Oleh      : Ibnu Hilmi 241511079
//             Dzakir Tsabit 241511071 (github : dzhax4499)

#include "include/tetris.h"
#include "include/board.h"
#include "include/blocks.h"
#include "include/rendering.h"
#include "include/scoring.h"
#include "include/main_menu.h"
#include "include/game_sound.h"
#include "include/timer.h"
#include "include/leaderboard.h"
#include "raylib.h"
#include "include/linkedlist_block.h"
#include <time.h>

#define WINDOW_WIDTH 1024
#define WINDOW_HEIGHT 768
#define HIGH_SCORE_FILE "assets/log/highscore.dat"

bool paused = false;

int main(void)
{
    // Debug: Print initialization start
    printf("Starting Tetris Game initialization...\n");

    SetExitKey(0); // Nonaktifkan tombol keluar bawaan
    InitWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "Tetris Game");
    SetTargetFPS(60);
    
    // Check if audio initialization succeeds
    InitAudioDevice();
    if (!IsAudioDeviceReady()) {
        printf("Warning: Audio device could not be initialized\n");
    }

    srand(time(NULL));

    // Initialize components with error checking
    printf("Initializing blocks...\n");
    InitBlocks();
    
    printf("Initializing main menu...\n");
    InitMainMenu();
    
    printf("Initializing game sound...\n");
    InitGameSound();

    // Initialize game objects
    TetrisBoard board;
    printf("Initializing board...\n");
    InitBoard1(&board);

    ScoreData scoreData;
    printf("Initializing scoring...\n");
    InitScoring(&scoreData);

    Leaderboard leaderboard;
    printf("Initializing leaderboard...\n");
    InitLeaderboard(&leaderboard);

    // Game state variables
    bool inGame = false;
    bool gameOver = false;
    bool wasPreviouslyInGame = false;
    float fallTimer = 0.0f;
    float fallDelay = 1.0f; // Waktu jatuh awal

    MenuState lastMenuState = MENU_STATE_MAIN;

    printf("Starting background music...\n");
    PlayBackgroundMusic(MUSIC_MENU);
    
    printf("Entering main game loop...\n");

    while (!WindowShouldClose())
    {
        BeginDrawing();
        ClearBackground(DARKGRAY);

        // Update audio and timer
        UpdateGameSound();
        UpdateGameTimer();

        // Handle ESC key
        if (IsKeyPressed(KEY_ESCAPE))
        {
            MenuState currentMenuState = GetCurrentMenuState();

            if (currentMenuState == MENU_STATE_PLAY && inGame)
            {
                SetMenuState(MENU_STATE_PAUSE);
            }
            else if (currentMenuState == MENU_STATE_PAUSE)
            {
                SetMenuState(MENU_STATE_PLAY);
            }
            else if (currentMenuState == MENU_STATE_CREDITS ||
                     currentMenuState == MENU_STATE_HIGHSCORE ||
                     currentMenuState == MENU_STATE_LEADERBOARD)
            {
                SetMenuState(MENU_STATE_MAIN);
            }
        }

        MenuState currentMenuState = GetCurrentMenuState();
        if (lastMenuState != currentMenuState) {
            if (currentMenuState == MENU_STATE_PAUSE) {
                PauseGameTimer();
            } else if (currentMenuState == MENU_STATE_PLAY && lastMenuState == MENU_STATE_PAUSE) {
                ResumeGameTimer();
            }
            lastMenuState = currentMenuState;
}


        if (currentMenuState == MENU_STATE_PLAY)
        {
            if (!inGame)
            {
                printf("Entering PLAY state...\n");
                PlayBackgroundMusic(MUSIC_GAMEPLAY);
                PlaySoundEffect(SOUND_CLICK);
                
                // Reinitialize game objects when entering play mode
                printf("Reinitializing game objects for new game...\n");
                InitBoard1(&board);
                InitScoring(&scoreData);
                InitGameTimer();
                gameOver = false;
                fallTimer = 0.0f;
                fallDelay = 1.0f;
                
                printf("Game objects reinitialized successfully\n");
            }
            inGame = true;
            paused = false;

            if (IsKeyPressed(KEY_P))
            {
                SetMenuState(MENU_STATE_PAUSE);
                PlaySoundEffect(SOUND_CLICK);
            }
        }
        else if (currentMenuState == MENU_STATE_CREDITS)
        {
            inGame = false;

            ClearBackground(LIGHTGRAY);
            DrawText("CREDITS", WINDOW_WIDTH / 2 - MeasureText("CREDITS", 40) / 2,
                     WINDOW_HEIGHT / 2 - 150, 40, BLACK);

            // Tampilkan nama dan ID anggota tim
            DrawText("Dzakir Tsabit \t\t 241511071", WINDOW_WIDTH / 2 - MeasureText("Dzakir Tsabit 241511071", 20) / 2,
                     WINDOW_HEIGHT / 2 - 50, 20, BLACK);
            DrawText("Fatimah Hawwa \t\t 241511074", WINDOW_WIDTH / 2 - MeasureText("Fatimah Hawwa 241511074", 20) / 2,
                     WINDOW_HEIGHT / 2 - 20, 20, BLACK);
            DrawText("Ibnu Hilmi \t\t\t\t 241511079", WINDOW_WIDTH / 2 - MeasureText("Ibnu Hilmi 241511079", 20) / 2,
                     WINDOW_HEIGHT / 2 - -10, 20, BLACK);
            DrawText("Rizky Satria \t\t\t 241511089", WINDOW_WIDTH / 2 - MeasureText("Rizky Satria 241511089", 20) / 2,
                     WINDOW_HEIGHT / 2 - -40, 20, BLACK);
            DrawText("Varian Abidarma \t 241511091", WINDOW_WIDTH / 2 - MeasureText("Varian Abidarma 241511091", 20) / 2,
                     WINDOW_HEIGHT / 2 - -70, 20, BLACK);

            // Tombol kembali ke menu utama
            Rectangle backBtnCR = {
                WINDOW_WIDTH / 2 - 100,
                WINDOW_HEIGHT / 2 + 150,
                200,
                50};

            DrawRectangleRec(backBtnCR, DARKBLUE);
            DrawRectangleLinesEx(backBtnCR, 3, SKYBLUE);
            DrawText("BACK TO MENU", backBtnCR.x + (backBtnCR.width / 2) - MeasureText("BACK TO MENU", 20) / 2 + 2,
                     backBtnCR.y + 15 + 2, 20, BLACK);
            DrawText("BACK TO MENU", backBtnCR.x + (backBtnCR.width / 2) - MeasureText("BACK TO MENU", 20) / 2,
                     backBtnCR.y + 15, 20, WHITE);

            // Efek hover tombol
            if (CheckCollisionPointRec(GetMousePosition(), backBtnCR))
            {
                DrawRectangleRec(backBtnCR, Fade(BLUE, 0.7f));
                DrawText("BACK TO MENU", backBtnCR.x + (backBtnCR.width / 2) - MeasureText("BACK TO MENU", 20) / 2,
                         backBtnCR.y + 15, 20, WHITE);

                if (IsMouseButtonReleased(MOUSE_LEFT_BUTTON))
                {
                    SetMenuState(MENU_STATE_MAIN);
                    PlaySoundEffect(SOUND_CLICK);
                }
            }
        
            if (IsKeyPressed(KEY_ESCAPE))
            {
                SetMenuState(MENU_STATE_MAIN);
                PlaySoundEffect(SOUND_CLICK);
            }
        }
        else if (currentMenuState == MENU_STATE_HIGHSCORE)
        {
            inGame = false;

            ClearBackground(LIGHTGRAY);
            DrawText("HIGHSCORES", WINDOW_WIDTH / 2 - MeasureText("HIGHSCORES", 40) / 2,
                     WINDOW_HEIGHT / 2 - 150, 40, BLACK);

            // Baca dan tampilkan high score
            int highScore = LoadGameHighScore();

            if (highScore > 0)
            {
                char scoreText[50];
                sprintf(scoreText, "Highest Score: %d", highScore);
                DrawText(scoreText, WINDOW_WIDTH / 2 - MeasureText(scoreText, 30) / 2,
                         WINDOW_HEIGHT / 2 - 30, 30, BLACK);
            }
            else
            {
                DrawText("No high scores yet!", WINDOW_WIDTH / 2 - MeasureText("No high scores yet!", 30) / 2,
                         WINDOW_HEIGHT / 2 - 30, 30, BLACK);
            }

            // Tombol kembali ke menu utama
            Rectangle backBtnHS = {
                WINDOW_WIDTH / 2 - 100,
                WINDOW_HEIGHT / 2 + 150,
                200,
                50};

            DrawRectangleRec(backBtnHS, DARKBLUE);
            DrawRectangleLinesEx(backBtnHS, 3, SKYBLUE);
            DrawText("BACK TO MENU", backBtnHS.x + (backBtnHS.width / 2) - MeasureText("BACK TO MENU", 20) / 2 + 2,
                     backBtnHS.y + 15 + 2, 20, BLACK);
            DrawText("BACK TO MENU", backBtnHS.x + (backBtnHS.width / 2) - MeasureText("BACK TO MENU", 20) / 2,
                     backBtnHS.y + 15, 20, WHITE);

            // Efek hover tombol
            if (CheckCollisionPointRec(GetMousePosition(), backBtnHS))
            {
                DrawRectangleRec(backBtnHS, Fade(BLUE, 0.7f));
                DrawText("BACK TO MENU", backBtnHS.x + (backBtnHS.width / 2) - MeasureText("BACK TO MENU", 20) / 2,
                         backBtnHS.y + 15, 20, WHITE);

                if (IsMouseButtonReleased(MOUSE_LEFT_BUTTON))
                {
                    SetMenuState(MENU_STATE_MAIN);
                    PlaySoundEffect(SOUND_CLICK);
                }
            }

            if (IsKeyPressed(KEY_ESCAPE))
            {
                SetMenuState(MENU_STATE_MAIN);
                PlaySoundEffect(SOUND_CLICK);
            }
        }
        else if (currentMenuState == MENU_STATE_LEADERBOARD)
        {
            inGame = false;
            ClearBackground(LIGHTGRAY);
            UpdateMainMenu();
            DrawMainMenu();
            DisplayLeaderboard(&leaderboard, WINDOW_WIDTH, WINDOW_HEIGHT);
        }
        else if (currentMenuState == MENU_STATE_EXIT)
        {
            break;
        }
        else if (currentMenuState == MENU_STATE_PAUSE)
        {
            if (!paused)
            {
                paused = true;
                StopBackgroundMusic();
                PlaySoundEffect(SOUND_CLICK);
            }
            // Tetap menggambar elemen permainan tanpa memperbarui logika
            DrawBoard(&board);
            DrawBlockShadow(&board.current_block, &board);
            DrawActiveTetromino(&board.current_block);
            DrawHoldBlock(&board);
            DrawNextBlock(&board);
            DrawScore(&board, &scoreData);

            // Tampilkan overlay menu jeda
            DrawPauseOverlay();

            // Mendefinisikan tombol-tombol jeda
            Vector2 mousePoint = GetMousePosition();
            Rectangle resumeBtn = {
                WINDOW_WIDTH / 2 - 100,
                WINDOW_HEIGHT / 2 - 20,
                200,
                50};

            Rectangle exitBtn = {
                WINDOW_WIDTH / 2 - 100,
                WINDOW_HEIGHT / 2 + 50,
                200,
                50};

            // Pengecekan klik tombol Resume
            if (CheckCollisionPointRec(mousePoint, resumeBtn) && IsMouseButtonReleased(MOUSE_LEFT_BUTTON))
            {
                SetMenuState(MENU_STATE_PLAY);
                PlaySoundEffect(SOUND_CLICK);
            }

            // Pengecekan klik tombol Exit
            if (CheckCollisionPointRec(mousePoint, exitBtn) && IsMouseButtonReleased(MOUSE_LEFT_BUTTON))
            {
                inGame = false;
                SetMenuState(MENU_STATE_MAIN);
                paused = false;
                PlayBackgroundMusic(MUSIC_MENU);
                PlaySoundEffect(SOUND_CLICK);
            }

            // Alternatif menggunakan tombol P untuk pindah ke menu jeda
            if (IsKeyPressed(KEY_P))
            {
                SetMenuState(MENU_STATE_PAUSE);
                PlaySoundEffect(SOUND_CLICK);
            }
        }
        else if (currentMenuState == MENU_STATE_MAIN)
        {
            inGame = false;

            if (wasPreviouslyInGame)
            {
                PlayBackgroundMusic(MUSIC_MENU);
                wasPreviouslyInGame = false;
            }

            UpdateMainMenu();
            DrawMainMenu();
        }

        // Main game logic
        if (inGame && !gameOver && !paused)
        {
            if (!wasPreviouslyInGame)
            {
                wasPreviouslyInGame = true;
            }

            // Perbarui kecepatan jatuh berdasarkan level saat ini
#ifdef HAVE_UPDATE_FALL_SPEED
            fallDelay = UpdateFallSpeed(&scoreData);
#else
            fallDelay = 1.0f - ((scoreData.level - 1) * 0.15f);
            if (fallDelay < 0.1f)
                fallDelay = 0.1f;
#endif

            // Perbarui timer jatuh dan logika penjatuhan blok otomatis
            fallTimer += GetFrameTime();

            if (fallTimer >= fallDelay)
            {
                if (!MoveBlockDown(&board.current_block, &board))
                {
                    // Block has landed, place it
                    PlaceBlock(&board.current_block, &board);
                    
                    // Generate new blocks
                    board.current_block = board.next_block;
                    board.next_block = GenerateRandomBlock();

                    // Check for game over
                    if (IsGameOver(&board.current_block, &board))
                    {
                        printf("Game Over detected!\n");
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

            if (IsKeyPressed(KEY_UP))
            {
                RotateBlock(&board.current_block, &board);
                PlaySoundEffect(SOUND_CLICK);
            }

            if (IsKeyPressed(KEY_DOWN))
                MoveBlockDown(&board.current_block, &board);

            if (IsKeyPressed(KEY_SPACE))
            {
                int dropDistance = CalculateDropDistance(&board.current_block, &board);
                board.current_block.y += dropDistance;
                AddDropScore(&scoreData, dropDistance);
                PlaceBlock(&board.current_block, &board);

                board.current_block = board.next_block;
                board.next_block = GenerateRandomBlock();
                PlaySoundEffect(SOUND_CLICK);
            }

            if (IsKeyPressed(KEY_C))
            {
                HoldCurrentBlock(&board);
                PlaySoundEffect(SOUND_CLICK);
            }

            // Toggle music
            if (IsKeyPressed(KEY_M))
            {
                ToggleBackgroundMusic();
            }

            // Clear full lines
            int linesCleared = ClearFullLines(&board);
            if (linesCleared > 0)
            {
                AddLineClearScore(&scoreData, linesCleared);
                CheckLevelUp(&scoreData);
                PlaySoundEffect(SOUND_LINE_CLEAR);
            }

            // Draw game elements
            DrawBlockShadow(&board.current_block, &board);
            DrawBoard(&board);
            DrawActiveTetromino(&board.current_block);
            DrawHoldBlock(&board);
            DrawNextBlock(&board);
            DrawScore(&board, &scoreData);

            // Control tips panel
            Rectangle controlTipsRect = {
                10,                  // Posisi X
                WINDOW_HEIGHT - 180, // Posisi Y
                220,                 // Lebar
                170                  // Tinggi
            };

            DrawRectangleRec(controlTipsRect, Fade(BLACK, 0.7f));
            DrawRectangleLinesEx(controlTipsRect, 2, WHITE);

            DrawText("KONTROL PEMAIN", controlTipsRect.x + 10, controlTipsRect.y + 10, 18, GOLD);

            int textY = controlTipsRect.y + 40;
            int textX = controlTipsRect.x + 15;
            int fontSize = 16;
            Color textColor = RAYWHITE;

            DrawText("< / >: Gerak Kiri/Kanan", textX, textY - 10, fontSize, textColor);
            DrawText("^    : Rotasi Blok", textX, textY + 10, fontSize, textColor);
            DrawText("v    : Turun Cepat", textX, textY + 30, fontSize, textColor);
            DrawText("SPACE: Hard Drop", textX, textY + 50, fontSize, textColor);
            DrawText("C    : Hold Block", textX, textY + 70, fontSize, textColor);
            DrawText("P/ESC: Pause Game", textX, textY + 90, fontSize, textColor);
            DrawText("M    : Mute/Unmute Musik", textX, textY + 110, fontSize, textColor);
        }
        else if (gameOver)
        {
            // Game over screen
            DrawText("GAME OVER", WINDOW_WIDTH / 2 - MeasureText("GAME OVER", 40) / 2,
                     WINDOW_HEIGHT / 2 - 100, 40, RED);
            DrawText(TextFormat("Final Score: %d", scoreData.score),
                     WINDOW_WIDTH / 2 - MeasureText(TextFormat("Final Score: %d", scoreData.score), 20) / 2,
                     WINDOW_HEIGHT / 2 - 40, 20, WHITE);

            // Restart button
            Rectangle restartBtn = {
                WINDOW_WIDTH / 2 - 100,
                WINDOW_HEIGHT / 2,
                200,
                50};

            DrawRectangleRec(restartBtn, GREEN);
            DrawRectangleLinesEx(restartBtn, 3, LIME);
            DrawText("RESTART", restartBtn.x + (restartBtn.width / 2) - MeasureText("RESTART", 20) / 2,
                     restartBtn.y + 15, 20, WHITE);

            // Back to menu button
            Rectangle backBtn = {
                WINDOW_WIDTH / 2 - 100,
                WINDOW_HEIGHT / 2 + 70,
                200,
                50};

            DrawRectangleRec(backBtn, BLUE);
            DrawRectangleLinesEx(backBtn, 3, SKYBLUE);
            DrawText("BACK TO MENU", backBtn.x + (backBtn.width / 2) - MeasureText("BACK TO MENU", 20) / 2,
                     backBtn.y + 15, 20, WHITE);

            // Leaderboard button
            Rectangle scoreBtn = {
                WINDOW_WIDTH / 2 - 100,
                WINDOW_HEIGHT / 2 + 140,
                200,
                50};

            DrawRectangleRec(scoreBtn, PURPLE);
            DrawRectangleLinesEx(scoreBtn, 3, DARKPURPLE);
            DrawText("LEADERBOARD", scoreBtn.x + (scoreBtn.width / 2) - MeasureText("LEADERBOARD", 20) / 2,
                     scoreBtn.y + 15, 20, WHITE);

            Vector2 mousePos = GetMousePosition();

            // Restart button handling
            if (CheckCollisionPointRec(mousePos, restartBtn))
            {
                DrawRectangleRec(restartBtn, Fade(GREEN, 0.8f));
                DrawText("RESTART", restartBtn.x + (restartBtn.width / 2) - MeasureText("RESTART", 20) / 2,
                         restartBtn.y + 15, 20, WHITE);

                if (IsMouseButtonReleased(MOUSE_LEFT_BUTTON))
                {
                    SaveGameScore(&scoreData);
                    InitBoard1(&board);
                    InitScoring(&scoreData);
                    InitGameTimer();    
                    gameOver = false;
                    inGame = true;
                    wasPreviouslyInGame = true;
                    fallTimer = 0;
                    PlaySoundEffect(SOUND_CLICK);
                    PlayBackgroundMusic(MUSIC_GAMEPLAY);
                    SetMenuState(MENU_STATE_PLAY);
                }
            }

            // Back to menu button handling
            if (CheckCollisionPointRec(mousePos, backBtn))
            {
                DrawRectangleRec(backBtn, Fade(BLUE, 0.8f));
                DrawText("BACK TO MENU", backBtn.x + (backBtn.width / 2) - MeasureText("BACK TO MENU", 20) / 2,
                         backBtn.y + 15, 20, WHITE);

                if (IsMouseButtonReleased(MOUSE_LEFT_BUTTON))
                {
                    SaveGameScore(&scoreData);
                    InitBoard1(&board);
                    InitScoring(&scoreData);
                    InitGameTimer();
                    gameOver = false;
                    inGame = false;
                    wasPreviouslyInGame = false;
                    fallTimer = 0;
                    PlaySoundEffect(SOUND_CLICK);
                    PlayBackgroundMusic(MUSIC_MENU);
                    SetMenuState(MENU_STATE_MAIN);
                }
            }

            // Leaderboard button handling
            if (CheckCollisionPointRec(mousePos, scoreBtn))
            {
                DrawRectangleRec(scoreBtn, Fade(PURPLE, 0.8f));
                DrawText("LEADERBOARD", scoreBtn.x + (scoreBtn.width / 2) - MeasureText("LEADERBOARD", 20) / 2,
                         scoreBtn.y + 15, 20, WHITE);

                if (IsMouseButtonReleased(MOUSE_LEFT_BUTTON))
                {
                    AddLeaderboard(&leaderboard, scoreData.score, scoreData.level);
                    SaveGameScore(&scoreData);
                    InitBoard1(&board);
                    InitScoring(&scoreData);
                    gameOver = false;
                    inGame = false;
                    wasPreviouslyInGame = false;
                    fallTimer = 0;
                    SetMenuState(MENU_STATE_LEADERBOARD);
                    PlaySoundEffect(SOUND_CLICK);
                }
            }

            SaveGameScore(&scoreData);

            // Restart with R key
            if (IsKeyPressed(KEY_R))
            {
                InitBoard1(&board);
                InitScoring(&scoreData);
                InitGameTimer();
                gameOver = false;
                inGame = false;
                wasPreviouslyInGame = false;
                fallTimer = 0;
                PlaySoundEffect(SOUND_CLICK);
                PlayBackgroundMusic(MUSIC_MENU);
                SetMenuState(MENU_STATE_MAIN);
            }
        }

        EndDrawing();
    }

    // Cleanup
    printf("Cleaning up resources...\n");
    UnloadMainMenu();
    UnloadGameSound();
    CloseAudioDevice();
    CloseWindow();

    return 0;
}