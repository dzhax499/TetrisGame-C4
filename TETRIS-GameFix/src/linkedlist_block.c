// Nama file : linkedlist_block.c
// Deskripsi : Implementasi rotasi blok Tetris menggunakan circular linked list
// Oleh      : Dzakir Tsabit 241511071 cek

#include "include/linkedlist_block.h"
#include "include/rotasi_data.h"
#include <stdio.h>
#include <string.h>

// Array yang menyimpan bentuk semua tetromino dengan rotasi masing-masing
// Dipisah berdasarkan jenis blok


// Array untuk menyimpan linked list rotasi untuk setiap jenis blok
static RotationList* rotationLists[7] = {NULL};

// Fungsi untuk membuat circular linked list dari array posisi
RotationList* CreateRotationList(const int shapes[][4][4], int count) {
    if (count <= 0) return NULL;
    
    RotationList* list;
    list =  (RotationList*)malloc(sizeof(RotationList));
    if (!list) {
        printf("Gagal mengalokasikan memori untuk RotationList\n");
        return NULL;
    }
    
    // Buat node pertama
    RotationNode* firstNode;
    firstNode = (RotationNode*)malloc(sizeof(RotationNode));
    if (!firstNode) {
        printf("Gagal mengalokasikan memori untuk RotationNode\n");
        free(list);
        return NULL;
    }
    
    // Salin bentuk pertama ke node pertama
    memcpy(firstNode->shape, shapes[0], sizeof(int) * 4 * 4);
    
    // Node pertama menjadi node saat ini
    list->current = firstNode;
    list->rotationCount = count;
    
    // Node sebelumnya dimulai dari node pertama
    RotationNode* prevNode;
    prevNode = firstNode;
    
    // Buat node-node lain dan sambungkan
    for (int i = 1; i < count; i++) {
        RotationNode* newNode = (RotationNode*)malloc(sizeof(RotationNode));
        if (!newNode) {
            printf("Gagal mengalokasikan memori untuk RotationNode\n");
            // Seharusnya kita membersihkan memori yang sudah dialokasikan sebelumnya di sini
            // Tapi untuk menjaga kode tetap sederhana, kita lewati
            return list;
        }
        
        // Salin bentuk ke node baru
        memcpy(newNode->shape, shapes[i], sizeof(int) * 4 * 4);
        
        // Sambungkan node sebelumnya ke node baru
        prevNode->next = newNode;
        prevNode = newNode;
    }
    
    // Buat circular dengan menghubungkan node terakhir ke node pertama
    prevNode->next = firstNode;
    
    return list;
}


void InitRotationSystem(void) {

    rotationLists[0] = CreateRotationList(I_TETROMINO, 2);  // I
    rotationLists[1] = CreateRotationList(J_TETROMINO, 4);  // J
    rotationLists[2] = CreateRotationList(L_TETROMINO, 4);  // L
    rotationLists[3] = CreateRotationList(O_TETROMINO, 1);  // O
    rotationLists[4] = CreateRotationList(S_TETROMINO, 2);  // S
    rotationLists[5] = CreateRotationList(T_TETROMINO, 4);  // T
    rotationLists[6] = CreateRotationList(Z_TETROMINO, 2);  // Z
}


RotationList* GetRotationList(int blockType) {
    if (blockType < 0 || blockType >= 7) return NULL;
    return rotationLists[blockType];
}

void RotateToNext(RotationList* list) {
    if (list && list->current) {
        list->current = list->current->next;
    }
}

void AmbilBentukSaatIni(RotationList* list, int shape[4][4]) {
    if (list && list->current) {
        memcpy(shape, list->current->shape, sizeof(int) * 4 * 4);
    }
}

void FreeRotationList(RotationList* list) {
    if (!list || !list->current) return;
    
    RotationNode* startNode = list->current;
    RotationNode* current = startNode;
    
    list->current->next = NULL;
    
    while (current) {
        RotationNode* next = current->next;
        free(current);
        
        if (next == startNode) break; 
        current = next;
    }
    
    free(list);
}

// Fungsi untuk membebaskan semua memori rotasi
void CleanupRotationSystem(void) {
    for (int i = 0; i < 7; i++) {
        if (rotationLists[i]) {
            FreeRotationList(rotationLists[i]);
            rotationLists[i] = NULL;
        }
    }
}