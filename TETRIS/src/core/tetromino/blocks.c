/**
 * @file blocks.c
 * @brief Implementasi operasi blok Tetromino dengan SDL3
 * @author Dzakit Tsabit Assyafiq
 */

 #include "blocks.h"
 #include <stdlib.h>
 #include <time.h>
 #include <string.h>
 
 // Definisi warna untuk setiap tetromino
 static const SDL_Color TETROMINO_COLORS[TETROMINO_COUNT] = {
     {0, 255, 255, 255},  // I - Cyan
     {0, 0, 255, 255},    // J - Biru
     {255, 165, 0, 255},  // L - Oranye
     {255, 255, 0, 255},  // O - Kuning
     {0, 255, 0, 255},    // S - Hijau
     {128, 0, 128, 255},  // T - Ungu
     {255, 0, 0, 255}     // Z - Merah
 };
 
 // Definisi bentuk tetromino (4x4 grid)
 static const int TETROMINO_SHAPES[TETROMINO_COUNT][4][4] = {
     // I
     {
         {0, 0, 0, 0},
         {1, 1, 1, 1},
         {0, 0, 0, 0},
         {0, 0, 0, 0}
     },
     // J
     {
         {1, 0, 0, 0},
         {1, 1, 1, 0},
         {0, 0, 0, 0},
         {0, 0, 0, 0}
     },
     // L
     {
         {0, 0, 1, 0},
         {1, 1, 1, 0},
         {0, 0, 0, 0},
         {0, 0, 0, 0}
     },
     // O
     {
         {0, 1, 1, 0},
         {0, 1, 1, 0},
         {0, 0, 0, 0},
         {0, 0, 0, 0}
     },
     // S
     {
         {0, 1, 1, 0},
         {1, 1, 0, 0},
         {0, 0, 0, 0},
         {0, 0, 0, 0}
     },
     // T
     {
         {0, 1, 0, 0},
         {1, 1, 1, 0},
         {0, 0, 0, 0},
         {0, 0, 0, 0}
     },
     // Z
     {
         {1, 1, 0, 0},
         {0, 1, 1, 0},
         {0, 0, 0, 0},
         {0, 0, 0, 0}
     }
 };
 
 // Array untuk mengacak urutan tetromino
 static TetrominoType shuffledTypes[TETROMINO_COUNT];
 
 /**
  * Pengacak array tetromino untuk memastikan distribusi yang adil
  */
 static void shuffleTetrominoes() {
     // Inisialisasi array dengan urutan tetromino
     for (int i = 0; i < TETROMINO_COUNT; i++) {
         shuffledTypes[i] = (TetrominoType)i;
     }
     
     // Algoritma Fisher-Yates untuk pengacakan
     for (int i = TETROMINO_COUNT - 1; i > 0; i--) {
         int j = rand() % (i + 1);
         TetrominoType temp = shuffledTypes[i];
         shuffledTypes[i] = shuffledTypes[j];
         shuffledTypes[j] = temp;
     }
 }
 
 bool initBlockSystem(TetrominoQueue* queue) {
     if (!queue) return false;
     
     // Seed random number generator
     srand((unsigned int)time(NULL));
     
     // Inisialisasi queue
     queue->canHold = true;
     
     // Buat 3 blok awal
     shuffleTetrominoes();
     for (int i = 0; i < 3; i++) {
         queue->next[i] = createTetromino(shuffledTypes[i]);
     }
     
     // Inisialisasi hold dengan nilai tidak valid
     queue->hold.type = TETROMINO_COUNT; // Nilai tidak valid sebagai penanda belum ada hold
     
     return true;
 }
 
 Tetromino createTetromino(TetrominoType type) {
     Tetromino tetromino;
     
     tetromino.type = type;
     tetromino.rotation = 0;
     tetromino.color = TETROMINO_COLORS[type];
     
     // Salin bentuk dari definisi tetromino
     memcpy(tetromino.blocks, TETROMINO_SHAPES[type], sizeof(int) * 4 * 4);
     
     // Posisi awal (tengah atas)
     tetromino.x = GRID_WIDTH / 2 - 2;
     tetromino.y = 0;
     
     return tetromino;
 }
 
 Tetromino getNextTetromino(TetrominoQueue* queue) {
     // Ambil tetromino dari awal queue
     Tetromino next = queue->next[0];
     
     // Geser queue ke depan
     for (int i = 0; i < 2; i++) {
         queue->next[i] = queue->next[i + 1];
     }
     
     // Shuffle ulang jika perlu
     static int count = 3;
     if (count >= TETROMINO_COUNT) {
         shuffleTetrominoes();
         count = 0;
     }
     
     // Tambahkan tetromino baru ke akhir queue
     queue->next[2] = createTetromino(shuffledTypes[count++]);
     
     return next;
 }
 
 void refillQueue(TetrominoQueue* queue) {
     shuffleTetrominoes();
     for (int i = 0; i < 3; i++) {
         queue->next[i] = createTetromino(shuffledTypes[i]);
     }
 }
 
 bool holdTetromino(TetrominoQueue* queue, Tetromino* current) {
     if (!queue->canHold) return false;
     
     queue->canHold = false;
     
     if (queue->hold.type == TETROMINO_COUNT) {
         // Belum ada hold, simpan current dan ambil next
         queue->hold = createTetromino(current->type);
         *current = getNextTetromino(queue);
     } else {
         // Tukar hold dengan current
         TetrominoType holdType = queue->hold.type;
         queue->hold = createTetromino(current->type);
         *current = createTetromino(holdType);
     }
     
     return true;
 }
 
 void resetHold(TetrominoQueue* queue) {
     queue->canHold = true;
 }
 
 void lockTetromino(GameBoard* board, const Tetromino* tetromino) {
     for (int y = 0; y < 4; y++) {
         for (int x = 0; x < 4; x++) {
             if (tetromino->blocks[y][x]) {
                 int boardX = tetromino->x + x;
                 int boardY = tetromino->y + y;
                 
                 // Pastikan koordinat valid
                 if (boardX >= 0 && boardX < GRID_WIDTH && boardY >= 0 && boardY < GRID_HEIGHT) {
                     board->grid[boardY][boardX] = 1;
                     board->colors[boardY][boardX] = tetromino->color;
                 }
             }
         }
     }
 }
 
 int checkLines(GameBoard* board, int* clearedLines) {
     int numCleared = 0;
     
     // Periksa setiap baris dari bawah ke atas
     for (int y = GRID_HEIGHT - 1; y >= 0; y--) {
         bool isLineFull = true;
         
         // Periksa apakah baris penuh
         for (int x = 0; x < GRID_WIDTH; x++) {
             if (board->grid[y][x] == 0) {
                 isLineFull = false;
                 break;
             }
         }
         
         // Jika baris penuh, tambahkan ke array
         if (isLineFull) {
             clearedLines[numCleared++] = y;
         }
     }
     
     return numCleared;
 }
 
 void clearLines(GameBoard* board, const int* lineIndices, int numLines) {
     // Proses setiap baris yang akan dihapus
     for (int i = 0; i < numLines; i++) {
         int lineIdx = lineIndices[i];
         
         // Geser semua baris di atas baris yang dihapus ke bawah
         for (int y = lineIdx; y > 0; y--) {
             for (int x = 0; x < GRID_WIDTH; x++) {
                 board->grid[y][x] = board->grid[y-1][x];
                 board->colors[y][x] = board->colors[y-1][x];
             }
         }
         
         // Kosongkan baris paling atas
         for (int x = 0; x < GRID_WIDTH; x++) {
             board->grid[0][x] = 0;
         }
     }
 }
 
 void renderTetromino(SDL_Renderer* renderer, const Tetromino* tetromino, int offsetX, int offsetY) {
     for (int y = 0; y < 4; y++) {
         for (int x = 0; x < 4; x++) {
             if (tetromino->blocks[y][x]) {
                 SDL_FRect rect = {
                     (float)((tetromino->x + x) * BLOCK_SIZE + offsetX),
                     (float)((tetromino->y + y) * BLOCK_SIZE + offsetY),
                     (float)BLOCK_SIZE,
                     (float)BLOCK_SIZE
                 };
                 
                 // Gunakan warna tetromino
                 SDL_SetRenderDrawColor(renderer, 
                                       tetromino->color.r, 
                                       tetromino->color.g, 
                                       tetromino->color.b, 
                                       tetromino->color.a);
                 SDL_RenderFillRect(renderer, &rect);
                 
                 // Gambar outline
                 SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
                 SDL_RenderRect(renderer, &rect);
             }
         }
     }
 }
 
 void renderBoard(SDL_Renderer* renderer, const GameBoard* board, int offsetX, int offsetY) {
     // Render grid
     SDL_SetRenderDrawColor(renderer, 50, 50, 50, 255);
     for (int x = 0; x <= GRID_WIDTH; x++) {
         SDL_FRect lineRect = {
             (float)(x * BLOCK_SIZE + offsetX),
             (float)(offsetY),
             1.0f,
             (float)(GRID_HEIGHT * BLOCK_SIZE)
         };
         SDL_RenderFillRect(renderer, &lineRect);
     }
     
     for (int y = 0; y <= GRID_HEIGHT; y++) {
         SDL_FRect lineRect = {
             (float)(offsetX),
             (float)(y * BLOCK_SIZE + offsetY),
             (float)(GRID_WIDTH * BLOCK_SIZE),
             1.0f
         };
         SDL_RenderFillRect(renderer, &lineRect);
     }
     
     // Render blok
     for (int y = 0; y < GRID_HEIGHT; y++) {
         for (int x = 0; x < GRID_WIDTH; x++) {
             if (board->grid[y][x]) {
                 SDL_FRect blockRect = {
                     (float)(x * BLOCK_SIZE + offsetX),
                     (float)(y * BLOCK_SIZE + offsetY),
                     (float)BLOCK_SIZE,
                     (float)BLOCK_SIZE
                 };
                 
                 // Gunakan warna yang tersimpan di board
                 SDL_Color color = board->colors[y][x];
                 SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);
                 SDL_RenderFillRect(renderer, &blockRect);
                 
                 // Gambar outline
                 SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
                 SDL_RenderRect(renderer, &blockRect);
             }
         }
     }
 }
 
 void resetBoard(GameBoard* board) {
     // Kosongkan seluruh grid
     memset(board->grid, 0, sizeof(int) * GRID_HEIGHT * GRID_WIDTH);
 }