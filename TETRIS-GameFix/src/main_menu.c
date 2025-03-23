// Nama file : main_menu.c
// Deskripsi : File ini berisi implementasi dari menu utama game Tetris, termasuk inisialisasi elemen menu, pembaruan interaksi pengguna, dan proses menggambar tampilan menu.
// Oleh      : Ibnu Hilmi 241511079

#include "include/main_menu.h"
#include "include/game_sound.h" // Add this include
#include <stdlib.h>

#define MAX_BUTTONS 5

// Variabel-variabel menu
static MenuState currentState = MENU_STATE_MAIN;
static MenuButton buttons[MAX_BUTTONS];
static Font menuFont;
static Texture2D backgroundTexture;

void InitMainMenu(void)
{
    // Inisialisasi background
    ClearBackground(LIGHTGRAY);
    backgroundTexture = LoadTexture("assets/textures/bg.png");
    if (backgroundTexture.id == 0) {
        TraceLog(LOG_WARNING, "Background texture could not be loaded");
    }

    // Inisialisasi font
    menuFont = GetFontDefault();
    
    // Inisialisasi tombol
    float buttonWidth = 300.0f;
    float buttonHeight = 50.0f;
    float startY = 350.0f;
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
        .isHovered = true
    };
    
    // Tombol Exit
    buttons[3] = (MenuButton){
        .rect = (Rectangle){ screenWidth/2 - buttonWidth/2, startY + spacing*3, buttonWidth, buttonHeight },
        .text = "Exit",
        .color = RED,
        .hoverColor = MAROON,
        .isHovered = false
    };

    // Tombol Highscore
    buttons[4] = (MenuButton){
        .rect = (Rectangle){ 860, 10, 150, 50 },
        .text = "Highscore",
        .color = PURPLE,
        .hoverColor = DARKPURPLE,
        .isHovered = false
    };
}

void UpdateMainMenu(void)
{
    Vector2 mousePoint = GetMousePosition();
    
    for (int i = 0; i < MAX_BUTTONS; i++)
    {
        // Check mouse hover
        bool wasHovered = buttons[i].isHovered;
        buttons[i].isHovered = CheckCollisionPointRec(mousePoint, buttons[i].rect);
        
        // Play hover sound if we just started hovering
        if (!wasHovered && buttons[i].isHovered) {
            PlaySoundEffect(SOUND_CLICK);
        }
        
        // Check mouse click
        if (buttons[i].isHovered && IsMouseButtonReleased(MOUSE_LEFT_BUTTON))
        {
            // Play click sound
            PlaySoundEffect(SOUND_CLICK);
            
            switch (i)
            {
                case 0: currentState = MENU_STATE_PLAY; break;
                case 1: currentState = MENU_STATE_OPTIONS; break;
                case 2: currentState = MENU_STATE_CREDITS; break;
                case 3: currentState = MENU_STATE_EXIT; break;
                case 4: currentState = MENU_STATE_HIGHSCORE; break;
            }
        }
    }
    
    // Toggle music with M key
    if (IsKeyPressed(KEY_M)) {
        ToggleBackgroundMusic();
    }
}

void DrawMainMenu(void)
{
    // Draw background
    DrawTextureEx(backgroundTexture, (Vector2){0, 0}, 0.0f, 1.0f, WHITE);
    
    // Draw title
    
    int titleSize = 120; // Larger font size
    const char* titleText = "TETRIS";
    int textWidth = MeasureText(titleText, titleSize);
    int posX = GetScreenWidth()/2 - textWidth/2;
    int posY = 120;

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

        DrawRectangleLinesEx(buttons[0].rect, 2, LIME);
        DrawRectangleLinesEx(buttons[1].rect, 2, GOLD);
        DrawRectangleLinesEx(buttons[2].rect, 2, DARKBLUE);
        DrawRectangleLinesEx(buttons[3].rect, 2, MAROON);
        DrawRectangleLinesEx(buttons[4].rect, 2, DARKPURPLE);

        // Center text in button
        int textWidth = MeasureText(buttons[i].text, 20);
        int textX = buttons[i].rect.x + buttons[i].rect.width/2 - textWidth/2;
        int textY = buttons[i].rect.y + buttons[i].rect.height/2 - 10;
        
        DrawText(buttons[i].text, textX, textY, 20, WHITE);
    }
    
    // Add audio control hint
    DrawText("Press M to toggle music", 10, GetScreenHeight() - 50, 20, RAYWHITE);
    
    // Draw footer
    DrawText("(c) 2025 D'Okeh Studio", 10, GetScreenHeight() - 25, 20, RAYWHITE);
}


void UnloadMainMenu(void)
{
    UnloadTexture(backgroundTexture);
}

MenuState GetCurrentMenuState(void)
{
    return currentState;
}