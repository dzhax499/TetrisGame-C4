
// Nama file : main_menu.c
// Deskripsi : File ini berisi implementasi dari menu utama game Tetris, termasuk inisialisasi elemen menu, pembaruan interaksi pengguna, dan proses menggambar tampilan menu.
// Oleh      : Ibnu Hilmi 241511079

#include "include/main_menu.h"
#include <stdlib.h>

#define MAX_BUTTONS 4

// Variabel-variabel menu
static MenuState currentState = MENU_STATE_MAIN;
static MenuButton buttons[MAX_BUTTONS];
static Font menuFont;
static Texture2D backgroundTexture;
// static Sound menuSound;
// static Music menuMusic;

void InitMainMenu(void)
{
    // Inisialisasi background
    backgroundTexture = LoadTexture("bg.png");

    // Inisialisasi font
    menuFont = GetFontDefault();
    
    // Inisialisasi tombol
    float buttonWidth = 200.0f;
    float buttonHeight = 50.0f;
    float startY = 200.0f;
    float spacing = 60.0f;
    float screenWidth = GetScreenWidth();
    
    // Tombol Play
    buttons[0] = (MenuButton){
        .rect = (Rectangle){ screenWidth/2 - buttonWidth/2, startY, buttonWidth, buttonHeight },
        .text = "Play",
        .color = GREEN,
        .hoverColor = LIME,
        .isHovered = false
    };
    
    // Tombol Settings
    buttons[1] = (MenuButton){
        .rect = (Rectangle){ screenWidth/2 - buttonWidth/2, startY + spacing, buttonWidth, buttonHeight },
        .text = "Audio Settings",
        .color = YELLOW,
        .hoverColor = GOLD,
        .isHovered = false
    };
    
    // Tombol Credits
    buttons[2] = (MenuButton){
        .rect = (Rectangle){ screenWidth/2 - buttonWidth/2, startY + spacing*2, buttonWidth, buttonHeight },
        .text = "Credits",
        .color = BLUE,
        .hoverColor = DARKBLUE,
        .isHovered = false
    };
    
    // Tombol Exit
    buttons[3] = (MenuButton){
        .rect = (Rectangle){ screenWidth/2 - buttonWidth/2, startY + spacing*3, buttonWidth, buttonHeight },
        .text = "Exit",
        .color = RED,
        .hoverColor = MAROON,
        .isHovered = false
    };
    
    // Load resources 
    // menuSound = LoadSound("resources/click.wav");
    // menuMusic = LoadMusicStream("mainnmenumusic.mp3");
    // PlayMusicStream(menuMusic);
}

void UpdateMainMenu(void)
{
    Vector2 mousePoint = GetMousePosition();
    
    // Update music (uncomment jika menggunakan music)
    // UpdateMusicStream(menuMusic);
    
    for (int i = 0; i < MAX_BUTTONS; i++)
    {
        // Check mouse hover
        buttons[i].isHovered = CheckCollisionPointRec(mousePoint, buttons[i].rect);
        
        // Check mouse click
        if (buttons[i].isHovered && IsMouseButtonReleased(MOUSE_LEFT_BUTTON))
        {
            // Play sound (uncomment jika menggunakan sound)
            // PlaySound(menuSound);
            
            switch (i)
            {
                case 0: currentState = MENU_STATE_PLAY; break;
                case 1: currentState = MENU_STATE_OPTIONS; break;
                case 2: currentState = MENU_STATE_CREDITS; break;
                case 3: currentState = MENU_STATE_EXIT; break;
            }
        }
    }
}

void DrawMainMenu(void)
{
    // Draw background
    DrawTexture(backgroundTexture, 0, 0, WHITE);
    
    // Draw title
    
    int titleSize = 60; // Larger font size
    const char* titleText = "TETRIS";
    int textWidth = MeasureText(titleText, titleSize);
    int posX = GetScreenWidth()/2 - textWidth/2;
    int posY = 60;

    // Draw the outline by drawing the text multiple times with offsets
    DrawText(titleText, posX-2, posY-2, titleSize, BLACK); // top-left
    DrawText(titleText, posX+2, posY-2, titleSize, BLACK); // top-right
    DrawText(titleText, posX-2, posY+2, titleSize, BLACK); // bottom-left
    DrawText(titleText, posX+2, posY+2, titleSize, BLACK); // bottom-right
    DrawText(titleText, posX-2, posY, titleSize, BLACK);   // left
    DrawText(titleText, posX+2, posY, titleSize, BLACK);   // right
    DrawText(titleText, posX, posY-2, titleSize, BLACK);   // top
    DrawText(titleText, posX, posY+2, titleSize, BLACK);   // bottom

    // Draw the main text in white on top
    DrawText(titleText, posX, posY, titleSize, WHITE);
        
    // Draw buttons
    for (int i = 0; i < MAX_BUTTONS; i++)
    {
        DrawRectangleRec(buttons[i].rect, buttons[i].isHovered ? buttons[i].hoverColor : buttons[i].color);
        DrawRectangleLinesEx(buttons[i].rect, 2, BLACK);
        
        // Center text in button
        int textWidth = MeasureText(buttons[i].text, 20);
        int textX = buttons[i].rect.x + buttons[i].rect.width/2 - textWidth/2;
        int textY = buttons[i].rect.y + buttons[i].rect.height/2 - 10;
        
        DrawText(buttons[i].text, textX, textY, 20, WHITE);
    }
    
    // Draw footer
    DrawText("(c) 2025 D'Okeh Studio", 10, GetScreenHeight() - 20, 10, GRAY);
}


void UnloadMainMenu(void)
{
    // Unload resources (uncomment jika menggunakan resources)
    UnloadTexture(backgroundTexture);
    // UnloadSound(menuSound);
    // UnloadMusicStream(menuMusic);
}

MenuState GetCurrentMenuState(void)
{
    return currentState;
}