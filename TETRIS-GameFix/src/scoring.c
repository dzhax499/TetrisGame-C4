/**
 * scoring.c
 * 
 * Menangani sistem skor, perkembangan level, dan tingkat kesulitan permainan
 * Penulis: Fatimah 
 */

 #include <raylib.h>
 #include "include/tetris.h"
 #include "include/scoring.h"
 
 #define HIGH_SCORE_FILE "../assets/log/highscore.dat"

 // Inisialisasi sistem skor
 void InitScoring(ScoreData* scoreData) {
     scoreData->score = 0;
     scoreData->level = 1;
     scoreData->linesCleared = 0;
     scoreData->fallSpeed = 1.0f;
     scoreData->linesToNextLevel = 10;
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
         
         // Putar suara level up (akan diimplementasikan di tempat lain)
     }
 }
 
 // Dapatkan tingkat kesulitan saat ini
 int GetDifficulty(ScoreData* scoreData) {
     return scoreData->level;
 }
 
 // Simpan skor tertinggi ke dalam file
 void SaveHighScore(ScoreData* scoreData) {
    int currentHighScore = LoadHighScore();
    
    if (scoreData->score > currentHighScore) {
        FILE* file = fopen(HIGH_SCORE_FILE, "w");  // Gunakan mode text
        if (file) {
            fprintf(file, "%d", scoreData->score);  // Tulis sebagai string angka
            fclose(file);
            printf("High score saved: %d\n", scoreData->score);  // Debug
        } else {
            printf("Failed to open file for writing\n");  // Debug
        }
    }
}
 
 // Muat skor tertinggi dari file
 int LoadHighScore(void) {
    int highScore = 0;
    FILE* file = fopen(HIGH_SCORE_FILE, "r");  // Gunakan mode text
    
    if (file) {
        fscanf(file, "%d", &highScore);  // Baca sebagai angka
        fclose(file);
        printf("High score loaded: %d\n", highScore);  // Debug
    } else {
        printf("High score file not found, using default 0\n");  // Debug
    }
    
    return highScore;
 }
 