#include "include/linkedlist_block.h"


void insert_AktifBlok(AktifBlok* list, TetrisBlock newBlock)
{
    BlockNode* NodeBaru;
    NodeBaru = (BlockNode*)malloc(sizeof(BlockNode));
    NodeBaru->block = newBlock;
    NodeBaru->next = NULL;
    NodeBaru->prev = NULL;

    if (list->head == NULL) {
        list->head = NodeBaru;
        list->tail = NodeBaru;
    } else {
        list->tail->next = NodeBaru;
        NodeBaru->prev = list->tail;
        list->tail = NodeBaru;
    }
}

void update_semuablok(AktifBlok* list, TetrisBoard* board)
{
    BlockNode* Pcur;
    Pcur=list->head;

    while (Pcur != NULL) {
        TetrisBlock* blk;
        blk = &Pcur->block;
        
        for (int i = 0; i < 4 ; i++) {
            for (int j = 0; j < blk->ukuranblok; j++) {
                if (blk->shape[i][j]) {
                    int x = blk->x + j;
                    int y = blk->y + i;

                    if (y >= 0 && y < BOARD_HEIGHT && x >= 0 && x < BOARD_WIDTH) {
                        board->grid[y][x] = blk->type;
                    }
                }
            }
        }

        Pcur = Pcur->next;
    }
}