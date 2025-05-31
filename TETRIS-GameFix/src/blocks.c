// FIXED: blocks.c - Auto rotation bug diperbaiki
// Nama file : blocks.c
// Deskripsi : Implementasi blok-blok game Tetris (FIXED - NO AUTO ROTATION BUG)
// Oleh      : Dzakit Tsabit 241511071

#include <string.h>
#include <time.h>
#include "include/blocks.h"
#include "include/rotasi_data.h"
#include "include/linkedlist_block.h"
#include <stdlib.h>
#include <stdio.h>
#include "raylib.h"

// Forward declaration for static functions
static void UpdateBlockShape(TetrisBlock *block);

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
    {{0, 0}, {-1, 0}, {-1, -1}, {0, 2}, {-1, 2}}
};

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
    block.rotation = 0;  // ALWAYS start at rotation 0
    block.ukuranblok = 4;
    block.efekmeledak = false;
    block.waktumeledak = 0;

    // FIXED: Proper initial positioning
    switch(block.type) {
        case 0: // I-piece
            block.x = BOARD_WIDTH / 2 - 2;
            block.y = -1;
            break;
        case 3: // O-piece
            block.x = BOARD_WIDTH / 2 - 2;
            block.y = 0;
            break;
        default: // J, L, S, T, Z pieces
            block.x = BOARD_WIDTH / 2 - 2;
            block.y = 0;
            break;
    }

    // CRITICAL FIX: Set rotation list to rotation 0 dan JANGAN panggil sync
    RotationList *rotList = GetRotationList(block.type);
    if (rotList == NULL)
    {
        printf("ERROR: rotList NULL saat generate blok type %d\n", block.type);
        // Emergency fallback
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
        // FIXED: Pastikan rotList di posisi 0 tanpa memanggil sync yang berbahaya
        SetRotation(rotList, 0); // Set ke rotation 0
        block.rotationNode = rotList->current;
        UpdateBlockShape(&block);
    }

    // Tetapkan warna
    block.color = TETROMINO_COLORS[block.type];

    return block;
}

// FIXED: IsValidBlockPosition yang tidak mengubah rotasi current
bool IsValidBlockPosition(TetrisBlock *block, TetrisBoard *board, int testX, int testY, int testRotation)
{
    if (!block || !board) return false;
    
    RotationList *rotList = GetRotationList(block->type);
    if (!rotList) return false;

    // CRITICAL FIX: Dapatkan shape untuk test rotation TANPA mengubah current state
    int bentuk[4][4];
    
    // Jika test rotation sama dengan current rotation, gunakan shape yang sudah ada
    if (testRotation == block->rotation) {
        memcpy(bentuk, block->shape, sizeof(int) * 4 * 4);
    } else {
        // FIXED: Untuk test rotation yang berbeda, kita perlu navigasi manual
        // Simpan current state
        RotationNode *originalCurrent = rotList->current;
        int originalIndex = rotList->currentRotationIndex;
        
        // Navigate ke test rotation
        SetRotation(rotList, testRotation);
        AmbilBentukSaatIni(rotList, bentuk);
        
        // CRITICAL: Restore original state
        rotList->current = originalCurrent;
        rotList->currentRotationIndex = originalIndex;
    }

    // Test collision dengan retrieved shape
    for (int y = 0; y < 4; y++)
    {
        for (int x = 0; x < 4; x++)
        {
            if (bentuk[y][x] == 0) continue;

            int boardX = testX + x;
            int boardY = testY + y;

            // Check bounds
            if (boardX < 0 || boardX >= BOARD_WIDTH) return false;
            if (boardY >= BOARD_HEIGHT) return false;
            if (boardY < 0) continue; // Allow spawning above visible area

            // Check collision with existing blocks
            if (GetBlockAt(board, boardY, boardX) != BLOCK_EMPTY) return false;
        }
    }

    return true;
}

