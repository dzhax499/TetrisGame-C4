#ifndef SCORE_H
#define SCORE_H

#include <stdbool.h>

// Struktur data untuk skor
typedef struct {
    int score;
    int combo;
    int level;
    int linesCleared;
} ScoreSystem;

// Inisialisasi sistem skor
void initScoreSystem(ScoreSystem *scoreSys);

// Update skor berdasarkan jumlah garis yang dihapus
void updateScore(ScoreSystem *scoreSys, int linesCleared, bool tetrisBonus, bool perfectClear);

// Reset sistem skor
void resetScore(ScoreSystem *scoreSys);

#endif
