#include "include/linkedlist_block.h"


void insert_AktifBlok(AktifBlokDzakir* list, TetrisBlock newBlock)
{
    BlockNodeDzakir* NodeBaru;
    NodeBaru = (BlockNodeDzakir*)malloc(sizeof(BlockNodeDzakir));
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

void update_semuablok(AktifBlokDzakir* list, TetrisBoard* board)
{
    BlockNodeDzakir* Pcur;
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

void delete_AktifBlok(AktifBlokDzakir* list, BlockNodeDzakir* node) {
    if (!list || !node) return;

    if (node->prev) {
        node->prev->next = node->next;
    } else {
        // node adalah head
        list->head = node->next;
    }

    if (node->next) {
        node->next->prev = node->prev;
    }

    free(node);
}

void gambar_semuablok(AktifBlokDzakir* list) {
    BlockNodeDzakir* curr = list->head;
    while (curr) {
        TetrisBlock* blk = &curr->block;

        for (int i = 0; i <4; i++) {
            for (int j = 0; j <4; j++) {
                if (blk->shape[i][j]) {
                    DrawActiveTetromino(blk);
                }
            }
        }

        curr = curr->next;
    }
}
