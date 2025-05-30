// File: game_sound.c
// Deskripsi: Implementasi sistem suara dalam game Tetris dengan Linked List
// Oleh: Abi (Dimodifikasi)

#include "include/game_sound.h"
#include "include/sound_node.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Kepala linked list untuk efek suara dan musik
static SoundNode* soundHead = NULL;
static MusicNode* musicHead = NULL;
static MusicNode* currentMusic = NULL; // Musik yang sedang diputar
static bool isMusicEnabled = true;
static float globalMusicVolume = 0.7f;
static float globalSoundVolume = 1.0f;

// Forward declaration untuk menghindari implicit declaration error
void SetGameMusicVolume(float volume);

// Fungsi utilitas untuk mencari node suara berdasarkan tipe
static SoundNode* FindSoundNode(SoundEffectType type) {
    SoundNode* current = soundHead;
    while (current != NULL) {
        if (current->type == (int)type) {  // Cast untuk menghindari warning
            return current;
        }
        current = current->next;
    }
    return NULL;
}

// Fungsi utilitas untuk mencari node musik berdasarkan tipe
static MusicNode* FindMusicNode(MusicTrackType type) {
    MusicNode* current = musicHead;
    while (current != NULL) {
        if (current->type == (int)type) {  // Cast untuk menghindari warning
            return current;
        }
        current = current->next;
    }
    return NULL;
}

void InitGameSound(void) {
    // Membebaskan sumber daya yang ada jika sudah diinisialisasi sebelumnya
    UnloadGameSound();
    
    // Inisialisasi dengan efek suara default
    AddSoundEffect("assets/sounds/menu_select.wav", SOUND_CLICK, "Menu Click");
    AddSoundEffect("assets/sounds/game_over.wav", SOUND_GAME_OVER, "Game Over");
    AddSoundEffect("assets/sounds/line_clear1.wav", SOUND_LINE_CLEAR, "Line Clear");
    
    // Inisialisasi dengan musik default
    AddMusicTrack("assets/sounds/game_menu1.mp3", MUSIC_MENU, "Menu Music", true);
    AddMusicTrack("assets/sounds/game_menu2.mp3", MUSIC_GAMEPLAY, "Gameplay Music", true);
    
    // Mengatur volume awal - gunakan nama fungsi yang benar dari header
    SetGameMusicVolume(globalMusicVolume);
    SetSoundEffectsVolume(globalSoundVolume);
}

bool AddSoundEffect(const char* filepath, SoundEffectType type, const char* name) {
    // Periksa apakah tipe yang sama sudah ada
    SoundNode* existingNode = FindSoundNode(type);
    if (existingNode != NULL) {
        // Hapus yang sudah ada jika mau diperbarui
        RemoveSoundEffect(type);
    }
    
    // Alokasi memori untuk node baru
    SoundNode* newNode = (SoundNode*)malloc(sizeof(SoundNode));
    if (newNode == NULL) {
        return false; // Gagal alokasi memori
    }
    
    // Alokasi memori untuk nama
    newNode->name = (char*)malloc(strlen(name) + 1);
    if (newNode->name == NULL) {
        free(newNode);
        return false; // Gagal alokasi memori
    }
    
    // Muat suara dari file
    newNode->sound = LoadSound(filepath);
    newNode->type = (int)type;  // Cast ke int sesuai dengan definisi struct
    strcpy(newNode->name, name);
    newNode->volume = globalSoundVolume;
    newNode->next = NULL;
    
    // Tambahkan ke linked list
    if (soundHead == NULL) {
        soundHead = newNode;
    } else {
        // Tambahkan ke akhir list
        SoundNode* current = soundHead;
        while (current->next != NULL) {
            current = current->next;
        }
        current->next = newNode;
    }
    
    return true;
}

bool RemoveSoundEffect(SoundEffectType type) {
    if (soundHead == NULL) {
        return false; // List kosong
    }
    
    SoundNode* toRemove = NULL;
    
    // Jika node yang akan dihapus adalah head
    if (soundHead->type == (int)type) {  // Cast untuk konsistensi
        toRemove = soundHead;
        soundHead = soundHead->next;
    } else {
        // Cari node yang akan dihapus
        SoundNode* current = soundHead;
        while (current->next != NULL && current->next->type != (int)type) {
            current = current->next;
        }
        
        if (current->next == NULL) {
            return false; // Node tidak ditemukan
        }
        
        toRemove = current->next;
        current->next = toRemove->next;
    }
    
    // Bebaskan sumber daya
    UnloadSound(toRemove->sound);
    free(toRemove->name);
    free(toRemove);
    
    return true;
}

void PlaySoundEffect(SoundEffectType soundType) {
    SoundNode* node = FindSoundNode(soundType);
    if (node != NULL) {
        // Sesuaikan volume dan mainkan
        SetSoundVolume(node->sound, node->volume);
        PlaySound(node->sound);
    }
}

