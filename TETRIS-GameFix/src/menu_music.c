// File: menu_music.c
// Deskripsi: File ini berisi implementasi dari musik yang digunakan dalam menu game Tetris.
// Oleh: Ibnu Hilmi 241511079

#include "include/menu_music.h"
#include <stdlib.h>

// Variabel-variabel menu
static Sound menuSound;
static Music menuMusic;

void InitMenuMusic(void)
{
    menuMusic = LoadMusicStream("../assets/sounds/menu_select.wav");
    PlayMusicStream(menuMusic);
}

void UpdateMenuMusic(void)
{
    // Update music 
    UpdateMusicStream(menuMusic);
}

void UnloadMenuMusic(void)
{
    // Unload resources
    UnloadMusicStream(menuMusic);
}
