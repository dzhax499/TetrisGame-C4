/**
 * rendering.c - FIXED VERSION
 *
 * Bertanggung jawab untuk semua proses rendering grafis dalam permainan Tetris
 * FIXED: Hold Block dan Next Block tidak mengikuti rotasi blok aktif
 * Penulis: Abi (Fixed by Assistant)
 */

#include <raylib.h>
#include "include/tetris.h"
#include "include/board.h"
#include "include/blocks.h"
#include "include/rendering.h"
#include "include/scoring.h"
#include "include/timer.h"
#include "include/linkedlist_block.h"
#include "include/rotasi_data.h"
#include <string.h>
// Deklarasi variabel global untuk font dan tekstur
static Font gameFont;
static Texture2D backgroundTexture;

// Inisialisasi sumber daya untuk rendering
void InitRendering(void)
{
    InitWindow(800, 600, "Tetris Game");
    SetTargetFPS(60);

    // Memuat font permainan
    int fontChars[] = {0}; // Memuat semua karakter default
    Font gameFont = LoadFontEx("../assets/fonts/game_font.ttf", 32, fontChars, 0);
    SetTextureFilter(gameFont.texture, TEXTURE_FILTER_BILINEAR);
}

// Membersihkan sumber daya rendering
void CloseRendering(void)
{
    UnloadFont(gameFont);
    UnloadTexture(backgroundTexture);
    CloseWindow();
}

// Render papan ke layar
void DrawBoard(TetrisBoard *board)
{
    for (int y = 0; y < BOARD_HEIGHT; y++)
    {
        for (int x = 0; x < BOARD_WIDTH; x++)
        {
            Color cellColor = GetBlockColor(board->grid[y][x]);
            int drawX = BOARD_OFFSET_X + x * BLOCK_SIZE;
            int drawY = BOARD_OFFSET_Y + y * BLOCK_SIZE;

            if (board->grid[y][x] != BLOCK_EMPTY)
            {
                DrawRectangle(drawX, drawY, BLOCK_SIZE, BLOCK_SIZE, cellColor);
                DrawRectangleLines(drawX, drawY, BLOCK_SIZE, BLOCK_SIZE, BLACK);
            }
        }
    }

    DrawRectangleLines(BOARD_OFFSET_X, BOARD_OFFSET_Y, BOARD_WIDTH * BLOCK_SIZE, BOARD_HEIGHT * BLOCK_SIZE, BLACK);
}

// Render skor dan level
void DrawScore(TetrisBoard *board, ScoreData *scoreData)
{
    (void)board; // Tidak digunakan

    int offsetX = BOARD_OFFSET_X + BOARD_WIDTH * BLOCK_SIZE + 70;

    // Background untuk area informasi
    DrawRectangle(offsetX - 20, BOARD_OFFSET_Y - 20,
                  200, BOARD_HEIGHT * BLOCK_SIZE + 40,
                  Fade(DARKGRAY, 0.7f));

    // Header informasi
    DrawText("GAME INFO", offsetX, BOARD_OFFSET_Y - 10, 20, WHITE);

    // Skor
    DrawText(TextFormat("SCORE: %d", scoreData->score),
             offsetX, BOARD_OFFSET_Y + 30, 20, WHITE);

    // Level
    DrawText(TextFormat("LEVEL: %d", scoreData->level),
             offsetX, BOARD_OFFSET_Y + 60, 20, WHITE);

    // Waktu
    DrawText(TextFormat("TIME: %.1f s", GetElapsedGameTime()), offsetX, BOARD_OFFSET_Y + 90, 20, WHITE);

    // Batas area Next Block
    DrawRectangleLines(offsetX, BOARD_OFFSET_Y + 120,
                       150, 100, WHITE);

    // Label Next Block
    DrawText("NEXT:", offsetX + 5, BOARD_OFFSET_Y + 125, 15, WHITE);

    // Batas area Hold Block
    DrawRectangleLines(offsetX, BOARD_OFFSET_Y + 240,
                       150, 100, WHITE);

    // Label Hold Block
    DrawText("HOLD:", offsetX + 5, BOARD_OFFSET_Y + 245, 15, WHITE);
}

