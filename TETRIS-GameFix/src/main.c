// Nama file : main.c
// Deskripsi : File utama yang menggabungkan semua komponen, menginisialisasi permainan, dan mengatur loop utama permainan.
// Oleh      : Ibnu Hilmi 241511079

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

#define HIGH_SCORE_FILE "assets/log/highscore.dat"

int main(void)
{
    SetExitKey(0); // Nonaktifkan tombol keluar bawaan
    // Inisialisasi jendela dengan konstanta yang konsisten
    InitWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "Tetris Game");
    SetTargetFPS(60);

    // Inisialisasi sistem audio
    InitAudioDevice();

    // Pengacakan seed
    srand(time(NULL));

    // Inisialisasi
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

    // Timer untuk jatuhnya blok secara otomatis
    float fallTimer = 0.0f;
    float fallDelay = 1.0f; // Waktu jatuh awal

    // Mulai dengan musik menu
    PlayBackgroundMusic(MUSIC_MENU);

    while (!WindowShouldClose())
    {
        BeginDrawing();
        ClearBackground(DARKGRAY);

        // Perbarui suara permainan
        UpdateGameSound();
        if (IsKeyPressed(KEY_ESCAPE))
        {
            MenuState currentMenuState = GetCurrentMenuState();

            if (currentMenuState == MENU_STATE_PLAY && inGame)
            {
                // If in gameplay, switch to pause
                SetMenuState(MENU_STATE_PAUSE);
            }
            else if (currentMenuState == MENU_STATE_PAUSE)
            {
                // If paused, resume gameplay
                SetMenuState(MENU_STATE_PLAY);
            }
            else if (currentMenuState == MENU_STATE_CREDITS ||
                     currentMenuState == MENU_STATE_HIGHSCORE)
            {
                // If in credits or highscore, return to main menu
                SetMenuState(MENU_STATE_MAIN);
            }
            // Note: In other states (like main menu), ESC won't do anything
        }

        MenuState currentMenuState = GetCurrentMenuState();

        // Menangani transisi status menu
        if (currentMenuState == MENU_STATE_PLAY)
        {
            if (!inGame)
            {
                // Mulai musik gameplay saat memasuki permainan
                PlayBackgroundMusic(MUSIC_GAMEPLAY);
                PlaySoundEffect(SOUND_CLICK);
            }
            inGame = true;

            // Handle ESC key for pausing
            if (IsKeyPressed(KEY_P))
            {
                SetMenuState(MENU_STATE_PAUSE);
                PlaySoundEffect(SOUND_CLICK);
            }
        }
        else if (currentMenuState == MENU_STATE_CREDITS)
        {
            // Perbaikan: Jangan menghentikan loop permainan, hanya tampilkan kredit
            inGame = false; // Pastikan kita tidak dalam permainan

            ClearBackground(LIGHTGRAY);
            DrawText("CREDITS", WINDOW_WIDTH / 2 - MeasureText("CREDITS", 40) / 2,
                     WINDOW_HEIGHT / 2 - 150, 40, BLACK);

            // Tampilkan nama dan ID
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

            // Enhanced back button:
            Rectangle backBtnCR = {
                WINDOW_WIDTH / 2 - 100,
                WINDOW_HEIGHT / 2 + 150, // Position it a bit lower
                200,
                50};

            DrawRectangleRec(backBtnCR, DARKBLUE);
            DrawRectangleLinesEx(backBtnCR, 3, SKYBLUE); // Thicker border

            // Draw text with shadow
            DrawText("BACK TO MENU", backBtnCR.x + (backBtnCR.width / 2) - MeasureText("BACK TO MENU", 20) / 2 + 2,
                     backBtnCR.y + 15 + 2, 20, BLACK); // Shadow
            DrawText("BACK TO MENU", backBtnCR.x + (backBtnCR.width / 2) - MeasureText("BACK TO MENU", 20) / 2,
                     backBtnCR.y + 15, 20, WHITE);

            // Hover effect
            if (CheckCollisionPointRec(GetMousePosition(), backBtnCR))
            {
                DrawRectangleRec(backBtnCR, Fade(BLUE, 0.7f)); // Highlight on hover
                DrawText("BACK TO MENU", backBtnCR.x + (backBtnCR.width / 2) - MeasureText("BACK TO MENU", 20) / 2,
                         backBtnCR.y + 15, 20, WHITE);

                // Check for click
                if (IsMouseButtonReleased(MOUSE_LEFT_BUTTON))
                {
                    SetMenuState(MENU_STATE_MAIN);
                    PlaySoundEffect(SOUND_CLICK);
                }
            }

            // Keep the ESC key functionality as a backup
            if (IsKeyPressed(KEY_ESCAPE))
            {
                SetMenuState(MENU_STATE_MAIN);
                PlaySoundEffect(SOUND_CLICK);
            }

            // Navigasi menu sekarang ditangani di UpdateMainMenu()
        }
        else if (currentMenuState == MENU_STATE_HIGHSCORE)
        {
            // Perbaikan: Jangan menghentikan loop permainan, hanya tampilkan skor tertinggi
            inGame = false;

            ClearBackground(LIGHTGRAY);
            DrawText("HIGHSCORES", WINDOW_WIDTH / 2 - MeasureText("HIGHSCORES", 40) / 2,
                     WINDOW_HEIGHT / 2 - 150, 40, BLACK);

            // Baca high score menggunakan fungsi dari scoring.c
            int highScore = LoadHighScore();

            // Tampilkan high score
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

            // Define back button with prominent styling
            Rectangle backBtnHS = {
                WINDOW_WIDTH / 2 - 100,
                WINDOW_HEIGHT / 2 + 150, // Position it a bit lower for better visibility
                200,
                50};

            // Use a more visible color scheme
            DrawRectangleRec(backBtnHS, DARKBLUE);
            DrawRectangleLinesEx(backBtnHS, 3, SKYBLUE); // Thicker border

            // Draw text with shadow for better visibility
            DrawText("BACK TO MENU", backBtnHS.x + (backBtnHS.width / 2) - MeasureText("BACK TO MENU", 20) / 2 + 2,
                     backBtnHS.y + 15 + 2, 20, BLACK); // Shadow
            DrawText("BACK TO MENU", backBtnHS.x + (backBtnHS.width / 2) - MeasureText("BACK TO MENU", 20) / 2,
                     backBtnHS.y + 15, 20, WHITE);

            // Check for mouse hover effect
            if (CheckCollisionPointRec(GetMousePosition(), backBtnHS))
            {
                DrawRectangleRec(backBtnHS, Fade(BLUE, 0.7f)); // Highlight on hover
                DrawText("BACK TO MENU", backBtnHS.x + (backBtnHS.width / 2) - MeasureText("BACK TO MENU", 20) / 2,
                         backBtnHS.y + 15, 20, WHITE);

                // Check for click
                if (IsMouseButtonReleased(MOUSE_LEFT_BUTTON))
                {
                    SetMenuState(MENU_STATE_MAIN);
                    PlaySoundEffect(SOUND_CLICK);
                }
            }

            // Keep the ESC key functionality as a backup
            if (IsKeyPressed(KEY_ESCAPE))
            {
                SetMenuState(MENU_STATE_MAIN);
                PlaySoundEffect(SOUND_CLICK);
            }
            // Navigasi menu sekarang ditangani di UpdateMainMenu()
        }
        else if (currentMenuState == MENU_STATE_EXIT)
        {
            // Keluar dari permainan
            break;
        }
        else if (currentMenuState == MENU_STATE_PAUSE)
        {
            // Don't update the game, but still draw it underneath
            DrawBoard(&board);
            DrawBlockShadow(&board.current_block, &board);
            DrawActiveTetromino(&board.current_block);
            DrawHoldBlock(&board);
            DrawNextBlock(&board);
            DrawScore(&board, &scoreData);

            // Draw pause menu overlay
            DrawPauseOverlay();

            // Handle pause menu interaction
            Vector2 mousePoint = GetMousePosition();

            // Define button rectangles (same as in DrawPauseOverlay)
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

            // Check for button clicks
            if (CheckCollisionPointRec(mousePoint, resumeBtn) && IsMouseButtonReleased(MOUSE_LEFT_BUTTON))
            {
                // Resume the game
                SetMenuState(MENU_STATE_PLAY);
                PlaySoundEffect(SOUND_CLICK);
            }

            if (CheckCollisionPointRec(mousePoint, exitBtn) && IsMouseButtonReleased(MOUSE_LEFT_BUTTON))
            {
                // Exit to main menu
                inGame = false;
                SetMenuState(MENU_STATE_MAIN);
                PlayBackgroundMusic(MUSIC_MENU);
                PlaySoundEffect(SOUND_CLICK);
            }

            if (IsKeyPressed(KEY_P))
            {
                SetMenuState(MENU_STATE_PAUSE);
                PlaySoundEffect(SOUND_CLICK);
            }
        }
        else if (currentMenuState == MENU_STATE_MAIN)
        {
            // Kita berada di menu utama
            inGame = false;

            if (wasPreviouslyInGame)
            {
                // Kita baru saja kembali ke menu dari gameplay
                PlayBackgroundMusic(MUSIC_MENU);
                wasPreviouslyInGame = false;
            }

            UpdateMainMenu();
            DrawMainMenu();
        }

        // Logika permainan - hanya berjalan jika kita dalam permainan
        if (inGame && !gameOver)
        {
            // Lacak jika kita baru saja memasuki permainan
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

            // Perbarui timer jatuh
            fallTimer += GetFrameTime();

            // Jatuhnya blok otomatis
            if (fallTimer >= fallDelay)
            {
                if (!MoveBlockDown(&board.current_block, &board))
                {
                    PlaceBlock(&board.current_block, &board);
                    board.current_block = board.next_block;
                    board.next_block = GenerateRandomBlock();

                    // Periksa game over
                    if (IsGameOver(&board.current_block, &board))
                    {
                        gameOver = true;
                        StopBackgroundMusic();
                        PlaySoundEffect(SOUND_GAME_OVER);
                    }
                }
                fallTimer = 0; // Reset timer
            }

            // Kontrol pemain
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

            // Mute/unmute musik latar belakang dengan tombol M
            if (IsKeyPressed(KEY_M))
            {
                ToggleBackgroundMusic();
            }

            // Bersihkan baris yang penuh
            int linesCleared = ClearFullLines(&board);
            if (linesCleared > 0)
            {
                AddLineClearScore(&scoreData, linesCleared);
                PlaySoundEffect(SOUND_LINE_CLEAR);
            }

            // Penggambaran

            DrawBlockShadow(&board.current_block, &board);

            DrawBoard(&board);
            DrawActiveTetromino(&board.current_block);

            DrawHoldBlock(&board);

            DrawNextBlock(&board);
            DrawScore(&board, &scoreData);
        }
        else if (gameOver)
        {
            // Game over text
                    DrawText("GAME OVER", WINDOW_WIDTH / 2 - MeasureText("GAME OVER", 40) / 2,
                             WINDOW_HEIGHT / 2 - 100, 40, RED);

            // Final score display
            DrawText(TextFormat("Final Score: %d", scoreData.score),
                     WINDOW_WIDTH / 2 - MeasureText(TextFormat("Final Score: %d", scoreData.score), 20) / 2,
                     WINDOW_HEIGHT / 2 - 40, 20, WHITE);

            // Restart button/text
            Rectangle restartBtn = {
                WINDOW_WIDTH / 2 - 100,
                WINDOW_HEIGHT / 2,
                200,
                50};

            DrawRectangleRec(restartBtn, GREEN);
            DrawRectangleLinesEx(restartBtn, 3, LIME); // Thicker border
            DrawText("RESTART", restartBtn.x + (restartBtn.width / 2) - MeasureText("RESTART", 20) / 2,
                     restartBtn.y + 15, 20, WHITE);

            // Back to menu button
            Rectangle backBtn = {
                WINDOW_WIDTH / 2 - 100,
                WINDOW_HEIGHT / 2 + 70,
                200,
                50};

            DrawRectangleRec(backBtn, BLUE);
            DrawRectangleLinesEx(backBtn, 3, SKYBLUE); // Thicker border
            DrawText("BACK TO MENU", backBtn.x + (backBtn.width / 2) - MeasureText("BACK TO MENU", 20) / 2,
                     backBtn.y + 15, 20, WHITE);

            // Check for button hover and clicks
            Vector2 mousePos = GetMousePosition();

            // Restart button hover and click
            if (CheckCollisionPointRec(mousePos, restartBtn))
            {
                DrawRectangleRec(restartBtn, Fade(GREEN, 0.8f)); // Highlight on hover
                DrawText("RESTART", restartBtn.x + (restartBtn.width / 2) - MeasureText("RESTART", 20) / 2,
                         restartBtn.y + 15, 20, WHITE);

                if (IsMouseButtonReleased(MOUSE_LEFT_BUTTON))
                {
                    SaveHighScore(&scoreData);
                    InitBoard1(&board);
                    InitScoring(&scoreData);
                    gameOver = false;
                    inGame = true; // Set this to true for restart
                    wasPreviouslyInGame = true;
                    fallTimer = 0;
                    PlaySoundEffect(SOUND_CLICK);
                    PlayBackgroundMusic(MUSIC_GAMEPLAY); // Use gameplay music

                    // Return to PLAY state instead of main menu
                    SetMenuState(MENU_STATE_PLAY);
                }
            }

            // Back button hover and click
            if (CheckCollisionPointRec(mousePos, backBtn))
            {
                DrawRectangleRec(backBtn, Fade(BLUE, 0.8f)); // Highlight on hover
                DrawText("BACK TO MENU", backBtn.x + (backBtn.width / 2) - MeasureText("BACK TO MENU", 20) / 2,
                         backBtn.y + 15, 20, WHITE);

                if (IsMouseButtonReleased(MOUSE_LEFT_BUTTON))
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

                    // Return to main menu
                    SetMenuState(MENU_STATE_MAIN);
                }
            }

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

                // Reset ke menu utama
                SetWindowState(MENU_STATE_MAIN);
            }
        }

        EndDrawing();
    }

    // Pembersihan
    UnloadMainMenu();
    UnloadGameSound();
    CloseAudioDevice();
    CloseWindow();

    return 0;
}