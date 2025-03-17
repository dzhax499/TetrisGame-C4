/**
 * rendering.h
 * 
 * Header file untuk fungsi-fungsi rendering grafis dalam permainan Tetris
 * Mendefinisikan fungsi-fungsi yang digunakan untuk menampilkan elemen visual permainan
 * Penulis: Abi
 */

 #ifndef RENDERING_H
 #define RENDERING_H
 
 #include <raylib.h>
 #include "tetris.h"
 #include "board.h"
 #include "blocks.h"
 #include "scoring.h"
 
 /**
  * Konstanta untuk rendering
  * Mendefinisikan ukuran blok dan posisi elemen pada layar
  */
 #define BLOCK_SIZE 30         // Ukuran satu blok tetromino dalam piksel
 #define BOARD_OFFSET_X 300    // Posisi X dari sudut kiri atas papan permainan
 #define BOARD_OFFSET_Y 40     // Posisi Y dari sudut kiri atas papan permainan
 #define PREVIEW_OFFSET_X 550  // Posisi X untuk area pratinjau tetromino berikutnya
 #define PREVIEW_OFFSET_Y 100  // Posisi Y untuk area pratinjau tetromino berikutnya
 
 /**
  * InitRendering
  * 
  * Menginisialisasi semua sumber daya yang diperlukan untuk rendering
  * Membuat jendela, memuat font, tekstur, dan menetapkan target FPS
  */
 void InitRendering(void);
 
 /**
  * CloseRendering
  * 
  * Membersihkan semua sumber daya rendering yang telah dialokasikan
  * Menutup jendela dan membebaskan memori yang digunakan
  */
 void CloseRendering(void);
 
 /**
  * DrawBlock
  * 
  * Menggambar satu blok tetromino pada koordinat yang ditentukan
  * 
  * @param x          Koordinat X blok pada grid papan permainan
  * @param y          Koordinat Y blok pada grid papan permainan
  * @param blockType  Tipe blok yang akan digambar (1-7 untuk berbagai tetromino)
  */
 void DrawBlock(int x, int y, int blockType);
 
 /**
  * DrawBoard
  * 
  * Menggambar papan permainan dengan semua blok yang sudah ditempatkan
  * Termasuk latar belakang papan dan garis grid
  * 
  * @param board  Pointer ke struktur Board yang berisi informasi grid permainan
  */
 void DrawBoard(Board* board);
 
 /**
  * DrawActiveTetromino
  * 
  * Menggambar tetromino aktif yang sedang dimainkan pada posisinya saat ini
  * 
  * @param tetromino  Pointer ke struktur Tetromino yang sedang aktif
  */
 void DrawActiveTetromino(Tetromino* tetromino);
 
 /**
  * DrawNextTetromino
  * 
  * Menggambar tetromino berikutnya di area pratinjau
  * 
  * @param nextTetromino  Pointer ke struktur Tetromino yang akan muncul berikutnya
  */
 void DrawNextBlock(Tetromino* nextTetromino);
 
 /**
  * DrawGameInterface
  * 
  * Menggambar antarmuka permainan termasuk skor, level, dan statistik lainnya
  * 
  * @param scoreData  Pointer ke struktur ScoreData yang berisi informasi skor pemain
  */
 void DrawGameInterface(ScoreData* scoreData);
 
 /**
  * DrawGameOver
  * 
  * Menampilkan layar game over saat permainan berakhir
  * 
  * @param scoreData  Pointer ke struktur ScoreData untuk menampilkan skor akhir
  */
 void DrawGameOver(ScoreData* scoreData);
 
 /**
  * DrawPauseScreen
  * 
  * Menampilkan layar jeda saat permainan dipause
  */
 void DrawPauseScreen(void);
 
 /**
  * DrawGame
  * 
  * Fungsi utama rendering yang memanggil semua fungsi rendering lainnya
  * Menggambar seluruh tampilan permainan berdasarkan status saat ini
  * 
  * @param gameState  Pointer ke struktur GameState yang berisi keadaan permainan saat ini
  */
 void DrawGame(GameState* gameState);
 
 #endif /* RENDERING_H */