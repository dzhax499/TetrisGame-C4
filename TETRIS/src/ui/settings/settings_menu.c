// settings_menu.c
#include "settings_menu.h"
#include "audio_manager.h"
#include "music.h"
#include "sfx.h"
#include <SDL3/SDL.h>
#include <SDL3/SDL_ttf.h>

// Menampilkan menu pengaturan dengan navigasi keyboard
void ShowSettingsMenu() {
    SDL_bool running = SDL_TRUE;
    int selectedOption = 0; // Menyimpan pilihan menu
    int volume = SDL_MIX_MAXVOLUME; // Volume awal

    while (running) {
        SDL_Event event;
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_EVENT_QUIT) {
                running = SDL_FALSE;
            } else if (event.type == SDL_EVENT_KEY_DOWN) {
                switch (event.key.keysym.sym) {
                    case SDLK_DOWN: 
                        selectedOption = (selectedOption + 1) % 3; // Navigasi ke bawah
                        break;
                    case SDLK_UP: 
                        selectedOption = (selectedOption - 1 + 3) % 3; // Navigasi ke atas
                        break;
                    case SDLK_RETURN:
                        if (selectedOption == 0) { // Mengatur volume
                            volume = (volume == SDL_MIX_MAXVOLUME) ? SDL_MIX_MAXVOLUME / 2 : SDL_MIX_MAXVOLUME;
                            SetVolume(volume);
                        } else if (selectedOption == 1) { // Mengaktifkan/mematikan mute
                            ToggleMute();
                        } else if (selectedOption == 2) { // Memainkan efek suara sebagai contoh
                            PlaySFX();
                        }
                        break;
                }
            }
        }
        SDL_Delay(16); // Simulasi rendering 60 FPS
    }
}