/**
 * blocks.c
 * Implementasi sistem Tetromino untuk game Tetris
 * Oleh: Dzakit Tsabit Assyafiq
 */

 #include "blocks.h"
 #include <stdlib.h>
 #include <string.h>
 #include <time.h>
 
 // Definisi bentuk-bentuk blok Tetromino dalam representasi 4x4
 // Setiap blok direpresentasikan sebagai array 2D (4x4)
 // dimana 1 menandakan bagian blok, 0 menandakan kosong
 const int TETROMINO_SHAPES[7][4][4][4] = {
     // I - bentuk garis (cyan)
     {
         {
             {0, 0, 0, 0},
             {1, 1, 1, 1},
             {0, 0, 0, 0},
             {0, 0, 0, 0}
         },
         {
             {0, 0, 1, 0},
             {0, 0, 1, 0},
             {0, 0, 1, 0},
             {0, 0, 1, 0}
         },
         {
             {0, 0, 0, 0},
             {0, 0, 0, 0},
             {1, 1, 1, 1},
             {0, 0, 0, 0}
         },
         {
             {0, 1, 0, 0},
             {0, 1, 0, 0},
             {0, 1, 0, 0},
             {0, 1, 0, 0}
         }
     },
     // J - bentuk L terbalik (biru)
     {
         {
             {1, 0, 0, 0},
             {1, 1, 1, 0},
             {0, 0, 0, 0},
             {0, 0, 0, 0}
         },
         {
             {0, 1, 1, 0},
             {0, 1, 0, 0},
             {0, 1, 0, 0},
             {0, 0, 0, 0}
         },
         {
             {0, 0, 0, 0},
             {1, 1, 1, 0},
             {0, 0, 1, 0},
             {0, 0, 0, 0}
         },
         {
             {0, 1, 0, 0},
             {0, 1, 0, 0},
             {1, 1, 0, 0},
             {0, 0, 0, 0}
         }
     },
     // L - bentuk L (oranye)
     {
         {
             {0, 0, 1, 0},
             {1, 1, 1, 0},
             {0, 0, 0, 0},
             {0, 0, 0, 0}
         },
         {
             {0, 1, 0, 0},
             {0, 1, 0, 0},
             {0, 1, 1, 0},
             {0, 0, 0, 0}
         },
         {
             {0, 0, 0, 0},
             {1, 1, 1, 0},
             {1, 0, 0, 0},
             {0, 0, 0, 0}
         },
         {
             {1, 1, 0, 0},
             {0, 1, 0, 0},
             {0, 1, 0, 0},
             {0, 0, 0, 0}
         }
     },
     // O - bentuk kotak (kuning)
     {
         {
             {0, 1, 1, 0},
             {0, 1, 1, 0},
             {0, 0, 0, 0},
             {0, 0, 0, 0}
         },
         {
             {0, 1, 1, 0},
             {0, 1, 1, 0},
             {0, 0, 0, 0},
             {0, 0, 0, 0}
         },
         {
             {0, 1, 1, 0},
             {0, 1, 1, 0},
             {0, 0, 0, 0},
             {0, 0, 0, 0}
         },
         {
             {0, 1, 1, 0},
             {0, 1, 1, 0},
             {0, 0, 0, 0},
             {0, 0, 0, 0}
         }
     },
     // S - bentuk S (hijau)
     {
         {
             {0, 1, 1, 0},
             {1, 1, 0, 0},
             {0, 0, 0, 0},
             {0, 0, 0, 0}
         },
         {
             {0, 1, 0, 0},
             {0, 1, 1, 0},
             {0, 0, 1, 0},
             {0, 0, 0, 0}
         },
         {
             {0, 0, 0, 0},
             {0, 1, 1, 0},
             {1, 1, 0, 0},
             {0, 0, 0, 0}
         },
         {
             {1, 0, 0, 0},
             {1, 1, 0, 0},
             {0, 1, 0, 0},
             {0, 0, 0, 0}
         }
     },
     // T - bentuk T (ungu)
     {
         {
             {0, 1, 0, 0},
             {1, 1, 1, 0},
             {0, 0, 0, 0},
             {0, 0, 0, 0}
         },
         {
             {0, 1, 0, 0},
             {0, 1, 1, 0},
             {0, 1, 0, 0},
             {0, 0, 0, 0}
         },
         {
             {0, 0, 0, 0},
             {1, 1, 1, 0},
             {0, 1, 0, 0},
             {0, 0, 0, 0}
         },
         {
             {0, 1, 0, 0},
             {1, 1, 0, 0},
             {0, 1, 0, 0},
             {0, 0, 0, 0}
         }
     },
     // Z - bentuk Z (merah)
     {
         {
             {1, 1, 0, 0},
             {0, 1, 1, 0},
             {0, 0, 0, 0},
             {0, 0, 0, 0}
         },
         {
             {0, 0, 1, 0},
             {0, 1, 1, 0},
             {0, 1, 0, 0},
             {0, 0, 0, 0}
         },
         {
             {0, 0, 0, 0},
             {1, 1, 0, 0},
             {0, 1, 1, 0},
             {0, 0, 0, 0}
         },
         {
             {0, 1, 0, 0},
             {1, 1, 0, 0},
             {1, 0, 0, 0},
             {0, 0, 0, 0}
         }
     }
 };
 
 // Warna untuk setiap jenis Tetromino (format RGBA)
 const SDL_Color TETROMINO_COLORS[7] = {
     {0, 255, 255, 255},   // I - Cyan
     {0, 0, 255, 255},     // J - Blue
     {255, 165, 0, 255},   // L - Orange
     {255, 255, 0, 255},   // O - Yellow
     {0, 255, 0, 255},     // S - Green
     {128, 0, 128, 255},   // T - Purple
     {255, 0, 0, 255}      // Z - Red
 };
 
 // Inisialisasi sistem Tetromino
 TetrominoSystem* tetromino_init() {
     TetrominoSystem* system = (TetrominoSystem*)malloc(sizeof(TetrominoSystem));
     if (!system) {
         return NULL;
     }
     
     // Inisialisasi RNG
     srand(time(NULL));
     
     // Inisialisasi bag Tetromino
     for (int i = 0; i < 7; i++) {
         system->bag[i] = i;
     }
     
     // Kocok bag awal
     tetromino_shuffle_bag(system);
     
     // Set posisi awal indeks bag ke 0
     system->bag_index = 0;
     
     // Buat Tetromino aktif dan berikutnya
     system->current = tetromino_create(tetromino_get_next_from_bag(system));
     system->next = tetromino_create(tetromino_get_next_from_bag(system));
     
     return system;
 }
 
 // Hapus sistem Tetromino
 void tetromino_free(TetrominoSystem* system) {
     if (system) {
         if (system->current) {
             free(system->current);
         }
         if (system->next) {
             free(system->next);
         }
         free(system);
     }
 }
 
 // Kocok bag Tetromino untuk random 7-bag
 void tetromino_shuffle_bag(TetrominoSystem* system) {
     for (int i = 6; i > 0; i--) {
         int j = rand() % (i + 1);
         // Tukar
         int temp = system->bag[i];
         system->bag[i] = system->bag[j];
         system->bag[j] = temp;
     }
 }
 
 // Dapatkan jenis Tetromino berikutnya dari bag
 int tetromino_get_next_from_bag(TetrominoSystem* system) {
     int next_type = system->bag[system->bag_index];
     
     // Tingkatkan indeks bag
     system->bag_index++;
     
     // Jika kita sudah menggunakan seluruh bag, kocok kembali
     if (system->bag_index >= 7) {
         system->bag_index = 0;
         tetromino_shuffle_bag(system);
     }
     
     return next_type;
 }
 
 // Buat Tetromino baru dengan jenis tertentu
 Tetromino* tetromino_create(int type) {
     Tetromino* tetromino = (Tetromino*)malloc(sizeof(Tetromino));
     if (!tetromino) {
         return NULL;
     }
     
     tetromino->type = type;
     tetromino->rotation = 0;
     
     // Posisi awal di bagian atas papan permainan
     // Posisi ini akan bervariasi tergantung pada jenis Tetromino
     tetromino->x = 3;  // Posisi tengah
     tetromino->y = 0;  // Bagian atas papan
     
     // Koreksi posisi awal untuk Tetromino I
     if (type == 0) {  // Piece I
         tetromino->y = -1;
     }
     
     return tetromino;
 }
 
 // Dapatkan blok Tetromino saat ini (berdasarkan jenis dan rotasi)
 void tetromino_get_blocks(const Tetromino* tetromino, int blocks[4][4]) {
     if (!tetromino) {
         memset(blocks, 0, sizeof(int) * 16);
         return;
     }
     
     // Salin bentuk berdasarkan jenis dan rotasi
     for (int y = 0; y < 4; y++) {
         for (int x = 0; x < 4; x++) {
             blocks[y][x] = TETROMINO_SHAPES[tetromino->type][tetromino->rotation][y][x];
         }
     }
 }
 
 // Rotasi Tetromino (kanan/searah jarum jam)
 void tetromino_rotate_right(Tetromino* tetromino) {
     tetromino->rotation = (tetromino->rotation + 1) % 4;
 }
 
 // Rotasi Tetromino (kiri/berlawanan arah jarum jam)
 void tetromino_rotate_left(Tetromino* tetromino) {
     tetromino->rotation = (tetromino->rotation + 3) % 4;  // +3 mod 4 = -1 mod 4
 }
 
 // Ganti Tetromino aktif dengan Tetromino berikutnya dan hasilkan Tetromino baru berikutnya
 void tetromino_swap_next(TetrominoSystem* system) {
     // Bebaskan Tetromino aktif
     if (system->current) {
         free(system->current);
     }
     
     // Aktifkan Tetromino berikutnya
     system->current = system->next;
     
     // Buat Tetromino berikutnya yang baru
     system->next = tetromino_create(tetromino_get_next_from_bag(system));
 }
 
 // Dapatkan warna Tetromino berdasarkan jenisnya
 SDL_Color tetromino_get_color(int type) {
     if (type >= 0 && type < 7) {
         return TETROMINO_COLORS[type];
     }
     
     // Warna default hitam jika jenis tidak valid
     SDL_Color black = {0, 0, 0, 255};
     return black;
 }
 
 // Render Tetromino pada posisi tertentu dengan renderer SDL
 void tetromino_render(SDL_Renderer* renderer, const Tetromino* tetromino, 
                       int offsetX, int offsetY, int cellSize) {
     if (!tetromino) {
         return;
     }
     
     // Dapatkan bentuk Tetromino
     int blocks[4][4];
     tetromino_get_blocks(tetromino, blocks);
     
     // Dapatkan warna Tetromino
     SDL_Color color = tetromino_get_color(tetromino->type);
     
     // Setel warna rendering
     SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);
     
     // Render setiap blok
     for (int y = 0; y < 4; y++) {
         for (int x = 0; x < 4; x++) {
             if (blocks[y][x]) {
                 SDL_Rect rect = {
                     offsetX + (tetromino->x + x) * cellSize,
                     offsetY + (tetromino->y + y) * cellSize,
                     cellSize,
                     cellSize
                 };
                 
                 // Gambar kotak terisi
                 SDL_RenderFillRect(renderer, &rect);
                 
                 // Gambar garis tepi dengan warna yang lebih gelap
                 SDL_SetRenderDrawColor(renderer, color.r / 2, color.g / 2, color.b / 2, color.a);
                 SDL_RenderDrawRect(renderer, &rect);
             }
         }
     }
 }