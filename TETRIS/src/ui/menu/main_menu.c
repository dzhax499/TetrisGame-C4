#include "main_menu.h"
#include "font.h"
#include <stdio.h>
#include <SDL3/SDL.h>

#define WINDOW_WIDTH 800
#define WINDOW_HEIGHT 600
#define MENU_ITEMS 2  // Reduced to just "Start Game" and "Exit"

static const char* menuItems[] = {
    "Start Game",
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
        SDL_Quit();
        return false;
    }

    menu->renderer = SDL_CreateRenderer(menu->window, NULL);
    if (!menu->renderer) {
        printf("Renderer could not be created! SDL_Error: %s\n", SDL_GetError());
        SDL_DestroyWindow(menu->window);
        SDL_Quit();
        return false;
    }

    // For simplicity, we'll skip actual font loading and just simulate it
    // In a real implementation, you would use proper font loading
    menu->titleFont.loaded = true;
    menu->menuFont.loaded = true;

    menu->currentState = MENU_STATE_MAIN;
    menu->selectedItem = 0;
    menu->isRunning = true;
    
    // Inisialisasi state untuk mendeteksi key press
    menu->upPressed = false;
    menu->downPressed = false;
    menu->enterPressed = false;
    menu->escapePressed = false;

    return true;
}

void renderText(MainMenu* menu, Font* font, const char* text, int x, int y, SDL_Color color) {
    // This is a simplified version without actual font rendering
    // In a real implementation, you would render text properly
    
    // For now, we'll just draw a colored rectangle to represent text
    int textWidth = 150;  // Simulated text width
    int textHeight = 30;  // Simulated text height
    
    SDL_FRect rect = {
        .x = x - textWidth / 2,
        .y = y,
        .w = textWidth,
        .h = textHeight
    };
    
    // Draw background for selected item
    if (color.r == 255 && color.g == 255 && color.b == 0) {
        // Selected item (yellow) gets a background
        SDL_SetRenderDrawColor(menu->renderer, 50, 50, 50, 255);
        SDL_RenderFillRect(menu->renderer, &rect);
    }
    
    // Draw border for all items
    SDL_SetRenderDrawColor(menu->renderer, color.r, color.g, color.b, color.a);
    SDL_RenderDrawRect(menu->renderer, &rect);
    
    // In a real implementation, you would render the text here
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

void handleMainMenuEvent(MainMenu* menu, SDL_Event* e) {
    if (e->type == SDL_EVENT_QUIT) {
        menu->isRunning = false;
        menu->currentState = MENU_STATE_EXIT;
    }
    
    // Mendeteksi key press dan release tanpa mengakses field spesifik event
    if (e->type == SDL_EVENT_KEY_DOWN) {
        // Berdasarkan scancode, update state
        int scancode = SDL_GetScancodeName(e->key.scancode);
        if (strcmp(scancode, "Up") == 0 || strcmp(scancode, "W") == 0) {
            menu->upPressed = true;
        }
        else if (strcmp(scancode, "Down") == 0 || strcmp(scancode, "S") == 0) {
            menu->downPressed = true;
        }
        else if (strcmp(scancode, "Return") == 0) {
            menu->enterPressed = true;
        }
        else if (strcmp(scancode, "Escape") == 0) {
            menu->escapePressed = true;
        }
    }
    else if (e->type == SDL_EVENT_KEY_UP) {
        // Reset state saat key dilepas
        int scancode = SDL_GetScancodeName(e->key.scancode);
        if (strcmp(scancode, "Up") == 0 || strcmp(scancode, "W") == 0) {
            menu->upPressed = false;
        }
        else if (strcmp(scancode, "Down") == 0 || strcmp(scancode, "S") == 0) {
            menu->downPressed = false;
        }
        else if (strcmp(scancode, "Return") == 0) {
            menu->enterPressed = false;
        }
        else if (strcmp(scancode, "Escape") == 0) {
            menu->escapePressed = false;
        }
    }
}

void handleMainMenuInput(MainMenu* menu) {
    // Process key states
    if (menu->upPressed) {
        menu->selectedItem = (menu->selectedItem - 1 + MENU_ITEMS) % MENU_ITEMS;
        menu->upPressed = false; // Reset to prevent multiple selections
    }
    
    if (menu->downPressed) {
        menu->selectedItem = (menu->selectedItem + 1) % MENU_ITEMS;
        menu->downPressed = false; // Reset to prevent multiple selections
    }
    
    if (menu->enterPressed) {
        if (menu->selectedItem == 0) {
            // Start Game selected
            printf("Starting game...\n");
            menu->currentState = MENU_STATE_GAME;
        } else if (menu->selectedItem == 1) {
            // Exit selected
            printf("Exiting...\n");
            menu->isRunning = false;
            menu->currentState = MENU_STATE_EXIT;
        }
        menu->enterPressed = false; // Reset to prevent multiple selections
    }
    
    if (menu->escapePressed) {
        menu->isRunning = false;
        menu->currentState = MENU_STATE_EXIT;
        menu->escapePressed = false; // Reset
    }
}

void cleanupMainMenu(MainMenu* menu) {
    // No need to free fonts in this simplified version
    // In a real implementation, you would call freeFont() here
    
    SDL_DestroyRenderer(menu->renderer);
    SDL_DestroyWindow(menu->window);
    SDL_Quit();
}

MenuState runMainMenu(void) {
    MainMenu menu;
    
    if (!initMainMenu(&menu)) {
        return MENU_STATE_EXIT;
    }
    
    SDL_Event e;
    while (menu.isRunning && menu.currentState == MENU_STATE_MAIN) {
        while (SDL_PollEvent(&e) != 0) {
            handleMainMenuEvent(&menu, &e);
        }
        
        // Process input based on current key states
        handleMainMenuInput(&menu);
        
        renderMainMenu(&menu);
        SDL_Delay(16);
    }
    
    MenuState finalState = menu.currentState;
    cleanupMainMenu(&menu);
    
    return finalState;
}