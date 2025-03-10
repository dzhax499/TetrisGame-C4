#include <stdio.h>
#include "main_menu.h"

//main.c//
int main(int argc, char* argv[]) {
    MainMenu menu;
    
    // Initialize main menu
    if (!initMainMenu(&menu)) {
        printf("Failed to initialize main menu!\n");
        return -1;
    }
    
    // Main game loop
    SDL_Event e;
    while (menu.isRunning) {
        // Handle events on queue
        while (SDL_PollEvent(&e) != 0) {
            if (e.type == SDL_EVENT_QUIT) {
                menu.isRunning = false;
            }
            void handleMainMenuInput(MainMenu* menu, SDL_Event* e);
        }
        
        // Render current menu state
        renderMainMenu(&menu);
        
        // Small delay to prevent maxing out CPU
        SDL_Delay(16); // ~60 FPS
    }
    
    // Clean up resources
    cleanupMainMenu(&menu);
    
    return 0;
}
