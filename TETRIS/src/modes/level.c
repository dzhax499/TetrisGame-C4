// Nama Program : level.c
// Deskripsi    : Implementasi pengaturan permainan,
//                bertujuan untuk memperbarui dan mengelola pengaturan permainan seperti level dan kecepatan.
// Pembuat      : [Rizky Satria Gunawan, 241511089, 24 February 2025, 8:09]

#include "level.h"

// Prosedur untuk memperbarui pengaturan permainan (kecepatan permainan)
// settings: parameter output passing by reference, tipe Settings, alamat dari objek yang akan diperbarui
// mode : parameter input passing by value, tipe integer, level permainan
// new_speed: parameter input passing by value, tipe integer, kecepatan permainan baru

void init_settings(Settings *settings) 
{
  settings->mode = 1;
  settings->speed = SPEEDS[0];
  settings->score = 0;
  settings->high_score = 0;
  settings->lines_cleared = 0;
}