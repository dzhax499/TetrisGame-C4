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
#include "include/timer.h"

//  // Konstanta untuk rendering
//  #define BLOCK_SIZE 30
//  #define BOARD_OFFSET_X 300
//  #define BOARD_OFFSET_Y 40
//  #define PREVIEW_OFFSET_X 550
//  #define PREVIEW_OFFSET_Y 100

// Warna untuk setiap blok tetromino
// static const Color BLOCK_COLORS[] = {
//     BLANK,    // Ruang kosong (0)
//     SKYBLUE,  // Blok I (1)
//     DARKBLUE, // Blok J (2)
//     ORANGE,   // Blok L (3)
//     YELLOW,   // Blok O (4)
//     LIME,     // Blok S (5)
//     PURPLE,   // Blok T (6)
//     RED       // Blok Z (7)
// };

// Deklarasi variabel global untuk font dan tekstur
static Font gameFont;
static Texture2D backgroundTexture;

// Inisialisasi sumber daya untuk rendering
void InitRendering(void)
{
    // Memuat tekstur, font, dll.
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
    // Melepaskan tekstur, font, dll.
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
            // **Dapatkan warna berdasarkan isi grid**
            Color cellColor = GetBlockColor(board->grid[y][x]);

            // **Sesuaikan posisi berdasarkan offset yang dipakai**
            int drawX = BOARD_OFFSET_X + x * BLOCK_SIZE;
            int drawY = BOARD_OFFSET_Y + y * BLOCK_SIZE;

            // **Gambar hanya jika bukan blok kosong**
            if (board->grid[y][x] != BLOCK_EMPTY)
            {
                DrawRectangle(drawX, drawY, BLOCK_SIZE, BLOCK_SIZE, cellColor);
                DrawRectangleLines(drawX, drawY, BLOCK_SIZE, BLOCK_SIZE, BLACK); // Outline
            }
        }
    }

    // **Gambar batas papan**
    DrawRectangleLines(BOARD_OFFSET_X, BOARD_OFFSET_Y, BOARD_WIDTH * BLOCK_SIZE, BOARD_HEIGHT * BLOCK_SIZE, BLACK);
}

// Render skor dan level
void DrawScore(TetrisBoard *board, ScoreData *scoreData)
{
    (void)board; // Tidak digunakan

    // Ubah posisi - Geser lebih jauh dari board
    int offsetX = BOARD_OFFSET_X + BOARD_WIDTH * BLOCK_SIZE + 70;

    // Tambahkan background untuk area informasi
    DrawRectangle(offsetX - 20, BOARD_OFFSET_Y - 20,
                  200, BOARD_HEIGHT * BLOCK_SIZE + 40,
                  Fade(DARKGRAY, 0.7f));

    // Header informasi
    DrawText("GAME INFO", offsetX, BOARD_OFFSET_Y - 10, 20, WHITE);

    // Skor - geser ke bawah
    DrawText(TextFormat("SCORE: %d", scoreData->score),
             offsetX, BOARD_OFFSET_Y + 30, 20, WHITE);

    // Level - geser ke bawah
    DrawText(TextFormat("LEVEL: %d", scoreData->level),
             offsetX, BOARD_OFFSET_Y + 60, 20, WHITE);

    // Waktu - geser ke bawah
    DrawText(TextFormat("TIME: %.1f s", GetElapsedGameTime()), offsetX, BOARD_OFFSET_Y + 90, 20, WHITE);

    // Batas area Next Block
    DrawRectangleLines(offsetX, BOARD_OFFSET_Y + 100,
                       150, 100, WHITE);

    // Label Next Block - HAPUS DARI SINI, PINDAH KE DrawNextBlock
    DrawText("NEXT:", offsetX, BOARD_OFFSET_Y + 100, 15, WHITE);

    // Batas area Hold Block
    DrawRectangleLines(offsetX, BOARD_OFFSET_Y + 230,
                       150, 100, WHITE);

    // Label Hold Block
    DrawText("HOLD:", offsetX, BOARD_OFFSET_Y + 230, 15, WHITE);
}

