// Nama file  : highscore.c
// Deskripsi  : Mengelola daftar high score permainan
// Oleh       : Fatimah Hawwa 241511074

#include <raylib.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "include/highscore.h"
#include "include/game_sound.h"
#include "include/main_menu.h"
#include "include/tetris.h"

#define HIGH_SCORE_FILE "assets/log/highscore.dat"
#define MAX_HIGH_SCORES 10
#define WINDOW_WIDTH 1024
#define WINDOW_HEIGHT 768

static Texture2D backgroundTexture;

// Inisialisasi highscoreboard
void InitHighScore(HighScoreBoard* highscoreboard) {
    backgroundTexture = LoadTexture("assets/textures/bg.png");
    highscoreboard->highScores = NULL;
    LoadHighScore(highscoreboard);
}

// Membersihkan memori highscoreboard
void UnloadHighScore(HighScoreBoard* highscoreboard) {
    // Membersihkan semua entri dalam linked list
    UnloadTexture (backgroundTexture);
    HighScoreEntry* current = highscoreboard->highScores;
    HighScoreEntry* next;
    
    while (current != NULL) {
        next = current->next;
        free(current);
        current = next;
    }
    
    highscoreboard->highScores = NULL;
}

// Tambahkan skor ke daftar high score
void AddHighScore(HighScoreBoard* highscoreboard, int score, int level) {
    HighScoreEntry* newEntry = (HighScoreEntry*)malloc(sizeof(HighScoreEntry));
    newEntry->score = score;
    newEntry->level = level;
    newEntry->next = NULL;

    // Hitung jumlah entri saat ini
    int count = 0;
    HighScoreEntry* current = highscoreboard->highScores;
    while (current) {
        count++;
        current = current->next;
    }

    // Jika daftar kosong atau skor baru lebih tinggi dari yang pertama
    if (!highscoreboard->highScores || highscoreboard->highScores->score < score) {
        newEntry->next = highscoreboard->highScores;
        highscoreboard->highScores = newEntry;
    } else {
        // Cari posisi untuk menyisipkan (urutkan dari besar ke kecil)
        current = highscoreboard->highScores;
        HighScoreEntry* prev = NULL;
        while (current && current->score >= score) {
            prev = current;
            current = current->next;
        }
        newEntry->next = current;
        if (prev) {
            prev->next = newEntry;
        } else {
            highscoreboard->highScores = newEntry;
        }
    }

    // Batasi hingga 10 entri
    count = 0;
    current = highscoreboard->highScores;
    HighScoreEntry* lastValid = NULL;
    while (current && count < MAX_HIGH_SCORES) {
        lastValid = current;
        current = current->next;
        count++;
    }
    if (lastValid) {
        // Hapus entri setelah yang ke-10
        HighScoreEntry* toDelete = lastValid->next;
        lastValid->next = NULL;
        while (toDelete) {
            HighScoreEntry* next = toDelete->next;
            free(toDelete);
            toDelete = next;
        }
    }

    // Simpan perubahan ke file
    SaveHighScore(highscoreboard);
}

// Simpan daftar high score ke file
void SaveHighScore(HighScoreBoard* highscoreboard) {
    FILE* file = fopen(HIGH_SCORE_FILE, "w");
    if (!file) {
        printf("Gagal menyimpan highscore! Pastikan direktori tersedia.\n");
        return;
    }

    HighScoreEntry* current = highscoreboard->highScores;
    while (current) {
        fprintf(file, "%d,%d\n", current->score, current->level);
        current = current->next;
    }
    fclose(file);
}

// Muat daftar high score dari file
void LoadHighScore(HighScoreBoard* highscoreboard) {
    FreeHighScoreList(highscoreboard); // Kosongkan daftar sebelum memuat
    FILE* file = fopen(HIGH_SCORE_FILE, "r");
    if (!file) {
        printf("File highscore tidak ditemukan, membuat file baru.\n");
        file = fopen(HIGH_SCORE_FILE, "w");
        if (file) fclose(file);
        return;
    }

    char line[128];
    while (fgets(line, sizeof(line), file)) {
        int score, level;
        if (sscanf(line, "%d,%d", &score, &level) == 2) {
            AddHighScore(highscoreboard, score, level);
        }
    }
    fclose(file);
}

