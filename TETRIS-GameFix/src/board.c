// Nama file : board.c
// Deskripsi : Implementasi logika papan permainan Tetris (Doubly Linked List of Rows)
// Oleh      : Ibnu Hilmi 241511079
//             Rizky Satria Gunawan 241511089
// Modifikasi: Menggunakan Doubly Linked List untuk representasi baris papan

#include "include/board.h"
#include "include/blocks.h"
#include <string.h> // For memset, memcpy
#include <stdlib.h> // For malloc, free
#include <stdio.h>  // For printf (debug)
#include "raylib.h"

Color GetBlockColor(BlockType block) {
    static const Color BLOCK_COLORS[] = {
        BLANK,      // 0 = Kosong
        SKYBLUE,    // 1 = I
        DARKBLUE,   // 2 = J
        ORANGE,     // 3 = L
        YELLOW,     // 4 = O
        LIME,       // 5 = S
        PURPLE,     // 6 = T
        RED         // 7 = Z
    };

    if (block >= BLOCK_EMPTY && block <= BLOCK_Z) {
        return BLOCK_COLORS[block];
    }
    return GRAY; // Jika tidak valid, kasih warna default
}

// Fungsi helper untuk mendapatkan node baris pada indeks tertentu
static RowNode* GetRowNodeAtIndex(TetrisBoard* board, int rowIndex) {
    if (rowIndex < 0 || rowIndex >= BOARD_HEIGHT || !board || !board->head_row) {
        // printf("Error: Invalid rowIndex %d or board not initialized properly.\n", rowIndex);
        return NULL;
    }
    RowNode* current = board->head_row;
    for (int i = 0; i < rowIndex; i++) {
        if (!current->next) {
            // printf("Error: Row list shorter than expected at index %d.\n", i);
            return NULL; // Should not happen if BOARD_HEIGHT is maintained
        }
        current = current->next;
    }
    return current;
}

// Inisialisasi papan permainan
void InitBoard1(TetrisBoard *board) {
    if (!board) {
        printf("ERROR: board is NULL in InitBoard1\n");
        return;
    }

    // Free existing board if any
    FreeBoard(board); // Clear previous list to prevent memory leaks

    board->head_row = NULL;
    board->tail_row = NULL;

    RowNode* prev_node = NULL;
    for (int y = 0; y < BOARD_HEIGHT; y++) {
        RowNode* new_node = (RowNode*)malloc(sizeof(RowNode));
        if (!new_node) {
            perror("Failed to allocate memory for RowNode");
            // Cleanup allocated nodes so far
            FreeBoard(board);
            return;
        }
        memset(new_node->columns, BLOCK_EMPTY, sizeof(BlockType) * BOARD_WIDTH);
        new_node->prev = prev_node;
        new_node->next = NULL;

        if (prev_node) {
            prev_node->next = new_node;
        } else {
            board->head_row = new_node; // First node is head
        }
        prev_node = new_node;
    }
    board->tail_row = prev_node; // Last node created is tail

    // Inisialisasi hold block
    memset(&board->hold_block, 0, sizeof(HoldBlock));
    board->hold_block.hasHeld = false;
    
    // Reset current_block dan next_block (akan diisi di main.c)
    memset(&board->current_block, 0, sizeof(TetrisBlock));
    memset(&board->next_block, 0, sizeof(TetrisBlock));

    board->current_score = 0;
    board->current_level = 1;
    board->lines_cleared = 0;
    board->game_over = false;
    // InitScoring(&board->score_data); // Assuming this is called elsewhere or integrated

 
}

// Fungsi untuk membebaskan memori yang dialokasikan untuk papan (linked list)
void FreeBoard(TetrisBoard* board) {
    if (!board) return;
    RowNode* current = board->head_row;
    RowNode* next_node;
    while (current != NULL) {
        next_node = current->next;
        free(current);
        current = next_node;
    }
    board->head_row = NULL;
    board->tail_row = NULL;
    printf(">> [DEBUG] FreeBoard completed.\n");
}

// Fungsi untuk mendapatkan tipe blok pada posisi (row, col) tertentu
BlockType GetBlockAt(TetrisBoard* board, int r, int c) {
    if (r < 0 || r >= BOARD_HEIGHT || c < 0 || c >= BOARD_WIDTH) {
        // Consider out-of-bounds as collision or empty depending on context
        // For IsValidPosition, out-of-bounds usually means invalid (like a wall)
        // Here, let's return BLOCK_EMPTY if asking for non-existent cell,
        // but callers must be careful.
        return BLOCK_EMPTY; // Or a special value like BLOCK_BOUNDARY
    }
    RowNode* row_node = GetRowNodeAtIndex(board, r);
    if (row_node) {
        return row_node->columns[c];
    }
    // printf("Warning: GetBlockAt failed for r=%d, c=%d. RowNode was NULL.\n", r,c);
    return BLOCK_EMPTY; // Should ideally not be reached if rowIndex is valid
}

// Fungsi untuk mengatur tipe blok pada posisi (row, col) tertentu
void SetBlockAt(TetrisBoard* board, int r, int c, BlockType type) {
    if (r < 0 || r >= BOARD_HEIGHT || c < 0 || c >= BOARD_WIDTH) {
        // printf("Warning: SetBlockAt called for out-of-bounds r=%d, c=%d.\n",r,c);
        return; // Do nothing if out of bounds
    }
    RowNode* row_node = GetRowNodeAtIndex(board, r);
    if (row_node) {
        row_node->columns[c] = type;
    } else {
        // printf("Warning: SetBlockAt failed for r=%d, c=%d. RowNode was NULL.\n", r,c);
    }
}