// FIXED: DrawHoldBlock - Selalu tampilkan di rotasi 0
void DrawHoldBlock(TetrisBoard *board)
{
    if (!board || !board->hold_block.hasHeld) return;

    int offsetX = BOARD_OFFSET_X + BOARD_WIDTH * BLOCK_SIZE + 70;
    TetrisBlock *holdBlock = &board->hold_block.block;

    // Posisi tengah untuk hold block
    int blockOffsetX = offsetX + 40;  // Center dalam kotak 150px
    int blockOffsetY = BOARD_OFFSET_Y + 265;

    // CRITICAL FIX: Dapatkan shape untuk rotasi 0 TANPA mengubah rotation system
    RotationList *rotList = GetRotationList(holdBlock->type);
    int displayShape[4][4];
    
    if (rotList && rotList->rotationCount > 0) {
        // FIXED: Ambil bentuk rotasi 0 secara langsung tanpa mengubah current state
        // Simpan current state terlebih dahulu
        RotationNode *originalCurrent = rotList->current;
        int originalIndex = rotList->currentRotationIndex;
        
        // Sementara set ke rotasi 0 untuk mengambil shape
        SetRotation(rotList, 0);
        AmbilBentukSaatIni(rotList, displayShape);
        
        // CRITICAL: Restore state asli segera
        rotList->current = originalCurrent;
        rotList->currentRotationIndex = originalIndex;
    } else {
        // Fallback: gunakan shape default dari hold block
        memcpy(displayShape, holdBlock->shape, sizeof(int) * 4 * 4);
    }

    // Render blok dengan shape rotasi 0
    for (int y = 0; y < 4; y++)
    {
        for (int x = 0; x < 4; x++)
        {
            if (displayShape[y][x] != 0)
            {
                DrawRectangle(
                    blockOffsetX + x * (BLOCK_SIZE - 5),  // Slightly smaller for preview
                    blockOffsetY + y * (BLOCK_SIZE - 5),
                    BLOCK_SIZE - 5, BLOCK_SIZE - 5,
                    holdBlock->color);
                DrawRectangleLines(
                    blockOffsetX + x * (BLOCK_SIZE - 5),
                    blockOffsetY + y * (BLOCK_SIZE - 5),
                    BLOCK_SIZE - 5, BLOCK_SIZE - 5,
                    BLACK);
            }
        }
    }
}

void DrawBlockShadow(TetrisBlock *block, TetrisBoard *board)
{
    TetrisBlock shadowBlock = *block;

    // Jatuhkan bayangan ke posisi paling bawah
    while (IsValidBlockPosition(&shadowBlock, board, shadowBlock.x, shadowBlock.y + 1, shadowBlock.rotation))
    {
        shadowBlock.y++;
    }

    // Dapatkan rotation list untuk tipe blok
    RotationList *rotList = GetRotationList(block->type);
    
    if (rotList && rotList->current)
    {
        // Ambil bentuk saat ini dari rotation system
        int currentShape[4][4];
        AmbilBentukSaatIni(rotList, currentShape);

        // Gambar bayangan dengan warna transparan berdasarkan rotation system
        for (int y = 0; y < 4; y++)
        {
            for (int x = 0; x < 4; x++)
            {
                if (currentShape[y][x] != 0)
                {
                    int drawY = shadowBlock.y + y;

                    if (drawY >= 0)
                    {
                        Color shadowColor = Fade(block->color, 0.3f);
                        DrawRectangle(
                            BOARD_OFFSET_X + (shadowBlock.x + x) * BLOCK_SIZE,
                            BOARD_OFFSET_Y + drawY * BLOCK_SIZE,
                            BLOCK_SIZE,
                            BLOCK_SIZE,
                            shadowColor);

                        DrawRectangleLines(
                            BOARD_OFFSET_X + (shadowBlock.x + x) * BLOCK_SIZE,
                            BOARD_OFFSET_Y + drawY * BLOCK_SIZE,
                            BLOCK_SIZE,
                            BLOCK_SIZE,
                            DARKGRAY);
                    }
                }
            }
        }
    }
}

