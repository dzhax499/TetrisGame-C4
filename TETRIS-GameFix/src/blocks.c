// Nama file : blocks.c
// Deskripsi : Implementasi blok-blok game Tetris
// Oleh      : Dzakit Tsabit 241511071
#include <string.h>
#include <time.h>  // ADD THIS LINE - needed for time() function
#include "include/blocks.h"
#include "include/rotasi_data.h"
#include "include/linkedlist_block.h"
#include <stdlib.h>
#include <stdio.h>
#include "raylib.h"

// Forward declaration for static function
static void SyncRotationToState(RotationList *rotList, int targetRotation);

static const Color TETROMINO_COLORS[7] = {
    SKYBLUE,  // I
    DARKBLUE, // J
    ORANGE,   // L
    YELLOW,   // O
    LIME,     // S
    PURPLE,   // T
    RED       // Z
};

static const int WallKickTests[4][5][2] = {
    // 0 -> R (Dari posisi awal ke kanan)
    {{0, 0}, {-1, 0}, {-1, 1}, {0, -2}, {-1, -2}},
    // R -> 2 (Dari kanan ke posisi terbalik)
    {{0, 0}, {1, 0}, {1, -1}, {0, 2}, {1, 2}},
    // 2 -> L (Dari posisi terbalik ke kiri)
    {{0, 0}, {1, 0}, {1, 1}, {0, -2}, {1, -2}},
    // L -> 0 (Dari kiri kembali ke posisi awal)
    {{0, 0}, {-1, 0}, {-1, -1}, {0, 2}, {-1, 2}}};

void InitBlocks(void)
{
    // Inisialisasi random seed
    srand(time(NULL));

    // Inisialisasi sistem rotasi blok
    InitRotationSystem();
}

TetrisBlock GenerateRandomBlock(void)
{
    TetrisBlock block;
    
    // Inisialisasi semua field dengan nilai default
    memset(&block, 0, sizeof(TetrisBlock));
    
    block.type = rand() % 7;
    block.rotation = 0;  // Always start at rotation 0
    block.ukuranblok = 4;
    block.efekmeledak = false;
    block.waktumeledak = 0;

    // Sesuaikan posisi awal agar blok muncul di bagian atas papan
    block.x = BOARD_WIDTH / 2 - 2;
    block.y = -1;

    // Dapatkan rotasi list untuk tipe blok ini
    RotationList *rotList = GetRotationList(block.type);
    if (rotList == NULL || rotList->current == NULL)
    {
        printf("ERROR: rotList NULL saat generate blok type %d\n", block.type);
        // Emergency fallback - create manual I-block
        block.type = 0;
        memset(block.shape, 0, sizeof(block.shape));
        block.shape[1][0] = 1;
        block.shape[1][1] = 1;
        block.shape[1][2] = 1;
        block.shape[1][3] = 1;
        block.rotationNode = NULL;
    }
    else
    {
        // Ensure we start at rotation 0
        SyncRotationToState(rotList, 0);
        block.rotationNode = rotList->current;
        memcpy(block.shape, rotList->current->shape, sizeof(block.shape));
    }

    // Tetapkan warna
    block.color = TETROMINO_COLORS[block.type];

    return block;
}

// Fungsi untuk memeriksa apakah blok dapat ditempatkan pada posisi tertentu
bool IsValidBlockPosition(TetrisBlock *block, TetrisBoard *board, int testX, int testY, int testRotation)
{
    RotationList *rotList = GetRotationList(block->type);
    if (!rotList) return false;

    // Save current rotation state
    RotationNode *savedRotation = rotList->current;
    
    // Sync to the test rotation
    SyncRotationToState(rotList, testRotation);

    // Get shape for the test rotation
    int bentuk[4][4];
    AmbilBentukSaatIni(rotList, bentuk);

    // Restore original rotation state
    rotList->current = savedRotation;

    // Test collision with the retrieved shape
    for (int y = 0; y < 4; y++)
    {
        for (int x = 0; x < 4; x++)
        {
            if (bentuk[y][x] == 0) continue;

            int boardX = testX + x;
            int boardY = testY + y;

            // Check bounds
            if (boardX < 0 || boardX >= BOARD_WIDTH || boardY >= BOARD_HEIGHT)
            {
                return false;
            }

            // Skip if above board
            if (boardY < 0) continue;

            // Check collision with existing blocks
            if (board->grid[boardY][boardX] != BLOCK_EMPTY)
            {
                return false;
            }
        }
    }

    return true;
}