// Hapus baris yang penuh dan update skor
int ClearFullLines(TetrisBoard* board) {
    int linesClearedThisTurn = 0;
    RowNode* currentRow = board->tail_row; // Start checking from the bottom

    // Need to iterate carefully as nodes might be removed.
    // A simple loop from BOARD_HEIGHT-1 down to 0 conceptually.
    for (int y = BOARD_HEIGHT - 1; y >= 0; y--) {
        if (!currentRow) break; // Should not happen if board is correctly initialized

        bool isFullLine = true;
        for (int x = 0; x < BOARD_WIDTH; x++) {
            if (currentRow->columns[x] == BLOCK_EMPTY) {
                isFullLine = false;
                break;
            }
        }

        RowNode* prevRowForLoop = currentRow->prev; // Store for next iteration

        if (isFullLine) {
            linesClearedThisTurn++;

            // Unlink the full row
            if (currentRow->prev) {
                currentRow->prev->next = currentRow->next;
            } else { // currentRow is head_row
                board->head_row = currentRow->next;
            }
            if (currentRow->next) {
                currentRow->next->prev = currentRow->prev;
            } else { // currentRow is tail_row
                board->tail_row = currentRow->prev;
            }
            free(currentRow);

            // Add a new empty row at the top (head)
            RowNode* newEmptyRow = (RowNode*)malloc(sizeof(RowNode));
            if (!newEmptyRow) {
                perror("Failed to allocate memory for new empty row in ClearFullLines");
                // This is a critical error, game state might be corrupted
                board->game_over = true; // Or handle more gracefully
                return linesClearedThisTurn;
            }
            memset(newEmptyRow->columns, BLOCK_EMPTY, sizeof(BlockType) * BOARD_WIDTH);
            newEmptyRow->prev = NULL;
            newEmptyRow->next = board->head_row;

            if (board->head_row) {
                board->head_row->prev = newEmptyRow;
            }
            board->head_row = newEmptyRow;

            if (!board->tail_row) { // If the board became empty (e.g., 1-line board cleared)
                board->tail_row = newEmptyRow;
            }
            
            // Since we removed a line and added one at the top,
            // the line we were about to check (prevRowForLoop) is effectively
            // shifted down. We need to re-evaluate the same conceptual 'y' index.
            // However, our loop structure based on traversing prev pointers handles this naturally
            // if we continue from prevRowForLoop. But the y index should not decrement.
            // So we increment y to re-check the same "position" which now has new content.
            y++; // Re-check the current logical row index, which now contains the row from above

        } // end if isFullLine
        currentRow = prevRowForLoop;
    } // end for y

    // Update score (this scoring logic might be in scoring.c or main.c)
    // For now, let's keep the basic scoring here as it was in the original snippet
    if (linesClearedThisTurn > 0) {
        switch (linesClearedThisTurn) {
            case 1: board->current_score += 100; break;
            case 2: board->current_score += 300; break;
            case 3: board->current_score += 500; break;
            case 4: board->current_score += 800; break; // Tetris!
        }
        board->lines_cleared += linesClearedThisTurn;
        // Level update logic would typically be:
        // board->current_level = 1 + (board->lines_cleared / LINES_PER_LEVEL);
        // LINES_PER_LEVEL usually 10
         board->current_level = 1 + (board->lines_cleared / 10); // Assuming 10 lines per level
    }
    return linesClearedThisTurn;
}


// Fungsi debug: Cetak papan ke konsol
void PrintBoard(TetrisBoard* board) {
    if (!board || !board->head_row) {
        printf("Board not initialized or empty.\n");
        return;
    }
    printf("Board State (Head: %p, Tail: %p):\n", board->head_row, board->tail_row);
    RowNode* current_row = board->head_row;
    int r_idx = 0;
    while (current_row != NULL) {
        printf("%2d: ", r_idx++);
        for (int x = 0; x < BOARD_WIDTH; x++) {
            printf("%d ", current_row->columns[x]);
        }
        // printf(" (Node: %p, Prev: %p, Next: %p)\n", current_row, current_row->prev, current_row->next);
        printf("\n");
        current_row = current_row->next;
    }
    printf("\n");
}

// Periksa apakah permainan berakhir
bool IsGameOver(TetrisBlock *block, TetrisBoard *board) {
    (void)block; // block parameter might not be needed if game over is just top row check
    if (!board || !board->head_row) {
        return false; // No board, no game over
    }

    for (int x = 0; x < BOARD_WIDTH; x++) {
        if (GetBlockAt(board, 0, x) != BLOCK_EMPTY) { // Check the first visible row
             // More robust check: if new block cannot spawn
            TetrisBlock tempBlock = GenerateRandomBlock(); // Get a new block to test spawn
            if (!IsValidBlockPosition(&tempBlock, board, tempBlock.x, tempBlock.y, tempBlock.rotation)) {
                 board->game_over = true;
                 return true;
            }
            // If the above check is too complex here, just use the simpler top row check:
            // board->game_over = true;
            // return true;
        }
    }
    return false;
}