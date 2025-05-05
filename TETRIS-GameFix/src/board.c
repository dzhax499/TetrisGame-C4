#include "include/board.h"
#include "include/blocks.h"
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "raylib.h"

TetrisBoard *board = NULL;

Color GetBlockColor(BlockType block) {
    static const Color BLOCK_COLORS[] = {
        BLANK,      // 0 = Kosong
        SKYBLUE,    // 1 = I
        DARKBLUE,   // 2 = J
        ORANGE,     // 3 = L
        YELLOW,     // 4 = O
        LIME,       // 5 = S
        PURPLE,     // 6 = T
        RED         // 7 = Z
    };
    return (block >= BLOCK_EMPTY && block <= BLOCK_Z) ? BLOCK_COLORS[block] : GRAY;
}

void InitBoard(TetrisBoard* board) {
    memset(board->grid, BLOCK_EMPTY, sizeof(board->grid));
    board->current_score = 0;
    board->current_level = 1;
    board->lines_cleared = 0;
    board->game_over = false;
    board->hasHeld = false;
    board->hold_history = NULL;
    
    // Initialize next_blocks as a doubly linked list with 3 random blocks
    InitNextBlocks(board, NEXT_BLOCKS_COUNT);
    board->current_block = GenerateRandomBlock();
}

void InitNextBlocks(TetrisBoard* board, int count) {
    if (count <= 0) {
        board->next_blocks = NULL;
        return;
    }

    // Create the first node
    NextBlockNode* head = (NextBlockNode*)malloc(sizeof(NextBlockNode));
    head->type = GenerateRandomBlock().type;
    head->prev = NULL;
    
    NextBlockNode* current = head;
    NextBlockNode* prev = NULL;

    // Create remaining nodes
    for (int i = 1; i < count; i++) {
        NextBlockNode* newNode = (NextBlockNode*)malloc(sizeof(NextBlockNode));
        newNode->type = GenerateRandomBlock().type;
        newNode->prev = current;
        
        current->next = newNode;
        prev = current;
        current = newNode;
    }

    // Complete the circular link
    current->next = head;
    head->prev = current;

    board->next_blocks = head;
}

BlockType GetNextBlock(TetrisBoard* board) {
    if (board->next_blocks == NULL) {
        return BLOCK_EMPTY;
    }

    // Get the current block type
    BlockType nextType = board->next_blocks->type;
    
    // Move to the next block in the circular list
    board->next_blocks = board->next_blocks->next;
    
    return nextType;
}

void AddNextBlock(TetrisBoard* board, BlockType type) {
    if (board->next_blocks == NULL) {
        // If list is empty, create a new circular list with one node
        NextBlockNode* newNode = (NextBlockNode*)malloc(sizeof(NextBlockNode));
        newNode->type = type;
        newNode->next = newNode;
        newNode->prev = newNode;
        board->next_blocks = newNode;
    } else {
        // Insert new node before the current head
        NextBlockNode* newNode = (NextBlockNode*)malloc(sizeof(NextBlockNode));
        newNode->type = type;
        
        NextBlockNode* tail = board->next_blocks->prev;
        
        newNode->next = board->next_blocks;
        newNode->prev = tail;
        
        tail->next = newNode;
        board->next_blocks->prev = newNode;
    }
}

void HoldCurrentBlock(TetrisBoard* board) {
    if (board->hasHeld) return;

    // Create new hold history node
    HoldHistoryNode* newNode = (HoldHistoryNode*)malloc(sizeof(HoldHistoryNode));
    newNode->type = board->current_block.type;
    
    if (board->hold_history == NULL) {
        // First hold - create circular list
        newNode->next = newNode;
        newNode->prev = newNode;
    } else {
        // Insert at head of circular list
        newNode->next = board->hold_history;
        newNode->prev = board->hold_history->prev;
        
        board->hold_history->prev->next = newNode;
        board->hold_history->prev = newNode;
    }
    
    board->hold_history = newNode;
    board->hasHeld = true;

    // Replace current block with next block
    board->current_block.type = GetNextBlock(board);
}

BlockType GetLastHoldBlock(TetrisBoard* board) {
    if (board->hold_history == NULL) {
        return BLOCK_EMPTY;
    }
    return board->hold_history->type;
}

int ClearFullLines(TetrisBoard* board) {
    int linesCleared = 0;
    
    for (int y = BOARD_HEIGHT - 1; y >= 0; y--) {
        bool lineFull = true;
        
        for (int x = 0; x < BOARD_WIDTH; x++) {
            if (board->grid[y][x] == BLOCK_EMPTY) {
                lineFull = false;
                break;
            }
        }
        
        if (lineFull) {
            linesCleared++;
            
            // Shift all lines above down
            for (int ny = y; ny > 0; ny--) {
                memcpy(board->grid[ny], board->grid[ny - 1], sizeof(board->grid[0]));
            }
            
            // Clear top line
            memset(board->grid[0], BLOCK_EMPTY, sizeof(board->grid[0]));
            
            // Check the same line again since we moved everything down
            y++;
        }
    }
    
    return linesCleared;
}

void PrintBoard(TetrisBoard* board) {
    printf("Current Board:\n");
    for (int y = 0; y < BOARD_HEIGHT; y++) {
        for (int x = 0; x < BOARD_WIDTH; x++) {
            printf("%d ", board->grid[y][x]);
        }
        printf("\n");
    }
}

bool IsGameOver(TetrisBlock *block, TetrisBoard *board) {
    // Check if the current block is colliding with any placed blocks at spawn position
    for (int y = 0; y < 4; y++) {
        for (int x = 0; x < 4; x++) {
            if (block->shape[block->rotation][y][x]) {
                int boardX = block->x + x;
                int boardY = block->y + y;
                
                if (boardY < 0 || boardY >= BOARD_HEIGHT || 
                    boardX < 0 || boardX >= BOARD_WIDTH || 
                    board->grid[boardY][boardX] != BLOCK_EMPTY) {
                    return true;
                }
            }
        }
    }
    return false;
}

void FreeBoard(TetrisBoard* board) {
    // Free next_blocks circular list
    if (board->next_blocks != NULL) {
        NextBlockNode* current = board->next_blocks;
        NextBlockNode* start = current;
        
        do {
            NextBlockNode* next = current->next;
            free(current);
            current = next;
        } while (current != start);
    }
    
    // Free hold_history circular list
    if (board->hold_history != NULL) {
        HoldHistoryNode* current = board->hold_history;
        HoldHistoryNode* start = current;
        
        do {
            HoldHistoryNode* next = current->next;
            free(current);
            current = next;
        } while (current != start);
    }
}