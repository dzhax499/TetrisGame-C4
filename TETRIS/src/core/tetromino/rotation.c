/**
 * rotation.c
 * Implementasi sistem rotasi Super Rotation System (SRS) untuk game Tetris
 * Oleh: Dzakit Tsabit Assyafiq
 */

 #include "rotation.h"
 #include "collision.h"
 #include <stdlib.h>
 
 // Data uji SRS (wall kick) untuk piece J, L, S, T, Z
 // [rotasi awal][rotasi akhir][uji ke-n][x, y]
 const int SRS_JLSTZ_TESTS[4][4][5][2] = {
     // Rotasi 0->1
     {
         {
             {0, 0}, {-1, 0}, {-1, -1}, {0, 2}, {-1, 2}
         },
         // Rotasi 1->2
         {
             {0, 0}, {1, 0}, {1, 1}, {0, -2}, {1, -2}
         },
         // Rotasi 2->3
         {
             {0, 0}, {1, 0}, {1, -1}, {0, 2}, {1, 2}
         },
         // Rotasi 3->0
         {
             {0, 0}, {-1, 0}, {-1, 1}, {0, -2}, {-1, -2}
         }
     },
     // Rotasi 1->0
     {
         {
             {0, 0}, {1, 0}, {1, -1}, {0, 2}, {1, 2}
         },
         // Rotasi 2->1
         {
             {0, 0}, {-1, 0}, {-1, 1}, {0, -2}, {-1, -2}
         },
         // Rotasi 3->2
         {
             {0, 0}, {-1, 0}, {-1, -1}, {0, 2}, {-1, 2}
         },
         // Rotasi 0->3
         {
             {0, 0}, {1, 0}, {1, 1}, {0, -2}, {1, -2}
         }
     }
 };
 
 // Data uji SRS (wall kick) untuk piece I
 // [rotasi awal][rotasi akhir][uji ke-n][x, y]
 const int SRS_I_TESTS[4][4][5][2] = {
     // Rotasi 0->1
     {
         {
             {0, 0}, {-2, 0}, {1, 0}, {-2, 1}, {1, -2}
         },
         // Rotasi 1->2
         {
             {0, 0}, {-1, 0}, {2, 0}, {-1, -2}, {2, 1}
         },
         // Rotasi 2->3
         {
             {0, 0}, {2, 0}, {-1, 0}, {2, -1}, {-1, 2}
         },
         // Rotasi 3->0
         {
             {0, 0}, {1, 0}, {-2, 0}, {1, 2}, {-2, -1}
         }
     },
     // Rotasi 1->0
     {
         {
             {0, 0}, {2, 0}, {-1, 0}, {2, -1}, {-1, 2}
         },
         // Rotasi 2->1
         {
             {0, 0}, {1, 0}, {-2, 0}, {1, 2}, {-2, -1}
         },
         // Rotasi 3->2
         {
             {0, 0}, {-2, 0}, {1, 0}, {-2, 1}, {1, -2}
         },
         // Rotasi 0->3
         {
             {0, 0}, {-1, 0}, {2, 0}, {-1, -2}, {2, 1}
         }
     }
 };
 
 // Performa rotasi dengan SRS
 RotationResult srs_rotate(Game* game, Tetromino* tetromino, int direction) {
     RotationResult result;
     result.success = 0;
     result.test_index = -1;
     result.offset_x = 0;
     result.offset_y = 0;
     
     if (!tetromino) {
         return result;
     }
     
     // Simpan rotasi awal
     int original_rotation = tetromino->rotation;
     
     // Performa rotasi (tanpa wall kick dulu)
     if (direction > 0) {
         tetromino_rotate_right(tetromino);
     } else {
         tetromino_rotate_left(tetromino);
     }
     
     // Dapatkan rotasi baru
     int new_rotation = tetromino->rotation;
     
     // Menentukan indeks dalam tabel uji SRS
     int from_to_index;
     if (direction > 0) { // Searah jarum jam
         from_to_index = 0;
     } else { // Berlawanan arah jarum jam
         from_to_index = 1;
     }
     
     // Dapatkan array uji yang benar berdasarkan jenis Tetromino
     const int (*tests)[5][2];
     int original_index = original_rotation;
     
     if (tetromino->type == TETROMINO_I) {
         tests = SRS_I_TESTS[from_to_index][original_index];
     } else if (tetromino->type != TETROMINO_O) { // Piece O tidak perlu wall kick
         tests = SRS_JLSTZ_TESTS[from_to_index][original_index];
     } else {
         // Piece O hanya mempunyai 1 orientasi yang unik
         result.success = 1;
         return result;
     }
     
     // Posisi awal
     int original_x = tetromino->x;
     int original_y = tetromino->y;
     
     // Coba setiap uji wall kick
     for (int i = 0; i < 5; i++) {
         int offset_x = tests[i][0];
         int offset_y = tests[i][1];
         
         // Terapkan offset
         tetromino->x = original_x + offset_x;
         tetromino->y = original_y + offset_y;
         
         // Periksa apakah posisi ini valid (tidak bertabrakan)
         if (!is_collision(game, tetromino)) {
             // Berhasil
             result.success = 1;
             result.test_index = i;
             result.offset_x = offset_x;
             result.offset_y = offset_y;
             return result;
         }
     }
     
     // Jika tidak ada uji yang berhasil, kembalikan ke rotasi semula
     tetromino->x = original_x;
     tetromino->y = original_y;
     tetromino->rotation = original_rotation;
     
     return result;
 }