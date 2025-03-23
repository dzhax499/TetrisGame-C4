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

int main(void)
{
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

        MenuState currentMenuState = GetCurrentMenuState();
        
        // Menangani transisi status menu
        if (currentMenuState == MENU_STATE_PLAY)
        {
            if (!inGame) {
                // Mulai musik gameplay saat memasuki permainan
                PlayBackgroundMusic(MUSIC_GAMEPLAY);
                PlaySoundEffect(SOUND_CLICK);
            }
            inGame = true;
        }
        else if (currentMenuState == MENU_STATE_CREDITS)
        {
            // Perbaikan: Jangan menghentikan loop permainan, hanya tampilkan kredit
            inGame = false; // Pastikan kita tidak dalam permainan
            
            ClearBackground(LIGHTGRAY);
            DrawText("CREDITS", WINDOW_WIDTH/2 - MeasureText("CREDITS", 40)/2, 
                WINDOW_HEIGHT/2 - 150, 40, BLACK);

            // Tampilkan nama dan ID
            DrawText("Dzakir Tsabit \t\t 241511071", WINDOW_WIDTH/2 - MeasureText("Dzakir Tsabit 241511071", 20)/2, 
                WINDOW_HEIGHT/2 - 50, 20, BLACK);
            DrawText("Fatimah Hawwa \t\t 241511074", WINDOW_WIDTH/2 - MeasureText("Fatimah Hawwa 241511074", 20)/2, 
                WINDOW_HEIGHT/2 - 20, 20, BLACK);
            DrawText("Ibnu Hilmi \t\t\t\t 241511079", WINDOW_WIDTH/2 - MeasureText("Ibnu Hilmi 241511079", 20)/2, 
                WINDOW_HEIGHT/2 - -10, 20, BLACK);
            DrawText("Rizky Satria \t\t\t 241511089", WINDOW_WIDTH/2 - MeasureText("Rizky Satria 241511089", 20)/2, 
                WINDOW_HEIGHT/2 - -40, 20, BLACK);
            DrawText("Varian Abidarma \t 241511091", WINDOW_WIDTH/2 - MeasureText("Varian Abidarma 241511091", 20)/2,
                WINDOW_HEIGHT/2 - -70, 20, BLACK);


            DrawText("Press ESC to return to menu", WINDOW_WIDTH/2 - MeasureText("Press ESC to return to menu", 20)/2, 
                WINDOW_HEIGHT/2 + 120, 20, BLACK);

            // Navigasi menu sekarang ditangani di UpdateMainMenu()
        }
        else if (currentMenuState == MENU_STATE_HIGHSCORE)
        {
            // Perbaikan: Jangan menghentikan loop permainan, hanya tampilkan skor tertinggi
            inGame = false; // Pastikan kita tidak dalam permainan
            
            ClearBackground(LIGHTGRAY);
            DrawText("HIGHSCORES", WINDOW_WIDTH/2 - MeasureText("HIGHSCORES", 40)/2, 
                WINDOW_HEIGHT/2 - 150, 40, BLACK);
            
            // Buka, baca, dan tampilkan skor tertinggi dari highscore.dat
            FILE *fileHighscore = fopen("assets/log/highscore.dat", "rb");
            
            if (fileHighscore)
            {
                // Definisikan struktur untuk data skor tertinggi
                typedef struct {
                    char name[30];
                    int score;
                } HighscoreData;
                
                HighscoreData highscores[5]; // Asumsikan kita menyimpan 5 skor teratas
                int count = 0;
                
                // Baca data skor tertinggi dari file
                count = fread(highscores, sizeof(HighscoreData), 5, fileHighscore);
                
                // Tutup file setelah membaca
                fclose(fileHighscore);
                
                // Tampilkan skor tertinggi
                if (count > 0)
                {
                    for (int i = 0; i < count; i++)
                    {
                        char scoreText[50];
                        sprintf(scoreText, "%d. %s - %d", i+1, highscores[i].name, highscores[i].score);
                        DrawText(scoreText, WINDOW_WIDTH/2 - MeasureText(scoreText, 25)/2, 
                            WINDOW_HEIGHT/2 - 80 + (i * 40), 25, BLACK);
                    }
                }
                else
                {
                    DrawText("No high scores yet!", WINDOW_WIDTH/2 - MeasureText("No high scores yet!", 30)/2, 
                        WINDOW_HEIGHT/2 - 30, 30, BLACK);
                }
            }
            else
            {
                DrawText("Highscore file not found!", WINDOW_WIDTH/2 - MeasureText("Highscore file not found!", 30)/2, 
                    WINDOW_HEIGHT/2 - 30, 30, BLACK);
            }
            
            DrawText("Press ESC to return to menu", WINDOW_WIDTH/2 - MeasureText("Press ESC to return to menu", 20)/2, 
                WINDOW_HEIGHT/2 + 120, 20, BLACK);
            
            // Navigasi menu sekarang ditangani di UpdateMainMenu()
        }
        else if (currentMenuState == MENU_STATE_EXIT)
        {
            // Keluar dari permainan
            break;
        }
        else if (currentMenuState == MENU_STATE_MAIN)
        {
            // Kita berada di menu utama
            inGame = false;
            
            if (wasPreviouslyInGame) {
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
            if (!wasPreviouslyInGame) {
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
                
                #ifdef SOUND_LINE_CLEAR
                    PlaySoundEffect(SOUND_LINE_CLEAR);
                #else
                    PlaySoundEffect(SOUND_CLICK);
                #endif
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