// Fungsi untuk memutar blok ke rotasi berikutnya jika memungkinkan
bool RotateBlock(TetrisBlock *block, TetrisBoard *board)
{
    // Dapatkan rotasi list untuk tipe blok ini
    RotationList *rotList = GetRotationList(block->type);
    if (!rotList)
        return false;

    // Cek apakah posisi setelah rotasi valid
    RotationNode *savedRotation = rotList->current;
    RotateToNext(rotList); // Coba rotasi berikutnya

    int tempShape[4][4];
    AmbilBentukSaatIni(rotList, tempShape);

    // Kembalikan rotasi dulu sebelum pemeriksaan
    rotList->current = savedRotation;

    // Cek validitas
    if (IsValidBlockPosition(block, board, block->x, block->y, (block->rotation + 1) % rotList->rotationCount))
    {
        // Jika valid, perbarui rotasi blok
        block->rotation = (block->rotation + 1) % rotList->rotationCount;
        RotateToNext(rotList); // Geser rotasi dalam list

        // Salin bentuk baru ke blok
        AmbilBentukSaatIni(rotList, block->shape);

        return true;
    }

    return false;
}

bool RotateBlockWithWallKick(TetrisBlock *block, TetrisBoard *board)
{
    // Validasi input
    if (!block || !board) return false;
    
    // Validasi tipe blok
    if (block->type < 0 || block->type >= 7) {
        printf("ERROR: Invalid block type %d in RotateBlockWithWallKick\n", block->type);
        return false;
    }
    
    RotationList *rotList = GetRotationList(block->type);
    if (!rotList) {
        printf("ERROR: rotList is NULL for block type %d\n", block->type);
        return false;
    }

    int initialRotation = block->rotation;
    int rotationCount = rotList->rotationCount;
    int newRotation = (block->rotation + 1) % rotationCount;

    // Sync rotation list to current block rotation state
    SyncRotationToState(rotList, initialRotation);
    
    // Get next rotation node
    RotationNode *nextRotationNode = rotList->current->next;
    if (!nextRotationNode) {
        printf("ERROR: nextRotationNode is NULL\n");
        return false;
    }

    // Test rotation at current position without wall kick
    // Temporarily move to next rotation to test
    RotationNode *savedCurrent = rotList->current;
    rotList->current = nextRotationNode;
    
    if (IsValidBlockPosition(block, board, block->x, block->y, newRotation))
    {
        // Rotation successful without wall kick
        block->rotation = newRotation;
        block->rotationNode = nextRotationNode;
        memcpy(block->shape, nextRotationNode->shape, sizeof(block->shape));
        return true;
    }

    // Try wall kick tests
    int wallKickIndex = initialRotation % 4;
    for (int testIndex = 0; testIndex < 5; testIndex++)
    {
        int kickX = WallKickTests[wallKickIndex][testIndex][0];
        int kickY = WallKickTests[wallKickIndex][testIndex][1];

        int testX = block->x + kickX;
        int testY = block->y + kickY;

        if (IsValidBlockPosition(block, board, testX, testY, newRotation))
        {
            // Wall kick successful
            block->x = testX;
            block->y = testY;
            block->rotation = newRotation;
            block->rotationNode = nextRotationNode;
            memcpy(block->shape, nextRotationNode->shape, sizeof(block->shape));
            return true;
        }
    }

    // Rotation failed, restore original state
    rotList->current = savedCurrent;
    return false;
}

// Fungsi untuk menghitung seberapa jauh blok bisa jatuh sebelum bertabrakan
int CalculateDropDistance(TetrisBlock *block, TetrisBoard *board)
{
    int dropDistance = 0;

    while (IsValidBlockPosition(block, board, block->x, block->y + dropDistance + 1, block->rotation))
    {
        dropDistance++;
    }

    return dropDistance;
}

// Fungsi untuk menggerakkan blok secara horizontal
bool MoveBlockHorizontal(TetrisBlock *block, TetrisBoard *board, int dx)
{
    int newX = block->x + dx;

    if (IsValidBlockPosition(block, board, newX, block->y, block->rotation))
    {
        block->x = newX;
        return true;
    }
    return false;
}

// Fungsi untuk menggerakkan blok turun satu langkah
bool MoveBlockDown(TetrisBlock *block, TetrisBoard *board)
{
    int newY = block->y + 1;

    if (IsValidBlockPosition(block, board, block->x, newY, block->rotation))
    {
        block->y = newY;
        return true;
    }
    return false;
}

// Fungsi untuk melakukan Hard Drop
void HardDropBlock(TetrisBlock *block, TetrisBoard *board)
{
    while (MoveBlockDown(block, board))
    {
    }
    PlaceBlock(block, board);
}

