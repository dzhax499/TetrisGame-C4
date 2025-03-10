/**
 * blocks.h
 * Header untuk sistem Tetromino game Tetris
 * Oleh: Dzakit Tsabit Assyafiq
 */

 #ifndef BLOCKS_H
 #define BLOCKS_H
 
 #include <SDL3/SDL.h>
 
 // Tipe-tipe Tetromino
 typedef enum {
     TETROMINO_I = 0,  // I - bentuk garis (cyan)
     TETROMINO_J,      // J - bentuk L terbalik (biru)
     TETROMINO_L,      // L - bentuk L (oranye)
     TETROMINO_O,      // O - bentuk kotak (kuning)
     TETROMINO_S,      // S - bentuk S (hijau)
     TETROMINO_T,      // T - bentuk T (ungu)
     TETROMINO_Z       // Z - bentuk Z (merah)
 } TetrominoType;
 
 // Struktur untuk merepresentasikan Tetromino
 typedef struct {
     int type;       // Jenis Tetromino (0-6)
     int rotation;   // Rotasi saat ini (0-3)
     int x;          // Posisi X di grid
     int y;          // Posisi Y di grid
 } Tetromino;
 
 // Sistem untuk mengelola Tetromino dan set berikutnya
 typedef struct {
     Tetromino* current;   // Tetromino yang aktif saat ini
     Tetromino* next;      // Tetromino berikutnya yang akan muncul
     int bag[7];           // Tas 7-bag untuk sistem acak
     int bag_index;        // Indeks saat ini dalam tas
 } TetrominoSystem;
 
 // Deklarasi array bentuk-bentuk Tetromino dan warna
 extern const int TETROMINO_SHAPES[7][4][4][4];
 extern const SDL_Color TETROMINO_COLORS[7];
 
 // Fungsi-fungsi untuk mengelola Tetromino
 TetrominoSystem* tetromino_init();
 void tetromino_free(TetrominoSystem* system);
 void tetromino_shuffle_bag(TetrominoSystem* system);
 int tetromino_get_next_from_bag(TetrominoSystem* system);
 Tetromino* tetromino_create(int type);
 void tetromino_get_blocks(const Tetromino* tetromino, int blocks[4][4]);
 void tetromino_rotate_right(Tetromino* tetromino);
 void tetromino_rotate_left(Tetromino* tetromino);
 void tetromino_swap_next(TetrominoSystem* system);
 SDL_Color tetromino_get_color(int type);
 void tetromino_render(SDL_Renderer* renderer, const Tetromino* tetromino, 
                       int offsetX, int offsetY, int cellSize);
 
 #endif /* BLOCKS_H */