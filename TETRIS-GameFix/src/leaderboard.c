// Nama file  : leaderboard.c
// Deskripsi  : Mengelola daftar high score permainan
// Oleh       : Fatimah Hawwa 241511074

#include <raylib.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "include/leaderboard.h"
#include "include/game_sound.h"
#include "include/main_menu.h"
#include "include/tetris.h"

#define HIGH_SCORE_FILE "assets/log/leaderboard.dat"
#define MAX_LEADERBOARD_ENTRIES 10

static Texture2D backgroundTexture;

// Inisialisasi leaderboard
void InitLeaderboard(Leaderboard* leaderboard) {
    backgroundTexture = LoadTexture("assets/textures/bg.png");
    leaderboard->highScores = NULL;
    LoadLeaderboard(leaderboard);
}

// Membersihkan memori leaderboard
void UnloadLeaderboard(Leaderboard* leaderboard) {
    // Membersihkan semua entri dalam linked list
    UnloadTexture (backgroundTexture);
    FreeLeaderboardList(leaderboard);
}

// Tambahkan skor ke daftar leaderboard
void AddLeaderboard(Leaderboard* leaderboard, int score, int level) {
    LeaderboardEntry* newEntry = (LeaderboardEntry*)malloc(sizeof(LeaderboardEntry));
    if (!newEntry) {
        printf("Gagal menambahkan entri leaderboard!\n");
        return;
    }
    newEntry->score = score;
    newEntry->level = level;
    newEntry->next = NULL;

    // Jika daftar kosong atau skor baru lebih tinggi dari yang pertama
    if (!leaderboard->highScores || leaderboard->highScores->score < score) {
        newEntry->next = leaderboard->highScores;
        leaderboard->highScores = newEntry;
    } else {
        // Cari posisi untuk menyisipkan (urutkan dari besar ke kecil)
        LeaderboardEntry* current = leaderboard->highScores;
        LeaderboardEntry* prev = NULL;
        while (current && current->score >= score) {
            prev = current;
            current = current->next;
        }
        newEntry->next = current;
        prev->next = newEntry;
    }

    // Batasi hingga 10 entri
    int count = 0;
    LeaderboardEntry* current = leaderboard->highScores;
    LeaderboardEntry* lastValid = NULL;
    while (current && count < MAX_LEADERBOARD_ENTRIES) {
        lastValid = current;
        current = current->next;
        count++;
    }
    if (lastValid) {
        // Hapus entri setelah yang ke-10
        LeaderboardEntry* toDelete = lastValid->next;
        lastValid->next = NULL;
        while (toDelete) {
            LeaderboardEntry* next = toDelete->next;
            free(toDelete);
            toDelete = next;
        }
    }

    // Simpan perubahan ke file
    SaveLeaderboard(leaderboard);
}

// Simpan daftar leaderboard ke file
void SaveLeaderboard(Leaderboard* leaderboard) {
    FILE* file = fopen(LEADERBOARD_FILE, "w");
    if (!file) {
        printf("Gagal menyimpan leaderboard! Pastikan direktori tersedia.\n");
        return;
    }

    LeaderboardEntry* current = leaderboard->highScores;
    while (current) {
        fprintf(file, "%d,%d\n", current->score, current->level);
        current = current->next;
    }
    fclose(file);
}

// Muat daftar leaderboard dari file
void LoadLeaderboard(Leaderboard* leaderboard) {
    FreeLeaderboardList(leaderboard); // Kosongkan daftar sebelum memuat
    FILE* file = fopen(LEADERBOARD_FILE, "r");
    if (!file) {
        printf("File leaderboard tidak ditemukan, membuat file baru.\n");
        file = fopen(LEADERBOARD_FILE, "w");
        if (file) fclose(file);
        return;
    }

    char line[128];
    while (fgets(line, sizeof(line), file)) {
        int score, level;
        if (sscanf(line, "%d,%d", &score, &level) == 2) {
            AddLeaderboard(leaderboard, score, level);
        }
    }
    fclose(file);
}

// Tampilkan daftar leaderboard di layar
void DisplayLeaderboard(Leaderboard* leaderboard, int screenWidth, int screenHeight) {
    // Muat ulang leaderboard untuk memastikan data terbaru
    LoadLeaderboard(leaderboard);
    // Gambar latar belakang
    DrawTextureEx(backgroundTexture, (Vector2){0, 0}, 0.0f, 1.0f, WHITE);
    // Buat persegi panjang untuk panel leaderboard
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
    DrawText("LEADERBOARD", panel.x + panel.width/2 - MeasureText("LEADERBOARD", 38)/2, 
             panel.y + 20, 40, YELLOW);
    
    // Gambar header tabel
    DrawText("RANK", panel.x + 50, panel.y + 80, 20, WHITE);
    DrawText("LEVEL", panel.x + 160, panel.y + 80, 20, WHITE);
    DrawText("SCORE", panel.x + 300, panel.y + 80, 20, WHITE);
    
    // Gambar garis pemisah
    DrawLine(panel.x + 20, panel.y + 110, panel.x + panel.width - 20, panel.y + 110, WHITE);
    
    // Gambar entri leaderboard
    int yPos = panel.y + 130;
    LeaderboardEntry* current = leaderboard->highScores;
    int rank = 1;

    while (current && rank <= MAX_LEADERBOARD_ENTRIES) {
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
    while (rank <= MAX_LEADERBOARD_ENTRIES) {
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

// Bebaskan memori daftar leaderboard
void FreeLeaderboardList(Leaderboard* leaderboard) {
    LeaderboardEntry* current = leaderboard->highScores;
    while (current) {
        LeaderboardEntry* next = current->next;
        free(current);
        current = next;
    }
    leaderboard->highScores = NULL;
}

// Fungsi untuk memeriksa apakah skor adalah high score
bool IsHighScore(Leaderboard* leaderboard, int score) {
    int count = 0;
    LeaderboardEntry* current = leaderboard->highScores;
    while (current && count < MAX_LEADERBOARD_ENTRIES) {
        if (score > current->score) {
            return true;
        }
        count++;
        current = current->next;
    }
    return count < MAX_LEADERBOARD_ENTRIES;
}