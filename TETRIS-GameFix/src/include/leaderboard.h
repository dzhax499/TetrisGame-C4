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
    struct LeaderboardEntry* next; // Pointer ke entri berikutnya
} LeaderboardEntry;

// Struktur data untuk menyimpan daftar high score
typedef struct {
    LeaderboardEntry* highScores; // Daftar high score (linked list)
} Leaderboard;

// Fungsi untuk menginisialisasi leaderboard
void InitLeaderboard(Leaderboard* leaderboard);

// Fungsi untuk menambahkan skor ke daftar leaderboard
void AddLeaderboard(Leaderboard* leaderboard, int score, int level);

// Fungsi untuk menyimpan daftar high score ke file
void SaveLeaderboard(Leaderboard* leaderboard);

// Fungsi untuk memuat daftar high score dari file
void LoadLeaderboard(Leaderboard* leaderboard);

// Fungsi untuk menampilkan daftar leaderboard di layar
void DisplayLeaderboard(Leaderboard* leaderboard, int screenWidth, int screenHeight);

// Fungsi untuk membebaskan memori daftar leaderboard
void FreeLeaderboardList(Leaderboard* leaderboard);

// Fungsi untuk menghapus entri setelah entri ke-10
void UnloadLeaderboard(Leaderboard* leaderboard);

// Fungsi untuk memeriksa apakah skor adalah leaderboard
bool IsLeaderboard(Leaderboard* leaderboard, int score);

#endif // LEADERBOARD_H