// Nama file : board_linkedlist.c
// Deskripsi : Implementasi logika papan permainan Tetris menggunakan Linked List
// Oleh      : Implementation dengan Linked List untuk manipulasi baris
// Catatan   : Menggunakan linked list untuk representasi baris-baris board

#include "include/board_linkedlist.h"
#include "include/board.h"
#include "include/blocks.h"
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "raylib.h"

// Fungsi untuk membuat node baris baru
static BoardRow* CreateBoardRow(int rowIndex) {
    BoardRow* newRow = (BoardRow*)malloc(sizeof(BoardRow));
    if (!newRow) {
        printf("ERROR: Failed to allocate memory for BoardRow\n");
        return NULL;
    }
    
    // Inisialisasi baris kosong
    for (int x = 0; x < BOARD_WIDTH; x++) {
        newRow->blocks[x] = BLOCK_EMPTY;
    }
    
    newRow->rowIndex = rowIndex;
    newRow->next = NULL;
    newRow->prev = NULL;
    
    return newRow;
}

// Inisialisasi board dengan linked list
LinkedBoard* InitLinkedBoard(void) {
    LinkedBoard* linkedBoard = (LinkedBoard*)malloc(sizeof(LinkedBoard));
    if (!linkedBoard) {
        printf("ERROR: Failed to allocate memory for LinkedBoard\n");
        return NULL;
    }
    
    linkedBoard->head = NULL;
    linkedBoard->tail = NULL;
    linkedBoard->rowCount = 0;
    
    // Buat BOARD_HEIGHT baris kosong
    for (int y = 0; y < BOARD_HEIGHT; y++) {
        BoardRow* newRow = CreateBoardRow(y);
        if (!newRow) {
            // Cleanup jika gagal
            CleanupLinkedBoard(linkedBoard);
            return NULL;
        }
        
        // Tambahkan ke linked list
        if (!linkedBoard->head) {
            // Baris pertama
            linkedBoard->head = newRow;
            linkedBoard->tail = newRow;
        } else {
            // Tambahkan di belakang (bottom)
            linkedBoard->tail->next = newRow;
            newRow->prev = linkedBoard->tail;
            linkedBoard->tail = newRow;
        }
        
        linkedBoard->rowCount++;
    }
    
    printf(">> [DEBUG] LinkedBoard initialized with %d rows\n", linkedBoard->rowCount);
    return linkedBoard;
}

// Konversi dari TetrisBoard array ke LinkedBoard
void SyncArrayToLinkedBoard(TetrisBoard* board, LinkedBoard* linkedBoard) {
    if (!board || !linkedBoard) return;
    
    BoardRow* currentRow = linkedBoard->head;
    
    for (int y = 0; y < BOARD_HEIGHT && currentRow; y++) {
        // Copy data dari array ke linked list
        memcpy(currentRow->blocks, board->grid[y], sizeof(BlockType) * BOARD_WIDTH);
        currentRow->rowIndex = y;
        currentRow = currentRow->next;
    }
}

// Konversi dari LinkedBoard ke TetrisBoard array
void SyncLinkedBoardToArray(LinkedBoard* linkedBoard, TetrisBoard* board) {
    if (!board || !linkedBoard) return;
    
    BoardRow* currentRow = linkedBoard->head;
    int y = 0;
    
    while (currentRow && y < BOARD_HEIGHT) {
        // Copy data dari linked list ke array
        memcpy(board->grid[y], currentRow->blocks, sizeof(BlockType) * BOARD_WIDTH);
        currentRow->rowIndex = y;  // Update index
        currentRow = currentRow->next;
        y++;
    }
    
    // Jika kurang dari BOARD_HEIGHT baris, isi sisanya dengan kosong
    while (y < BOARD_HEIGHT) {
        memset(board->grid[y], BLOCK_EMPTY, sizeof(BlockType) * BOARD_WIDTH);
        y++;
    }
}

// Cek apakah baris penuh
static bool IsRowFull(BoardRow* row) {
    if (!row) return false;
    
    for (int x = 0; x < BOARD_WIDTH; x++) {
        if (row->blocks[x] == BLOCK_EMPTY) {
            return false;
        }
    }
    return true;
}

