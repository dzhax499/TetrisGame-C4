// audio_manager.h
#ifndef AUDIO_MANAGER_H
#define AUDIO_MANAGER_H

#include <SDL3/SDL.h>

// Inisialisasi sistem audio
void InitAudio();

// Mengatur volume audio
void SetVolume(int volume);

// Mengaktifkan/mematikan suara
void ToggleMute();

// Membersihkan sistem audio saat keluar
void CleanupAudio();

#endif // AUDIO_MANAGER_H