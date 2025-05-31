// FIXED: linkedlist_block.h - Header rotasi blok Tetris dengan circular linked list
// Nama file : linkedlist_block.h
// Deskripsi : Deklarasi struktur dan fungsi rotasi blok Tetris berbasis circular linked list
// Oleh      : Dzakit Tsabit 241511071 (github : dzhax4499)

#ifndef LINKEDLIST_BLOCK_H
#define LINKEDLIST_BLOCK_H

typedef struct RotationNode {
    int shape[4][4];
    int rotationIndex;
    struct RotationNode* next;
} RotationNode;

typedef struct RotationList {
    RotationNode* current;
    int rotationCount;
    int currentRotationIndex; 
} RotationList;


RotationList* CreateRotationList(const int shapes[][4][4], int count);
void InitRotationSystem(void);
RotationList* GetRotationList(int blockType);
void RotateToNext(RotationList* list);
void SetRotation(RotationList* list, int targetRotation);
void AmbilBentukSaatIni(RotationList* list, int shape[4][4]);
void FreeRotationList(RotationList* list);
void CleanupRotationSystem(void);
int GetCurrentRotationIndex(RotationList* list);

#endif