// File: game_sound.h
// Deskripsi: Header file untuk mengatur semua suara dalam game Tetris dengan Linked List
// Oleh: Abi (Dimodifikasi)

#ifndef GAME_SOUND_H
#define GAME_SOUND_H

#include "raylib.h"
#include <stdbool.h>
#include "sound_node.h"

// ID untuk efek suara
typedef enum {
    SOUND_CLICK,        // Efek suara saat tombol ditekan
    SOUND_GAME_OVER,    // Efek suara saat permainan berakhir
    SOUND_LINE_CLEAR,   // Efek suara saat baris terhapus
    SOUND_COUNT         // Jumlah total efek suara (untuk kompatibilitas)
} SoundEffectType;

// ID untuk trek musik
typedef enum {
    MUSIC_MENU,         // Musik latar untuk menu
    MUSIC_GAMEPLAY,     // Musik latar saat permainan berlangsung
    MUSIC_COUNT         // Jumlah total trek musik (untuk kompatibilitas)
} MusicTrackType;

// HAPUS REDEFINISI STRUCT - sudah didefinisikan di sound_node.h
// Struct SoundNode dan MusicNode sudah tersedia dari sound_node.h

// Inisialisasi sistem suara dengan linked list
void InitGameSound(void);

// Memainkan efek suara berdasarkan tipe
void PlaySoundEffect(SoundEffectType soundType);

// Menambahkan efek suara baru ke linked list
bool AddSoundEffect(const char* filepath, SoundEffectType type, const char* name);

// Menghapus efek suara dari linked list
bool RemoveSoundEffect(SoundEffectType type);

// Memainkan musik latar berdasarkan tipe
void PlayBackgroundMusic(MusicTrackType musicType);

// Menambahkan trek musik baru ke linked list
bool AddMusicTrack(const char* filepath, MusicTrackType type, const char* name, bool looping);

// Menghapus trek musik dari linked list
bool RemoveMusicTrack(MusicTrackType type);

// Menghentikan semua musik yang sedang diputar
void StopBackgroundMusic(void);

// Memperbarui aliran musik (harus dipanggil setiap frame)
void UpdateGameSound(void);

// Menjeda/melanjutkan musik latar yang sedang diputar
void ToggleBackgroundMusic(void);

// Mengatur volume musik dan efek suara (0.0 hingga 1.0)
// RENAME function untuk menghindari konflik dengan raylib
void SetGameMusicVolume(float volume);
void SetSoundEffectsVolume(float volume);

// Memeriksa apakah ada musik yang sedang diputar
bool IsBackgroundMusicPlaying(void);

// Membebaskan semua sumber daya suara
void UnloadGameSound(void);

#endif // GAME_SOUND_H