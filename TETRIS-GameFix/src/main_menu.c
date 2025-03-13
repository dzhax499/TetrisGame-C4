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
static Sound menuSound;
static Music menuMusic;

void InitMainMenu(void)
{
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
        .color = DARKBLUE,
        .hoverColor = BLUE,
        .isHovered = false
    };
    
    // Tombol Settings
    buttons[1] = (MenuButton){
        .rect = (Rectangle){ screenWidth/2 - buttonWidth/2, startY + spacing, buttonWidth, buttonHeight },
        .text = "Settings",
        .color = DARKBLUE,
        .hoverColor = BLUE,
        .isHovered = false
    };
    
    // Tombol Credits
    buttons[2] = (MenuButton){
        .rect = (Rectangle){ screenWidth/2 - buttonWidth/2, startY + spacing*2, buttonWidth, buttonHeight },
        .text = "Credits",
        .color = DARKBLUE,
        .hoverColor = BLUE,
        .isHovered = false
    };
    
    // Tombol Exit
    buttons[3] = (MenuButton){
        .rect = (Rectangle){ screenWidth/2 - buttonWidth/2, startY + spacing*3, buttonWidth, buttonHeight },
        .text = "Exit",
        .color = DARKBLUE,
        .hoverColor = BLUE,
        .isHovered = false
    };
    
    // Load resources (uncomment jika diperlukan)
    // backgroundTexture = LoadTexture("resources/background.png");
    // menuSound = LoadSound("resources/click.wav");
    // menuMusic = LoadMusicStream("resources/menu_music.mp3");
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
    ClearBackground(RAYWHITE);
    
    // Draw background (uncomment jika menggunakan background)
    // DrawTexture(backgroundTexture, 0, 0, WHITE);
    
    // Draw title
    DrawText("TETRIS", GetScreenWidth()/2 - MeasureText("TETRIS", 40)/2, 100, 40, BLACK);
    
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
    DrawText("(c) 2025 Your Game Studio", 10, GetScreenHeight() - 20, 10, GRAY);
}

void UnloadMainMenu(void)
{
    // Unload resources (uncomment jika menggunakan resources)
    // UnloadTexture(backgroundTexture);
    // UnloadSound(menuSound);
    // UnloadMusicStream(menuMusic);
}

MenuState GetCurrentMenuState(void)
{
    return currentState;
}