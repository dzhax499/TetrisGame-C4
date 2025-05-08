#include "include/linkedlist_block.h"


void insert_AktifBlok(AktifBlok* list, TetrisBlock newBlock)
{
    BlockNode* newNode = (BlockNode*)malloc(sizeof(BlockNode));
    newNode->block = newBlock;
    newNode->next = NULL;
    newNode->prev = NULL;

    if (list->head == NULL) {
        list->head = newNode;
        list->tail = newNode;
    } else {
        list->tail->next = newNode;
        newNode->prev = list->tail;
        list->tail = newNode;
    }
}

void update_semuablok(AktifBlok* list, TetrisBoard* board)
{
    BlockNode* Pcur=list->head;
    while (Pcur != NULL) {
        TetrisBlock* blk = &Pcur->block;
        
        for (int i = 0; i < 4 ; i++) {
            for (int j = 0; j < blk->ukuranblok; j++) {
                if (blk->shape[i][j]) {
                    int x = blk->x + j;
                    int y = blk->y + i;

                    if (y >= 0 && y < BOARD_HEIGHT && x >= 0 && x < BOARD_WIDTH) {
                        board->grid[y][x] = blk->color;  // atau kode warna/textur
                    }
                }
            }
        }

        Pcur = Pcur->next;
    }
}