// FIXED: RotateBlock yang hanya berubah saat dipanggil
bool RotateBlock(TetrisBlock *block, TetrisBoard *board)
{
    if (!block || !board) return false;
    
    RotationList *rotList = GetRotationList(block->type);
    if (!rotList) return false;

    int currentRotation = block->rotation;
    int newRotation = (currentRotation + 1) % rotList->rotationCount;

    // Test if rotation is valid at current position
    if (IsValidBlockPosition(block, board, block->x, block->y, newRotation))
    {
        // CRITICAL: Update block rotation
        block->rotation = newRotation;
        
        // FIXED: Set rotation list ke new rotation
        SetRotation(rotList, newRotation);
        block->rotationNode = rotList->current;
        UpdateBlockShape(block);
        
        printf("DEBUG: Block rotated to %d\n", newRotation);
        return true;
    }

    printf("DEBUG: Rotation blocked\n");
    return false;
}

bool RotateBlockWithWallKick(TetrisBlock *block, TetrisBoard *board)
{
    if (!block || !board) return false;
    
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

    // Test rotation at current position without wall kick
    if (IsValidBlockPosition(block, board, block->x, block->y, newRotation))
    {
        block->rotation = newRotation;
        SetRotation(rotList, newRotation);
        block->rotationNode = rotList->current;
        UpdateBlockShape(block);
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
            block->x = testX;
            block->y = testY;
            block->rotation = newRotation;
            SetRotation(rotList, newRotation);
            block->rotationNode = rotList->current;
            UpdateBlockShape(block);
            return true;
        }
    }

    return false;
}

// FIXED: Calculate drop distance yang tidak mengubah rotasi
int CalculateDropDistance(TetrisBlock *block, TetrisBoard *board)
{
    if (!block || !board) return 0;
    
    int dropDistance = 0;

    // CRITICAL: Gunakan rotasi current block, JANGAN ubah apapun
    while (IsValidBlockPosition(block, board, block->x, block->y + dropDistance + 1, block->rotation))
    {
        dropDistance++;
    }

    return dropDistance;
}

bool MoveBlockHorizontal(TetrisBlock *block, TetrisBoard *board, int dx)
{
    if (!block || !board) return false;
    
    int newX = block->x + dx;

    // CRITICAL: Gunakan rotasi current saja
    if (IsValidBlockPosition(block, board, newX, block->y, block->rotation))
    {
        block->x = newX;
        return true;
    }
    return false;
}

bool MoveBlockDown(TetrisBlock *block, TetrisBoard *board)
{
    if (!block || !board) return false;
    
    int newY = block->y + 1;

    // CRITICAL: Gunakan rotasi current saja - JANGAN ubah rotasi
    if (IsValidBlockPosition(block, board, block->x, newY, block->rotation))
    {
        block->y = newY;
        return true;
    }
    return false;
}

void HardDropBlock(TetrisBlock *block, TetrisBoard *board)
{
    if (!block || !board) return;
    
    while (MoveBlockDown(block, board))
    {
        // Keep dropping until it can't move down anymore
    }
    PlaceBlock(block, board);
}

// FIXED: PlaceBlock yang menggunakan current shape
void PlaceBlock(TetrisBlock *block, TetrisBoard *board)
{
    if (!block || !board) return;
    
    // CRITICAL: Gunakan shape yang sudah ada di block, JANGAN sync ulang
    for (int y = 0; y < 4; y++)
    {
        for (int x = 0; x < 4; x++)
        {
            if (block->shape[y][x] != 0)
            {
                int boardX = block->x + x;
                int boardY = block->y + y;

                if (boardY >= 0 && boardY < BOARD_HEIGHT &&
                    boardX >= 0 && boardX < BOARD_WIDTH)
                {
                   SetBlockAt(board, boardY, boardX, (BlockType)(block->type + 1)); // +1 karena BLOCK_EMPTY adalah 0
                }
            }
        }
    }
}

// FIXED: UpdateBlockShape yang aman
static void UpdateBlockShape(TetrisBlock *block) {
    if (!block) return;
    
    RotationList *rotList = GetRotationList(block->type);
    if (!rotList) return;
    
    // CRITICAL: JANGAN panggil SetRotation lagi, langsung copy shape
    AmbilBentukSaatIni(rotList, block->shape);
}

// Other utility functions
Color GetTetrominoColor(int blockType)
{
    if (blockType < 0 || blockType >= 7)
    {
        return WHITE;
    }
    return TETROMINO_COLORS[blockType];
}

