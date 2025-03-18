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
 
//  // Konstanta untuk rendering
//  #define BLOCK_SIZE 30
//  #define BOARD_OFFSET_X 300
//  #define BOARD_OFFSET_Y 40
//  #define PREVIEW_OFFSET_X 550
//  #define PREVIEW_OFFSET_Y 100
 
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
    Font gameFont = LoadFontEx("C:/Users/Windows 11/Documents/GitHub/TetrisGame-C4/TETRIS-GameFix/assets/fonts/game_font.ttf", 32, fontChars, 0);
    SetTextureFilter(gameFont.texture, TEXTURE_FILTER_BILINEAR);
}    
// Membersihkan sumber daya rendering
void CloseRendering(void) {
    // Melepaskan tekstur, font, dll.
    UnloadFont(gameFont);
    UnloadTexture(backgroundTexture);
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
 
// Render papan ke layar
void DrawBoard(TetrisBoard* board) {
    for (int y = 0; y < BOARD_HEIGHT; y++) {
        for (int x = 0; x < BOARD_WIDTH; x++) {
            // **Dapatkan warna berdasarkan isi grid**
            Color cellColor = GetBlockColor(board->grid[y][x]);

            // **Sesuaikan posisi berdasarkan offset yang dipakai**
            int drawX = BOARD_OFFSET_X + x * BLOCK_SIZE;
            int drawY = BOARD_OFFSET_Y + y * BLOCK_SIZE;

            // **Gambar hanya jika bukan blok kosong**
            if (board->grid[y][x] != BLOCK_EMPTY) {
                DrawRectangle(drawX, drawY, BLOCK_SIZE, BLOCK_SIZE, cellColor);
                DrawRectangleLines(drawX, drawY, BLOCK_SIZE, BLOCK_SIZE, BLACK); // Outline
            }
        }
    }

    // **Gambar batas papan**
    DrawRectangleLines(BOARD_OFFSET_X, BOARD_OFFSET_Y, BOARD_WIDTH * BLOCK_SIZE, BOARD_HEIGHT * BLOCK_SIZE, BLACK);
}


// Render skor dan level
void DrawScore(TetrisBoard* board, ScoreData* scoreData) {
    (void)board; // Tidak digunakan
    int offsetX = BOARD_OFFSET_X + BOARD_WIDTH * BLOCK_SIZE + 50;
    
    // Skor
    DrawText(TextFormat("SCORE: %d", scoreData->score), 
             offsetX, BOARD_OFFSET_Y, 20, WHITE);
    
    // Level
    DrawText(TextFormat("LEVEL: %d", scoreData->level), 
             offsetX, BOARD_OFFSET_Y + 30, 20, WHITE);
    
    // Blok Selanjutnya
    DrawText("NEXT:", offsetX, BOARD_OFFSET_Y + 60, 20, WHITE);
    
    // Blok Hold
    DrawText("HOLD:", offsetX, BOARD_OFFSET_Y + 200, 20, WHITE);
}

void DrawHoldBlock(TetrisBoard* board) {
    if (board->hold_block.hasHeld) {
        TetrisBlock* holdBlock = &board->hold_block.block;
        int offsetX = BOARD_OFFSET_X + BOARD_WIDTH * BLOCK_SIZE + 50;
        
        for (int y = 0; y < 4; y++) {
            for (int x = 0; x < 4; x++) {
                if (holdBlock->shape[y][x] != 0) {
                    DrawRectangle(
                        offsetX + x * BLOCK_SIZE, 
                        BOARD_OFFSET_Y + 230 + y * BLOCK_SIZE, 
                        BLOCK_SIZE, BLOCK_SIZE, 
                        holdBlock->color
                    );
                    DrawRectangleLines(
                        offsetX + x * BLOCK_SIZE, 
                        BOARD_OFFSET_Y + 230 + y * BLOCK_SIZE, 
                        BLOCK_SIZE, BLOCK_SIZE, 
                        BLACK
                    );
                }
            }
        }
    }
}

void DrawBlockShadow(TetrisBlock* block, TetrisBoard* board) {
    TetrisBlock shadowBlock = *block;
    
    // Jatuhkan bayangan ke posisi paling bawah
    while (IsValidBlockPosition(&shadowBlock, board, shadowBlock.x, shadowBlock.y + 1, shadowBlock.rotation)) {
        shadowBlock.y++;
    }
    
    // Gambar bayangan dengan warna transparan
    for (int y = 0; y < 4; y++) {
        for (int x = 0; x < 4; x++) {
            if (TETROMINO_SHAPES[block->type][block->rotation][y][x] != 0) {
                int drawY = shadowBlock.y + y;
                
                if (drawY >= 0) {
                    Color shadowColor = Fade(block->color, 0.3f);
                    DrawRectangle(
                        BOARD_OFFSET_X + (shadowBlock.x + x) * BLOCK_SIZE, 
                        BOARD_OFFSET_Y + drawY * BLOCK_SIZE, 
                        BLOCK_SIZE, 
                        BLOCK_SIZE, 
                        shadowColor
                    );
                    
                    DrawRectangleLines(
                        BOARD_OFFSET_X + (shadowBlock.x + x) * BLOCK_SIZE, 
                        BOARD_OFFSET_Y + drawY * BLOCK_SIZE, 
                        BLOCK_SIZE, 
                        BLOCK_SIZE, 
                        DARKGRAY
                    );
                }
            }
        }
    }
}

 
 // Menggambar tetromino aktif di posisinya saat ini
 void DrawActiveTetromino(TetrisBlock* tetromino) {
    for (int y = 0; y < 4; y++) {
        for (int x = 0; x < 4; x++) {
            // Gambar hanya sel yang terisi dan dalam batas papan
            if (TETROMINO_SHAPES[tetromino->type][tetromino->rotation][y][x] != 0) {
                int drawY = tetromino->y + y;
                
                // Gambar hanya jika sel berada dalam batas vertikal papan
                if (drawY >= 0) {
                    DrawRectangle(
                        BOARD_OFFSET_X + (tetromino->x + x) * BLOCK_SIZE, 
                        BOARD_OFFSET_Y + drawY * BLOCK_SIZE, 
                        BLOCK_SIZE, 
                        BLOCK_SIZE, 
                        tetromino->color
                    );
                    
                    // Tambahkan outline
                    DrawRectangleLines(
                        BOARD_OFFSET_X + (tetromino->x + x) * BLOCK_SIZE, 
                        BOARD_OFFSET_Y + drawY * BLOCK_SIZE, 
                        BLOCK_SIZE, 
                        BLOCK_SIZE, 
                        BLACK
                    );
                }
            }
        }
    }
}

 // Render blok berikutnya
 void DrawNextBlock(TetrisBoard* board) {
    TetrisBlock nextBlock = board->next_block;
    
    DrawText("NEXT", BOARD_OFFSET_X + BOARD_WIDTH * BLOCK_SIZE + 50, BOARD_OFFSET_Y, 20, WHITE);
    
    for (int y = 0; y < 4; y++) {
        for (int x = 0; x < 4; x++) {
            if (TETROMINO_SHAPES[nextBlock.type][0][y][x] != 0) {
                DrawRectangle(
                    BOARD_OFFSET_X + BOARD_WIDTH * BLOCK_SIZE + 50 + x * BLOCK_SIZE, 
                    BOARD_OFFSET_Y + 50 + y * BLOCK_SIZE, 
                    BLOCK_SIZE, BLOCK_SIZE, 
                    nextBlock.color
                );
                DrawRectangleLines(
                    BOARD_OFFSET_X + BOARD_WIDTH * BLOCK_SIZE + 50 + x * BLOCK_SIZE, 
                    BOARD_OFFSET_Y + 50 + y * BLOCK_SIZE, 
                    BLOCK_SIZE, BLOCK_SIZE, 
                    BLACK
                );
            }
        }
    }
}

 
void DrawGame(TetrisBoard* board) {
    BeginDrawing();
    ClearBackground(DARKGRAY); // Latar belakang lebih nyaman

    // Header Judul
    DrawText("TETRIS", 350, 10, 30, RAYWHITE);
    
    // Menggambar papan permainan
    DrawBoard(board);
    DrawActiveTetromino(&board->current_block);
    DrawNextBlock(board);
    DrawScore(board,&board->score_data);

    // Garis pemisah UI
    DrawLine(BOARD_OFFSET_X + BOARD_WIDTH * BLOCK_SIZE + 20, 30, 
             BOARD_OFFSET_X + BOARD_WIDTH * BLOCK_SIZE + 20, 580, RAYWHITE);
    
    // Menampilkan overlay berdasarkan status permainan
    if (board->game_over) {
        DrawRectangle(100, 200, 600, 200, Fade(RED, 0.8f));
        DrawText("GAME OVER", 320, 250, 40, WHITE);
        DrawText("Press R to Restart", 280, 320, 20, WHITE);
    }

    EndDrawing();
}
