// Nama file : tetromino_rotation.h
// Deskripsi : Header untuk rotasi blok Tetris menggunakan circular linked list
// Oleh      : [Nama Mahasiswa]

#ifndef LINKEDLIST_BLOCK_H
#define LINKEDLIST_BLOCK_H

#include <stdlib.h>
#include <stdbool.h>


// Node untuk linked list rotasi
typedef struct RotationNode {
    int shape[4][4];             
    struct RotationNode* next;    
} RotationNode;

// Daftar circular untuk rotasi blok
typedef struct {
    RotationNode* current;        
    int rotationCount;            
} RotationList;

// Inisialisasi sistem rotasi blok
void InitRotationSystem(void);

// Fungsi untuk mendapatkan linked list rotasi sesuai tipe blok
RotationList* GetRotationList(int blockType);

// Fungsi untuk memutar ke rotasi berikutnya dalam linked list
void RotateToNext(RotationList* list);

// Fungsi untuk mendapatkan bentuk blok saat ini 
void AmbilBentukSaatIni(RotationList* list, int shape[4][4]);

// Fungsi untuk membebaskan memori linked list
void FreeRotationList(RotationList* list);

// Fungsi untuk membuat linked list dari array posisi
RotationList* CreateRotationList(const int shapes[][4][4], int count);

#endif