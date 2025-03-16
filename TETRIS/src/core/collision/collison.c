/**
 * collision.c
 * Implementasi deteksi tabrakan untuk game Tetris
 * Oleh: Dzakit Tsabit Assyafiq
 */

 #include "collision.h"
 #include <stdlib.h>
 
 // Periksa apakah terjadi tabrakan dengan Tetromino dan grid/batas papan
 int is_collision(Game* game, const Tetromino* tetromino) {
     if (!game || !tetromino) {
         return 1; // Anggap tabrakan jika data tidak valid
     }
     
     // Dapatkan bentuk Tetromino
     int blocks[4][4];
     tetromino_get_blocks(tetromino, blocks);
     
     // Periksa setiap sel dari bentuk Tetromino
     for (int y = 0; y < 4; y++) {
         for (int x = 0; x < 4; x++) {
             if (blocks[y][x]) {
                 int board_x = tetromino->x + x;
                 int board_y = tetromino->y + y;
                 
                 // Periksa tabrakan dengan batas papan
                 if (board_x < 0 || board_x >= GAME_BOARD_WIDTH || board_y >= GAME_BOARD_HEIGHT) {
                     return 1; // Tabrakan dengan batas papan
                 }
                 
                 // Izinkan Tetromino berada di luar bagian atas papan
                 if (board_y < 0) {
                     continue;
                 }
                 
                 // Periksa tabrakan dengan blok-blok yang sudah ada di papan
                 if (game->board[board_y][board_x]) {
                     return 1; // Tabrakan dengan blok yang sudah ada
                 }
             }
         }
     }
     
     return 0; // Tidak ada tabrakan
 }
 
 // Periksa apakah Tetromino saat ini bisa bergerak ke bawah
 int can_move_down(Game* game, const Tetromino* tetromino) {
     if (!game || !tetromino) {
         return 0;
     }
     
     // Buat Tetromino sementara untuk pengujian
     Tetromino temp = *tetromino;
     temp.y++;
     
     // Periksa tabrakan dengan posisi yang baru
     return !is_collision(game, &temp);
 }
 
 // Periksa apakah Tetromino saat ini bisa bergerak ke kiri
 int can_move_left(Game* game, const Tetromino* tetromino) {
     if (!game || !tetromino) {
         return 0;
     }
     
     // Buat Tetromino sementara untuk pengujian
     Tetromino temp = *tetromino;
     temp.x--;
     
     // Periksa tabrakan dengan posisi yang baru
     return !is_collision(game, &temp);
 }
 
 // Periksa apakah Tetromino saat ini bisa bergerak ke kanan
 int can_move_right(Game* game, const Tetromino* tetromino) {
     if (!game || !tetromino) {
         return 0;
     }
     
     // Buat Tetromino sementara untuk pengujian
     Tetromino temp = *tetromino;
     temp.x++;
     
     // Periksa tabrakan dengan posisi yang baru
     return !is_collision(game, &temp);
 }
 
 // Kunci Tetromino ke dalam papan permainan
 void lock_tetromino(Game* game, const Tetromino* tetromino) {
     if (!game || !tetromino) {
         return;
     }
     
     // Dapatkan bentuk Tetromino
     int blocks[4][4];
     tetromino_get_blocks(tetromino, blocks);
     
     // Salin blok-blok Tetromino ke dalam papan permainan
     for (int y = 0; y < 4; y++) {
         for (int x = 0; x < 4; x++) {
             if (blocks[y][x]) {
                 int board_x = tetromino->x + x;
                 int board_y = tetromino->y + y;
                 
                 // Pastikan koordinat valid
                 if (board_x >= 0 && board_x < GAME_BOARD_WIDTH && 
                     board_y >= 0 && board_y < GAME_BOARD_HEIGHT) {
                     // Simpan jenis Tetromino (warna) di dalam papan
                     game->board[board_y][board_x] = tetromino->type + 1;
                 }
             }
         }
     }
 }
 
 // Hitung bayangan (ghost) Tetromino (posisi di mana Tetromino akan jatuh)
 void calculate_ghost_position(Game* game, const Tetromino* tetromino, Tetromino* ghost) {
     if (!game || !tetromino || !ghost) {
         return;
     }
     
     // Salin Tetromino untuk ghost
     *ghost = *tetromino;
     
     // Tetapkan ghost ke bawah sampai bertabrakan
     while (!is_collision(game, ghost)) {
         ghost->y++;
     }
     
     // Kembalikan satu langkah karena sudah terjadi tabrakan
     ghost->y--;
 }