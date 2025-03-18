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
 #include "board.h"
 #include "blocks.h"
 #include "scoring.h"
 
 /**
  * Konstanta untuk rendering
  * Mendefinisikan ukuran blok dan posisi elemen pada layar
//   */
//  // Konstanta ukuran papan
// #define BOARD_WIDTH 10
// #define BOARD_HEIGHT 20
// #define BLOCK_SIZE 30

// #ifndef BOARD_OFFSET_X
// #define BOARD_OFFSET_X 100
// #endif

// #ifndef BOARD_OFFSET_Y
// #define BOARD_OFFSET_Y 50
// #endif
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
 void DrawBoard(TetrisBoard* board);
 void DrawScore(TetrisBoard* board, ScoreData* scoreData);
 
 /**
  * DrawActiveTetromino
  * 
  * Menggambar tetromino aktif yang sedang dimainkan pada posisinya saat ini
  * 
  * @param tetromino  Pointer ke struktur Tetromino yang sedang aktif
  */
 void DrawActiveTetromino(TetrisBlock* tetromino);
 
 /**
  * DrawNextTetromino
  * 
  * Menggambar tetromino berikutnya di area pratinjau
  * 
  * @param nextTetromino  Pointer ke struktur Tetromino yang akan muncul berikutnya
  */
 void DrawNextBlock(TetrisBoard* board);
 void DrawBlockShadow(TetrisBlock* block, TetrisBoard* board);
 void DrawHoldBlock(TetrisBoard* board);
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
 void DrawGame(TetrisBoard* board);
 
 #endif /* RENDERING_H */