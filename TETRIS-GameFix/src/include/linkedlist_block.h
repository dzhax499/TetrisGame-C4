#ifndef LINKEDLIST_BLOCK_H
#define LINKEDLIST_BLOCK_H

#include "tetris.h"
#include "blocks.h"
#include "board.h"
#include "rendering.h"

typedef struct BlockNode {
    TetrisBlock block;
    struct BlockNode* next;
    struct BlockNode* prev;
} BlockNode;

typedef struct {
    BlockNode* head;
    BlockNode* tail;
} AktifBlok;


void insert_AktifBlok(AktifBlok* list, TetrisBlock newBlock);
void delete_AktifBlok(AktifBlok* list,BlockNode* node);
void update_semuablok(AktifBlok* list, TetrisBoard* board);
void gambar_semuablok(AktifBlok* list);

#endif