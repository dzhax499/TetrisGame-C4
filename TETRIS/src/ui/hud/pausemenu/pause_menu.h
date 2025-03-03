// pause_menu.h
#ifndef PAUSE_MENU_H
#define PAUSE_MENU_H

#include <SDL2/SDL.h>
#include <stdbool.h>

typedef struct {
    bool isPaused;
    SDL_Renderer *renderer;
    SDL_Texture *pauseTexture;
    SDL_Rect menuRect;
} PauseMenu;

void initPauseMenu(PauseMenu *menu, SDL_Renderer *renderer);
void handlePauseInput(PauseMenu *menu, SDL_Event *event);
void renderPauseMenu(PauseMenu *menu);
void destroyPauseMenu(PauseMenu *menu);

#endif // PAUSE_MENU_H