bool AddMusicTrack(const char* filepath, MusicTrackType type, const char* name, bool looping) {
    // Periksa apakah tipe yang sama sudah ada
    MusicNode* existingNode = FindMusicNode(type);
    if (existingNode != NULL) {
        // Hapus yang sudah ada jika mau diperbarui
        RemoveMusicTrack(type);
    }
    
    // Alokasi memori untuk node baru
    MusicNode* newNode = (MusicNode*)malloc(sizeof(MusicNode));
    if (newNode == NULL) {
        return false; // Gagal alokasi memori
    }
    
    // Alokasi memori untuk nama
    newNode->name = (char*)malloc(strlen(name) + 1);
    if (newNode->name == NULL) {
        free(newNode);
        return false; // Gagal alokasi memori
    }
    
    // Muat musik dari file
    newNode->music = LoadMusicStream(filepath);
    newNode->type = (int)type;  // Cast ke int sesuai dengan definisi struct
    strcpy(newNode->name, name);
    newNode->volume = globalMusicVolume;
    newNode->looping = looping;
    newNode->next = NULL;
    
    // Setel musik untuk berulang jika diminta
    newNode->music.looping = looping;
    
    // Tambahkan ke linked list
    if (musicHead == NULL) {
        musicHead = newNode;
    } else {
        // Tambahkan ke akhir list
        MusicNode* current = musicHead;
        while (current->next != NULL) {
            current = current->next;
        }
        current->next = newNode;
    }
    
    return true;
}

bool RemoveMusicTrack(MusicTrackType type) {
    if (musicHead == NULL) {
        return false; // List kosong
    }
    
    // Jika musik yang akan dihapus sedang diputar, hentikan dulu
    if (currentMusic != NULL && currentMusic->type == (int)type) {
        StopMusicStream(currentMusic->music);
        currentMusic = NULL;
    }
    
    MusicNode* toRemove = NULL;
    
    // Jika node yang akan dihapus adalah head
    if (musicHead->type == (int)type) {
        toRemove = musicHead;
        musicHead = musicHead->next;
    } else {
        // Cari node yang akan dihapus
        MusicNode* current = musicHead;
        while (current->next != NULL && current->next->type != (int)type) {
            current = current->next;
        }
        
        if (current->next == NULL) {
            return false; // Node tidak ditemukan
        }
        
        toRemove = current->next;
        current->next = toRemove->next;
    }
    
    // Bebaskan sumber daya
    UnloadMusicStream(toRemove->music);
    free(toRemove->name);
    free(toRemove);
    
    return true;
}

void PlayBackgroundMusic(MusicTrackType musicType) {
    // Hentikan musik yang sedang diputar
    StopBackgroundMusic();
    
    // Cari musik yang diminta
    MusicNode* node = FindMusicNode(musicType);
    
    if (node != NULL && isMusicEnabled) {
        // Menggunakan fungsi raylib untuk mengatur volume musik
        SetMusicVolume(node->music, node->volume);
        PlayMusicStream(node->music);
        currentMusic = node;
    }
}

void StopBackgroundMusic(void) {
    if (currentMusic != NULL) {
        StopMusicStream(currentMusic->music);
        currentMusic = NULL;
    }
}

void UpdateGameSound(void) {
    // Perbarui aliran musik yang sedang diputar
    if (currentMusic != NULL && isMusicEnabled) {
        UpdateMusicStream(currentMusic->music);
    }
}

void ToggleBackgroundMusic(void) {
    isMusicEnabled = !isMusicEnabled;
    
    if (currentMusic != NULL) {
        if (isMusicEnabled) {
            ResumeMusicStream(currentMusic->music);
        } else {
            PauseMusicStream(currentMusic->music);
        }
    }
}

// Implementasi fungsi yang sesuai dengan nama di header file
void SetGameMusicVolume(float volume) {
    globalMusicVolume = volume;
    
    // Perbarui volume semua trek musik
    MusicNode* current = musicHead;
    while (current != NULL) {
        current->volume = volume;
        if (current == currentMusic) {
            // Menggunakan fungsi raylib untuk mengatur volume musik
            SetMusicVolume(current->music, volume);
        }
        current = current->next;
    }
}

void SetSoundEffectsVolume(float volume) {
    globalSoundVolume = volume;
    
    // Perbarui volume semua efek suara
    SoundNode* current = soundHead;
    while (current != NULL) {
        current->volume = volume;
        current = current->next;
    }
}

bool IsBackgroundMusicPlaying(void) {
    if (currentMusic != NULL) {
        return IsMusicStreamPlaying(currentMusic->music);
    }
    return false;
}

void UnloadGameSound(void) {
    // Hentikan musik yang sedang diputar
    StopBackgroundMusic();
    
    // Bebaskan semua node efek suara
    SoundNode* soundCurrent = soundHead;
    while (soundCurrent != NULL) {
        SoundNode* temp = soundCurrent;
        soundCurrent = soundCurrent->next;
        
        UnloadSound(temp->sound);
        free(temp->name);
        free(temp);
    }
    soundHead = NULL;
    
    // Bebaskan semua node musik
    MusicNode* musicCurrent = musicHead;
    while (musicCurrent != NULL) {
        MusicNode* temp = musicCurrent;
        musicCurrent = musicCurrent->next;
        
        UnloadMusicStream(temp->music);
        free(temp->name);
        free(temp);
    }
    musicHead = NULL;
    currentMusic = NULL;
}