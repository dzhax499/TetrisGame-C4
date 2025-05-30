// Nama file : main.c
// Deskripsi : File utama yang menggabungkan semua komponen, menginisialisasi permainan, dan mengatur loop utama permainan.
// Oleh      : Ibnu Hilmi 241511079
//             Dzakir Tsabit 241511071

/**
 * ===================================
 * KOMENTAR PENJELASAN KODE PROGRAM
 * ===================================
 */

/**
 * Include Header Files
 * -------------------
 * Header-header berikut mencakup semua komponen game:
 * - tetris.h: Definisi utama dan konstanta game
 * - board.h: Struktur dan fungsi untuk papan permainan
 * - blocks.h: Definisi dan fungsi untuk blok Tetris
 * - rendering.h: Fungsi-fungsi untuk menggambar elemen game
 * - scoring.h: Sistem skor dan level
 * - main_menu.h: Sistem menu game
 * - game_sound.h: Sistem audio dan efek suara
 * - raylib.h: Library grafis untuk menggambar game
 */
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
#include <time.h>

/**
 * Definisi Konstanta
 * -----------------
 * - WINDOW_WIDTH, WINDOW_HEIGHT: Ukuran jendela game
 * - HIGH_SCORE_FILE: Lokasi file untuk menyimpan skor tertinggi
 */
#define WINDOW_WIDTH 1024
#define WINDOW_HEIGHT 768

#define HIGH_SCORE_FILE "assets/log/highscore.dat"
static Texture2D backgroundTexture;

/**
 * Fungsi Utama (main)
 * ------------------
 * Fungsi utama game yang berisi:
 * 1. Inisialisasi komponen game
 * 2. Loop utama game
 * 3. Pembersihan sumber daya sebelum program berakhir
 */
