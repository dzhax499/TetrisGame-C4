// Nama Program : level.c
// Deskripsi    : Implementasi pengaturan permainan,
//                bertujuan untuk memperbarui dan mengelola pengaturan permainan seperti level dan kecepatan.
// Pembuat      : [Rizky Satria Gunawan, 241511089, 24 February 2025, 8:09]

#include "level.h"

void init_settings(Settings *settings) {
    settings->mode = 1;
    settings->speed = SPEEDS[0];
    settings->score = 0;
    settings->high_score = 0;
    settings->lines_cleared = 0;
}

void update_settings(Settings *settings, int mode, int new_speed) {
    settings->mode = mode;
    settings->speed = new_speed;
}

void increase_level(Settings *settings) {
    if (settings->mode < 10) {
        settings->mode++;
        settings->speed = SPEEDS[settings->mode - 1];
    }
}

void update_score(Settings *settings, int points) {
    settings->score += points;
    if (settings->score > settings->high_score) {
        settings->high_score = settings->score;
    }
}