
#include "score.h"

void initScoreSystem(ScoreSystem *scoreSys) {
    scoreSys->score = 0;
    scoreSys->combo = 0;
    scoreSys->level = 1;
    scoreSys->linesCleared = 0;
}

void updateScore(ScoreSystem *scoreSys, int linesCleared, bool tetrisBonus, bool perfectClear) {
    int basePoints[] = {0, 100, 300, 500, 800}; // Skor dasar untuk 1, 2, 3, 4 garis

    if (linesCleared > 0) {
        scoreSys->score += basePoints[linesCleared] * (scoreSys->level);
        scoreSys->linesCleared += linesCleared;

        // Bonus Tetris (4 lines clear dalam sekali drop)
        if (tetrisBonus) {
            scoreSys->score += 400 * scoreSys->level;
        }

        // Bonus Perfect Clear (menghapus semua blok di papan)
        if (perfectClear) {
            scoreSys->score += 800 * scoreSys->level;
        }

        // Combo bonus
        scoreSys->combo++;
        scoreSys->score += scoreSys->combo * 50;
    } else {
        scoreSys->combo = 0; // Reset combo jika tidak ada garis yang terhapus
    }

    // Naik level setiap 10 garis yang dihapus
    if (scoreSys->linesCleared >= scoreSys->level * 10) {
        scoreSys->level++;
    }
}

void resetScore(ScoreSystem *scoreSys) {
    initScoreSystem(scoreSys);
}
