// Nama Program : endless.c
// Deskripsi    : Program ini digunakan untuk mode endless dimana permainan akan terus berlanjut 
//                sampai pemain memilih untuk berhenti / mengalami kekalahan.
// Pembuat      : [Rizky Satria Gunawan, 241511089, 24 February 2024, 8:32]


#include "endless.h"

#define LINES_TO_INCREASE_LEVEL 30

void endless_mode(Settings *settings, int lines_cleared) {
    if (lines_cleared >= LINES_TO_INCREASE_LEVEL) {
        int level_increase = lines_cleared / LINES_TO_INCREASE_LEVEL;
        for (int i = 0; i < level_increase; i++) {
            if (settings->mode < 10) {
                settings->mode++;
                settings->speed = SPEEDS[settings->mode - 1];
            }
        }
        settings->lines_cleared = lines_cleared % LINES_TO_INCREASE_LEVEL;
    }
}