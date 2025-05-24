// Nama file  : scoring.h
// Deskripsi  : header untuk sistem skor, perkembangan level, dan tingkat kesulitan permainan
// Oleh       : Fatimah Hawwa 241511074

#ifndef SCORING_H
#define SCORING_H

#include "tetris.h"
#include <raylib.h>

// Struktur data untuk menyimpan informasi skor dan level
typedef struct {
    int score;              // Skor pemain saat ini
    int level;              // Level permainan saat ini
    int linesCleared;       // Total garis yang telah dibersihkan
    float fallSpeed;        // Kecepatan jatuh tetromino
    int linesToNextLevel;   // Garis yang dibutuhkan untuk naik level
    int highScore;  // Tambahkan field untuk high score
} ScoreData;

// Fungsi untuk menginisialisasi sistem skor
void InitScoring(ScoreData* scoreData);

// Fungsi untuk memperbarui kecepatan jatuh berdasarkan level
float UpdateFallSpeed(ScoreData* scoreData);

// Fungsi untuk menambahkan skor saat melakukan hard drop
void AddDropScore(ScoreData* scoreData, int dropDistance);

// Fungsi untuk menambahkan skor saat membersihkan garis
void AddLineClearScore(ScoreData* scoreData, int lineCount);

// Fungsi untuk memeriksa apakah level harus dinaikkan
int CheckLevelUp(ScoreData* scoreData);

// Fungsi untuk mendapatkan tingkat kesulitan saat ini
int GetDifficulty(ScoreData* scoreData);

// Fungsi untuk menyimpan skor tertinggi ke dalam file
void SaveGameScore(ScoreData* scoreData);

// Fungsi untuk memuat skor tertinggi dari file
int LoadGameHighScore(void);

// Tambahkan fungsi getter untuk high score
int GetHighScore(ScoreData* scoreData); 
 
#endif // SCORING_H