void DrawActiveTetromino(TetrisBlock *tetromino)
{
    // Dapatkan rotation list untuk tipe blok
    RotationList *rotList = GetRotationList(tetromino->type);
    
    if (rotList && rotList->current)
    {
        // Ambil bentuk saat ini dari rotation system
        int currentShape[4][4];
        AmbilBentukSaatIni(rotList, currentShape);

        // Gambar blok aktif berdasarkan rotation system
        for (int y = 0; y < 4; y++)
        {
            for (int x = 0; x < 4; x++)
            {
                if (currentShape[y][x] != 0)
                {
                    int drawY = tetromino->y + y;

                    if (drawY >= 0)
                    {
                        DrawRectangle(
                            BOARD_OFFSET_X + (tetromino->x + x) * BLOCK_SIZE,
                            BOARD_OFFSET_Y + drawY * BLOCK_SIZE,
                            BLOCK_SIZE,
                            BLOCK_SIZE,
                            tetromino->color);

                        DrawRectangleLines(
                            BOARD_OFFSET_X + (tetromino->x + x) * BLOCK_SIZE,
                            BOARD_OFFSET_Y + drawY * BLOCK_SIZE,
                            BLOCK_SIZE,
                            BLOCK_SIZE,
                            BLACK);
                    }
                }
            }
        }
    }
    else
    {
        // Fallback: gunakan shape yang tersimpan di tetromino
        for (int y = 0; y < 4; y++)
        {
            for (int x = 0; x < 4; x++)
            {
                if (tetromino->shape[y][x] != 0)
                {
                    int drawY = tetromino->y + y;

                    if (drawY >= 0)
                    {
                        DrawRectangle(
                            BOARD_OFFSET_X + (tetromino->x + x) * BLOCK_SIZE,
                            BOARD_OFFSET_Y + drawY * BLOCK_SIZE,
                            BLOCK_SIZE,
                            BLOCK_SIZE,
                            tetromino->color);

                        DrawRectangleLines(
                            BOARD_OFFSET_X + (tetromino->x + x) * BLOCK_SIZE,
                            BOARD_OFFSET_Y + drawY * BLOCK_SIZE,
                            BLOCK_SIZE,
                            BLOCK_SIZE,
                            BLACK);
                    }
                }
            }
        }
    }
}

void DrawPauseOverlay(void)
{
    int screenWidth = GetScreenWidth();
    int screenHeight = GetScreenHeight();

    // Semi-transparent background
    DrawRectangle(0, 0, screenWidth, screenHeight, Fade(BLACK, 0.7f));

    // Draw PAUSE text
    DrawText("PAUSED", screenWidth / 2 - MeasureText("PAUSED", 40) / 2,
             screenHeight / 2 - 100, 40, WHITE);

    // Define button dimensions and positions
    Rectangle resumeBtn = {
        screenWidth / 2 - 100,
        screenHeight / 2 - 20,
        200,
        50};

    Rectangle exitBtn = {
        screenWidth / 2 - 100,
        screenHeight / 2 + 50,
        200,
        50};

    // Draw Resume button
    DrawRectangleRec(resumeBtn, BLUE);
    DrawRectangleLinesEx(resumeBtn, 2, SKYBLUE);
    DrawText("RESUME", resumeBtn.x + (resumeBtn.width / 2) - MeasureText("RESUME", 20) / 2,
             resumeBtn.y + 15, 20, WHITE);

    // Draw Exit button
    DrawRectangleRec(exitBtn, RED);
    DrawRectangleLinesEx(exitBtn, 2, PINK);
    DrawText("EXIT", exitBtn.x + (exitBtn.width / 2) - MeasureText("EXIT", 20) / 2,
             exitBtn.y + 15, 20, WHITE);

    // Controls hint
    DrawText("Press P to resume", GetScreenWidth() / 2 - MeasureText("Press P to resume", 20) / 2,
             GetScreenHeight() - 50, 20, GRAY);
}

