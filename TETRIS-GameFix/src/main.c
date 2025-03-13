// Nama file : main.c
// Deskripsi : File utama program game Tetris dengan manajemen state yang lebih baik
// Oleh      : Dzakit Tsabit 241511071 (diperbarui (ibunss))

#include "raylib.h"
#include "include/main_menu.h"

#define SCREEN_WIDTH 800
#define SCREEN_HEIGHT 600
#define SCREEN_TITLE "Tetris Game"

// Game states
typedef enum {
    GAME_STATE_MENU,
    GAME_STATE_GAMEPLAY,
    GAME_STATE_OPTIONS,
    GAME_STATE_CREDITS,
    GAME_STATE_EXIT
} GameState;

// Fungsi untuk menginisialisasi game
static void InitGame(void)
{
    // Inisialisasi window
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, SCREEN_TITLE);
    SetTargetFPS(60);
    
    // Inisialisasi audio
    InitAudioDevice();
    
    // Inisialisasi menu
    InitMainMenu();
}

// Fungsi untuk membersihkan sumber daya game
static void CleanupGame(void)
{
    // Bersihkan sumber daya menu
    UnloadMainMenu();
    
    // Tutup audio device
    CloseAudioDevice();
    
    // Tutup window
    CloseWindow();
}

// Fungsi untuk menangani logika gameplay
static void UpdateGameplay(void)
{
    // TODO: Implementasi logika gameplay Tetris
    
    // Sementara: kembali ke menu dengan tombol ESC
    if (IsKeyPressed(KEY_ESCAPE))
    {
        // Reset ke menu utama
        InitMainMenu();
    }
}

// Fungsi untuk menggambar layar gameplay
static void DrawGameplay(void)
{
    ClearBackground(SKYBLUE);
    DrawText("GAMEPLAY SCREEN", 
             SCREEN_WIDTH/2 - MeasureText("GAMEPLAY SCREEN", 40)/2, 
             SCREEN_HEIGHT/2 - 40, 
             40, WHITE);
    DrawText("Press ESC to return to menu", 
             SCREEN_WIDTH/2 - MeasureText("Press ESC to return to menu", 20)/2, 
             SCREEN_HEIGHT/2 + 20, 
             20, WHITE);
}

// Fungsi untuk menggambar layar pengaturan
static void DrawOptions(void)
{
    ClearBackground(BEIGE);
    DrawText("OPTIONS SCREEN", 
             SCREEN_WIDTH/2 - MeasureText("OPTIONS SCREEN", 40)/2, 
             SCREEN_HEIGHT/2 - 40, 
             40, BLACK);
    DrawText("Press ESC to return to menu", 
             SCREEN_WIDTH/2 - MeasureText("Press ESC to return to menu", 20)/2, 
             SCREEN_HEIGHT/2 + 20, 
             20, BLACK);
}

// Fungsi untuk menggambar layar kredit
static void DrawCredits(void)
{
    ClearBackground(LIGHTGRAY);
    DrawText("CREDITS SCREEN", 
             SCREEN_WIDTH/2 - MeasureText("CREDITS SCREEN", 40)/2, 
             SCREEN_HEIGHT/2 - 40, 
             40, BLACK);
    DrawText("Press ESC to return to menu", 
             SCREEN_WIDTH/2 - MeasureText("Press ESC to return to menu", 20)/2, 
             SCREEN_HEIGHT/2 + 20, 
             20, BLACK);
}

int main(void)
{
    // Inisialisasi game
    InitGame();
    
    // Game state
    GameState currentGameState = GAME_STATE_MENU;
    
    // Game loop
    while (!WindowShouldClose())
    {
        // Update
        switch (currentGameState)
        {
            case GAME_STATE_MENU:
            {
                UpdateMainMenu();
                MenuState menuState = GetCurrentMenuState();
                
                // Handle menu state changes
                switch (menuState)
                {
                    case MENU_STATE_PLAY:
                        currentGameState = GAME_STATE_GAMEPLAY;
                        break;
                    case MENU_STATE_OPTIONS:
                        currentGameState = GAME_STATE_OPTIONS;
                        break;
                    case MENU_STATE_CREDITS:
                        currentGameState = GAME_STATE_CREDITS;
                        break;
                    case MENU_STATE_EXIT:
                        currentGameState = GAME_STATE_EXIT;
                        break;
                    default:
                        break;
                }
                break;
            }
                
            case GAME_STATE_GAMEPLAY:
                UpdateGameplay();
                break;
                
            case GAME_STATE_OPTIONS:
                // Kembali ke menu jika ESC ditekan
                if (IsKeyPressed(KEY_ESCAPE))
                {
                    currentGameState = GAME_STATE_MENU;
                    InitMainMenu();
                }
                break;
                
            case GAME_STATE_CREDITS:
                // Kembali ke menu jika ESC ditekan
                if (IsKeyPressed(KEY_ESCAPE))
                {
                    currentGameState = GAME_STATE_MENU;
                    InitMainMenu();
                }
                break;
                
            case GAME_STATE_EXIT:
                break;
        }
        
        // Draw
        BeginDrawing();
        
        switch (currentGameState)
        {
            case GAME_STATE_MENU:
                DrawMainMenu();
                break;
                
            case GAME_STATE_GAMEPLAY:
                DrawGameplay();
                break;
                
            case GAME_STATE_OPTIONS:
                DrawOptions();
                break;
                
            case GAME_STATE_CREDITS:
                DrawCredits();
                break;
                
            case GAME_STATE_EXIT:
                break;
        }
        
        EndDrawing();
        
        // Keluar dari loop jika diperlukan
        if (currentGameState == GAME_STATE_EXIT)
        {
            break;
        }
    }
    
    // Bersihkan sumber daya
    CleanupGame();
    
    return 0;
}