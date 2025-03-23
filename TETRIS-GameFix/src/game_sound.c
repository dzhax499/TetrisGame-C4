// File: game_sound.c
// Deskripsi: Implementasi sistem suara dalam game Tetris
// Oleh: [Your Name]

#include "include/game_sound.h"
#include <stdio.h>

// Static variables for sounds and music
static Sound soundEffects[SOUND_COUNT];
static Music musicTracks[MUSIC_COUNT];
static MusicTrackType currentMusicTrack = MUSIC_COUNT; // Nothing playing initially
static bool isMusicEnabled = true;
static float musicVolume = 0.7f;
static float soundVolume = 1.0f;

void InitGameSound(void) {
    // Load sound effects
    soundEffects[SOUND_CLICK] = LoadSound("assets/sounds/menu_select.wav");
    soundEffects[SOUND_GAME_OVER] = LoadSound("assets/sounds/game_over.wav");
    
    // Load music
    musicTracks[MUSIC_MENU] = LoadMusicStream("assets/sounds/game_music.mp3");
    musicTracks[MUSIC_GAMEPLAY] = LoadMusicStream("assets/sounds/game_music.mp3");
    
    // Set initial volumes
    SetMusicVolume1(musicVolume);
    SetSoundEffectsVolume(soundVolume);
    
    // Set music to loop
    for (int i = 0; i < MUSIC_COUNT; i++) {
        musicTracks[i].looping = true;
    }
}

void PlaySoundEffect(SoundEffectType soundType) {
    if (soundType < SOUND_COUNT) {
        // Play the sound with the current volume
        SetSoundVolume(soundEffects[soundType], soundVolume);
        PlaySound(soundEffects[soundType]);
    }
}

void PlayBackgroundMusic(MusicTrackType musicType) {
    // Stop any currently playing music
    StopBackgroundMusic();
    
    if (musicType < MUSIC_COUNT && isMusicEnabled) {
        PlayMusicStream(musicTracks[musicType]);
        SetMusicVolume1(musicVolume);
        currentMusicTrack = musicType;
    }
}

void StopBackgroundMusic(void) {
    if (currentMusicTrack < MUSIC_COUNT) {
        StopMusicStream(musicTracks[currentMusicTrack]);
        currentMusicTrack = MUSIC_COUNT;
    }
}

void UpdateGameSound(void) {
    // Update the currently playing music stream
    if (currentMusicTrack < MUSIC_COUNT && isMusicEnabled) {
        UpdateMusicStream(musicTracks[currentMusicTrack]);
    }
}

void ToggleBackgroundMusic(void) {
    isMusicEnabled = !isMusicEnabled;
    
    if (currentMusicTrack < MUSIC_COUNT) {
        if (isMusicEnabled) {
            ResumeMusicStream(musicTracks[currentMusicTrack]);
        } else {
            PauseMusicStream(musicTracks[currentMusicTrack]);
        }
    }
}

void SetMusicVolume1(float volume) {
    musicVolume = volume;
    
    // Apply to currently playing music
    if (currentMusicTrack < MUSIC_COUNT) {
        SetMusicVolume1(musicVolume);
    }
}

void SetSoundEffectsVolume(float volume) {
    soundVolume = volume;
    // No need to apply to sounds - it will be applied when played
}

bool IsBackgroundMusicPlaying(void) {
    if (currentMusicTrack < MUSIC_COUNT) {
        return IsMusicStreamPlaying(musicTracks[currentMusicTrack]);
    }
    return false;
}

void UnloadGameSound(void) {
    // Stop any playing music first
    StopBackgroundMusic();
    
    // Unload sound effects
    for (int i = 0; i < SOUND_COUNT; i++) {
        UnloadSound(soundEffects[i]);
    }
    
    // Unload music tracks
    for (int i = 0; i < MUSIC_COUNT; i++) {
        UnloadMusicStream(musicTracks[i]);
    }
}