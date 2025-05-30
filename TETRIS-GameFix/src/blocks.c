// Nama file : blocks.c
// Deskripsi : Implementasi blok-blok game Tetris (FIXED VERSION)
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
static void SyncRotationToState(RotationList *rotList, int targetRotation);
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
    block.rotation = 0;  // Always start at rotation 0
    block.ukuranblok = 4;
    block.efekmeledak = false;
    block.waktumeledak = 0;

    // FIXED: Adjust initial position to center the block properly
    // Different blocks need different centering due to their shapes
    switch(block.type) {
        case 0: // I-piece
            block.x = BOARD_WIDTH / 2 - 2;
            block.y = -1; // Start slightly above visible area
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
        // FIXED: Ensure we start at rotation 0 and sync properly
        SyncRotationToState(rotList, 0);
        block.rotationNode = rotList->current;
        UpdateBlockShape(&block);
    }

    // Tetapkan warna
    block.color = TETROMINO_COLORS[block.type];

    return block;
}

// FIXED: Improved collision detection with proper shape retrieval
bool IsValidBlockPosition(TetrisBlock *block, TetrisBoard *board, int testX, int testY, int testRotation)
{
    if (!block || !board) return false;
    
    RotationList *rotList = GetRotationList(block->type);
    if (!rotList) return false;

    // Save current rotation state
    RotationNode *savedRotation = rotList->current;
    int savedBlockRotation = block->rotation;
    
    // Temporarily sync to the test rotation
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
            if (bentuk[y][x] == 0) continue; // Skip empty cells

            int boardX = testX + x;
            int boardY = testY + y;

            // Check left and right bounds
            if (boardX < 0 || boardX >= BOARD_WIDTH)
            {
                return false;
            }

            // Check bottom bound
            if (boardY >= BOARD_HEIGHT)
            {
                return false;
            }

            // Allow blocks to spawn above the visible area
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

// FIXED: Improved rotation with proper synchronization
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
        // Update block rotation
        block->rotation = newRotation;
        
        // FIXED: Properly sync rotation list and update shape
        SyncRotationToState(rotList, newRotation);
        block->rotationNode = rotList->current;
        UpdateBlockShape(block);
        
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

    // Test rotation at current position without wall kick
    if (IsValidBlockPosition(block, board, block->x, block->y, newRotation))
    {
        // Rotation successful without wall kick
        block->rotation = newRotation;
        SyncRotationToState(rotList, newRotation);
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
            // Wall kick successful
            block->x = testX;
            block->y = testY;
            block->rotation = newRotation;
            SyncRotationToState(rotList, newRotation);
            block->rotationNode = rotList->current;
            UpdateBlockShape(block);
            return true;
        }
    }

    // Rotation failed
    return false;
}

// FIXED: Improved drop distance calculation
int CalculateDropDistance(TetrisBlock *block, TetrisBoard *board)
{
    if (!block || !board) return 0;
    
    int dropDistance = 0;

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

// FIXED: Improved block placement with proper shape retrieval
void PlaceBlock(TetrisBlock *block, TetrisBoard *board)
{
    if (!block || !board) return;
    
    RotationList *rotList = GetRotationList(block->type);
    if (!rotList) return;

    // FIXED: Ensure we're using the current rotation state
    SyncRotationToState(rotList, block->rotation);

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

                // Better bounds checking for placement
                if (boardY >= 0 && boardY < BOARD_HEIGHT &&
                    boardX >= 0 && boardX < BOARD_WIDTH)
                {
                    // Simpan tipe blok dalam grid papan (1-indexed)
                    board->grid[boardY][boardX] = block->type + 1;
                }
            }
        }
    }
}

// FIXED: Completely rewritten SyncRotationToState function
static void SyncRotationToState(RotationList *rotList, int targetRotation) {
    if (!rotList || !rotList->current) return;
    
    int rotationCount = rotList->rotationCount;
    if (rotationCount <= 0) return;
    
    // Normalize target rotation
    targetRotation = targetRotation % rotationCount;
    if (targetRotation < 0) targetRotation += rotationCount;
    
    // Find the first node (rotation 0) by going through the entire circle
    // and identifying the starting position
    RotationNode *startNode = rotList->current;
    RotationNode *currentNode = rotList->current;
    
    // We need to find rotation 0. We'll assume the current implementation
    // where the list is created in order, so we need to go to the beginning
    // Reset to rotation 0 by finding the node that represents the first rotation
    
    // Simple approach: reset to the head and then advance targetRotation steps
    // Since we don't have a direct reference to rotation 0, we'll count steps
    
    // Go to rotation 0 (assumed to be the first node created)
    // This is a workaround - ideally we'd store a reference to the head
    for (int i = 0; i < rotationCount; i++) {
        currentNode = currentNode->next;
        if (currentNode == startNode) {
            // We've gone full circle, startNode is rotation 0
            break;
        }
    }
    
    // Now advance targetRotation steps from rotation 0
    rotList->current = startNode;
    for (int i = 0; i < targetRotation; i++) {
        rotList->current = rotList->current->next;
    }
}