void ResetBlockRotation(TetrisBlock *block)
{
    if (!block) return;
    
    RotationList *rotList = GetRotationList(block->type);
    if (!rotList) return;

    block->rotation = 0;
    SetRotation(rotList, 0);
    block->rotationNode = rotList->current;
    UpdateBlockShape(block);
}

bool CanSpawnBlock(TetrisBlock *block, TetrisBoard *board)
{
    if (!block || !board) return false;
    return IsValidBlockPosition(block, board, block->x, block->y, block->rotation);
}

TetrisBlock GetGhostBlock(TetrisBlock *block, TetrisBoard *board)
{
    TetrisBlock ghost = *block;
    
    if (block && board) {
        int dropDistance = CalculateDropDistance(block, board);
        ghost.y += dropDistance;
    }

    return ghost;
}

bool HasCollisionAt(TetrisBlock *block, TetrisBoard *board, int x, int y)
{
    if (!block || !board) return true;
    return !IsValidBlockPosition(block, board, x, y, block->rotation);
}

void GetBlockBounds(TetrisBlock *block, int *minX, int *maxX, int *minY, int *maxY)
{
    if (!block || !minX || !maxX || !minY || !maxY) return;
    
    *minX = 4;
    *maxX = -1;
    *minY = 4;
    *maxY = -1;

    // FIXED: Gunakan shape yang sudah ada di block
    for (int y = 0; y < 4; y++)
    {
        for (int x = 0; x < 4; x++)
        {
            if (block->shape[y][x] != 0)
            {
                if (x < *minX) *minX = x;
                if (x > *maxX) *maxX = x;
                if (y < *minY) *minY = y;
                if (y > *maxY) *maxY = y;
            }
        }
    }
}

bool RotateBlockCounterClockwise(TetrisBlock *block, TetrisBoard *board)
{
    if (!block || !board) return false;
    
    RotationList *rotList = GetRotationList(block->type);
    if (!rotList) return false;

    int currentRotation = block->rotation;
    int rotationCount = rotList->rotationCount;
    int newRotation = (currentRotation - 1 + rotationCount) % rotationCount;

    if (IsValidBlockPosition(block, board, block->x, block->y, newRotation))
    {
        block->rotation = newRotation;
        SetRotation(rotList, newRotation);
        block->rotationNode = rotList->current;
        UpdateBlockShape(block);
        return true;
    }

    return false;
}

bool SoftDropBlock(TetrisBlock *block, TetrisBoard *board)
{
    return MoveBlockDown(block, board);
}

int CalculateDropScore(int dropDistance, bool isHardDrop)
{
    if (isHardDrop)
    {
        return dropDistance * 2;
    }
    else
    {
        return dropDistance;
    }
}

void PrintBlockShape(TetrisBlock *block)
{
    if (!block) return;
    
    printf("Block Type: %d, Rotation: %d\n", block->type, block->rotation);
    printf("Position: (%d, %d)\n", block->x, block->y);
    printf("Shape:\n");

    // FIXED: Gunakan shape yang sudah ada di block
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

void HoldCurrentBlock(TetrisBoard *board)
{
    if (!board) return;
    
    if (!board->hold_block.hasHeld)
    {
        board->hold_block.block = board->current_block;
        board->hold_block.hasHeld = true;

        ResetBlockRotation(&board->hold_block.block);
        board->hold_block.block.x = BOARD_WIDTH / 2 - 2;
        board->hold_block.block.y = 0;

        board->current_block = board->next_block;
        board->current_block.x = BOARD_WIDTH / 2 - 2;
        board->current_block.y = 0;

        board->next_block = GenerateRandomBlock();
    }
    else
    {
        TetrisBlock tempBlock = board->current_block;

        board->current_block = board->hold_block.block;
        ResetBlockRotation(&board->current_block);
        board->current_block.x = BOARD_WIDTH / 2 - 2;
        board->current_block.y = 0;

        board->hold_block.block = tempBlock;
        ResetBlockRotation(&board->hold_block.block);
    }
}

void CleanupBlocks(void)
{
    CleanupRotationSystem();
}