// Tampilkan daftar high score di layar
void DisplayHighScore(HighScoreBoard* highscoreboard, int screenWidth, int screenHeight) {
    // Muat ulang highscore untuk memastikan data terbaru
    LoadHighScore(highscoreboard);
    // Gambar latar belakang
    DrawTextureEx(backgroundTexture, (Vector2){0, 0}, 0.0f, 1.0f, WHITE);
    // Buat persegi panjang untuk panel highscoreboard
    Rectangle panel = {
        screenWidth / 2 - 250,
        screenHeight / 2 - 300,
        500,
        600
    };
    
    // Gambar panel dengan latar belakang
    DrawRectangleRec(panel, Fade(DARKBLUE, 0.9f));
    DrawRectangleLinesEx(panel, 3, SKYBLUE);
    
    // Gambar judul
    DrawText("HIGHSCORE", panel.x + panel.width/2 - MeasureText("HIGHSCORE", 38)/2, 
             panel.y + 20, 40, GOLD);
    
    // Gambar header tabel
    DrawText("RANK", panel.x + 50, panel.y + 80, 20, WHITE);
    DrawText("LEVEL", panel.x + 160, panel.y + 80, 20, WHITE);
    DrawText("SCORE", panel.x + 300, panel.y + 80, 20, WHITE);
    
    // Gambar garis pemisah
    DrawLine(panel.x + 20, panel.y + 110, panel.x + panel.width - 20, panel.y + 110, WHITE);
    
    // Gambar entri highscoreboard
    int yPos = panel.y + 130;
    HighScoreEntry* current = highscoreboard->highScores;
    int rank = 1;
    
    while (current && rank <= MAX_HIGHSCORE_ENTRIES) {
        // Rank
        char rankText[5];
        sprintf(rankText, "%d", rank);
        DrawText(rankText, panel.x + 75, yPos, 20, WHITE);
        
        // Level
        char levelText[5];
        sprintf(levelText, "%d", current->level);
        DrawText(levelText, panel.x + 190, yPos, 20, WHITE);

        // Score
        char scoreText[20];
        sprintf(scoreText, "%d", current->score);
        DrawText(scoreText, panel.x + 310, yPos, 20, WHITE);
        
        yPos += 40;
        current = current->next;
        rank++;
    }
    
    // Tambahkan placeholder untuk entri kosong
    while (rank <= MAX_HIGH_SCORES) {
        char rankText[5];
        sprintf(rankText, "%d", rank);
        DrawText(rankText, panel.x + 75, yPos, 20, WHITE);
        DrawText("-", panel.x + 190, yPos, 20, WHITE);
        DrawText("-", panel.x + 310, yPos, 20, WHITE);
        yPos += 40;
        rank++;
    }
    
    Rectangle backBtn = {
                WINDOW_WIDTH / 2 - 100,
                WINDOW_HEIGHT / 2 + 230,
                200,
                50};

            DrawRectangleRec(backBtn, BLUE);
            DrawRectangleLinesEx(backBtn, 3, SKYBLUE);
            DrawText("BACK TO MENU", backBtn.x + (backBtn.width / 2) - MeasureText("BACK TO MENU", 20) / 2,
                     backBtn.y + 15, 20, WHITE);
                     
    // Cek tombol kembali
    if (CheckCollisionPointRec(GetMousePosition(), backBtn)) {
        DrawRectangleRec(backBtn, Fade(BLUE, 0.7f));
        DrawText("BACK TO MENU", backBtn.x + backBtn.width/2 - MeasureText("BACK TO MENU", 20)/2,
                 backBtn.y + 15, 20, WHITE);
        
        if (IsMouseButtonReleased(MOUSE_LEFT_BUTTON)) {
            // Kembali ke menu utama
            PlaySoundEffect(SOUND_CLICK);
            SetMenuState(MENU_STATE_MAIN);
        }
    }
}

// Bebaskan memori daftar high score
void FreeHighScoreList(HighScoreBoard* highscoreboard) {
    HighScoreEntry* current = highscoreboard->highScores;
    while (current) {
        HighScoreEntry* next = current->next;
        free(current);
        current = next;
    }
    highscoreboard->highScores = NULL;
}

// Fungsi untuk memeriksa apakah skor adalah high score
bool IsHighScore(HighScoreBoard* highscore, int score) {
    int count = 0;
    HighScoreEntry* current = highscore->highScores;
    while (current && count < MAX_HIGH_SCORES) {
        if (score > current->score) {
            return true;
        }
        count++;
        current = current->next;
    }
    return count < MAX_HIGH_SCORES;
}