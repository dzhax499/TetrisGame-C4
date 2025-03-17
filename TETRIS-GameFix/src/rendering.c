/**
 * rendering.c
 * 
 * Bertanggung jawab untuk semua proses rendering grafis dalam permainan Tetris
 * Penulis: Abi
 */

 #include <raylib.h>
 #include "include/tetris.h"
 #include "include/board.h"
 #include "include/blocks.h"
 #include "include/rendering.h"
 #include "include/scoring.h"
 
 // Konstanta untuk rendering
 #define BLOCK_SIZE 30
 #define BOARD_OFFSET_X 300
 #define BOARD_OFFSET_Y 40
 #define PREVIEW_OFFSET_X 550
 #define PREVIEW_OFFSET_Y 100
 
 // Warna untuk setiap blok tetromino
 static const Color BLOCK_COLORS[] = {
     BLANK,                  // Ruang kosong (0)
     SKYBLUE,                // Blok I (1)
     DARKBLUE,               // Blok J (2)
     ORANGE,                 // Blok L (3)
     YELLOW,                 // Blok O (4)
     LIME,                   // Blok S (5)
     PURPLE,                 // Blok T (6)
     RED                     // Blok Z (7)
 };
 
// Deklarasi variabel global untuk font dan tekstur
static Font gameFont;
static Texture2D backgroundTexture;

// Inisialisasi sumber daya untuk rendering
void InitRendering(void) {
    // Memuat tekstur, font, dll.
    InitWindow(800, 600, "Tetris Game");
    SetTargetFPS(60);
    
    // Memuat font permainan
    int fontChars[] = { 0 }; // Memuat semua karakter default
    gameFont = LoadFontEx("assets/fonts/game_font.ttf", 32, fontChars, 0);
    SetTextureFilter(gameFont.texture, TEXTURE_FILTER_BILINEAR);
}    
// Membersihkan sumber daya rendering
void CloseRendering(void) {
    // Melepaskan tekstur, font, dll.
    UnloadFont(gameFont);
    UnloadTexture(backgroundTexture);
    CloseWindow();
}
 void CloseRendering(void) {
     // Melepaskan tekstur, font, dll.
     CloseWindow();
 }
 
 // Menggambar satu blok pada koordinat papan yang ditentukan
 void DrawBlock(int x, int y, int blockType) {
     if (blockType > 0) {
         DrawRectangle(
             BOARD_OFFSET_X + x * BLOCK_SIZE, 
             BOARD_OFFSET_Y + y * BLOCK_SIZE, 
             BLOCK_SIZE, BLOCK_SIZE, 
             BLOCK_COLORS[blockType]
         );
         
         // Menggambar garis tepi
         DrawRectangleLines(
             BOARD_OFFSET_X + x * BLOCK_SIZE, 
             BOARD_OFFSET_Y + y * BLOCK_SIZE, 
             BLOCK_SIZE, BLOCK_SIZE, 
             BLACK
         );
     }
 }
 
 // Menggambar papan permainan dengan semua blok yang sudah ditempatkan
 void DrawBoard(Board* board) {
     // Menggambar latar belakang papan
     DrawRectangle(
         BOARD_OFFSET_X, 
         BOARD_OFFSET_Y, 
         board->width * BLOCK_SIZE, 
         board->height * BLOCK_SIZE, 
         DARKGRAY
     );
     
     // Menggambar garis grid
     for (int x = 0; x <= board->width; x++) {
         DrawLine(
             BOARD_OFFSET_X + x * BLOCK_SIZE, 
             BOARD_OFFSET_Y, 
             BOARD_OFFSET_X + x * BLOCK_SIZE, 
             BOARD_OFFSET_Y + board->height * BLOCK_SIZE, 
             GRAY
         );
     }
     
     for (int y = 0; y <= board->height; y++) {
         DrawLine(
             BOARD_OFFSET_X, 
             BOARD_OFFSET_Y + y * BLOCK_SIZE, 
             BOARD_OFFSET_X + board->width * BLOCK_SIZE, 
             BOARD_OFFSET_Y + y * BLOCK_SIZE, 
             GRAY
         );
     }
     
     // Menggambar blok yang telah ditempatkan
     for (int y = 0; y < board->height; y++) {
         for (int x = 0; x < board->width; x++) {
             DrawBlock(x, y, board->grid[y][x]);
         }
     }
 }
 
 // Menggambar tetromino aktif di posisinya saat ini
 void DrawActiveTetromino(Tetromino* tetromino) {
     for (int y = 0; y < 4; y++) {
         for (int x = 0; x < 4; x++) {
             if (tetromino->shape[y][x] > 0) {
                 DrawBlock(
                     tetromino->x + x, 
                     tetromino->y + y, 
                     tetromino->type
                 );
             }
         }
     }
 }
 
 // Menggambar tetromino berikutnya di area pratinjau
 void DrawNextTetromino(Tetromino* nextTetromino) {
     // Menggambar kotak pratinjau
     DrawRectangle(
         PREVIEW_OFFSET_X, 
         PREVIEW_OFFSET_Y, 
         4 * BLOCK_SIZE, 
         4 * BLOCK_SIZE, 
         LIGHTGRAY
     );
     
     // Menampilkan teks "NEXT"
     DrawText("NEXT", PREVIEW_OFFSET_X, PREVIEW_OFFSET_Y - 30, 20, BLACK);
     
     // Menggambar tetromino berikutnya dalam kotak pratinjau
     for (int y = 0; y < 4; y++) {
         for (int x = 0; x < 4; x++) {
             if (nextTetromino->shape[y][x] > 0) {
                 DrawRectangle(
                     PREVIEW_OFFSET_X + x * BLOCK_SIZE, 
                     PREVIEW_OFFSET_Y + y * BLOCK_SIZE, 
                     BLOCK_SIZE, BLOCK_SIZE, 
                     BLOCK_COLORS[nextTetromino->type]
                 );
                 
                 // Menggambar garis tepi
                 DrawRectangleLines(
                     PREVIEW_OFFSET_X + x * BLOCK_SIZE, 
                     PREVIEW_OFFSET_Y + y * BLOCK_SIZE, 
                     BLOCK_SIZE, BLOCK_SIZE, 
                     BLACK
                 );
             }
         }
     }
 }
 
 // Menggambar tampilan utama permainan (memanggil fungsi gambar lainnya)
 void DrawGame(GameState* gameState) {
     BeginDrawing();
     ClearBackground(RAYWHITE);
     
     // Menggambar latar belakang
     DrawRectangle(0, 0, GetScreenWidth(), GetScreenHeight(), RAYWHITE);
     
     // Menggambar papan permainan dan tetromino aktif
     DrawBoard(gameState->board);
     DrawActiveTetromino(gameState->currentTetromino);
     DrawNextTetromino(gameState->nextTetromino);
     
     // Menampilkan skor dan informasi permainan
     DrawGameInterface(gameState->scoreData);
     
     // Menampilkan overlay berdasarkan status permainan
     if (gameState->state == STATE_GAME_OVER) {
         DrawGameOver(gameState->scoreData);
     } else if (gameState->state == STATE_PAUSED) {
         DrawPauseScreen();
     }
     
     EndDrawing();
 }
 