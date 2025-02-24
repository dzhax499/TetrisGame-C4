// Nama Program : settings.h
// Deskripsi    : Header file untuk pengaturan permainan,
//                mendefinisikan struktur Settings dan fungsi-fungsi terkait untuk
//                memperbarui dan mengelola pengaturan permainan seperti level dan kecepatan.
// Pembuat      : [Rizky Satria Gunawan, 241511089, 24 February 2024, 8:09]

#ifndef LEVEL_H
#define LEVEL_H

// frame delay berdasarkan level (dalam microsecond)
#define SPEED_1 200000
#define SPEED_2 180000
#define SPEED_3 170000
#define SPEED_4 150000
#define SPEED_5 140000
#define SPEED_6 130000
#define SPEED_7 120000
#define SPEED_8 100000
#define SPEED_9 90000
#define SPEED_10 80000


// struct untuk menyimpan pengaturan permainan
typedef struct
{
  int level; // level 1-10
  int speed; // Kecepatan permainan
} Level;

// Prosedur untuk memperbarui pengaturan permainan (kecepatan turunnya block di permainan tetris)
// settings: parameter output passing by reference, tipe Settings, alamat dari objek yang akan diperbarui
// level: parameter input passing by value, tipe integer, level permainan
// new_speed: parameter input passing by value, tipe integer, kecepatan permainan baru
void update_settings(Level *level, int level, int new_speed);

#endif // LEVEL_H