// main_menu.h
#ifndef MAIN_MENU_H
#define MAIN_MENU_H

#include <SDL3/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <stdbool.h>

typedef enum {
    MENU_STATE_MAIN,
    MENU_STATE_MODE_SELECT,
    MENU_STATE_HIGH_SCORES,
    MENU_STATE_EXIT
} MenuState;

typedef struct {
    SDL_Window* window;
    SDL_Renderer* renderer;
    TTF_Font* titleFont;
    TTF_Font* menuFont;
    MenuState currentState;
    int selectedItem;
    bool isRunning;
} MainMenu;

bool initMainMenu(MainMenu* menu);
void cleanupMainMenu(MainMenu* menu);
void handleMainMenuInput(MainMenu* menu, SDL_Event* e);
void renderMainMenu(MainMenu* menu);
MenuState runMainMenu(void);

#endif // MAIN_MENU_H