// Nama file  : leaderboard.h
// Deskripsi  : Header untuk mengelola daftar high score permainan
// Oleh       : Fatimah Hawwa 241511074 & Ibnu Hilmi Athaillah 241511079

#ifndef LEADERBOARD_H
#define LEADERBOARD_H

#include <raylib.h>
#include <stdbool.h>

#define MAX_LEADERBOARD_ENTRIES 10
#define LEADERBOARD_FILE "assets/log/leaderboard.dat"

// Struktur data untuk entri high score
typedef struct LeaderboardEntry {
    int score;              // Skor yang dicapai pemain
    int level;              // Level tertinggi yang dicapai pemain
    float time;             // Waktu bermain dalam detik
    char name[50];          // Nama pemain (maksimal 49 karakter + null terminator)
    struct LeaderboardEntry* next; // Pointer ke entri berikutnya dalam linked list
} LeaderboardEntry;

// Struktur data untuk menyimpan daftar high score
typedef struct {
    LeaderboardEntry* highScores; // Head pointer untuk linked list entri high score
} Leaderboard;

/**
 * Menginisialisasi struktur leaderboard dan memuat data dari file
 * @param leaderboard Pointer ke struktur Leaderboard yang akan diinisialisasi
 */
void InitLeaderboard(Leaderboard* leaderboard);

/**
 * Menambahkan entri baru ke leaderboard dengan urutan descending berdasarkan skor
 * @param leaderboard Pointer ke struktur Leaderboard
 * @param score Skor yang dicapai pemain
 * @param level Level yang dicapai pemain
 * @param name Nama pemain
 * @param time Waktu bermain dalam detik
 */
void AddLeaderboard(Leaderboard* leaderboard, int score, int level, const char* name, float time);

/**
 * Menambahkan entri baru atau memperbarui entri yang sudah ada untuk pemain tertentu
 * @param leaderboard Pointer ke struktur Leaderboard
 * @param score Skor yang dicapai pemain
 * @param level Level yang dicapai pemain
 * @param name Nama pemain
 * @param time Waktu bermain dalam detik
 */
void AddOrUpdateLeaderboard(Leaderboard* leaderboard, int score, int level, const char* name, float time);

/**
 * Menyimpan data leaderboard ke file dalam format CSV
 * @param leaderboard Pointer ke struktur Leaderboard yang akan disimpan
 */
void SaveLeaderboard(Leaderboard* leaderboard);

/**
 * Memuat data leaderboard dari file CSV
 * @param leaderboard Pointer ke struktur Leaderboard yang akan diisi dengan data dari file
 */
void LoadLeaderboard(Leaderboard* leaderboard);

/**
 * Menampilkan leaderboard di layar dengan interface grafis
 * @param leaderboard Pointer ke struktur Leaderboard yang akan ditampilkan
 * @param screenWidth Lebar layar dalam pixel
 * @param screenHeight Tinggi layar dalam pixel
 */
void DisplayLeaderboard(Leaderboard* leaderboard, int screenWidth, int screenHeight);

/**
 * Membebaskan semua memori yang dialokasikan untuk linked list leaderboard
 * @param leaderboard Pointer ke struktur Leaderboard yang akan dibersihkan
 */
void FreeLeaderboardList(Leaderboard* leaderboard);

/**
 * Membersihkan semua resource yang digunakan oleh leaderboard termasuk tekstur
 * @param leaderboard Pointer ke struktur Leaderboard yang akan dibersihkan
 */
void UnloadLeaderboard(Leaderboard* leaderboard);

/**
 * Mengecek apakah skor tertentu layak masuk ke dalam leaderboard
 * @param leaderboard Pointer ke struktur Leaderboard
 * @param score Skor yang akan dicek
 * @return true jika skor layak masuk leaderboard, false jika tidak
 */
bool IsLeaderboard(Leaderboard* leaderboard, int score);

#endif // LEADERBOARD_H