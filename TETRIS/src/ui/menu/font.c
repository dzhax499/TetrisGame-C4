#include "font.h"
#include <stdio.h>

bool loadFont(Font* font, const char* filename) {
    // This is a simplified implementation
    printf("Loading font: %s\n", filename);
    font->loaded = true;
    return true;
}

void freeFont(Font* font) {
    // This is a simplified implementation
    font->loaded = false;
}