// FIXED: DrawNextBlock - Selalu tampilkan di rotasi 0 dan berfungsi penuh
void DrawNextBlock(TetrisBoard *board)
{
    if (!board) {
        printf("ERROR: board is NULL in DrawNextBlock\n");
        return;
    }
    
    // Validasi next_block
    if (board->next_block.type < 0 || board->next_block.type >= 7) {
        printf("ERROR: Invalid next_block type %d\n", board->next_block.type);
        return;
    }
    
    int offsetX = BOARD_OFFSET_X + BOARD_WIDTH * BLOCK_SIZE + 70;
    TetrisBlock *nextBlock = &board->next_block;

    // Posisi tengah untuk next block
    int blockOffsetX = offsetX + 40;  // Center dalam kotak 150px
    int blockOffsetY = BOARD_OFFSET_Y + 145;

    // CRITICAL FIX: Dapatkan shape untuk rotasi 0 TANPA mengubah rotation system
    RotationList *rotList = GetRotationList(nextBlock->type);
    int displayShape[4][4];
    
    if (rotList && rotList->rotationCount > 0) {
        // FIXED: Ambil bentuk rotasi 0 secara langsung tanpa mengubah current state
        // Simpan current state terlebih dahulu
        RotationNode *originalCurrent = rotList->current;
        int originalIndex = rotList->currentRotationIndex;
        
        // Sementara set ke rotasi 0 untuk mengambil shape
        SetRotation(rotList, 0);
        AmbilBentukSaatIni(rotList, displayShape);
        
        // CRITICAL: Restore state asli segera
        rotList->current = originalCurrent;
        rotList->currentRotationIndex = originalIndex;
        
        printf("DEBUG: DrawNextBlock type %d - successfully got rotation 0 shape\n", nextBlock->type);
    } else {
        printf("ERROR: Cannot get rotation list for next block type %d\n", nextBlock->type);
        // Emergency fallback - generate minimal shape
        memset(displayShape, 0, sizeof(displayShape));
        displayShape[1][1] = 1; // Single block as fallback
    }

    // Debug: Print shape yang akan di-render
    printf("Next Block Shape (type %d):\n", nextBlock->type);
    for (int y = 0; y < 4; y++) {
        for (int x = 0; x < 4; x++) {
            printf("%c ", displayShape[y][x] ? '#' : '.');
        }
        printf("\n");
    }

    // Render blok dengan shape rotasi 0
    bool hasRenderedBlock = false;
    for (int y = 0; y < 4; y++)
    {
        for (int x = 0; x < 4; x++)
        {
            if (displayShape[y][x] != 0)
            {
                hasRenderedBlock = true;
                DrawRectangle(
                    blockOffsetX + x * (BLOCK_SIZE - 5),  // Slightly smaller for preview
                    blockOffsetY + y * (BLOCK_SIZE - 5),
                    BLOCK_SIZE - 5, BLOCK_SIZE - 5,
                    nextBlock->color);
                DrawRectangleLines(
                    blockOffsetX + x * (BLOCK_SIZE - 5),
                    blockOffsetY + y * (BLOCK_SIZE - 5),
                    BLOCK_SIZE - 5, BLOCK_SIZE - 5,
                    BLACK);
            }
        }
    }
    
    if (!hasRenderedBlock) {
        printf("WARNING: No blocks rendered for next block type %d\n", nextBlock->type);
    } else {
        printf("SUCCESS: Next block rendered successfully\n");
    }
}

void DrawGame(TetrisBoard *board)
{
    BeginDrawing();
    ClearBackground(DARKGRAY);

    // Header Judul
    DrawText("TETRIS", 350, 10, 30, RAYWHITE);

    // Menggambar papan permainan
    DrawBoard(board);
    
    // Gambar shadow terlebih dahulu (di belakang blok aktif)
    DrawBlockShadow(&board->current_block, board);
    
    // Gambar blok aktif
    DrawActiveTetromino(&board->current_block);
    
    // Gambar preview blocks
    DrawNextBlock(board);
    DrawHoldBlock(board);
    
    // Gambar informasi skor
    DrawScore(board, &board->score_data);

    // Garis pemisah UI
    DrawLine(BOARD_OFFSET_X + BOARD_WIDTH * BLOCK_SIZE + 20, 30,
             BOARD_OFFSET_X + BOARD_WIDTH * BLOCK_SIZE + 20, 580, RAYWHITE);

    // Menampilkan overlay berdasarkan status permainan
    if (board->game_over)
    {
        DrawRectangle(100, 200, 600, 200, Fade(RED, 0.8f));
        DrawText("GAME OVER", 320, 250, 40, WHITE);
        DrawText("Press R to Restart", 280, 320, 20, WHITE);
    }

    EndDrawing();
}