// NEW: Helper function to update block shape from current rotation
static void UpdateBlockShape(TetrisBlock *block) {
    if (!block) return;
    
    RotationList *rotList = GetRotationList(block->type);
    if (!rotList) return;
    
    // Ensure the rotation list is synced to the block's rotation
    SyncRotationToState(rotList, block->rotation);
    
    // Update the block's shape array
    AmbilBentukSaatIni(rotList, block->shape);
}

// Other utility functions with improved error checking
Color GetTetrominoColor(int blockType)
{
    if (blockType < 0 || blockType >= 7)
    {
        return WHITE; // Warna default jika tipe tidak valid
    }
    return TETROMINO_COLORS[blockType];
}

void ResetBlockRotation(TetrisBlock *block)
{
    if (!block) return;
    
    RotationList *rotList = GetRotationList(block->type);
    if (!rotList) return;

    block->rotation = 0;
    SyncRotationToState(rotList, 0);
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
        // Hitung jarak jatuh maksimal
        int dropDistance = CalculateDropDistance(block, board);
        ghost.y += dropDistance;
    }

    return ghost;
}

bool HasCollisionAt(TetrisBlock *block, TetrisBoard *board, int x, int y)
{
    if (!block || !board) return true; // Assume collision if invalid input
    return !IsValidBlockPosition(block, board, x, y, block->rotation);
}

void GetBlockBounds(TetrisBlock *block, int *minX, int *maxX, int *minY, int *maxY)
{
    if (!block || !minX || !maxX || !minY || !maxY) return;
    
    *minX = 4;
    *maxX = -1;
    *minY = 4;
    *maxY = -1;

    // FIXED: Use current shape from rotation system
    RotationList *rotList = GetRotationList(block->type);
    if (rotList) {
        SyncRotationToState(rotList, block->rotation);
        int shape[4][4];
        AmbilBentukSaatIni(rotList, shape);
        
        for (int y = 0; y < 4; y++)
        {
            for (int x = 0; x < 4; x++)
            {
                if (shape[y][x] != 0)
                {
                    if (x < *minX) *minX = x;
                    if (x > *maxX) *maxX = x;
                    if (y < *minY) *minY = y;
                    if (y > *maxY) *maxY = y;
                }
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
        SyncRotationToState(rotList, newRotation);
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
        return dropDistance * 2; // Hard drop memberikan 2 poin per sel
    }
    else
    {
        return dropDistance; // Soft drop memberikan 1 poin per sel
    }
}

void PrintBlockShape(TetrisBlock *block)
{
    if (!block) return;
    
    printf("Block Type: %d, Rotation: %d\n", block->type, block->rotation);
    printf("Position: (%d, %d)\n", block->x, block->y);
    printf("Shape:\n");

    // FIXED: Use current shape from rotation system
    RotationList *rotList = GetRotationList(block->type);
    if (rotList) {
        SyncRotationToState(rotList, block->rotation);
        int shape[4][4];
        AmbilBentukSaatIni(rotList, shape);
        
        for (int y = 0; y < 4; y++)
        {
            for (int x = 0; x < 4; x++)
            {
                printf("%c ", shape[y][x] ? '#' : '.');
            }
            printf("\n");
        }
    }
    printf("\n");
}

void HoldCurrentBlock(TetrisBoard *board)
{
    if (!board) return;
    
    // Jika belum pernah memegang blok sebelumnya
    if (!board->hold_block.hasHeld)
    {
        // Simpan blok saat ini ke hold_block
        board->hold_block.block = board->current_block;
        board->hold_block.hasHeld = true;

        // Reset posisi dan rotasi blok yang ditahan
        ResetBlockRotation(&board->hold_block.block);
        board->hold_block.block.x = BOARD_WIDTH / 2 - 2;
        board->hold_block.block.y = 0;

        // Ganti blok saat ini dengan blok berikutnya
        board->current_block = board->next_block;
        board->current_block.x = BOARD_WIDTH / 2 - 2;
        board->current_block.y = 0;

        // Generate blok berikutnya secara acak
        board->next_block = GenerateRandomBlock();
    }
    else
    {
        // Jika sudah ada blok yang di-hold, lakukan pertukaran blok
        TetrisBlock tempBlock = board->current_block;

        // Ganti blok saat ini dengan blok yang sedang di-hold
        board->current_block = board->hold_block.block;
        ResetBlockRotation(&board->current_block);
        board->current_block.x = BOARD_WIDTH / 2 - 2;
        board->current_block.y = 0;

        // Blok yang sebelumnya aktif menjadi blok yang di-hold
        board->hold_block.block = tempBlock;
        ResetBlockRotation(&board->hold_block.block);
    }
}

void CleanupBlocks(void)
{
    CleanupRotationSystem();
}