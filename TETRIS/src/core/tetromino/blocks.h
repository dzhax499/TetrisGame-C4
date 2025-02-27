/**
 * Nama File : blocks.h
 * Deskripsi : Definisi dan operasi blok Tetromino menggunakan SDL3
 * @author Dzakit Tsabit Assyafiq
 */

 #ifndef BLOCKS_H
 #define BLOCKS_H
 
 #include <SDL3/SDL.h>
 #include <stdbool.h>
 
 // Definisi ukuran blok
 #define BLOCK_SIZE 30
 #define GRID_WIDTH 10
 #define GRID_HEIGHT 20
 
 // Jenis-jenis tetromino
 typedef enum {
     TETROMINO_I,
     TETROMINO_J,
     TETROMINO_L,
     TETROMINO_O,
     TETROMINO_S,
     TETROMINO_T,
     TETROMINO_Z,
     TETROMINO_COUNT
 } TetrominoType;
 
 // Struktur untuk tetromino
 typedef struct {
     TetrominoType type;
     int x, y;          // Posisi grid
     int rotation;      // 0, 1, 2, atau 3 (0, 90, 180, 270 derajat)
     SDL_Color color;   // Warna tetromino
     int blocks[4][4];  // Bentuk tetromino (matriks 4x4)
 } Tetromino;
 
 // Struktur untuk sistem next piece
 typedef struct {
     Tetromino next[3];  // Tiga blok selanjutnya
     Tetromino hold;     // Blok yang disimpan
     bool canHold;       // Apakah pemain dapat menyimpan blok
 } TetrominoQueue;
 
 // Struktur untuk game board
 typedef struct {
     int grid[GRID_HEIGHT][GRID_WIDTH];
     SDL_Color colors[GRID_HEIGHT][GRID_WIDTH];
 } GameBoard;
 
 /**
  * Menginisialisasi sistem blok dan queue
  * @param queue Pointer ke TetrominoQueue
  * @return true jika berhasil, false jika gagal
  */
 bool initBlockSystem(TetrominoQueue* queue);
 
 /**
  * Membuat tetromino baru dengan jenis tertentu
  * @param type Jenis tetromino yang akan dibuat
  * @return Tetromino baru
  */
 Tetromino createTetromino(TetrominoType type);
 
 /**
  * Mendapatkan blok berikutnya dari queue
  * @param queue Pointer ke TetrominoQueue
  * @return Tetromino berikutnya
  */
 Tetromino getNextTetromino(TetrominoQueue* queue);
 
 /**
  * Mengisi kembali queue dengan blok acak
  * @param queue Pointer ke TetrominoQueue
  */
 void refillQueue(TetrominoQueue* queue);
 
 /**
  * Menyimpan tetromino aktif dan mengambil tetromino berikutnya
  * @param queue Pointer ke TetrominoQueue
  * @param current Pointer ke tetromino aktif
  * @return true jika berhasil, false jika tidak bisa hold
  */
 bool holdTetromino(TetrominoQueue* queue, Tetromino* current);
 
 /**
  * Mengatur ulang hold sehingga bisa digunakan lagi
  * @param queue Pointer ke TetrominoQueue
  */
 void resetHold(TetrominoQueue* queue);
 
 /**
  * Mengisi board dengan blok tetromino yang telah terkunci
  * @param board Pointer ke GameBoard
  * @param tetromino Tetromino yang akan dikunci
  */
 void lockTetromino(GameBoard* board, const Tetromino* tetromino);
 
 /**
  * Mengecek apakah baris sudah penuh
  * @param board Pointer ke GameBoard
  * @param clearedLines Array untuk menyimpan indeks baris yang dihapus
  * @return Jumlah baris yang dihapus
  */
 int checkLines(GameBoard* board, int* clearedLines);
 
 /**
  * Menghapus baris yang sudah penuh
  * @param board Pointer ke GameBoard
  * @param lineIndices Array berisi indeks baris yang akan dihapus
  * @param numLines Jumlah baris yang akan dihapus
  */
 void clearLines(GameBoard* board, const int* lineIndices, int numLines);
 
 /**
  * Render tetromino saat ini ke permukaan SDL
  * @param renderer Renderer SDL
  * @param tetromino Tetromino yang akan dirender
  * @param offsetX Offset X (piksel)
  * @param offsetY Offset Y (piksel)
  */
 void renderTetromino(SDL_Renderer* renderer, const Tetromino* tetromino, int offsetX, int offsetY);
 
 /**
  * Render game board ke permukaan SDL
  * @param renderer Renderer SDL
  * @param board Pointer ke GameBoard
  * @param offsetX Offset X (piksel)
  * @param offsetY Offset Y (piksel)
  */
 void renderBoard(SDL_Renderer* renderer, const GameBoard* board, int offsetX, int offsetY);
 
 /**
  * Reset game board
  * @param board Pointer ke GameBoard
  */
 void resetBoard(GameBoard* board);
 
 #endif /* BLOCKS_H */