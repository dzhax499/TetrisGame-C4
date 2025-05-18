// Nama file  : scoring.c
// Deskripsi  : Menangani sistem skor, perkembangan level, dan tingkat kesulitan permainan
// Oleh       : Fatimah Hawwa 241511074

#include <raylib.h>
#include <stdio.h>  
#include "include/tetris.h"
#include "include/scoring.h"
#include "include/game_sound.h"  

#define HIGH_SCORE_FILE "assets/log/highscore.dat"

// Inisialisasi sistem skor
void InitScoring(ScoreData* scoreData) {
    scoreData->score = 0;
    scoreData->level = 1;
    scoreData->linesCleared = 0;
    scoreData->fallSpeed = 1.0f;
    scoreData->linesToNextLevel = 10;
    
    // Muat high score saat inisialisasi
    scoreData->highScore = LoadHighScore();
}

// Perbarui kecepatan jatuh berdasarkan level saat ini
float UpdateFallSpeed(ScoreData* scoreData) {
    // Kecepatan jatuh dasar adalah 1.0, berkurang (menjadi lebih cepat) sebesar 0.05 per level
    scoreData->fallSpeed = 1.0f - ((scoreData->level - 1) * 0.05f);
    
    // Pastikan tidak terlalu cepat
    if (scoreData->fallSpeed < 0.1f) {
        scoreData->fallSpeed = 0.1f;
    }
    
    return scoreData->fallSpeed;
}

// Hitung skor untuk menjatuhkan tetromino
void AddDropScore(ScoreData* scoreData, int dropDistance) {
    if (dropDistance > 0) {
        // 1 poin per sel untuk hard drop
        scoreData->score += dropDistance;
    }
}

// Hitung skor untuk membersihkan garis
void AddLineClearScore(ScoreData* scoreData, int lineCount) {
    int basePoints = 0;
    
    // Sistem Skor Nintendo Asli
    switch (lineCount) {
        case 1:
            basePoints = 40;
            break;
        case 2:
            basePoints = 100;
            break;
        case 3:
            basePoints = 300;
            break;
        case 4:
            basePoints = 1200; 
            break;
        default:
            return;
    }
    
    // Dikalikan dengan level untuk bonus kesulitan
    int pointsAwarded = basePoints * scoreData->level;
    scoreData->score += pointsAwarded;
    
    // Perbarui jumlah garis yang telah dibersihkan
    scoreData->linesCleared += lineCount;
    
    // Periksa apakah harus naik level
    CheckLevelUp(scoreData);
}

// Fungsi untuk memeriksa apakah level harus dinaikkan berdasarkan baris yang dibersihkan 
int CheckLevelUp(ScoreData* scoreData)  {
    // Hitung level baru berdasarkan jumlah baris yang dibersihkan (1 level per 3 baris) 
    int newLevel = 1 + (scoreData->linesCleared / 3);
        // Update level jika telah bertambah
        if (newLevel > scoreData->level) {
            scoreData->level = newLevel;
            scoreData->linesToNextLevel = 3 * newLevel - scoreData->linesCleared;
            return 1; // Level bertambah
        } else {
            // Perbarui sisa baris untuk level berikutnya
            scoreData->linesToNextLevel = 3 * (scoreData->level + 1) - scoreData->linesCleared;
        }

return 0; // Level tidak berubah
}

// Dapatkan tingkat kesulitan saat ini
int GetDifficulty(ScoreData* scoreData) {
    return scoreData->level;
}

// Simpan skor tertinggi ke dalam file
void SaveHighScore(ScoreData* scoreData) {
    if (scoreData->score > LoadHighScore()) {  // Periksa langsung dari file
        FILE* file = fopen(HIGH_SCORE_FILE, "w");

        if (file) {
            fprintf(file, "%d", scoreData->score);
            fclose(file);
            scoreData->highScore = scoreData->score;  // Update setelah sukses menyimpan
        } else {
            printf("Gagal menyimpan highscore! Pastikan direktori tersedia.\n");
        }
    }
}


// Muat skor tertinggi dari file
int LoadHighScore(void) {
    int highScore = 0;
    FILE* file = fopen(HIGH_SCORE_FILE, "r");

    if (file) {
        if (fscanf(file, "%d", &highScore) != 1) {
            highScore = 0;
        }
        fclose(file);
        return highScore;
    }
    return 0; 
}


// Mendapatkan high score saat ini
int GetHighScore(ScoreData* scoreData) {
    return scoreData->highScore;
}


