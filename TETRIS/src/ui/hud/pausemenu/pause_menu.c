/*// pause_menu.c
#include "pause_menu.h"

void initPauseMenu(PauseMenu *menu, SDL_Renderer *renderer) {
    menu->isPaused = false;
    menu->renderer = renderer;
    
    // Create a surface for pause menu (placeholder color)
    SDL_Surface *surface = SDL_CreateRGBSurfaceWithFormat(0, 300, 200, 32, SDL_PIXELFORMAT_RGBA32);
    SDL_FillRect(surface, NULL, SDL_MapRGBA(surface->format, 0, 0, 0, 150));
    menu->pauseTexture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_FreeSurface(surface);
    
    // Set menu position
    menu->menuRect.x = 100;
    menu->menuRect.y = 100;
    menu->menuRect.w = 300;
    menu->menuRect.h = 200;
}

void handlePauseInput(PauseMenu *menu, SDL_Event *event) {
    if (event->type == SDL_KEYDOWN) {
        if (event->key.keysym.scancode == SDL_SCANCODE_ESCAPE) {
            menu->isPaused = !menu->isPaused;
        }
    }
}

void renderPauseMenu(PauseMenu *menu) {
    if (menu->isPaused) {
        SDL_RenderCopy(menu->renderer, menu->pauseTexture, NULL, &menu->menuRect);
    }
}

void destroyPauseMenu(PauseMenu *menu) {
    SDL_DestroyTexture(menu->pauseTexture);
}
*/