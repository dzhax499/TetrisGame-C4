/**
 * rotation.h
 * Header untuk sistem rotasi Super Rotation System (SRS) game Tetris
 * Oleh: Dzakit Tsabit Assyafiq
 */

 #ifndef ROTATION_H
 #define ROTATION_H
 
 #include "blocks.h"
 
 // Forward declaration untuk struktur Game
 typedef struct Game Game;
 
 // Hasil dari percobaan rotasi
 typedef struct {
     int success;      // 1 jika rotasi berhasil, 0 jika gagal
     int test_index;   // Indeks tes SRS yang berhasil (-1 jika gagal)
     int offset_x;     // Offset X dari wall kick
     int offset_y;     // Offset Y dari wall kick
 } RotationResult;
 
 // Deklarasi array untuk uji wall kick SRS
 extern const int SRS_JLSTZ_TESTS[4][4][5][2];
 extern const int SRS_I_TESTS[4][4][5][2];
 
 // Performa rotasi dengan SRS
 RotationResult srs_rotate(Game* game, Tetromino* tetromino, int direction);
 
 #endif /* ROTATION_H */