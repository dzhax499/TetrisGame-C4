// main_menu.c
#include "main_menu.h"
#include <stdio.h>

#define WINDOW_WIDTH 800
#define WINDOW_HEIGHT 600
#define MENU_ITEMS 4

static const char* menuItems[] = {
    "Start Game",
    "High Scores",
    "Settings",
    "Exit"
};

bool initMainMenu(MainMenu* menu) {
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        printf("SDL could not initialize! SDL_Error: %s\n", SDL_GetError());
        return false;
    }

    if (TTF_Init() < 0) {
        printf("TTF could not initialize! TTF_Error: %s\n", TTF_GetError());
        return false;
    }

    menu->window = SDL_CreateWindow("Tetris",
        SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
        WINDOW_WIDTH, WINDOW_HEIGHT, SDL_WINDOW_SHOWN);
    
    if (!menu->window) {
        printf("Window could not be created! SDL_Error: %s\n", SDL_GetError());
        return false;
    }

    menu->renderer = SDL_CreateRenderer(menu->window, -1,
        SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    
    if (!menu->renderer) {
        printf("Renderer could not be created! SDL_Error: %s\n", SDL_GetError());
        return false;
    }

    menu->titleFont = TTF_OpenFont("assets/fonts/title.ttf", 64);
    menu->menuFont = TTF_OpenFont("assets/fonts/menu.ttf", 32);
    
    if (!menu->titleFont || !menu->menuFont) {
        printf("Failed to load font! TTF_Error: %s\n", TTF_GetError());
        return false;
    }

    menu->currentState = MENU_STATE_MAIN;
    menu->selectedItem = 0;
    menu->isRunning = true;

    return true;
}

void renderText(MainMenu* menu, TTF_Font* font, const char* text, int x, int y, SDL_Color color) {
    SDL_Surface* surface = TTF_RenderText_Blended(font, text, color);
    SDL_Texture* texture = SDL_CreateTextureFromSurface(menu->renderer, surface);
    
    SDL_Rect rect = {
        .x = x - surface->w / 2,
        .y = y,
        .w = surface->w,
        .h = surface->h
    };
    
    SDL_RenderCopy(menu->renderer, texture, NULL, &rect);
    
    SDL_FreeSurface(surface);
    SDL_DestroyTexture(texture);
}

void renderMainMenu(MainMenu* menu) {
    SDL_SetRenderDrawColor(menu->renderer, 0, 0, 0, 255);
    SDL_RenderClear(menu->renderer);

    // Render title
    SDL_Color titleColor = {255, 255, 255, 255};
    renderText(menu, menu->titleFont, "TETRIS", 
        WINDOW_WIDTH / 2, 100, titleColor);

    // Render menu items
    for (int i = 0; i < MENU_ITEMS; i++) {
        SDL_Color color = (i == menu->selectedItem) 
            ? (SDL_Color){255, 255, 0, 255}  // Selected item: Yellow
            : (SDL_Color){255, 255, 255, 255}; // Normal item: White
        
        renderText(menu, menu->menuFont, menuItems[i],
            WINDOW_WIDTH / 2, 250 + i * 60, color);
    }

    SDL_RenderPresent(menu->renderer);
}

void handleMainMenuInput(MainMenu* menu, SDL_Event* e) {
    if (e->type == SDL_KEYDOWN) {
        switch (e->key.keysym.sym) {
            case SDLK_UP:
                menu->selectedItem = (menu->selectedItem - 1 + MENU_ITEMS) % MENU_ITEMS;
                break;
            case SDLK_DOWN:
                menu->selectedItem = (menu->selectedItem + 1) % MENU_ITEMS;
                break;
            case SDLK_RETURN:
                switch (menu->selectedItem) {
                    case 0: // Start Game
                        menu->currentState = MENU_STATE_MODE_SELECT;
                        break;
                    case 1: // High Scores
                        menu->currentState = MENU_STATE_HIGH_SCORES;
                        break;
                    case 2: // Settings
                        // Implement settings menu
                        break;
                    case 3: // Exit
                        menu->currentState = MENU_STATE_EXIT;
                        menu->isRunning = false;
                        break;
                }
                break;
        }
    }
}

void cleanupMainMenu(MainMenu* menu) {
    TTF_CloseFont(menu->titleFont);
    TTF_CloseFont(menu->menuFont);
    SDL_DestroyRenderer(menu->renderer);
    SDL_DestroyWindow(menu->window);
    TTF_Quit();
    SDL_Quit();
}

MenuState runMainMenu(void) {
    MainMenu menu;
    if (!initMainMenu(&menu)) {
        return MENU_STATE_EXIT;
    }

    SDL_Event e;
    while (menu.isRunning) {
        while (SDL_PollEvent(&e)) {
            if (e.type == SDL_QUIT) {
                menu.isRunning = false;
                menu.currentState = MENU_STATE_EXIT;
                break;
            }
            handleMainMenuInput(&menu, &e);
        }
        renderMainMenu(&menu);
    }

    MenuState finalState = menu.currentState;
    cleanupMainMenu(&menu);
    return finalState;
}
