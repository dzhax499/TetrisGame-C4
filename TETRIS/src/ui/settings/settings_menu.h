// setting.h
#ifndef SETTING_H
#define SETTING_H

#include <SDL2/SDL.h>

#define DEFAULT_VOLUME 50
#define DEFAULT_SPEED 5

typedef struct {
    int volume;          // Volume level (0-100)
    int speed;           // Game speed (1-10)
    SDL_Scancode left;   // Key for moving left
    SDL_Scancode right;  // Key for moving right
    SDL_Scancode rotate; // Key for rotating
    SDL_Scancode drop;   // Key for dropping
} Settings;

void initSettings(Settings *settings);
void loadSettings(Settings *settings, const char *filename);
void saveSettings(const Settings *settings, const char *filename);
void applySettings(const Settings *settings);

#endif // SETTING_H