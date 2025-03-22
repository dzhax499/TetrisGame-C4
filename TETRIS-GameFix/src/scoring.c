// Nama file  : scoring.c
// Deskripsi  : Menangani sistem skor, perkembangan level, dan tingkat kesulitan permainan
// Oleh       : Fatimah Hawwa 241511074/**

#include <raylib.h>                  // Library utama untuk grafis dan audio
#include "include/tetris.h"          // Header utama untuk game Tetris
#include "include/scoring.h"         // Header untuk sistem skor

#define HIGH_SCORE_FILE "../assets/log/highscore.dat" // Path untuk menyimpan skor tertinggi

// Inisialisasi sistem skor
void InitScoring(ScoreData* scoreData) {
    scoreData->score = 0;             // Set skor awal ke 0
    scoreData->level = 1;             // Mulai dari level 1
    scoreData->linesCleared = 0;      // Tidak ada garis yang dibersihkan saat awal
    scoreData->fallSpeed = 1.0f;      // Kecepatan jatuh awal (1.0 detik)
    scoreData->linesToNextLevel = 10; // Dibutuhkan 10 garis untuk naik level
}

// Perbarui kecepatan jatuh berdasarkan level saat ini
float UpdateFallSpeed(ScoreData* scoreData) {
    // Kecepatan jatuh dasar adalah 1.0 detik, berkurang 0.05 detik setiap naik level
    scoreData->fallSpeed = 1.0f - ((scoreData->level - 1) * 0.05f);
    
    // Pastikan kecepatan tidak terlalu cepat (batas minimal = 0.1 detik)
    if (scoreData->fallSpeed < 0.1f) {
        scoreData->fallSpeed = 0.1f;
    }
    
    return scoreData->fallSpeed; // Kembalikan nilai kecepatan jatuh terbaru
}

// Tambahkan skor berdasarkan jarak jatuh tetromino
void AddDropScore(ScoreData* scoreData, int dropDistance) {
    if (dropDistance > 0) {
        scoreData->score += dropDistance; // 1 poin per sel yang dilewati saat hard drop
    }
}

// Tambahkan skor berdasarkan jumlah garis yang dibersihkan
void AddLineClearScore(ScoreData* scoreData, int lineCount) {
    int basePoints = 0;

    // Gunakan sistem skor dari Tetris versi Nintendo
    switch (lineCount) {
        case 1:
            basePoints = 40;   // 1 garis = 40 poin
            break;
        case 2:
            basePoints = 100;  // 2 garis = 100 poin
            break;
        case 3:
            basePoints = 300;  // 3 garis = 300 poin
            break;
        case 4:
            basePoints = 1200; // 4 garis = 1200 poin (Tetris!)
            break;
        default:
            return; // Jika tidak ada garis yang dibersihkan, keluar dari fungsi
    }

    // Kalikan skor dasar dengan level saat ini untuk bonus
    int pointsAwarded = basePoints * scoreData->level;
    scoreData->score += pointsAwarded; // Tambahkan ke total skor

    // Tambahkan jumlah garis yang telah dibersihkan
    scoreData->linesCleared += lineCount;

    // Periksa apakah pemain naik level setelah membersihkan garis
    CheckLevelUp(scoreData);
}

// Periksa apakah pemain harus naik level
void CheckLevelUp(ScoreData* scoreData) {
    if (scoreData->linesCleared >= scoreData->linesToNextLevel) {
        scoreData->level++; // Naik ke level berikutnya
        scoreData->linesToNextLevel += 10; // Tambahkan kebutuhan garis untuk level berikutnya
        UpdateFallSpeed(scoreData); // Perbarui kecepatan jatuh
        
        // Di sini seharusnya ada efek suara atau notifikasi naik level
    }
}

// Mengembalikan tingkat kesulitan berdasarkan level saat ini
int GetDifficulty(ScoreData* scoreData) {
    return scoreData->level; // Level yang lebih tinggi = permainan lebih sulit
}

// Simpan skor tertinggi ke dalam file jika lebih tinggi dari yang tersimpan
void SaveHighScore(ScoreData* scoreData) {
    int currentHighScore = LoadHighScore(); // Muat skor tertinggi yang tersimpan sebelumnya

    if (scoreData->score > currentHighScore) { // Jika skor baru lebih tinggi
        FILE* file = fopen(HIGH_SCORE_FILE, "w"); // Buka file dalam mode tulis

        if (file) {
            fprintf(file, "%d", scoreData->score); // Tulis skor sebagai angka ke file
            fclose(file); // Tutup file
            printf("High score saved: %d\n", scoreData->score); // Debug: tampilkan skor yang disimpan
        } else {
            printf("Failed to open file for writing\n"); // Debug: gagal menyimpan skor
        }
    }
}

// Muat skor tertinggi dari file
int LoadHighScore(void) {
    int highScore = 0; // Default jika tidak ada skor tersimpan
    FILE* file = fopen(HIGH_SCORE_FILE, "r"); // Buka file dalam mode baca

    if (file) {
        fscanf(file, "%d", &highScore); // Baca skor dari file
        fclose(file); // Tutup file
        printf("High score loaded: %d\n", highScore); // Debug: tampilkan skor yang dimuat
    } else {
        printf("High score file not found, using default 0\n"); // Debug: file tidak ditemukan
    }

    return highScore; // Kembalikan skor yang telah dimuat
} 