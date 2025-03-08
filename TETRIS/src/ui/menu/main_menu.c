#include "main_menu.h"
#include "font.h"
#include <stdio.h>
#include <SDL3/SDL.h>

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

    menu->window = SDL_CreateWindow("Tetris", WINDOW_WIDTH, WINDOW_HEIGHT, SDL_WINDOW_RESIZABLE);
    if (!menu->window) {
        printf("Window could not be created! SDL_Error: %s\n", SDL_GetError());
        return false;
    }

    menu->renderer = SDL_CreateRenderer(menu->window);
    menu->renderer = SDL_CreateRenderer(menu->window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    if (!menu->renderer) {
        printf("Renderer could not be created! SDL_Error: %s\n", SDL_GetError());
        return false;
    }

    // Load fonts using custom font handling
    if (!loadFont(&menu->titleFont, "assets/fonts/title.ttf") || !loadFont(&menu->menuFont, "assets/fonts/menu.ttf")) {
        printf("Failed to load font!\n");
        return false;
    }

    menu->currentState = MENU_STATE_MAIN;
    menu->selectedItem = 0;
    menu->isRunning = true;

    return true;
}

void renderText(MainMenu* menu, Font* font, const char* text, int x, int y, SDL_Color color) {
    // Implement rendering text using your custom font handling
    // This is a placeholder implementation
    SDL_Surface* surface = SDL_CreateRGBSurfaceWithFormat(0, 100, 50, 32, SDL_PIXELFORMAT_RGBA32);
    SDL_Texture* texture = SDL_CreateTextureFromSurface(menu->renderer, surface);
    
    SDL_Rect rect = {
        .x = x - surface->w / 2,
        .y = y,
        .w = surface->w,
        .h = surface->h
    };
    
    SDL_RenderTexture(menu->renderer, texture, NULL, &rect);
    
    SDL_DestroySurface(surface);
    SDL_DestroyTexture(texture);
}

void renderMainMenu(MainMenu* menu) {
    SDL_SetRenderDrawColor(menu->renderer, 0, 0, 0, 255);
    SDL_RenderClear(menu->renderer);

    // Render title
    SDL_Color titleColor = {255, 255, 255, 255};
    renderText(menu, &menu->titleFont, "TETRIS", WINDOW_WIDTH / 2, 100, titleColor);

    // Render menu items
    for (int i = 0; i < MENU_ITEMS; i++) {
        SDL_Color color = (i == menu->selectedItem) 
            ? (SDL_Color){255, 255, 0, 255}  // Selected item: Yellow
            : (SDL_Color){255, 255, 255, 255}; // Normal item: White
        
        renderText(menu, &menu->menuFont, menuItems[i], WINDOW_WIDTH / 2, 250 + i * 60, color);
    }

    SDL_RenderPresent(menu->renderer);
}

void handleMainMenuInput(MainMenu* menu, SDL_Event* e) {
    if (e->type == SDL_EVENT_KEY_DOWN) {
        switch (e->key.keysym) {
            case SDLK_UP:
                menu->selectedItem = (menu->selectedItem - 1 + MENU_ITEMS) % MENU_ITEMS;
                break;
            case SDLK_DOWN:
                menu->selectedItem = (menu->selectedItem + 1) % MENU_ITEMS;
                break;
            case SDLK_RETURN:
                // Handle menu item selection
                break;
            default:
                break;
        }
    }
}

void cleanupMainMenu(MainMenu* menu) {
    freeFont(&menu->titleFont);
    freeFont(&menu->menuFont);
    SDL_DestroyRenderer(menu->renderer);
    SDL_DestroyWindow(menu->window);
    SDL_Quit();
}
