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
void SetRotation(RotationList* list, int targetRotation);  // NEW
void AmbilBentukSaatIni(RotationList* list, int shape[4][4]);
void FreeRotationList(RotationList* list);
void CleanupRotationSystem(void);
int GetCurrentRotationIndex(RotationList* list);  // NEW

#endif