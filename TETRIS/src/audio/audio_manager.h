/**
//audio_manager.h
//Definisi struktur dan fungsi untuk manajemen audio permainan
//Varian Abidarma Syuhada
 */

 #ifndef AUDIO_MANAGER_H
 #define AUDIO_MANAGER_H
 
 #include <SDL3/SDL.h>
 #include <SDL3/SDL_mixer.h>
 #include "settings_menu.h"
 
 /** 
  * @brief Struktur untuk mengelola elemen audio permainan 
  */
 typedef struct {
     Mix_Music* background_music;   /< Pointer musik latar belakang */
     Mix_Chunk** sound_effects;     /< Array chunk untuk efek suara */
     int num_sound_effects;         /< Jumlah efek suara */
     GameSettings* settings;        /< Pointer ke pengaturan permainan */
 } AudioManager;
 
 /**
  * @brief Inisialisasi sistem audio
  * @param settings Pointer ke pengaturan permainan
  * @return Pointer ke AudioManager yang baru diinisialisasi, atau NULL jika gagal
  */
 AudioManager* audio_manager_init(GameSettings* settings);
 
 /**
  * @brief Membebaskan sumber daya audio
  * @param audio_manager Pointer ke AudioManager yang akan dibebaskan
  */
 void audio_manager_free(AudioManager* audio_manager);
 
 /**
  * @brief Memuat musik latar belakang
  * @param audio_manager Pointer ke AudioManager
  * @param file_path Path file musik
  * @return SDL_TRUE jika berhasil, SDL_FALSE jika gagal
  */
 SDL_bool audio_manager_load_music(AudioManager* audio_manager, const char* file_path);
 
 /**
  * @brief Memuat efek suara
  * @param audio_manager Pointer ke AudioManager
  * @param file_path Path file efek suara
  * @return Indeks efek suara yang dimuat, atau -1 jika gagal
  */
 int audio_manager_load_sound_effect(AudioManager* audio_manager, const char* file_path);
 
 /**
  * @brief Memainkan musik latar belakang
  * @param audio_manager Pointer ke AudioManager
  * @param loops Jumlah pengulangan (-1 untuk loop tak terbatas)
  */
 void audio_manager_play_music(AudioManager* audio_manager, int loops);
 
 /**
  * @brief Memainkan efek suara
  * @param audio_manager Pointer ke AudioManager
  * @param sound_index Indeks efek suara yang akan dimainkan
  * @param loops Jumlah pengulangan
  */
 void audio_manager_play_sound_effect(AudioManager* audio_manager, int sound_index, int loops);
 
 /**
  * @brief Mengatur volume musik
  * @param audio_manager Pointer ke AudioManager
  * @param volume Volume baru (0.0 - 1.0)
  */
 void audio_manager_set_music_volume(AudioManager* audio_manager, float volume);
 
 /**
  * @brief Mengatur volume efek suara
  * @param audio_manager Pointer ke AudioManager
  * @param volume Volume baru (0.0 - 1.0)
  */
 void audio_manager_set_sfx_volume(AudioManager* audio_manager, float volume);
 
 /**
  * @brief Meng-mute atau meng-unmute musik
  * @param audio_manager Pointer ke AudioManager
  * @param mute SDL_TRUE untuk mute, SDL_FALSE untuk unmute
  */
 void audio_manager_mute_music(AudioManager* audio_manager, SDL_bool mute);
 
 /**
  * @brief Meng-mute atau meng-unmute efek suara
  * @param audio_manager Pointer ke AudioManager
  * @param mute SDL_TRUE untuk mute, SDL_FALSE untuk unmute
  */
 void audio_manager_mute_sfx(AudioManager* audio_manager, SDL_bool mute);
 
 #endif // AUDIO_MANAGER_H