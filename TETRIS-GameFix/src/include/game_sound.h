// File: game_sound.h
// Deskripsi: Header file untuk mengatur semua suara dalam game Tetris
// Oleh: [Your Name]

#ifndef GAME_SOUND_H
#define GAME_SOUND_H

#include "raylib.h"
#include <stdbool.h>

// Sound effect IDs
typedef enum {
    SOUND_CLICK,
    SOUND_GAME_OVER,
    SOUND_COUNT  // Total number of sound effects
} SoundEffectType;

// Music track IDs
typedef enum {
    MUSIC_MENU,
    MUSIC_GAMEPLAY,
    MUSIC_COUNT  // Total number of music tracks
} MusicTrackType;

// Initialize all sound resources
void InitGameSound(void);

// Play a specific sound effect
void PlaySoundEffect(SoundEffectType soundType);

// Play background music for the specified context
void PlayBackgroundMusic(MusicTrackType musicType);

// Stop all currently playing music
void StopBackgroundMusic(void);

// Update music streams (must be called each frame)
void UpdateGameSound(void);

// Pause/unpause the currently playing background music
void ToggleBackgroundMusic(void);

// Set music and sound effects volume (0.0 to 1.0)
void SetMusicVolume1(float volume);
void SetSoundEffectsVolume(float volume);

// Check if any music is currently playing
bool IsBackgroundMusicPlaying(void);

// Free all sound resources
void UnloadGameSound(void);

#endif // GAME_SOUND_H