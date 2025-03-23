// File: game_sound.c
// Deskripsi: Implementasi sistem suara dalam game Tetris
// Oleh: Abi

#include "include/game_sound.h"
#include <stdio.h>

// Variabel statis untuk menyimpan efek suara dan musik
static Sound soundEffects[SOUND_COUNT];
static Music musicTracks[MUSIC_COUNT];
static MusicTrackType currentMusicTrack = MUSIC_COUNT; // Tidak ada musik yang diputar saat awal
static bool isMusicEnabled = true;
static float musicVolume = 0.7f;
static float soundVolume = 1.0f;

void InitGameSound(void) {
    // Memuat efek suara
    soundEffects[SOUND_CLICK] = LoadSound("assets/sounds/menu_select.wav");
    soundEffects[SOUND_GAME_OVER] = LoadSound("assets/sounds/game_over.wav");
    soundEffects[SOUND_LINE_CLEAR] = LoadSound("assets/sounds/line_clear1.wav");
    
    // Memuat musik latar
    musicTracks[MUSIC_MENU] = LoadMusicStream("assets/sounds/game_menu1.mp3");
    musicTracks[MUSIC_GAMEPLAY] = LoadMusicStream("assets/sounds/game_menu2.mp3");
    
    // Mengatur volume awal
    SetMusicVolume1(musicVolume);
    SetSoundEffectsVolume(soundVolume);
    
    // Mengatur musik agar berulang (loop)
    for (int i = 0; i < MUSIC_COUNT; i++) {
        musicTracks[i].looping = true;
    }
}

void PlaySoundEffect(SoundEffectType soundType) {
    if (soundType < SOUND_COUNT) {
        // Memainkan efek suara dengan volume saat ini
        SetSoundVolume(soundEffects[soundType], soundVolume);
        PlaySound(soundEffects[soundType]);
    }
}

void PlayBackgroundMusic(MusicTrackType musicType) {
    // Menghentikan musik yang sedang diputar
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
    // Memperbarui aliran musik yang sedang dimainkan
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
    
    // Menerapkan volume ke musik yang sedang dimainkan
    if (currentMusicTrack < MUSIC_COUNT) {
        SetMusicVolume1(musicVolume);
    }
}

void SetSoundEffectsVolume(float volume) {
    soundVolume = volume;
    // Tidak perlu diterapkan langsung ke efek suara, akan digunakan saat dimainkan
}

bool IsBackgroundMusicPlaying(void) {
    if (currentMusicTrack < MUSIC_COUNT) {
        return IsMusicStreamPlaying(musicTracks[currentMusicTrack]);
    }
    return false;
}

void UnloadGameSound(void) {
    // Menghentikan musik yang sedang dimainkan terlebih dahulu
    StopBackgroundMusic();
    
    // Membebaskan memori efek suara
    for (int i = 0; i < SOUND_COUNT; i++) {
        UnloadSound(soundEffects[i]);
    }
    
    // Membebaskan memori trek musik
    for (int i = 0; i < MUSIC_COUNT; i++) {
        UnloadMusicStream(musicTracks[i]);
    }
}
