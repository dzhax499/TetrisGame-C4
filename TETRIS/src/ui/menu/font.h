#ifndef FONT_H
#define FONT_H

typedef struct {
    // Tambahkan properti yang diperlukan untuk font Anda
    void* fontData;
} Font;

bool loadFont(Font* font, const char* filePath);
void freeFont(Font* font);

#endif // FONT_H
