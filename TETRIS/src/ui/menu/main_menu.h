#ifndef MAIN_MENU_H
#define MAIN_MENU_H

//#include "C:/Users/Windows 11/Documents/GitHub/TetrisGame-C4/TETRIS/include/SDL3/SDL.h"
#include <stdbool.h>
#include "font.h"

typedef enum {
    MENU_STATE_MAIN,
    MENU_STATE_GAME,
    MENU_STATE_EXIT
} MenuState;

typedef struct {
    SDL_Window* window;
    SDL_Renderer* renderer;
    Font titleFont;
    Font menuFont;
    MenuState currentState;
    int selectedItem;
    bool isRunning;
    
    // Key state tracking
    bool upPressed;
    bool downPressed;
    bool enterPressed;
    bool escapePressed;
} MainMenu;

bool initMainMenu(MainMenu* menu);
void cleanupMainMenu(MainMenu* menu);
void handleMainMenuEvent(MainMenu* menu, SDL_Event* e);
void handleMainMenuInput(MainMenu* menu, SDL_Event* e);
void renderMainMenu(MainMenu* menu);
MenuState runMainMenu(void);

#endif // MAIN_MENU_H