int main(void)
{
    /**
     * Pengaturan Awal
     * --------------
     * - SetExitKey(0): Menonaktifkan tombol ESC bawaan agar bisa dikustomisasi
     * - InitWindow(): Membuat jendela game dengan ukuran dan judul tertentu
     * - SetTargetFPS(): Mengatur batas frame per detik
     * - InitAudioDevice(): Menginisialisasi sistem audio
     */
    SetExitKey(0); // Nonaktifkan tombol keluar bawaan
    InitWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "Tetris Game");
    SetTargetFPS(60);
    InitAudioDevice();

    /**
     * Inisialisasi Generator Angka Acak
     * --------------------------------
     * Menggunakan waktu saat ini sebagai seed untuk generator angka acak,
     * memastikan blok yang muncul selalu berbeda setiap kali game dimainkan
     */
    srand(time(NULL));

    /**
     * Inisialisasi Komponen Game
     * -------------------------
     * - InitBlocks0(): Menyiapkan sistem blok Tetris
     * - InitMainMenu(): Menyiapkan menu utama
     * - InitGameSound(): Menyiapkan sistem audio
     * - InitBoard1(): Menyiapkan papan permainan kosong
     * - InitScoring(): Menyiapkan sistem skor dan level
     */
    InitBlocks0();
    InitMainMenu();
    InitGameSound();

    TetrisBoard board;
    InitBoard1(&board);

    ScoreData scoreData;
    InitScoring(&scoreData);

    Leaderboard leaderboard;
    InitLeaderboard(&leaderboard);

    /**
     * Variabel Status Permainan
     * ------------------------
     * - inGame: Menandakan apakah sedang di dalam permainan aktif
     * - gameOver: Menandakan apakah permainan berakhir (kalah)
     * - wasPreviouslyInGame: Pelacak status sebelumnya untuk transisi musik
     * - fallTimer: Penghitung waktu untuk gerakan blok ke bawah
     * - fallDelay: Waktu tunggu antara gerakan blok ke bawah (dipengaruhi level)
     */
    bool inGame = false;
    bool gameOver = false;
    bool wasPreviouslyInGame = false;
    float fallTimer = 0.0f;
    float fallDelay = 1.0f; // Waktu jatuh awal

    /**
     * Mulai dengan Musik Menu
     * ----------------------
     * Memulai game dengan memainkan musik menu utama
     */
    PlayBackgroundMusic(MUSIC_MENU);

    /**
     * Loop Utama Game
     * --------------
     * Loop ini berjalan terus selama jendela game masih terbuka.
     * Di dalamnya terdapat logika game, pemrosesan input, dan penggambaran.
     */
    while (!WindowShouldClose())
    {
        /**
         * Memulai Frame Gambar Baru
         * -------------------------
         * - BeginDrawing(): Menandai awal proses menggambar frame baru
         * - ClearBackground(): Mengisi layar dengan warna latar belakang
         */
        BeginDrawing();
        ClearBackground(DARKGRAY);

        /**
         * Update Audio
         * -----------
         * Memperbarui status music stream yang sedang diputar
         */
        UpdateGameSound();

        /**
         * Memperbarui Timer Game
         * ---------------------
         * Memperbarui timer game untuk menghitung waktu yang telah berlalu
         */
        UpdateGameTimer();

        /**
         * Penanganan Tombol ESC Global
         * ---------------------------
         * Mengatur perilaku tombol ESC berdasarkan status menu saat ini:
         * - Saat bermain: Jeda permainan
         * - Saat jeda: Lanjutkan permainan
         * - Saat di kredit/highscore: Kembali ke menu utama
         * - Saat di menu utama: Tidak melakukan apa-apa
         */
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

        /**
         * Mendapatkan Status Menu Saat Ini
         * -------------------------------
         * Variabel currentMenuState digunakan untuk menentukan tampilan dan
         * logika yang akan dijalankan pada frame ini
         */
        MenuState currentMenuState = GetCurrentMenuState();

        /**
         * Penanganan Status MENU_STATE_PLAY (Bermain)
         * ------------------------------------------
         * Logika saat status menu adalah PLAY (bermain):
         * - Memainkan musik gameplay jika baru masuk permainan
         * - Mengatur flag inGame menjadi true
         * - Menangani tombol P untuk jeda
         */
        if (currentMenuState == MENU_STATE_PLAY)
        {
            if (!inGame)
            {
                PlayBackgroundMusic(MUSIC_GAMEPLAY);
                PlaySoundEffect(SOUND_CLICK);
            }
            inGame = true;

            if (IsKeyPressed(KEY_P))
            {
                SetMenuState(MENU_STATE_PAUSE);
                PlaySoundEffect(SOUND_CLICK);
            }
        }
        /**
         * Penanganan Status MENU_STATE_CREDITS (Layar Kredit)
         * -------------------------------------------------
         * Menampilkan informasi kredit dan tim pengembang:
         * - Menggambar judul dan nama-nama anggota tim
         * - Menampilkan tombol kembali ke menu utama
         * - Menangani input mouse dan keyboard
         */
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
        /**
         * Penanganan Status MENU_STATE_HIGHSCORE (Layar Skor Tertinggi)
         * -----------------------------------------------------------
         * Menampilkan skor tertinggi yang pernah dicapai:
         * - Membaca skor tertinggi dari file
         * - Menggambar tampilan skor
         * - Menampilkan tombol kembali ke menu utama
         * - Menangani input mouse dan keyboard
         */
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
            UpdateMainMenu();
            DrawMainMenu();
            DisplayLeaderboard(&leaderboard, WINDOW_WIDTH, WINDOW_HEIGHT);
        }
        /**
         * Penanganan Status MENU_STATE_EXIT (Keluar)
         * ----------------------------------------
         * Menghentikan loop permainan untuk keluar dari aplikasi
         */
        else if (currentMenuState == MENU_STATE_EXIT)
        {
            break;
        }
        /**
         * Penanganan Status MENU_STATE_PAUSE (Jeda)
         * ---------------------------------------
         * Menampilkan menu jeda dan overlay saat permainan dijeda:
         * - Menggambar permainan yang sedang berlangsung di latar belakang
         * - Menampilkan menu jeda dengan tombol Resume dan Exit
         * - Menangani input mouse dan keyboard
         */
        else if (currentMenuState == MENU_STATE_PAUSE)
        {
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
        /**
         * Penanganan Status MENU_STATE_MAIN (Menu Utama)
         * --------------------------------------------
         * Menampilkan dan mengelola menu utama:
         * - Mengatur flag inGame menjadi false
         * - Memainkan musik menu jika sebelumnya dalam permainan
         * - Mengupdate dan menggambar menu utama
         */
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

        /**
         * Logika Utama Permainan
         * ---------------------
         * Menjalankan logika permainan saat status inGame aktif dan tidak game over:
         * - Mengatur kecepatan jatuh blok berdasarkan level
         * - Memperbarui timer jatuh blok
         * - Menangani penjatuhan dan penempatan blok
         * - Memeriksa kondisi game over
         * - Menangani input pemain (gerakan dan rotasi blok)
         * - Memperbarui skor dan baris yang dihapus
         * - Menggambar semua elemen permainan
         */
        if (inGame && !gameOver)
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
                    PlaceBlock(&board.current_block, &board);
                    board.current_block = board.next_block;
                    board.next_block = GenerateRandomBlock();

                    // Periksa kondisi game over
                    if (IsGameOver(&board.current_block, &board))
                    {
                        gameOver = true;
                        StopBackgroundMusic();
                        PlaySoundEffect(SOUND_GAME_OVER);
                    }
                }
                fallTimer = 0; // Reset timer
            }

            // Kontrol pemain untuk menggerakan dan merotasi blok
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

            // Tombol M untuk mute/unmute musik
            if (IsKeyPressed(KEY_M))
            {
                ToggleBackgroundMusic();
            }

            // Logika pembersihan baris yang penuh
            int linesCleared = ClearFullLines(&board);
            if (linesCleared > 0)
            {
                AddLineClearScore(&scoreData, linesCleared);
                CheckLevelUp(&scoreData);
                scoreData.level = board.current_level;
                PlaySoundEffect(SOUND_LINE_CLEAR);
            }

            // Menggambar elemen permainan
            DrawBlockShadow(&board.current_block, &board);
            DrawBoard(&board);
            DrawActiveTetromino(&board.current_block);
            DrawHoldBlock(&board);
            DrawNextBlock(&board);
            DrawScore(&board, &scoreData);

            // Tambahkan panel tips kontrol pemain
            // Posisikan di sisi kiri bawah layar
            Rectangle controlTipsRect = {
                10,                  // Posisi X
                WINDOW_HEIGHT - 180, // Posisi Y
                220,                 // Lebar
                170                  // Tinggi
            };

            // Gambar panel dengan latar belakang semi-transparan
            DrawRectangleRec(controlTipsRect, Fade(BLACK, 0.7f));
            DrawRectangleLinesEx(controlTipsRect, 2, WHITE);

            // Judul panel
            DrawText("KONTROL PEMAIN", controlTipsRect.x + 10, controlTipsRect.y + 10, 18, GOLD);

            // Daftar kontrol pemain
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
        /**
         * Penanganan Status Game Over
         * -------------------------
         * Menampilkan layar game over dengan opsi restart dan kembali ke menu:
         * - Menggambar teks game over dan skor akhir
         * - Menampilkan tombol restart dan back to menu
         * - Menangani input pemain untuk kedua tombol tersebut
         */
        else if (gameOver)
        {
            // Tampilkan teks Game Over dan skor akhir
            DrawText("GAME OVER", WINDOW_WIDTH / 2 - MeasureText("GAME OVER", 40) / 2,
                     WINDOW_HEIGHT / 2 - 100, 40, RED);
            DrawText(TextFormat("Final Score: %d", scoreData.score),
                     WINDOW_WIDTH / 2 - MeasureText(TextFormat("Final Score: %d", scoreData.score), 20) / 2,
                     WINDOW_HEIGHT / 2 - 40, 20, WHITE);

            // Tombol Restart
            Rectangle restartBtn = {
                WINDOW_WIDTH / 2 - 100,
                WINDOW_HEIGHT / 2,
                200,
                50};

            DrawRectangleRec(restartBtn, GREEN);
            DrawRectangleLinesEx(restartBtn, 3, LIME);
            DrawText("RESTART", restartBtn.x + (restartBtn.width / 2) - MeasureText("RESTART", 20) / 2,
                     restartBtn.y + 15, 20, WHITE);

            // Tombol Back to Menu
            Rectangle backBtn = {
                WINDOW_WIDTH / 2 - 100,
                WINDOW_HEIGHT / 2 + 70,
                200,
                50};

            DrawRectangleRec(backBtn, BLUE);
            DrawRectangleLinesEx(backBtn, 3, SKYBLUE);
            DrawText("BACK TO MENU", backBtn.x + (backBtn.width / 2) - MeasureText("BACK TO MENU", 20) / 2,
                     backBtn.y + 15, 20, WHITE);

            Rectangle scoreBtn = {
                WINDOW_WIDTH / 2 - 100,
                WINDOW_HEIGHT / 2 + 140,
                200,
                50};

            DrawRectangleRec(scoreBtn, PURPLE);
            DrawRectangleLinesEx(scoreBtn, 3, DARKPURPLE);
            DrawText("LEADERBOARD", scoreBtn.x + (scoreBtn.width / 2) - MeasureText("LEADERBOARD", 20) / 2,
                     scoreBtn.y + 15, 20, WHITE);

            // Penanganan hover dan klik tombol
            Vector2 mousePos = GetMousePosition();

            // Tombol Restart
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
                    inGame = true; // Langsung mulai permainan baru
                    wasPreviouslyInGame = true;
                    fallTimer = 0;
                    PlaySoundEffect(SOUND_CLICK);
                    PlayBackgroundMusic(MUSIC_GAMEPLAY);
                    SetMenuState(MENU_STATE_PLAY);
                }
            }

            // Tombol Back to Menu
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

            // Simpan skor tertinggi
            SaveGameScore(&scoreData);

            // Restart dengan tombol R
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
                SetWindowState(MENU_STATE_MAIN);
            }
        }

        /**
         * Akhiri Frame Gambar
         * ------------------
         * EndDrawing(): Menyelesaikan proses menggambar dan menampilkan frame ke layar
         */
        EndDrawing();
    }

    /**
     * Pembersihan Sumber Daya
     * ----------------------
     * - UnloadMainMenu(): Membersihkan sumber daya menu
     * - UnloadGameSound(): Membersihkan sumber daya audio
     * - CloseAudioDevice(): Menutup perangkat audio
     * - CloseWindow(): Menutup jendela aplikasi
     */
    UnloadMainMenu();
    UnloadGameSound();
    CloseAudioDevice();
    CloseWindow();

    return 0;
}