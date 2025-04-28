// Nama file : main_menu.c
// Deskripsi : File ini berisi implementasi dari menu utama game Tetris
// Oleh      : Ibnu Hilmi 241511079

#include "include/main_menu.h"
#include "include/game_sound.h"
#include <stdlib.h>

#define MAX_BUTTONS 4

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
    if (backgroundTexture.id == 0)
    {
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
        .rect = (Rectangle){screenWidth / 2 - buttonWidth / 2, startY, buttonWidth, buttonHeight},
        .text = "Play",
        .color = GREEN,
        .hoverColor = LIME,
        .isHovered = false};

    // Tombol Credits - switched positions with Highscore
    buttons[1] = (MenuButton){
        .rect = (Rectangle){screenWidth / 2 - buttonWidth / 2, startY + spacing * 1, buttonWidth, buttonHeight},
        .text = "Credits",
        .color = BLUE,
        .hoverColor = DARKBLUE,
        .isHovered = false};

    // Tombol Highscore - switched positions with Credits
    buttons[2] = (MenuButton){
        .rect = (Rectangle){screenWidth / 2 - buttonWidth / 2, startY + spacing * 2, buttonWidth, buttonHeight},
        .text = "Highscore",
        .color = YELLOW,
        .hoverColor = GOLD,
        .isHovered = false};

    // Tombol Exit
    buttons[3] = (MenuButton){
        .rect = (Rectangle){screenWidth / 2 - buttonWidth / 2, startY + spacing * 3, buttonWidth, buttonHeight},
        .text = "Exit",
        .color = RED,
        .hoverColor = MAROON,
        .isHovered = false};
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
        if (!wasHovered && buttons[i].isHovered)
        {
// Use SOUND_CLICK if SOUND_HOVER is not defined
#ifdef SOUND_HOVER
            PlaySoundEffect(SOUND_HOVER);
#else
            PlaySoundEffect(SOUND_CLICK);
#endif
        }

        // Check mouse click
        if (buttons[i].isHovered && IsMouseButtonReleased(MOUSE_LEFT_BUTTON))
        {
            // Play click sound
            PlaySoundEffect(SOUND_CLICK);

            // Fixed: Map button labels to appropriate states
            switch (i)
            {
            case 0:
                currentState = MENU_STATE_PLAY;
                break;
            case 1:
                currentState = MENU_STATE_CREDITS;
                break; // Credits is button 1
            case 2:
                currentState = MENU_STATE_HIGHSCORE;
                break; // Highscore is button 2
            case 3:
                currentState = MENU_STATE_EXIT;
                break;
            }
        }
    }

    // Toggle music with M key
    if (IsKeyPressed(KEY_M))
    {
        ToggleBackgroundMusic();
    }

    // Allow ESC key to return to main menu from other states
    if (IsKeyPressed(KEY_ESCAPE))
    {
        if (currentState == MENU_STATE_HIGHSCORE || currentState == MENU_STATE_CREDITS)
        {
            // For these states, return to main menu
            currentState = MENU_STATE_MAIN;
        }
        else if (currentState == MENU_STATE_PLAY)
        {
            // For the play state, switch to pause state
            currentState = MENU_STATE_PAUSE;
        }
        else if (currentState == MENU_STATE_PAUSE)
        {
            // For the pause state, return to play state
            currentState = MENU_STATE_PLAY;
        }
    }
}

void DrawMainMenu(void)
{
    // Draw background
    DrawTextureEx(backgroundTexture, (Vector2){0, 0}, 0.0f, 1.0f, WHITE);

    // Draw title
    int titleSize = 120;
    const char *titleText = "TETRIS";
    int textWidth = MeasureText(titleText, titleSize);
    int posX = GetScreenWidth() / 2 - textWidth / 2;
    int posY = 120;

    // Draw the outline by drawing the text multiple times with offsets
    DrawText(titleText, posX - 2, posY - 2, titleSize, BLACK);
    DrawText(titleText, posX + 2, posY - 2, titleSize, BLACK);
    DrawText(titleText, posX - 2, posY + 2, titleSize, BLACK);
    DrawText(titleText, posX + 2, posY + 2, titleSize, BLACK);
    DrawText(titleText, posX - 2, posY, titleSize, BLACK);
    DrawText(titleText, posX + 2, posY, titleSize, BLACK);
    DrawText(titleText, posX, posY - 2, titleSize, BLACK);
    DrawText(titleText, posX, posY + 2, titleSize, BLACK);

    // Draw the main text in white on top
    DrawText(titleText, posX, posY, titleSize, WHITE);

    // Draw buttons
    for (int i = 0; i < MAX_BUTTONS; i++)
    {
        DrawRectangleRec(buttons[i].rect, buttons[i].isHovered ? buttons[i].hoverColor : buttons[i].color);

        // Fixed: Match border colors to actual button colors
        Color borderColor;
        switch (i)
        {
        case 0:
            borderColor = LIME;
            break;
        case 1:
            borderColor = DARKBLUE;
            break;
        case 2:
            borderColor = GOLD;
            break;
        case 3:
            borderColor = MAROON;
            break;
        default:
            borderColor = WHITE;
        }
        DrawRectangleLinesEx(buttons[i].rect, 2, borderColor);

        // Center text in button
        int textWidth = MeasureText(buttons[i].text, 20);
        int textX = buttons[i].rect.x + buttons[i].rect.width / 2 - textWidth / 2;
        int textY = buttons[i].rect.y + buttons[i].rect.height / 2 - 10;

        DrawText(buttons[i].text, textX, textY, 20, WHITE);
    }

    // Add audio control hint
    DrawText("Press M to toggle music", 10, GetScreenHeight() - 735, 20, RAYWHITE);

    // Draw footer
    DrawText("(c) 2025 D'Okeh Studio", 10, GetScreenHeight() - 760, 20, RAYWHITE);
}

void UnloadMainMenu(void)
{
    UnloadTexture(backgroundTexture);
}

MenuState GetCurrentMenuState(void)
{
    return currentState;
}

// Added: New function to set the menu state externally
void SetMenuState(MenuState newState)
{
    currentState = newState;
}