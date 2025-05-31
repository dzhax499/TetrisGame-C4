// Nama file : timer.h
// Deskripsi : Header untuk pengelolaan timer dalam permainan Tetris
// Oleh      : Ibnu Hilmi 241511079

#ifndef TIMER_H
#define TIMER_H

#include <raylib.h>
#include <stdbool.h>

/**
 * Menginisialisasi timer permainan
 * Mengatur waktu awal permainan dan mereset semua variabel timer
 * Dipanggil saat permainan dimulai
 */
void InitGameTimer(void);

/**
 * Memperbarui waktu yang telah berlalu dalam permainan
 * Menghitung waktu elapsed dengan memperhitungkan durasi pause
 * Hanya menghitung waktu jika timer tidak dalam keadaan pause
 * Dipanggil setiap frame saat permainan berjalan
 */
void UpdateGameTimer(void);

/**
 * Mendapatkan waktu yang telah berlalu dalam permainan
 * @return Waktu dalam detik sejak permainan dimulai (tidak termasuk waktu pause)
 */
float GetElapsedGameTime(void);

/**
 * Menjeda timer permainan
 * Menyimpan waktu saat pause dimulai untuk menghitung durasi pause
 * Tidak akan melakukan apa-apa jika timer sudah dalam keadaan pause
 */
void PauseGameTimer(void);

/**
 * Melanjutkan timer permainan setelah pause
 * Menghitung total durasi pause dan menambahkannya ke totalPausedDuration
 * Tidak akan melakukan apa-apa jika timer tidak dalam keadaan pause
 */
void ResumeGameTimer(void);

/**
 * Mengecek apakah timer sedang dalam keadaan pause
 * @return true jika timer sedang pause, false jika sedang berjalan
 */
bool IsTimerPaused(void);

#endif // TIMER_H