// setting.c
#include "settings_menu.h"
#include <stdio.h>
#include <stdlib.h>

void initSettings(Settings *settings) {
    settings->volume = DEFAULT_VOLUME;
    settings->speed = DEFAULT_SPEED;
    settings->left = SDL_SCANCODE_LEFT;
    settings->right = SDL_SCANCODE_RIGHT;
    settings->rotate = SDL_SCANCODE_UP;
    settings->drop = SDL_SCANCODE_DOWN;
}

void loadSettings(Settings *settings, const char *filename) {
    FILE *file = fopen(filename, "r");
    if (!file) {
        initSettings(settings);
        return;
    }
    fscanf(file, "%d %d %d %d %d %d",
        &settings->volume,
        &settings->speed,
        (int*)&settings->left,
        (int*)&settings->right,
        (int*)&settings->rotate,
        (int*)&settings->drop);
    fclose(file);
}

void saveSettings(const Settings *settings, const char *filename) {
    FILE *file = fopen(filename, "w");
    if (!file) return;
    fprintf(file, "%d %d %d %d %d %d\n",
            settings->volume,
            settings->speed,
            settings->left,
            settings->right,
            settings->rotate,
            settings->drop);
    fclose(file);
}

void applySettings(const Settings *settings) {
    // Placeholder function to apply settings like volume, speed, etc.
}

