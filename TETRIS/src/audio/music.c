/**
//music.c
//Implementasi manajemen musik permainan
//Varian Abidarma Syuhada
 */

 #include <SDL3/SDL.h>
 #include <SDL3/SDL_mixer.h>
 #include "music.h"
 #include "../settings/settings.h"
 
 /**
  * @brief Inisialisasi manajer musik
  * @param settings Pointer ke pengaturan permainan
  * @return Pointer ke MusicManager yang baru diinisialisasi
  */
 MusicManager* music_manager_init(GameSettings* settings) {
     // Periksa parameter input
     if (!settings) {
         SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, 
                      "Gagal inisialisasi: Pengaturan NULL!");
         return NULL;
     }
     
     // Alokasikan memori untuk MusicManager
     MusicManager* music_manager = SDL_malloc(sizeof(MusicManager));
     if (!music_manager) {
         SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, 
                      "Gagal mengalokasikan memori untuk music manager!");
         return NULL;
     }
     
     // Inisialisasi semua track musik menjadi NULL
     for (int i = 0; i < MUSIC_COUNT; i++) {
         music_manager->tracks[i] = NULL;
     }
     
     // Atur track musik saat ini menjadi tidak ada
     music_manager->current_track = -1;
     
     // Simpan pointer ke pengaturan
     music_manager->settings = settings;
     
     return music_manager;
 }
 
 /**
  * @brief Membebaskan sumber daya manajer musik
  * @param music_manager Pointer ke MusicManager
  */
 void music_manager_free(MusicManager* music_manager) {
     // Periksa input
     if (!music_manager) return;
     
     // Bebaskan semua track musik
     for (int i = 0; i < MUSIC_COUNT; i++) {
         if (music_manager->tracks[i]) {
             Mix_FreeMusic(music_manager->tracks[i]);
         }
     }
     
     // Bebaskan struktur music manager
     SDL_free(music_manager);
 }
 
 /**
  * @brief Memuat musik dari file
  * @param music_manager Pointer ke MusicManager
  * @param music_type Tipe musik yang akan dimuat
  * @param file_path Path file musik
  * @return SDL_TRUE jika berhasil, SDL_FALSE jika gagal
  */
 SDL_bool music_manager_load(MusicManager* music_manager, 
                              MusicType music_type, 
                              const char* file_path) {
     // Periksa parameter input
     if (!music_manager || music_type < 0 || music_type >= MUSIC_COUNT || !file_path) {
         SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, 
                      "Parameter tidak valid untuk memuat musik!");
         return SDL_FALSE;
     }
     
     // Bebaskan track musik sebelumnya jika ada
     if (music_manager->tracks[music_type]) {
         Mix_FreeMusic(music_manager->tracks[music_type]);
     }
     
     // Muat musik baru
     music_manager->tracks[music_type] = Mix_LoadMUS(file_path);
     if (!music_manager->tracks[music_type]) {
         SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, 
                      "Gagal memuat musik %d: %s", 
                      music_type, Mix_GetError());
         return SDL_FALSE;
     }
     
     return SDL_TRUE;
 }
 
 /**
  * @brief Memainkan musik
  * @param music_manager Pointer ke MusicManager
  * @param music_type Tipe musik yang akan dimainkan
  * @param loops Jumlah pengulangan (-1 untuk loop tak terbatas)
  */
 void music_manager_play(MusicManager* music_manager, 
                         MusicType music_type, 
                         int loops) {
     // Periksa parameter input
     if (!music_manager || music_type < 0 || music_type >= MUSIC_COUNT) {
         SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, 
                      "Parameter tidak valid untuk memainkan musik!");
         return;
     }
     
     // Periksa apakah musik di-mute
     if (music_manager->settings->music_mute) return;
     
     // Periksa apakah track musik tersedia
     if (!music_manager->tracks[music_type]) {
         SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, 
                      "Track musik %d belum dimuat!", music_type);
         return;
     }
     
     // Mainkan musik
     if (Mix_PlayMusic(music_manager->tracks[music_type], loops) == -1) {
         SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, 
                      "Gagal memainkan musik: %s", Mix_GetError());
         return;
     }
     
     // Atur track musik saat ini
     music_manager->current_track = music_type;
 }
 
 /**
  * @brief Berhenti memainkan musik
  * @param music_manager Pointer ke MusicManager
  */
 void music_manager_stop(MusicManager* music_manager) {
     // Periksa parameter input
     if (!music_manager) return;
     
     // Hentikan musik
     Mix_HaltMusic();
     
     // Reset track musik saat ini
     music_manager->current_track = -1;
 }
 
 /**
  * @brief Menjeda musik
  * @param music_manager Pointer ke MusicManager
  */
 void music_manager_pause(MusicManager* music_manager) {
     // Periksa parameter input
     if (!music_manager) return;
     
     // Jeda musik
     Mix_PauseMusic();
 }
 
 /**
  * @brief Melanjutkan musik yang dijeda
  * @param music_manager Pointer ke MusicManager
  */
 void music_manager_resume(MusicManager* music_manager) {
     // Periksa parameter input
     if (!music_manager) return;
     
     // Periksa apakah musik di-mute
     if (music_manager->settings->music_mute) return;
     
     // Lanjutkan musik
     Mix_ResumeMusic();
 }
 
 /**
  * @brief Mengatur volume musik
  * @param music_manager Pointer ke MusicManager
  * @param volume Volume baru (0.0 - 1.0)
  */
 void music_manager_set_volume(MusicManager* music_manager, float volume) {
     // Periksa parameter input
     if (!music_manager) return;
     
     // Konversi volume ke skala SDL_mixer (0-128)
     int sdl_volume = (int)(volume * MIX_MAX_VOLUME * 
                      music_manager->settings->master_volume);
     
     // Atur volume musik
     Mix_VolumeMusic(sdl_volume);
 }