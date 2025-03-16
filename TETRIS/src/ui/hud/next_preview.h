/**
 * next_preview.h
 * Header untuk preview blok berikutnya game Tetris
 * Oleh: Dzakit Tsabit Assyafiq
 */

 #ifndef NEXT_PREVIEW_H
 #define NEXT_PREVIEW_H
 
 #include "blocks.h"
 #include <SDL3/SDL.h>
 
 // Sistem untuk mengelola preview Tetromino berikutnya
 typedef struct {
     int x;                      // Posisi X preview
     int y;                      // Posisi Y preview
     int cell_size;              // Ukuran sel di preview
     SDL_Color background_color; // Warna latar preview
     SDL_Color border_color;     // Warna batas preview
     int border_width;           // Lebar batas preview
 } NextPreviewSystem;
 
 // Fungsi-fungsi untuk mengelola preview
 NextPreviewSystem* next_preview_init();
 void next_preview_free(NextPreviewSystem* system);
 void next_preview_set_position(NextPreviewSystem* system, int x, int y, int cell_size);
 void next_preview_set_colors(NextPreviewSystem* system, 
                            SDL_Color background_color, 
                            SDL_Color border_color, 
                            int border_width);
 void next_preview_render(SDL_Renderer* renderer, 
                        NextPreviewSystem* system, 
                        TetrominoSystem* tetromino_system);
 
 #endif /* NEXT_PREVIEW_H */