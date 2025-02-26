// Nama Program : level.c
// Deskripsi    : Implementasi pengaturan permainan,
//                bertujuan untuk memperbarui dan mengelola pengaturan permainan seperti level dan kecepatan.
// Pembuat      : [Rizky Satria Gunawan, 241511089, 24 February 2025, 8:09]

#include "level.h"

// Prosedur untuk memperbarui pengaturan permainan (kecepatan permainan)
// settings: parameter output passing by reference, tipe Settings, alamat dari objek yang akan diperbarui
// mode : parameter input passing by value, tipe integer, level permainan
// new_speed: parameter input passing by value, tipe integer, kecepatan permainan baru

void update_settings(Level *level, int mode, int new_speed)
{
  // update level dan speed
  level->mode = mode;
  level->speed = new_speed;
}