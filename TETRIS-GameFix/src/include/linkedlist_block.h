#ifndef LINKEDLIST_BLOCK_H
#define LINKEDLIST_BLOCK_H

#include "tetris.h"
#include "blocks.h"
#include "board.h"
#include "rendering.h"

typedef struct BlockNodeDzakir {
    TetrisBlock block;
    struct BlockNodeDzakir* next;
    struct BlockNodeDzakir* prev;
} BlockNodeDzakir;

typedef struct {
    BlockNodeDzakir* head;
    BlockNodeDzakir* tail;
} AktifBlokDzakir;


void insert_AktifBlok(AktifBlokDzakir* list, TetrisBlock newBlock);
void delete_AktifBlok(AktifBlokDzakir* list,BlockNodeDzakir* node);
void update_semuablok(AktifBlokDzakir* list, TetrisBoard* board);
void gambar_semuablok(AktifBlokDzakir* list);
bool cekBlokKosong(TetrisBlock* blk, TetrisBoard* board);


#endif