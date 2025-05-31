// Nama file : main.c
// Deskripsi : File utama yang menggabungkan semua komponen, menginisialisasi permainan, dan mengatur loop utama permainan.
// Oleh      : Ibnu Hilmi 241511079
//             Dzakir Tsabit 241511071 (github : dzhax4499)

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "include/tetris.h"
#include "include/board_linkedlist.h"
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
static Texture2D backgroundTexture;

bool paused = false;
char playerName[50] = "";
bool hasEnteredName = false;

LinkedBoard* linkedBoard = NULL;

int main(void)
{
    // Debug: Print initialization start
    printf("Starting Tetris Game initialization...\n");

    SetExitKey(0); // Nonaktifkan tombol keluar bawaan
    InitWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "Tetris Game");
    SetTargetFPS(60);

    // Check if audio initialization succeeds
    InitAudioDevice();
    if (!IsAudioDeviceReady())
    {
        printf("Warning: Audio device could not be initialized\n");
    }

    srand(time(NULL));

    // Initialize components with error checking

    printf(">> Initializing main menu...\n");
    InitMainMenu();

    printf(">> Initializing game sound...\n");
    InitGameSound();

    // Initialize game objects
    TetrisBoard board;
    printf(">> Initializing board...\n");
    printf(">> Alamat board utama di main: %p\n", &board);
    InitBlocks();
    printf(">> Inisialisasi blok-blok Tetris...\n");
    linkedBoard = InitLinkedBoard();
    ScoreData scoreData;
    printf(">> Initializing scoring...\n");
    InitScoring(&scoreData);

    Leaderboard leaderboard;
    printf(">> Initializing leaderboard...\n");
    InitLeaderboard(&leaderboard);

    // Game state variables
    bool inGame = false;
    bool gameOver = false;
    bool wasPreviouslyInGame = false;
    float fallTimer = 0.0f;
    float fallDelay = 1.0f; // Waktu jatuh awal

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

        if (currentMenuState == MENU_STATE_PLAY)
        {
            if (!hasEnteredName) {
                DrawText("ENTER YOUR NAME:", WINDOW_WIDTH / 2 - 150, WINDOW_HEIGHT / 2 - 60, 30, WHITE);
                DrawRectangle(WINDOW_WIDTH / 2 - 150, WINDOW_HEIGHT / 2, 300, 50, RAYWHITE);
                DrawText(playerName, WINDOW_WIDTH / 2 - 140, WINDOW_HEIGHT / 2 + 10, 30, BLACK);

                int key = GetCharPressed();
                while (key > 0) {
                    if (strlen(playerName) < 49 && key != '\n') {
                        int len = strlen(playerName);
                        playerName[len] = (char)key;
                        playerName[len + 1] = '\0';
                    }
                    key = GetCharPressed();
                }

                if (IsKeyPressed(KEY_BACKSPACE) && strlen(playerName) > 0) {
                    playerName[strlen(playerName) - 1] = '\0';
                }

                if (IsKeyPressed(KEY_ENTER) && strlen(playerName) > 0) {
                    hasEnteredName = true;
                }

                EndDrawing(); continue; // Tahan loop sampai nama diisi
            }

            if (!inGame)
            {
                printf(">> Masuk ke MENU_STATE_PLAY\n");

                PlayBackgroundMusic(MUSIC_GAMEPLAY);
                PlaySoundEffect(SOUND_CLICK);

                printf(">> InitBoard1\n");
                InitBoardWithLinkedList(&board, linkedBoard);
                ResetHoldCount(&board);
                printf(">> Hold count reset to 0\n");
                printf(">> InitScoring\n");
                InitScoring(&scoreData);
                printf(">> InitGameTimer\n");
                InitGameTimer();

                printf(">> Generate current_block\n");
                board.current_block = GenerateRandomBlock();
                // Validasi current_block
                if (board.current_block.type < 0 || board.current_block.type >= 7)
                {
                    printf("ERROR: Invalid current_block generated, type = %d\n", board.current_block.type);
                    // Fallback ke I-block
                    board.current_block.type = 0;
                    board.current_block = GenerateRandomBlock();
                }

                printf(">> Generate next_block\n");
                board.next_block = GenerateRandomBlock();
                // Validasi next_block
                if (board.next_block.type < 0 || board.next_block.type >= 7)
                {
                    printf("ERROR: Invalid next_block generated, type = %d\n", board.next_block.type);
                    // Fallback ke I-block
                    board.next_block.type = 0;
                    board.next_block = GenerateRandomBlock();
                }

                printf(">> SELESAI semua setup\n");
            }
            inGame = true;
            paused = false;

            if (IsKeyPressed(KEY_P))
            {
                SetMenuState(MENU_STATE_PAUSE);
                ResumeGameTimer();
                PlaySoundEffect(SOUND_CLICK);
            }
        }
        else if (currentMenuState == MENU_STATE_CREDITS)
        {
            inGame = false;

            // Draw background texture
            DrawTextureEx(backgroundTexture, (Vector2){0, 0}, 0.0f, 1.0f, WHITE);
            backgroundTexture = LoadTexture("assets/textures/bg.png");

            // Create panel for Credits
            Rectangle panel = {
                WINDOW_WIDTH / 2 - 250,
                WINDOW_HEIGHT / 2 - 210,
                500,
                425
            };

            // Draw panel with background
            DrawRectangleRec(panel, Fade(DARKBLUE, 0.9f));
            DrawRectangleLinesEx(panel, 3, SKYBLUE);

            // Draw title
            DrawText("CREDITS", panel.x + panel.width/2 - MeasureText("CREDITS", 38)/2, 
                     panel.y + 20, 40, YELLOW);
            
            // Display team members
            DrawText("241511071     Dzakir Tsabit Asy-syafiq", panel.x + 50, panel.y + 100, 20, WHITE);
            DrawText("241511074     Fatimah Hawwa Alkhansa", panel.x + 50, panel.y + 140, 20, WHITE);
            DrawText("241511079     Ibnu Hilmi Athaillah", panel.x + 50, panel.y + 180, 20, WHITE);
            DrawText("241511089     Rizky Satria Gunawan", panel.x + 50, panel.y + 220, 20, WHITE);
            DrawText("241511091     Varian Abidarma Syuhada", panel.x + 50, panel.y + 260, 20, WHITE);

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

            DrawTextureEx(backgroundTexture, (Vector2){0, 0}, 0.0f, 1.0f, WHITE);
            backgroundTexture = LoadTexture("assets/textures/bg.png");

            Rectangle panel = {
                WINDOW_WIDTH / 2 - 200,
                WINDOW_HEIGHT / 2 - 200,
                400,
                350
            };

            // Draw panel with background
            DrawRectangleRec(panel, Fade(DARKBLUE, 0.9f));
            DrawRectangleLinesEx(panel, 2, SKYBLUE);

            DrawText("HIGHSCORES", panel.x + panel.width/2 - MeasureText("HIGHSCORE", 43)/2, 
                     panel.y + 20, 40, YELLOW);

            // Baca dan tampilkan high score
            int highScore = LoadGameHighScore();
            int yPos = panel.y + 100;

            if (highScore > 0)
            {
                char scoreText[50];
                sprintf(scoreText, "Highest Score: %d", highScore);
                DrawText(scoreText, panel.x + 50, yPos, 30, WHITE);
            }
            else
            {
                DrawText("No high scores yet!", panel.x + 50, yPos, 20, WHITE);
            }

            // Tombol kembali ke menu utama
            Rectangle backBtnHS = {
                WINDOW_WIDTH / 2 - 100,
                WINDOW_HEIGHT / 2 + 50,
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
                PauseGameTimer();
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
                PlayBackgroundMusic(MUSIC_GAMEPLAY);
                ResumeGameTimer();
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
                    PlaceBlockToLinkedBoard(&board.current_block, &board, linkedBoard);

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

                        // Simpan skor di sini, saat game over
                        AddOrUpdateLeaderboard(&leaderboard, scoreData.score, scoreData.level, playerName, GetElapsedGameTime());
                    }
                    else
                    {
                        // Reset fall timer for next block
                        fallTimer = 0;
                        PlaySoundEffect(SOUND_CLICK);
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
                RotateBlockWithWallKick(&board.current_block, &board);
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
                 if (CanUseHold(&board))
                {
                    HoldCurrentBlock(&board);
                    PlaySoundEffect(SOUND_CLICK);
                    printf("Hold digunakan! Sisa: %d kali\n", GetRemainingHolds(&board));
                }
                else
                {
                    PlaySoundEffect(SOUND_GAME_OVER); // or create SOUND_ERROR
                    printf("Hold sudah habis! Tidak bisa hold lagi.\n");
                }
            }

            // Toggle music
            if (IsKeyPressed(KEY_M))
            {
                ToggleBackgroundMusic();
            }

            // Clear full lines
            int linesCleared = ClearFullLinesLinked(&board, linkedBoard);
            if (linesCleared > 0)
            {
                AddLineClearScore(&scoreData, linesCleared);
                CheckLevelUp(&scoreData);
                PlaySoundEffect(SOUND_LINE_CLEAR);
                PrintLinkedBoard(linkedBoard);
            }

            printf(">> Mulai render elemen\n");

            // Coba satu-satu
            DrawBoard(&board);
            printf(">> Setelah DrawBoard\n");

            DrawActiveTetromino(&board.current_block);
            printf(">> Setelah DrawActiveTetromino\n");

            DrawBlockShadow(&board.current_block, &board);
            printf(">> Setelah DrawBlockShadow\n");

            DrawHoldBlock(&board);
            printf(">> Setelah DrawHoldBlock\n");

            DrawNextBlock(&board);
            printf(">> Setelah DrawNextBlock\n");

            DrawScore(&board, &scoreData);
            printf(">> Setelah DrawScore\n");

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
                printf("Kursor Disimpan diatas tombol\n");

                if (IsMouseButtonReleased(MOUSE_LEFT_BUTTON))
                {
                    printf("kursor restart game dilepas\n");
                    InitBoard1(&board);
                    InitScoring(&scoreData);
                    InitGameTimer();
                    
                    // Generate new current and next block
                    printf("Before GenerateRandomBlock\n");
                    board.current_block = GenerateRandomBlock();
                    board.next_block = GenerateRandomBlock();
                    printf("After GenerateRandomBlock\n");

                    gameOver = false;
                    inGame = true;
                    wasPreviouslyInGame = true;
                    fallTimer = 0;
                    PlaySoundEffect(SOUND_CLICK);
                    PlayBackgroundMusic(MUSIC_GAMEPLAY);
                    SetMenuState(MENU_STATE_PLAY);
                    printf("Restart game initiated\n");
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
                    //AddOrUpdateLeaderboard(&leaderboard, scoreData.score, scoreData.level, playerName, GetElapsedGameTime());
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
                    //AddOrUpdateLeaderboard(&leaderboard, scoreData.score, scoreData.level, playerName, GetElapsedGameTime());
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
    if (linkedBoard) {
        CleanupLinkedBoard(linkedBoard);
        linkedBoard = NULL;
    }
    UnloadMainMenu();
    UnloadGameSound();
    CloseAudioDevice();
    CloseWindow();

    return 0;
}