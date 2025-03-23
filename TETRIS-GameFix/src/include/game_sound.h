// File: game_sound.h
// Deskripsi: Header file untuk mengatur semua suara dalam game Tetris
// Oleh: Abi

#ifndef GAME_SOUND_H
#define GAME_SOUND_H

#include "raylib.h"
#include <stdbool.h>

// ID untuk efek suara
typedef enum {
    SOUND_CLICK,       // Efek suara saat tombol ditekan
    SOUND_GAME_OVER,   // Efek suara saat permainan berakhir
    SOUND_LINE_CLEAR,   // Efek suara saat baris terhapus
    SOUND_COUNT        // Jumlah total efek suara
} SoundEffectType;

// ID untuk trek musik
typedef enum {
    MUSIC_MENU,       // Musik latar untuk menu
    MUSIC_GAMEPLAY,   // Musik latar saat permainan berlangsung
    MUSIC_COUNT       // Jumlah total trek musik
} MusicTrackType;

// Inisialisasi semua sumber daya suara
void InitGameSound(void);

// Memainkan efek suara tertentu
void PlaySoundEffect(SoundEffectType soundType);

// Memainkan musik latar untuk konteks yang ditentukan
void PlayBackgroundMusic(MusicTrackType musicType);

// Menghentikan semua musik yang sedang diputar
void StopBackgroundMusic(void);

// Memperbarui aliran musik (harus dipanggil setiap frame)
void UpdateGameSound(void);

// Menjeda/melanjutkan musik latar yang sedang diputar
void ToggleBackgroundMusic(void);

// Mengatur volume musik dan efek suara (0.0 hingga 1.0)
void SetMusicVolume1(float volume);
void SetSoundEffectsVolume(float volume);

// Memeriksa apakah ada musik yang sedang diputar
bool IsBackgroundMusicPlaying(void);

// Membebaskan semua sumber daya suara
void UnloadGameSound(void);

#endif // GAME_SOUND_H
