// audio_manager.c
#include "audio_manager.h"
#include <SDL3/SDL_audio.h>
#include <stdio.h>

// Variabel statis untuk menyimpan status audio
static int volume = SDL_MIX_MAXVOLUME; // Menyimpan volume saat ini
static int isMuted = 0; // Status mute (0: tidak mute, 1: mute)

// Inisialisasi sistem audio SDL3
void InitAudio() {
    if (SDL_Init(SDL_INIT_AUDIO) < 0) {
        printf("SDL Audio init failed: %s\n", SDL_GetError());
    }
}

// Mengatur volume dengan mempertimbangkan mute
void SetVolume(int vol) {
    volume = vol;
    if (isMuted) {
        volume = 0; // Jika mute aktif, volume diatur ke 0
    }
}

// Mengaktifkan/mematikan suara
void ToggleMute() {
    isMuted = !isMuted; // Toggle status mute
    SetVolume(volume); // Perbarui volume sesuai dengan status mute
}

// Membersihkan sistem audio sebelum keluar
void CleanupAudio() {
    SDL_QuitSubSystem(SDL_INIT_AUDIO);
}