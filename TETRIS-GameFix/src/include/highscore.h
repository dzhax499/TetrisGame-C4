// Nama file  : highscore.h
// Deskripsi  : Header untuk mengelola daftar high score permainan
// Oleh       : Fatimah Hawwa 241511074

#ifndef HIGHSCORE_H
#define HIGHSCORE_H

#include <raylib.h>
#include <stdbool.h>

#define MAX_HIGHSCORE_ENTRIES 10

// Struktur data untuk entri high score
typedef struct HighScoreEntry {
    int score;              // Skor
    int level;              // Level yang dicapai
    struct HighScoreEntry* next; // Pointer ke entri berikutnya
} HighScoreEntry;

// Struktur data untuk menyimpan daftar high score
typedef struct {
    HighScoreEntry* highScores; // Daftar high score (linked list)
} HighScoreBoard;

// Fungsi untuk menginisialisasi highscore
void InitHighScore(HighScoreBoard* highscoreboard);

// Fungsi untuk menambahkan skor ke daftar high score
void AddHighScore(HighScoreBoard* highscoreboard, int score, int level);

// Fungsi untuk menyimpan daftar high score ke file
void SaveHighScore(HighScoreBoard* highscoreboard);

// Fungsi untuk memuat daftar high score dari file
void LoadHighScore(HighScoreBoard* highscoreboard);

// Fungsi untuk menampilkan daftar high score di layar
void DisplayHighScore(HighScoreBoard* highscoreboard, int screenWidth, int screenHeight);

// Fungsi untuk membebaskan memori daftar high score
void FreeHighScoreList(HighScoreBoard* highscoreboard);

void UnloadHighScore(HighScoreBoard* highscoreboard);

// Fungsi untuk memeriksa apakah skor adalah high score
bool IsHighScore(HighScoreBoard* highscoreboard, int score);

#endif // HIGHSCORE_H