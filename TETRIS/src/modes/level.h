// Nama Program : settings.h
// Deskripsi    : Header file untuk pengaturan permainan,
//                mendefinisikan struktur Settings dan fungsi-fungsi terkait untuk
//                memperbarui dan mengelola pengaturan permainan seperti level dan kecepatan.
// Pembuat      : [Rizky Satria Gunawan, 241511089, 24 February 2025, 8:09]

#ifndef SETTINGS_H
#define SETTINGS_H

// Kecepatan berdasarkan level (dalam mikrodetik)
const int SPEEDS[] = 
{
    200000, // Level 1
    180000, // Level 2
    160000, // Level 3
    140000, // Level 4
    120000, // Level 5
    100000, // Level 6
    90000,  // Level 7
    80000,  // Level 8
    70000,  // Level 9
    60000   // Level 10
};

/**
 * Struct untuk menyimpan pengaturan permainan.
 * - mode: Level permainan (1-10).
 * - speed: Kecepatan permainan (dalam mikrodetik).
 * - score: Skor saat ini.
 * - high_score: Skor tertinggi yang pernah dicapai.
 */
typedef struct 
{
    int mode;
    int speed;
    int score;
    int high_score;
} Settings;

/**
 * Menginisialisasi pengaturan permainan dengan nilai default.
 * - settings: Pointer ke objek Settings yang akan diinisialisasi.
 */
void init_settings(Settings *settings);

/**
 * Memperbarui pengaturan permainan berdasarkan level dan kecepatan baru.
 * - settings: Pointer ke objek Settings yang akan diperbarui.
 * - mode: Level baru (1-10).
 * - new_speed: Kecepatan baru (dalam mikrodetik).
 */
void update_settings(Settings *settings, int mode, int new_speed);

/**
 * Meningkatkan level permainan.
 * - settings: Pointer ke objek Settings yang akan diperbarui.
 */
void increase_level(Settings *settings);

/**
 * Memperbarui skor permainan.
 * - settings: Pointer ke objek Settings yang akan diperbarui.
 * - points: Poin yang akan ditambahkan ke skor.
 */
void update_score(Settings *settings, int points);

#endif // SETTINGS_H