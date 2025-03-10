#ifndef FONT_H
#define FONT_H

#include <stdbool.h>

// Simplified font structure for the example
typedef struct {
    bool loaded;
    // In a real implementation, you would have more fields here
} Font;

bool loadFont(Font* font, const char* filename);
void freeFont(Font* font);

#endif // FONT_H