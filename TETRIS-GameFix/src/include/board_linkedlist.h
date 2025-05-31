// Nama file : board_linkedlist.h
// Deskripsi : Header untuk implementasi papan permainan Tetris menggunakan Linked List
// Oleh      : Implementation dengan Linked List untuk manipulasi baris
// Catatan   : Setiap node merepresentasikan satu baris di papan Tetris

#ifndef BOARD_LINKEDLIST_H
#define BOARD_LINKEDLIST_H

#include "board.h"
#include "blocks.h"
#include <stdbool.h>

// Struktur untuk node baris dalam linked list
typedef struct BoardRow {
    BlockType blocks[BOARD_WIDTH];  // Array blok untuk satu baris
    int rowIndex;                   // Indeks baris (0 = atas, BOARD_HEIGHT-1 = bawah)
    struct BoardRow* next;          // Pointer ke baris berikutnya (ke bawah)
    struct BoardRow* prev;          // Pointer ke baris sebelumnya (ke atas)
} BoardRow;

// Struktur untuk linked list board
typedef struct {
    BoardRow* head;     // Pointer ke baris paling atas
    BoardRow* tail;     // Pointer ke baris paling bawah
    int rowCount;       // Jumlah baris saat ini
} LinkedBoard;

// === FUNGSI INISIALISASI ===
// Inisialisasi linked board kosong
LinkedBoard* InitLinkedBoard(void);

// Inisialisasi TetrisBoard dengan backend linked list
void InitBoardWithLinkedList(TetrisBoard *board, LinkedBoard* linkedBoard);

// === FUNGSI SINKRONISASI ===
// Konversi dari array TetrisBoard ke LinkedBoard
void SyncArrayToLinkedBoard(TetrisBoard* board, LinkedBoard* linkedBoard);

// Konversi dari LinkedBoard ke array TetrisBoard
void SyncLinkedBoardToArray(LinkedBoard* linkedBoard, TetrisBoard* board);

// === FUNGSI UTAMA GAME LOGIC ===
// Hapus baris penuh menggunakan linked list (replacement untuk ClearFullLines)
int ClearFullLinesLinked(TetrisBoard* board, LinkedBoard* linkedBoard);

// Tempatkan blok ke linked board
void PlaceBlockToLinkedBoard(TetrisBlock* block, TetrisBoard* board, LinkedBoard* linkedBoard);

// === FUNGSI VALIDASI ===
// Cek validitas posisi blok dengan linked board
bool IsValidPositionLinked(TetrisBlock* block, LinkedBoard* linkedBoard, int testX, int testY, int testRotation);

// === FUNGSI UTILITAS ===
// Dapatkan baris pada indeks tertentu
BoardRow* GetRowAtIndex(LinkedBoard* linkedBoard, int index);

// Hitung jumlah baris kosong dari atas
int GetEmptyRowsFromTop(LinkedBoard* linkedBoard);

// Compact board (hapus baris kosong berlebihan di atas)
void CompactLinkedBoard(LinkedBoard* linkedBoard);

// === FUNGSI DEBUG ===
// Print linked board ke konsol
void PrintLinkedBoard(LinkedBoard* linkedBoard);

// === FUNGSI CLEANUP ===
// Bersihkan memori linked board
void CleanupLinkedBoard(LinkedBoard* linkedBoard);

#endif // BOARD_LINKEDLIST_H