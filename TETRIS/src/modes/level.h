// Nama Program : level.h
// Deskripsi    : Header file untuk pengaturan permainan,
//                mendefinisikan struktur Settings dan fungsi-fungsi terkait untuk
//                memperbarui dan mengelola pengaturan permainan seperti level dan kecepatan.
// Pembuat      : [Rizky Satria Gunawan, 241511089, 24 February 2025, 8:09]

#ifndef LEVEL_H
#define LEVEL_H

const int SPEEDS[] = {
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

typedef struct {
    int mode;
    int speed;
    int score;
    int high_score;
    int lines_cleared;
} Settings;

void init_settings(Settings *settings);
void update_settings(Settings *settings, int mode, int new_speed);
void increase_level(Settings *settings);
void update_score(Settings *settings, int points);

#endif // LEVEL_H