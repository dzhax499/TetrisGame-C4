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
void AddLeaderboard(Leaderboard* leaderboard, int score, int level, const char* name, float time) {
    LeaderboardEntry* newEntry = (LeaderboardEntry*)malloc(sizeof(LeaderboardEntry));
    if (!newEntry) {
        printf("Gagal menambahkan entri leaderboard!\n");
        return;
    }
    newEntry->score = score;
    newEntry->level = level;
    newEntry->time = time;
    strncpy(newEntry->name, name, sizeof(newEntry->name) - 1);
    newEntry->name[sizeof(newEntry->name) - 1] = '\0'; // Pastikan string null-terminated
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

void FreeLeaderboardList(Leaderboard* leaderboard) {
    LeaderboardEntry* current = leaderboard->highScores;
    while (current) {
        LeaderboardEntry* next = current->next;
        free(current);
        current = next;
    }
    leaderboard->highScores = NULL;
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
        fprintf(file, "%d,%d,%s,%.2f\n", current->score, current->level, current->name, current->time);
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
        char name[50];
        float time;
        if (sscanf(line, "%d,%d,%49[^,],%f", &score, &level, name, &time) == 4) {
            AddLeaderboard(leaderboard, score, level, name, time);
        }
    }
    fclose(file);
}

void AddOrUpdateLeaderboard(Leaderboard* leaderboard, int score, int level, const char* name, float time) {
    LeaderboardEntry* current = leaderboard->highScores;
    LeaderboardEntry* existingEntry = NULL;

    // Cek apakah nama sudah ada
    while (current) {
        if (strcmp(current->name, name) == 0) {
            existingEntry = current;
            break;
        }
        current = current->next;
    }

    // Jika ada dan skor baru lebih tinggi, update
    if (existingEntry) {
        if (score > existingEntry->score) {
            existingEntry->score = score;
            existingEntry->level = level;
            existingEntry->time = time;
            SaveLeaderboard(leaderboard);
        }
    } else {
        // Kalau belum ada, tambahkan baru
        AddLeaderboard(leaderboard, score, level, name, time);
    }
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
    DrawText("LEADERBOARD", panel.x + panel.width / 2 - MeasureText("LEADERBOARD", 38) / 2,
             panel.y + 20, 40, PURPLE);

    // Gambar header tabel
    DrawText("RANK", panel.x + 20, panel.y + 80, 20, WHITE);
    DrawText("NAME", panel.x + 80, panel.y + 80, 20, WHITE);
    DrawText("LEVEL", panel.x + 200, panel.y + 80, 20, WHITE);
    DrawText("SCORE", panel.x + 310, panel.y + 80, 20, WHITE);
    DrawText("TIME", panel.x + 420, panel.y + 80, 20, WHITE);

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
        DrawText(rankText, panel.x + 20, yPos, 20, WHITE);

        // Name
        DrawText(current->name, panel.x + 80, yPos, 20, WHITE);

        // Level
        char levelText[5];
        sprintf(levelText, "%d", current->level);
        DrawText(levelText, panel.x + 200, yPos, 20, WHITE);

        // Score
        char scoreText[20];
        sprintf(scoreText, "%d", current->score);
        DrawText(scoreText, panel.x + 310, yPos, 20, WHITE);

        // Time
        char timeText[10];
        sprintf(timeText, "%.2f", current->time);
        DrawText(timeText, panel.x + 420, yPos, 20, WHITE);

        yPos += 40;
        current = current->next;
        rank++;
    }

    // Tambahkan placeholder untuk entri kosong
    while (rank <= MAX_LEADERBOARD_ENTRIES) {
        char rankText[5];
        sprintf(rankText, "%d", rank);
        DrawText(rankText, panel.x + 20, yPos, 20, WHITE);
        DrawText("-", panel.x + 80, yPos, 20, WHITE);
        DrawText("-", panel.x + 200, yPos, 20, WHITE);
        DrawText("-", panel.x + 310, yPos, 20, WHITE);
        DrawText("-", panel.x + 420, yPos, 20, WHITE);
        yPos += 40;
        rank++;
    }

    // Tombol kembali ke menu utama
    Rectangle backBtn = {
        screenWidth / 2 - 100,
        screenHeight / 2 + 230,
        200,
        50
    };

    DrawRectangleRec(backBtn, BLUE);
    DrawRectangleLinesEx(backBtn, 3, SKYBLUE);
    DrawText("BACK TO MENU",
             backBtn.x + (backBtn.width / 2) - MeasureText("BACK TO MENU", 20) / 2,
             backBtn.y + 15, 20, WHITE);

    // Cek tombol kembali
    if (CheckCollisionPointRec(GetMousePosition(), backBtn)) {
        DrawRectangleRec(backBtn, Fade(BLUE, 0.7f));
        DrawText("BACK TO MENU",
                 backBtn.x + (backBtn.width / 2) - MeasureText("BACK TO MENU", 20) / 2,
                 backBtn.y + 15, 20, WHITE);

        if (IsMouseButtonReleased(MOUSE_LEFT_BUTTON)) {
            PlaySoundEffect(SOUND_CLICK);
            SetMenuState(MENU_STATE_MAIN);
        }
    }
}
