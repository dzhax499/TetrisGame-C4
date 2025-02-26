// Nama program : scoretetris.h
// Deskripsi    : header untuk pengelolaan mode endless (tanpa batas), yang akan berhenti bila kondisi
//                tertentu terpenuhi.
// Pembuat      : [Rizky Satria Gunawan, 241511089, 24 February 2025, 8:40]

#ifndef ENDLESS_H
#define ENDLESS_H

#include "level.h" // Mengimpor pengaturan permainan

/**
 * Mode Endless: Setelah menyelesaikan 30 lines, level kesulitan meningkat.
 * - settings: Pointer ke objek Settings yang akan diperbarui.
 * - lines_cleared: Jumlah lines yang berhasil diselesaikan.
 */
void endless_mode(Settings *settings, int lines_cleared);

#endif // ENDLESS_H