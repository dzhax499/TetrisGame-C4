/**
 * next_preview.c
 * Implementasi preview blok berikutnya untuk game Tetris
 * Oleh: Dzakit Tsabit Assyafiq
 */

 #include "next_preview.h"
 #include <stdlib.h>
 
 // Inisialisasi sistem preview
 NextPreviewSystem* next_preview_init() {
     NextPreviewSystem* system = (NextPreviewSystem*)malloc(sizeof(NextPreviewSystem));
     if (!system) {
         return NULL;
     }
     
     // Inisialisasi nilai default
     system->x = 0;
     system->y = 0;
     system->cell_size = 20;
     system->background_color = (SDL_Color){50, 50, 50, 255};
     system->border_color = (SDL_Color){200, 200, 200, 255};
     system->border_width = 2;
     
     return system;
 }
 
 // Hapus sistem preview
 void next_preview_free(NextPreviewSystem* system) {
     if (system) {
         free(system);
     }
 }
 
 // Atur posisi dan ukuran preview
 void next_preview_set_position(NextPreviewSystem* system, int x, int y, int cell_size) {
     if (!system) {
         return;
     }
     
     system->x = x;
     system->y = y;
     system->cell_size = cell_size;
 }
 
 // Atur warna latar dan batas preview
 void next_preview_set_colors(NextPreviewSystem* system, 
                             SDL_Color background_color, 
                             SDL_Color border_color, 
                             int border_width) {
     if (!system) {
         return;
     }
     
     system->background_color = background_color;
     system->border_color = border_color;
     system->border_width = border_width;
 }
 
 // Render preview Tetromino berikutnya
 void next_preview_render(SDL_Renderer* renderer, 
                         NextPreviewSystem* system, 
                         TetrominoSystem* tetromino_system) {
     if (!renderer || !system || !tetromino_system || !tetromino_system->next) {
         return;
     }
     
     // Ukuran kotak preview (4x2 sel)
     int preview_width = 4 * system->cell_size;
     int preview_height = 2 * system->cell_size;
     
     // Kotak latar untuk preview
     SDL_Rect background = {
         system->x,
         system->y,
         preview_width + 2 * system->border_width,
         preview_height + 2 * system->border_width
     };
     
     // Render latar
     SDL_SetRenderDrawColor(renderer, 
                           system->background_color.r, 
                           system->background_color.g, 
                           system->background_color.b, 
                           system->background_color.a);
     SDL_RenderFillRect(renderer, &background);
     
     // Render batas
     if (system->border_width > 0) {
         SDL_SetRenderDrawColor(renderer, 
                               system->border_color.r, 
                               system->border_color.g, 
                               system->border_color.b, 
                               system->border_color.a);
         
         for (int i = 0; i < system->border_width; i++) {
             SDL_Rect border = {
                 background.x + i,
                 background.y + i,
                 background.w - 2 * i,
                 background.h - 2 * i
             };
             SDL_RenderDrawRect(renderer, &border);
         }
     }
     
     // Dapatkan bentuk Tetromino berikutnya
     Tetromino* next = tetromino_system->next;
     int blocks[4][4];
     tetromino_get_blocks(next, blocks);
     
     // Dapatkan warna Tetromino
     SDL_Color color = tetromino_get_color(next->type);
     
     // Offset untuk memusatkan Tetromino dalam preview
     int offset_x = 0;
     int offset_y = 0;
     
     // Penyesuaian posisi berdasarkan jenis Tetromino
     switch (next->type) {
         case TETROMINO_I:
             offset_x = system->cell_size / 2;
             offset_y = 0;
             break;
         case TETROMINO_O:
             offset_x = system->cell_size;
             offset_y = 0;
             break;
         default:
             offset_x = system->cell_size;
             offset_y = 0;
             break;
     }
     
     // Render setiap blok dari Tetromino berikutnya
     SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);
     
     for (int y = 0; y < 4; y++) {
         for (int x = 0; x < 4; x++) {
             if (blocks[y][x]) {
                 SDL_Rect rect = {
                     system->x + system->border_width + offset_x + x * system->cell_size,
                     system->y + system->border_width + offset_y + y * system->cell_size,
                     system->cell_size,
                     system->cell_size
                 };
                 
                 // Render blok
                 SDL_RenderFillRect(renderer, &rect);
                 
                 // Render garis tepi blok
                 SDL_SetRenderDrawColor(renderer, color.r / 2, color.g / 2, color.b / 2, color.a);
                 SDL_RenderDrawRect(renderer, &rect);
                 
                 // Kembalikan warna untuk blok berikutnya
                 SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);
             }
         }
     }
 }