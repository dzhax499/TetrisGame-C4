// Nama file : linkedlist_block.c
// Deskripsi : Implementasi rotasi blok Tetris menggunakan circular linked list (FIXED)
// Oleh      : Dzakir Tsabit 241511071

#include "include/linkedlist_block.h"
#include "include/blocks.h"
#include "include/rotasi_data.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

// Array untuk menyimpan linked list rotasi untuk setiap jenis blok
static RotationList* rotationLists[7] = {NULL};

// FIXED: Improved CreateRotationList with better error handling
RotationList* CreateRotationList(const int shapes[][4][4], int count) {
    if (count <= 0) {
        printf("ERROR: Invalid count %d in CreateRotationList\n", count);
        return NULL;
    }
    
    RotationList* list = (RotationList*)malloc(sizeof(RotationList));
    if (!list) {
        printf("ERROR: Failed to allocate memory for RotationList\n");
        return NULL;
    }
    
    // Initialize list properties
    list->rotationCount = count;
    list->current = NULL;
    
    // Create all nodes first
    RotationNode** nodes = (RotationNode**)malloc(count * sizeof(RotationNode*));
    if (!nodes) {
        printf("ERROR: Failed to allocate memory for nodes array\n");
        free(list);
        return NULL;
    }
    
    // Create all nodes
    for (int i = 0; i < count; i++) {
        nodes[i] = (RotationNode*)malloc(sizeof(RotationNode));
        if (!nodes[i]) {
            printf("ERROR: Failed to allocate memory for RotationNode %d\n", i);
            // Clean up previously allocated nodes
            for (int j = 0; j < i; j++) {
                free(nodes[j]);
            }
            free(nodes);
            free(list);
            return NULL;
        }
        
        // Copy shape data
        memcpy(nodes[i]->shape, shapes[i], sizeof(int) * 4 * 4);
    }
    
    // Link all nodes in a circular fashion
    for (int i = 0; i < count; i++) {
        nodes[i]->next = nodes[(i + 1) % count];
    }
    
    // Set current to first node (rotation 0)
    list->current = nodes[0];
    
    // Clean up temporary array
    free(nodes);
    
    printf("Successfully created rotation list with %d rotations\n", count);
    return list;
}

void InitRotationSystem(void) {
    printf("Initializing rotation system...\n");
    
    rotationLists[0] = CreateRotationList(I_TETROMINO, 2);  // I
    rotationLists[1] = CreateRotationList(J_TETROMINO, 4);  // J
    rotationLists[2] = CreateRotationList(L_TETROMINO, 4);  // L
    rotationLists[3] = CreateRotationList(O_TETROMINO, 1);  // O
    rotationLists[4] = CreateRotationList(S_TETROMINO, 2);  // S
    rotationLists[5] = CreateRotationList(T_TETROMINO, 4);  // T
    rotationLists[6] = CreateRotationList(Z_TETROMINO, 2);  // Z

    // Verify all lists were created successfully
    for (int i = 0; i < 7; i++) {
        if (rotationLists[i] == NULL) {
            printf("ERROR: Failed to create rotation list for block type %d\n", i);
            exit(1);
        } else {
            printf("Block type %d: %d rotations\n", i, rotationLists[i]->rotationCount);
        }
    }
    
    printf("Rotation system initialized successfully!\n");
}

RotationList* GetRotationList(int blockType) {
    if (blockType < 0 || blockType >= 7) {
        printf("ERROR: Invalid block type %d in GetRotationList\n", blockType);
        return NULL;
    }
    
    if (rotationLists[blockType] == NULL) {
        printf("ERROR: Rotation list for block type %d is NULL\n", blockType);
    }
    
    return rotationLists[blockType];
}

void RotateToNext(RotationList* list) {
    if (list && list->current && list->current->next) {
        list->current = list->current->next;
    } else {
        printf("WARNING: Cannot rotate to next - invalid list or current node\n");
    }
}

void AmbilBentukSaatIni(RotationList* list, int shape[4][4]) {
    if (!list) {
        printf("ERROR: list is NULL in AmbilBentukSaatIni\n");
        memset(shape, 0, sizeof(int) * 4 * 4);
        return;
    }
    
    if (!list->current) {
        printf("ERROR: list->current is NULL in AmbilBentukSaatIni\n");
        memset(shape, 0, sizeof(int) * 4 * 4);
        return;
    }
    
    // Copy the current shape
    memcpy(shape, list->current->shape, sizeof(int) * 4 * 4);
}

// FIXED: Improved FreeRotationList
void FreeRotationList(RotationList* list) {
    if (!list) return;
    
    if (!list->current) {
        free(list);
        return;
    }
    
    RotationNode* startNode = list->current;
    RotationNode* current = startNode;
    
    // Break the circular link first
    // Find the last node and break its connection to startNode
    RotationNode* prev = NULL;
    do {
        prev = current;
        current = current->next;
    } while (current != startNode);
    
    // Break the circular link
    prev->next = NULL;
    
    // Now free all nodes starting from startNode
    current = startNode;
    while (current) {
        RotationNode* next = current->next;
        free(current);
        current = next;
    }
    
    free(list);
}

// Fungsi untuk membebaskan semua memori rotasi
void CleanupRotationSystem(void) {
    printf("Cleaning up rotation system...\n");
    
    for (int i = 0; i < 7; i++) {
        if (rotationLists[i]) {
            FreeRotationList(rotationLists[i]);
            rotationLists[i] = NULL;
        }
    }
    
    printf("Rotation system cleaned up successfully!\n");
}