// Hapus baris dari linked list
static void RemoveRow(LinkedBoard* linkedBoard, BoardRow* row) {
    if (!linkedBoard || !row) return;
    
    // Update pointer prev
    if (row->prev) {
        row->prev->next = row->next;
    } else {
        // Ini adalah head
        linkedBoard->head = row->next;
    }
    
    // Update pointer next
    if (row->next) {
        row->next->prev = row->prev;
    } else {
        // Ini adalah tail
        linkedBoard->tail = row->prev;
    }
    
    linkedBoard->rowCount--;
    free(row);
}

// Tambahkan baris kosong di atas (head)
static void AddEmptyRowAtTop(LinkedBoard* linkedBoard) {
    if (!linkedBoard) return;
    
    BoardRow* newRow = CreateBoardRow(0);
    if (!newRow) return;
    
    if (!linkedBoard->head) {
        // List kosong
        linkedBoard->head = newRow;
        linkedBoard->tail = newRow;
    } else {
        // Tambahkan di depan
        newRow->next = linkedBoard->head;
        linkedBoard->head->prev = newRow;
        linkedBoard->head = newRow;
    }
    
    linkedBoard->rowCount++;
}

// Update indeks semua baris setelah perubahan
static void UpdateRowIndices(LinkedBoard* linkedBoard) {
    if (!linkedBoard) return;
    
    BoardRow* current = linkedBoard->head;
    int index = 0;
    
    while (current) {
        current->rowIndex = index;
        current = current->next;
        index++;
    }
}

// Implementasi ClearFullLines menggunakan linked list
int ClearFullLinesLinked(TetrisBoard* board, LinkedBoard* linkedBoard) {
    if (!board || !linkedBoard) return 0;
    
    // Sync array ke linked list dulu
    SyncArrayToLinkedBoard(board, linkedBoard);
    
    int linesCleared = 0;
    BoardRow* current = linkedBoard->head;
    
    // Iterasi melalui semua baris
    while (current) {
        BoardRow* next = current->next; // Simpan next sebelum kemungkinan dihapus
        
        if (IsRowFull(current)) {
            printf(">> [DEBUG] Removing full row at index %d\n", current->rowIndex);
            RemoveRow(linkedBoard, current);
            linesCleared++;
            
            // Tambahkan baris kosong di atas
            AddEmptyRowAtTop(linkedBoard);
        }
        
        current = next;
    }
    
    if (linesCleared > 0) {
        // Update indeks baris
        UpdateRowIndices(linkedBoard);
        
        // Sync kembali ke array
        SyncLinkedBoardToArray(linkedBoard, board);
        
        // Update skor
        switch (linesCleared) {
            case 1: board->current_score += 100; break;
            case 2: board->current_score += 300; break;
            case 3: board->current_score += 500; break;
            case 4: board->current_score += 800; break;
        }
        
        // Update level
        board->lines_cleared += linesCleared;
        board->current_level = 1 + (board->lines_cleared / 10);
        
        printf(">> [DEBUG] Cleared %d lines, new score: %d\n", linesCleared, board->current_score);
    }
    
    return linesCleared;
}

// Inisialisasi board dengan menggunakan linked list sebagai backend
void InitBoardWithLinkedList(TetrisBoard *board, LinkedBoard* linkedBoard) {
    if (!board || !linkedBoard) {
        printf("ERROR: board or linkedBoard is NULL in InitBoardWithLinkedList\n");
        return;
    }
    
    printf(">> [DEBUG] InitBoardWithLinkedList started\n");
    
    // Panggil fungsi inisialisasi array original
    InitBoard1(board);
    
    // Sync array ke linked list
    SyncArrayToLinkedBoard(board, linkedBoard);
    
    printf(">> [DEBUG] InitBoardWithLinkedList completed successfully\n");
}

// Fungsi untuk menempatkan blok ke linked board
void PlaceBlockToLinkedBoard(TetrisBlock* block, TetrisBoard* board, LinkedBoard* linkedBoard) {
    if (!block || !board || !linkedBoard) return;
    
    // Gunakan fungsi PlaceBlock original untuk array
    PlaceBlock(block, board);
    
    // Sync perubahan ke linked list
    SyncArrayToLinkedBoard(board, linkedBoard);
}