void DrawHoldBlock(TetrisBoard *board)
{
    if (board->hold_block.hasHeld)
    {
        // Gunakan cara perhitungan yang sama dengan DrawScore
        int offsetX = BOARD_OFFSET_X + BOARD_WIDTH * BLOCK_SIZE + 70;

        TetrisBlock *holdBlock = &board->hold_block.block;

        // Posisi tengah untuk hold block (25 = offset untuk tengah kotak 150px)
        int blockOffsetX = offsetX + 25;
        int blockOffsetY = BOARD_OFFSET_Y + 250;

        // Dapatkan rotation list untuk tipe blok yang sedang di-hold
        RotationList *rotList = GetRotationList(holdBlock->type);
        
        if (rotList && rotList->current)
        {
            // Ambil bentuk saat ini dari rotation system
            int currentShape[4][4];
            AmbilBentukSaatIni(rotList, currentShape);

            // Render blok berdasarkan bentuk dari rotation system
            for (int y = 0; y < 4; y++)
            {
                for (int x = 0; x < 4; x++)
                {
                    if (currentShape[y][x] != 0)
                    {
                        DrawRectangle(
                            blockOffsetX + x * BLOCK_SIZE,
                            blockOffsetY + y * BLOCK_SIZE,
                            BLOCK_SIZE, BLOCK_SIZE,
                            holdBlock->color);
                        DrawRectangleLines(
                            blockOffsetX + x * BLOCK_SIZE,
                            blockOffsetY + y * BLOCK_SIZE,
                            BLOCK_SIZE, BLOCK_SIZE,
                            BLACK);
                    }
                }
            }
        }
        else
        {
            // Fallback: gunakan shape yang tersimpan di holdBlock jika rotation system gagal
            for (int y = 0; y < 4; y++)
            {
                for (int x = 0; x < 4; x++)
                {
                    if (holdBlock->shape[y][x] != 0)
                    {
                        DrawRectangle(
                            blockOffsetX + x * BLOCK_SIZE,
                            blockOffsetY + y * BLOCK_SIZE,
                            BLOCK_SIZE, BLOCK_SIZE,
                            holdBlock->color);
                        DrawRectangleLines(
                            blockOffsetX + x * BLOCK_SIZE,
                            blockOffsetY + y * BLOCK_SIZE,
                            BLOCK_SIZE, BLOCK_SIZE,
                            BLACK);
                    }
                }
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
    else
    {
        // Fallback: gunakan shape yang tersimpan di block jika rotation system gagal
        for (int y = 0; y < 4; y++)
        {
            for (int x = 0; x < 4; x++)
            {
                if (block->shape[y][x] != 0)
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
    
    PrintBlockShape(tetromino); // Debug: cetak bentuk blok
    
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
                // Gambar hanya sel yang terisi dan dalam batas papan
                if (currentShape[y][x] != 0)
                {
                    int drawY = tetromino->y + y;

                    // Gambar hanya jika sel berada dalam batas vertikal papan
                    if (drawY >= 0)
                    {
                        DrawRectangle(
                            BOARD_OFFSET_X + (tetromino->x + x) * BLOCK_SIZE,
                            BOARD_OFFSET_Y + drawY * BLOCK_SIZE,
                            BLOCK_SIZE,
                            BLOCK_SIZE,
                            tetromino->color);

                        // Tambahkan outline
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
        // Fallback: gunakan shape yang tersimpan di tetromino jika rotation system gagal
        for (int y = 0; y < 4; y++)
        {
            for (int x = 0; x < 4; x++)
            {
                // Gambar hanya sel yang terisi dan dalam batas papan
                if (tetromino->shape[y][x] != 0)
                {
                    int drawY = tetromino->y + y;

                    // Gambar hanya jika sel berada dalam batas vertikal papan
                    if (drawY >= 0)
                    {
                        DrawRectangle(
                            BOARD_OFFSET_X + (tetromino->x + x) * BLOCK_SIZE,
                            BOARD_OFFSET_Y + drawY * BLOCK_SIZE,
                            BLOCK_SIZE,
                            BLOCK_SIZE,
                            tetromino->color);

                        // Tambahkan outline
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

// Render blok berikutnya
void DrawNextBlock(TetrisBoard *board)
{
    printf(">> [DEBUG] Mulai DrawNextBlock\n");
    
    if (!board) {
        printf("ERROR: board is NULL in DrawNextBlock\n");
        return;
    }
    
    // Validasi next_block
    if (board->next_block.type < 0 || board->next_block.type >= 7) {
        printf("ERROR: Invalid next_block type %d, skipping DrawNextBlock\n", board->next_block.type);
        return;
    }
    
    // Debug info
    printf("Block Type: %d, Rotation: %d\n", board->next_block.type, board->next_block.rotation);
    printf("Position: (%d, %d)\n", board->next_block.x, board->next_block.y);
    
    // Validasi shape
    bool hasValidShape = false;
    for (int y = 0; y < 4 && !hasValidShape; y++) {
        for (int x = 0; x < 4; x++) {
            if (board->next_block.shape[y][x] != 0) {
                hasValidShape = true;
                break;
            }
        }
    }
    
    if (!hasValidShape) {
        printf("WARNING: next_block has empty shape, regenerating...\n");
        board->next_block = GenerateRandomBlock();
        return;
    }
    
    // Print shape untuk debug
    printf("Shape:\n");
    for (int y = 0; y < 4; y++)
    {
        for (int x = 0; x < 4; x++)
        {
            printf("%c ", board->next_block.shape[y][x] ? '#' : '.');
        }
        printf("\n");
    }
    
    // Lanjutkan dengan rendering normal...
    // (sisanya kode DrawNextBlock yang sudah ada)
}


void DrawGame(TetrisBoard *board)
{
    BeginDrawing();
    ClearBackground(DARKGRAY); // Latar belakang lebih nyaman

    // Header Judul
    DrawText("TETRIS", 350, 10, 30, RAYWHITE);

    // Menggambar papan permainan
    DrawBoard(board);
    DrawActiveTetromino(&board->current_block);
    DrawNextBlock(board);
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
