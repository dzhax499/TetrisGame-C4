// File: click_sound.c
// Deskripsi: File ini berisi implementasi dari musik yang digunakan jika klik dijalankan.
// Oleh: Ibnu Hilmi 241511079

#include "include/click_sound.h"
#include <stdlib.h>

// Variabel-variabel menu
static Sound menuSound;

void InitClickSound(void)
{
    menuSound = LoadSound("../assets/sounds/sound.mp3");
}

void UpdateClickSound(void)
{
    // Update sound
    PlaySound(menuSound);
}

void UnloadClickSound(void)
{
    // Unload resources
    UnloadSound(menuSound);
}