// Fungsi untuk menahan blok (hold)
void HoldCurrentBlock(TetrisBoard *board)
{
    // Jika belum pernah memegang blok sebelumnya
    if (!board->hold_block.hasHeld)
    {
        // Simpan blok saat ini ke hold_block
        board->hold_block.block = board->current_block;
        board->hold_block.hasHeld = true;

        // Reset posisi dan rotasi blok yang ditahan
        board->hold_block.block.rotation = 0;
        board->hold_block.block.x = BOARD_WIDTH / 2 - 2;
        board->hold_block.block.y = 0;

        // Perbarui bentuk blok yang di-hold berdasarkan rotasi awal
        RotationList *rotList = GetRotationList(board->hold_block.block.type);

        // Kembalikan ke rotasi awal (posisi awal linked list)
        if (rotList)
        {
            SyncRotationToState(rotList, 0);
            AmbilBentukSaatIni(rotList, board->hold_block.block.shape);
        }

        // Ganti blok saat ini dengan blok berikutnya
        board->current_block = board->next_block;

        // Reset posisi blok baru
        board->current_block.x = BOARD_WIDTH / 2 - 2;
        board->current_block.y = 0;

        // Generate blok berikutnya secara acak
        board->next_block = GenerateRandomBlock();
    }
    // Jika sudah ada blok yang di-hold, lakukan pertukaran blok
    else
    {
        // Simpan blok saat ini ke dalam variabel sementara
        TetrisBlock tempBlock = board->current_block;

        // Ganti blok saat ini dengan blok yang sedang di-hold
        board->current_block = board->hold_block.block;

        // Reset rotasi dan posisi blok
        board->current_block.rotation = 0;
        board->current_block.x = BOARD_WIDTH / 2 - 2;
        board->current_block.y = 0;

        // Perbarui bentuk blok berdasarkan rotasi awal
        RotationList *rotList = GetRotationList(board->current_block.type);

        if (rotList)
        {
            SyncRotationToState(rotList, 0);
            AmbilBentukSaatIni(rotList, board->current_block.shape);
        }

        // Blok yang sebelumnya aktif menjadi blok yang di-hold
        board->hold_block.block = tempBlock;

        // Reset rotasi blok yang di-hold
        board->hold_block.block.rotation = 0;

        // Perbarui bentuk blok yang di-hold
        rotList = GetRotationList(board->hold_block.block.type);

        if (rotList)
        {
            SyncRotationToState(rotList, 0);
            AmbilBentukSaatIni(rotList, board->hold_block.block.shape);
        }
    }
}

// Fungsi untuk menempatkan blok ke papan permainan
void PlaceBlock(TetrisBlock *block, TetrisBoard *board)
{
    RotationList *rotList = GetRotationList(block->type);
    if (!rotList)
        return;

    // Dapatkan bentuk saat ini
    int shape[4][4];
    AmbilBentukSaatIni(rotList, shape);

    // Loop melalui setiap sel dalam matriks 4x4
    for (int y = 0; y < 4; y++)
    {
        for (int x = 0; x < 4; x++)
        {
            // Hanya tempatkan sel yang tidak kosong
            if (shape[y][x] != 0)
            {
                int boardX = block->x + x;
                int boardY = block->y + y;

                // Pastikan koordinat berada dalam batas papan
                if (boardY >= 0 && boardY < BOARD_HEIGHT &&
                    boardX >= 0 && boardX < BOARD_WIDTH)
                {
                    // Simpan tipe blok dalam grid papan
                    board->grid[boardY][boardX] = block->type + 1;
                }
            }
        }
    }
}

// Fungsi untuk mendapatkan warna berdasarkan tipe blok
Color GetTetrominoColor(int blockType)
{
    if (blockType < 0 || blockType >= 7)
    {
        return WHITE; // Warna default jika tipe tidak valid
    }
    return TETROMINO_COLORS[blockType];
}

// Fungsi untuk mengembalikan blok ke rotasi awal
void ResetBlockRotation(TetrisBlock *block)
{
    RotationList *rotList = GetRotationList(block->type);
    if (!rotList)
        return;

    SyncRotationToState(rotList, 0);
    block->rotation = 0;
    AmbilBentukSaatIni(rotList, block->shape);
}

// Fungsi untuk memeriksa apakah blok dapat spawn dengan aman
bool CanSpawnBlock(TetrisBlock *block, TetrisBoard *board)
{
    return IsValidBlockPosition(block, board, block->x, block->y, block->rotation);
}

