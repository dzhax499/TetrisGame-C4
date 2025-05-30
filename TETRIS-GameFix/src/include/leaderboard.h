// Nama file  : leaderboard.h
// Deskripsi  : Header untuk mengelola daftar high score permainan
// Oleh       : Fatimah Hawwa 241511074

#ifndef LEADERBOARD_H
#define LEADERBOARD_H

#include <raylib.h>
#include <stdbool.h>

#define MAX_LEADERBOARD_ENTRIES 10
#define LEADERBOARD_FILE "assets/log/leaderboard.dat"

// Struktur data untuk entri high score
typedef struct LeaderboardEntry {
    int score;              // Skor
    int level;              // Level yang dicapai
    float time;              // Waktu permainan
    char name[50];        // Nama pemain
    struct LeaderboardEntry* next; // Pointer ke entri berikutnya
} LeaderboardEntry;

// Struktur data untuk menyimpan daftar high score
typedef struct {
    LeaderboardEntry* highScores; // Daftar high score (linked list)
} Leaderboard;

void InitLeaderboard(Leaderboard* leaderboard);
void AddLeaderboard(Leaderboard* leaderboard, int score, int level, const char* name, float time);
void AddOrUpdateLeaderboard(Leaderboard* leaderboard, int score, int level, const char* name, float time);
void SaveLeaderboard(Leaderboard* leaderboard);
void LoadLeaderboard(Leaderboard* leaderboard);
void DisplayLeaderboard(Leaderboard* leaderboard, int screenWidth, int screenHeight);
void FreeLeaderboardList(Leaderboard* leaderboard);   // <--- Tambahkan ini!
void UnloadLeaderboard(Leaderboard* leaderboard);
bool IsLeaderboard(Leaderboard* leaderboard, int score);

#endif // LEADERBOARD_H