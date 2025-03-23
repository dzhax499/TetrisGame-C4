/**
 * scoring.c
 *
 * Menangani sistem skor, perkembangan level, dan tingkat kesulitan permainan
 * Penulis: Fatimah
 */
#include <raylib.h>
#include <stdio.h>  // Untuk fungsi file I/O
#include "include/tetris.h"
#include "include/scoring.h"
#include "include/game_sound.h"  // Untuk efek suara level up

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
            basePoints = 1200; // Bonus Tetris!
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

// Periksa apakah pemain harus naik level
void CheckLevelUp(ScoreData* scoreData) {
    if (scoreData->linesCleared >= scoreData->linesToNextLevel) {
        // Naik level
        scoreData->level++;
        
        // Perbarui jumlah garis yang dibutuhkan untuk level berikutnya
        scoreData->linesToNextLevel += 10;
        
        // Perbarui kecepatan jatuh
        UpdateFallSpeed(scoreData);
        
        // Putar suara level up
        PlaySoundEffect(SOUND_CLICK); // Gunakan sound effect yang tersedia
    }
    
    // Update high score jika skor saat ini lebih tinggi
    if (scoreData->score > scoreData->highScore) {
        scoreData->highScore = scoreData->score;
    }
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
            printf("File highscore.dat ditemukan, tetapi tidak bisa dibaca. Set highscore = 0\n");
            highScore = 0;
        } else {
            printf("Highscore saat ini: %d\n", highScore);
        }
        fclose(file);
    } else {
        printf("File highscore.dat tidak ditemukan, menggunakan highscore default = 0\n");
    }
    return highScore;
}

// Mendapatkan high score saat ini
int GetHighScore(ScoreData* scoreData) {
    return scoreData->highScore;
}