// Fungsi untuk mendapatkan blok ghost (preview posisi jatuh)
TetrisBlock GetGhostBlock(TetrisBlock *block, TetrisBoard *board)
{
    TetrisBlock ghost = *block;

    // Hitung jarak jatuh maksimal
    int dropDistance = CalculateDropDistance(block, board);
    ghost.y += dropDistance;

    return ghost;
}

// Fungsi untuk memeriksa apakah ada collision dengan blok lain pada posisi spesifik
bool HasCollisionAt(TetrisBlock *block, TetrisBoard *board, int x, int y)
{
    return !IsValidBlockPosition(block, board, x, y, block->rotation);
}

// Fungsi untuk mendapatkan bounding box blok
void GetBlockBounds(TetrisBlock *block, int *minX, int *maxX, int *minY, int *maxY)
{
    *minX = 4;
    *maxX = -1;
    *minY = 4;
    *maxY = -1;

    for (int y = 0; y < 4; y++)
    {
        for (int x = 0; x < 4; x++)
        {
            if (block->shape[y][x] != 0)
            {
                if (x < *minX)
                    *minX = x;
                if (x > *maxX)
                    *maxX = x;
                if (y < *minY)
                    *minY = y;
                if (y > *maxY)
                    *maxY = y;
            }
        }
    }
}

// Fungsi untuk memeriksa apakah rotasi counter-clockwise memungkinkan
bool RotateBlockCounterClockwise(TetrisBlock *block, TetrisBoard *board)
{
    if (!block || !board) return false;
    
    RotationList *rotList = GetRotationList(block->type);
    if (!rotList) return false;

    int initialRotation = block->rotation;
    int rotationCount = rotList->rotationCount;
    int newRotation = (block->rotation - 1 + rotationCount) % rotationCount;

    // Sync to current rotation
    SyncRotationToState(rotList, initialRotation);
    
    // For counter-clockwise, we need to go backwards
    // Since we have circular linked list, go forward (rotationCount - 1) times
    RotationNode *targetNode = rotList->current;
    for (int i = 0; i < rotationCount - 1; i++) {
        targetNode = targetNode->next;
    }

    if (IsValidBlockPosition(block, board, block->x, block->y, newRotation))
    {
        block->rotation = newRotation;
        block->rotationNode = targetNode;
        memcpy(block->shape, targetNode->shape, sizeof(block->shape));
        
        // Update the rotation list current pointer
        rotList->current = targetNode;
        return true;
    }

    return false;
}

// Fungsi untuk soft drop (drop lebih cepat)
bool SoftDropBlock(TetrisBlock *block, TetrisBoard *board)
{
    return MoveBlockDown(block, board);
}

// Fungsi untuk menghitung skor berdasarkan jenis drop
int CalculateDropScore(int dropDistance, bool isHardDrop)
{
    if (isHardDrop)
    {
        return dropDistance * 2; // Hard drop memberikan 2 poin per sel
    }
    else
    {
        return dropDistance; // Soft drop memberikan 1 poin per sel
    }
}

// Fungsi untuk debugging - print bentuk blok ke console
void PrintBlockShape(TetrisBlock *block)
{
    printf("Block Type: %d, Rotation: %d\n", block->type, block->rotation);
    printf("Position: (%d, %d)\n", block->x, block->y);
    printf("Shape:\n");

    for (int y = 0; y < 4; y++)
    {
        for (int x = 0; x < 4; x++)
        {
            printf("%c ", block->shape[y][x] ? '#' : '.');
        }
        printf("\n");
    }
    printf("\n");
}

// FIXED: SyncRotationToState function with proper logic
static void SyncRotationToState(RotationList *rotList, int targetRotation) {
    if (!rotList || !rotList->current) return;
    
    int rotationCount = rotList->rotationCount;
    if (rotationCount <= 0) return;
    
    // Normalize target rotation
    targetRotation = targetRotation % rotationCount;
    if (targetRotation < 0) targetRotation += rotationCount;
    
    // Find current rotation position by traversing the list
    RotationNode *startNode = rotList->current;
    int currentRotation = 0;
    
    // Count current position
    RotationNode *temp = rotList->current;
    for (int i = 0; i < rotationCount; i++) {
        if (temp == startNode && i > 0) {
            // We've come full circle, current position is i
            currentRotation = i;
            break;
        }
        temp = temp->next;
    }
    
    // Calculate how many steps to reach target
    int stepsNeeded = (targetRotation - currentRotation + rotationCount) % rotationCount;
    
    // Move to target rotation
    for (int i = 0; i < stepsNeeded; i++) {
        rotList->current = rotList->current->next;
    }
}

// Fungsi cleanup untuk membebaskan memori
void CleanupBlocks(void)
{
    CleanupRotationSystem();
}