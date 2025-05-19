// Nama file : main_menu.h
// Deskripsi : File header yang mendeklarasikan fungsi dan struktur data yang digunakan dalam sistem menu game Tetris, termasuk definisi tombol menu dan status menu.
// Oleh      : Ibnu Hilmi 241511079 cek

#ifndef MAIN_MENU_H
#define MAIN_MENU_H

#include <raylib.h>

// Enum untuk status menu
typedef enum {
    MENU_STATE_MAIN,
    MENU_STATE_PLAY,
    MENU_STATE_HIGHSCORE,
    MENU_STATE_CREDITS,
    MENU_STATE_EXIT,
    MENU_STATE_PAUSE
} MenuState;

// Forward declaration of the struct
typedef struct MenuButton MenuButton;

// Struktur untuk tombol menu
struct MenuButton {
    Rectangle rect;
    const char *text;
    Color color;
    Color hoverColor;
    bool isHovered;
    MenuButton *next; // Pointer ke tombol berikutnya
};

// Fungsi-fungsi menu
void InitMainMenu(void);
void UpdateMainMenu(void);
void DrawMainMenu(void);
void UnloadMainMenu(void);
MenuState GetCurrentMenuState(void);
void SetMenuState(MenuState newState);

// Fungsi untuk membuat dan mengelola tombol menu
MenuButton* CreateMenuButton(float x, float y, float width, float height, const char* text, Color color, Color hoverColor);
void AddMenuButton(MenuButton* button);
void FreeAllMenuButtons(void);

#endif // MAIN_MENU_H