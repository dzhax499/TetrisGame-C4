// Nama file : timer.c
// Deskripsi : Implementasi pengelolaan timer dalam permainan Tetris
// Oleh      : Ibnu Hilmi 241511079

#include "include/timer.h"
#include <stdlib.h>

static float startTime = 0.0f;
static float elapsedTime = 0.0f;
static float pauseStartTime = 0.0f;
static float totalPausedDuration = 0.0f;
static bool isPaused = false;

/**
 * Menginisialisasi timer permainan
 * Mengatur waktu awal permainan dan mereset semua variabel timer
 * Dipanggil saat permainan dimulai
 */
void InitGameTimer(void) {
    startTime = GetTime();
    elapsedTime = 0.0f;
    pauseStartTime = 0.0f;
    totalPausedDuration = 0.0f;
    isPaused = false;
}

/**
 * Memperbarui waktu yang telah berlalu dalam permainan
 * Menghitung waktu elapsed dengan memperhitungkan durasi pause
 * Hanya menghitung waktu jika timer tidak dalam keadaan pause
 * Dipanggil setiap frame saat permainan berjalan
 */
void UpdateGameTimer(void) {
    if (!isPaused) {
        elapsedTime = GetTime() - startTime - totalPausedDuration;
    }
}

/**
 * Mendapatkan waktu yang telah berlalu dalam permainan
 * @return Waktu dalam detik sejak permainan dimulai (tidak termasuk waktu pause)
 */
float GetElapsedGameTime(void) {
    return elapsedTime;
}

/**
 * Menjeda timer permainan
 * Menyimpan waktu saat pause dimulai untuk menghitung durasi pause
 * Tidak akan melakukan apa-apa jika timer sudah dalam keadaan pause
 */
void PauseGameTimer(void) {
    if (!isPaused) {
        pauseStartTime = GetTime();
        isPaused = true;
    }
}

/**
 * Melanjutkan timer permainan setelah pause
 * Menghitung total durasi pause dan menambahkannya ke totalPausedDuration
 * Tidak akan melakukan apa-apa jika timer tidak dalam keadaan pause
 */
void ResumeGameTimer(void) {
    if (isPaused) {
        totalPausedDuration += GetTime() - pauseStartTime;
        isPaused = false;
    }
}

/**
 * Mengecek apakah timer sedang dalam keadaan pause
 * @return true jika timer sedang pause, false jika sedang berjalan
 */
bool IsTimerPaused(void) {
    return isPaused;
}