// Debug: Print linked board
void PrintLinkedBoard(LinkedBoard* linkedBoard) {
    if (!linkedBoard) {
        printf("LinkedBoard is NULL\n");
        return;
    }
    
    printf("=== LinkedBoard Debug (Rows: %d) ===\n", linkedBoard->rowCount);
    
    BoardRow* current = linkedBoard->head;
    int rowNum = 0;
    
    while (current && rowNum < BOARD_HEIGHT) {
        printf("Row %d (idx:%d): ", rowNum, current->rowIndex);
        
        for (int x = 0; x < BOARD_WIDTH; x++) {
            if (current->blocks[x] == BLOCK_EMPTY) {
                printf(". ");
            } else {
                printf("%d ", current->blocks[x]);
            }
        }
        printf("\n");
        
        current = current->next;
        rowNum++;
    }
    printf("================================\n");
}

// Cleanup linked board
void CleanupLinkedBoard(LinkedBoard* linkedBoard) {
    if (!linkedBoard) return;
    
    BoardRow* current = linkedBoard->head;
    
    while (current) {
        BoardRow* next = current->next;
        free(current);
        current = next;
    }
    
    free(linkedBoard);
    printf(">> [DEBUG] LinkedBoard cleaned up\n");
}

// Fungsi utilitas untuk mendapatkan baris pada indeks tertentu
BoardRow* GetRowAtIndex(LinkedBoard* linkedBoard, int index) {
    if (!linkedBoard || index < 0) return NULL;
    
    BoardRow* current = linkedBoard->head;
    int currentIndex = 0;
    
    while (current && currentIndex < index) {
        current = current->next;
        currentIndex++;
    }
    
    return current;
}

// Fungsi untuk mengecek validitas posisi blok dengan linked board
bool IsValidPositionLinked(TetrisBlock* block, LinkedBoard* linkedBoard, int testX, int testY, int testRotation) {
    if (!block || !linkedBoard) return false;
    
    // Kita perlu mengkonversi ke format TetrisBoard dulu untuk menggunakan fungsi existing
    // Atau bisa mengimplementasikan logic collision detection langsung dengan linked list
    
    // Untuk sekarang, gunakan approach konversi ke array
    TetrisBoard tempBoard;
    memset(&tempBoard, 0, sizeof(TetrisBoard));
    
    SyncLinkedBoardToArray(linkedBoard, &tempBoard);
    
    return IsValidBlockPosition(block, &tempBoard, testX, testY, testRotation);
}

// Fungsi untuk mendapatkan jumlah baris kosong di atas
int GetEmptyRowsFromTop(LinkedBoard* linkedBoard) {
    if (!linkedBoard) return 0;
    
    BoardRow* current = linkedBoard->head;
    int emptyRows = 0;
    
    while (current) {
        bool isEmpty = true;
        
        for (int x = 0; x < BOARD_WIDTH; x++) {
            if (current->blocks[x] != BLOCK_EMPTY) {
                isEmpty = false;
                break;
            }
        }
        
        if (isEmpty) {
            emptyRows++;
        } else {
            break; // Jika ketemu baris yang tidak kosong, stop counting
        }
        
        current = current->next;
    }
    
    return emptyRows;
}

// Fungsi untuk compact board (hapus baris kosong berlebihan di atas)
void CompactLinkedBoard(LinkedBoard* linkedBoard) {
    if (!linkedBoard) return;
    
    int emptyRows = GetEmptyRowsFromTop(linkedBoard);
    int maxEmptyRows = 4; // Biarkan maksimal 4 baris kosong di atas
    
    if (emptyRows > maxEmptyRows) {
        int rowsToRemove = emptyRows - maxEmptyRows;
        
        for (int i = 0; i < rowsToRemove && linkedBoard->head; i++) {
            BoardRow* toRemove = linkedBoard->head;
            linkedBoard->head = linkedBoard->head->next;
            
            if (linkedBoard->head) {
                linkedBoard->head->prev = NULL;
            } else {
                linkedBoard->tail = NULL;
            }
            
            free(toRemove);
            linkedBoard->rowCount--;
        }
        
        UpdateRowIndices(linkedBoard);
        printf(">> [DEBUG] Compacted board, removed %d empty rows\n", rowsToRemove);
    }
}