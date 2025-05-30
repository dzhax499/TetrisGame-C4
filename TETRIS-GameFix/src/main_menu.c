#include "include/main_menu.h"
#include "include/game_sound.h"
#include <stdlib.h>

static MenuState currentState = MENU_STATE_MAIN;
static MenuButton* menuButtonsHead = NULL;
static Font menuFont;
static Texture2D backgroundTexture;

// Fungsi membuat tombol
MenuButton* CreateMenuButton(float x, float y, float width, float height, const char* text, Color color, Color hoverColor) {
    MenuButton* newButton = (MenuButton*)malloc(sizeof(MenuButton));
    if (!newButton) {
        TraceLog(LOG_ERROR, "Gagal mengalokasi tombol menu");
        return NULL;
    }

    newButton->rect = (Rectangle){ x, y, width, height };
    newButton->text = text;
    newButton->color = color;
    newButton->hoverColor = hoverColor;
    newButton->isHovered = false;
    newButton->next = NULL;
    return newButton;
}

// Fungsi menambahkan tombol ke linked list
void AddMenuButton(MenuButton* button) {
    if (!button) return;

    if (!menuButtonsHead) {
        menuButtonsHead = button;
    } else {
        MenuButton* current = menuButtonsHead;
        while (current->next) {
            current = current->next;
        }
        current->next = button;
    }
}

// Fungsi untuk membebaskan memori semua tombol
void FreeAllMenuButtons(void) {
    MenuButton* current = menuButtonsHead;
    while (current) {
        MenuButton* next = current->next;
        free(current);
        current = next;
    }
    menuButtonsHead = NULL;
}

void InitMainMenu(void) {
    ClearBackground(LIGHTGRAY);
    backgroundTexture = LoadTexture("assets/textures/bg.png");
    if (backgroundTexture.id == 0) {
        TraceLog(LOG_WARNING, "Background gagal dimuat");
    }

    menuFont = GetFontDefault();
    FreeAllMenuButtons();

    float buttonWidth = 300.0f;
    float buttonHeight = 50.0f;
    float startY = 350.0f;
    float spacing = 60.0f;
    float screenWidth = GetScreenWidth();

    AddMenuButton(CreateMenuButton(screenWidth / 2 - buttonWidth / 2, startY, buttonWidth, buttonHeight, "Play", GREEN, LIME));
    AddMenuButton(CreateMenuButton(screenWidth / 2 - buttonWidth / 2, startY + spacing * 1, buttonWidth, buttonHeight, "Credits", BLUE, DARKBLUE));
    AddMenuButton(CreateMenuButton(screenWidth / 2 - buttonWidth / 2, startY + spacing * 2, buttonWidth, buttonHeight, "Highscore", YELLOW, GOLD));
    AddMenuButton(CreateMenuButton(screenWidth / 2 - buttonWidth / 2, startY + spacing * 3, buttonWidth, buttonHeight, "Exit", RED, MAROON));
    AddMenuButton(CreateMenuButton(GetScreenWidth() - 200, 20, 180, 40, "Leaderboard", PURPLE, DARKPURPLE));
}

void UpdateMainMenu(void) {
    Vector2 mousePoint = GetMousePosition();
    MenuButton* current = menuButtonsHead;
    int index = 0;

    while (current) {
        bool wasHovered = current->isHovered;
        current->isHovered = CheckCollisionPointRec(mousePoint, current->rect);

        if (!wasHovered && current->isHovered) {
#ifdef SOUND_HOVER
            PlaySoundEffect(SOUND_HOVER);
#else
            PlaySoundEffect(SOUND_CLICK);
#endif
        }

        if (current->isHovered && IsMouseButtonReleased(MOUSE_LEFT_BUTTON)) {
            PlaySoundEffect(SOUND_CLICK);

            switch (index) {
                case 0: currentState = MENU_STATE_PLAY; break;
                case 1: currentState = MENU_STATE_CREDITS; break;
                case 2: currentState = MENU_STATE_HIGHSCORE; break;
                case 3: currentState = MENU_STATE_EXIT; break;
                case 4: currentState = MENU_STATE_LEADERBOARD; break;
            }
        }

        current = current->next;
        index++;
    }

    if (IsKeyPressed(KEY_M)) ToggleBackgroundMusic();

    if (IsKeyPressed(KEY_ESCAPE)) {
        if (currentState == MENU_STATE_HIGHSCORE || currentState == MENU_STATE_CREDITS || currentState == MENU_STATE_LEADERBOARD) {
            currentState = MENU_STATE_MAIN;
        } else if (currentState == MENU_STATE_PLAY) {
            currentState = MENU_STATE_PAUSE;
        } else if (currentState == MENU_STATE_PAUSE) {
            currentState = MENU_STATE_PLAY;
        }
    }
}

void DrawMainMenu(void) {
    DrawTextureEx(backgroundTexture, (Vector2){ 0, 0 }, 0.0f, 1.0f, WHITE);

    const char* titleText = "TETRIS";
    int titleSize = 120;
    int textWidth = MeasureText(titleText, titleSize);
    int posX = GetScreenWidth() / 2 - textWidth / 2;
    int posY = 120;

    // Outline
    for (int dx = -2; dx <= 2; dx += 2) {
        for (int dy = -2; dy <= 2; dy += 2) {
            DrawText(titleText, posX + dx, posY + dy, titleSize, BLACK);
        }
    }
    DrawText(titleText, posX, posY, titleSize, WHITE);

    // Gambar tombol
    MenuButton* current = menuButtonsHead;
    int index = 0;
    while (current) {
        DrawRectangleRec(current->rect, current->isHovered ? current->hoverColor : current->color);

        Color borderColor;
        switch (index) {
            case 0: borderColor = LIME; break;
            case 1: borderColor = DARKBLUE; break;
            case 2: borderColor = GOLD; break;
            case 3: borderColor = MAROON; break;
            case 4: borderColor = DARKPURPLE; break;
            default: borderColor = WHITE;
        }

        DrawRectangleLinesEx(current->rect, 2, borderColor);

        int textWidth = MeasureText(current->text, 20);
        int textX = current->rect.x + current->rect.width / 2 - textWidth / 2;
        int textY = current->rect.y + current->rect.height / 2 - 10;
        DrawText(current->text, textX, textY, 20, WHITE);

        current = current->next;
        index++;
    }

    DrawText("Press M to toggle music", 10, GetScreenHeight() - 735, 20, RAYWHITE);
    DrawText("(c) 2025 D'Okeh Studio", 10, GetScreenHeight() - 760, 20, RAYWHITE);
}

void UnloadMainMenu(void) {
    UnloadTexture(backgroundTexture);
    FreeAllMenuButtons();
}

MenuState GetCurrentMenuState(void) {
    return currentState;
}

void SetMenuState(MenuState newState) {
    currentState = newState;
}
