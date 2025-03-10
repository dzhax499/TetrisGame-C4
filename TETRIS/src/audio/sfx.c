/**
//sfx.c
//Implementasi manajemen efek suara permainan
//Varian Abidarma Syuhada
 */

 #include <SDL3/SDL.h>
 #include <SDL3/SDL_mixer.h>
 #include "sfx.h"
 #include "../settings/settings.h"
 
 /**
  * @brief Jumlah channel audio maksimum untuk efek suara
  */
 #define MAX_SFX_CHANNELS 16
 
 /**
  * @brief Inisialisasi manajer efek suara
  * @param settings Pointer ke pengaturan permainan
  * @return Pointer ke SfxManager yang baru diinisialisasi
  */
 SfxManager* sfx_manager_init(GameSettings* settings) {
     // Periksa parameter input
     if (!settings) {
         SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, 
                      "Gagal inisialisasi: Pengaturan NULL!");
         return NULL;
     }
     
     // Alokasikan memori untuk SfxManager
     SfxManager* sfx_manager = SDL_malloc(sizeof(SfxManager));
     if (!sfx_manager) {
         SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, 
                      "Gagal mengalokasikan memori untuk sfx manager!");
         return NULL;
     }
     
     // Inisialisasi semua pointer efek suara menjadi NULL
     for (int i = 0; i < SFX_COUNT; i++) {
         sfx_manager->sounds[i] = NULL;
     }
     
     // Simpan pointer ke pengaturan
     sfx_manager->settings = settings;
     
     // Alokasikan channel untuk efek suara
     Mix_AllocateChannels(MAX_SFX_CHANNELS);
     
     return sfx_manager;
 }
 
 /**
  * @brief Membebaskan sumber daya manajer efek suara
  * @param sfx_manager Pointer ke SfxManager
  */
 void sfx_manager_free(SfxManager* sfx_manager) {
     // Periksa input
     if (!sfx_manager) return;
     
     // Bebaskan semua chunk efek suara
     for (int i = 0; i < SFX_COUNT; i++) {
         if (sfx_manager->sounds[i]) {
             Mix_FreeChunk(sfx_manager->sounds[i]);
         }
     }
     
     // Bebaskan struktur sfx manager
     SDL_free(sfx_manager);
 }
 
 /**
  * @brief Memuat efek suara dari file
  * @param sfx_manager Pointer ke SfxManager
  * @param sfx_type Tipe efek suara yang akan dimuat
  * @param file_path Path file efek suara
  * @return SDL_TRUE jika berhasil, SDL_FALSE jika gagal
  */
 SDL_bool sfx_manager_load(SfxManager* sfx_manager, 
                            SfxType sfx_type, 
                            const char* file_path) {
     // Periksa parameter input
     if (!sfx_manager || sfx_type < 0 || sfx_type >= SFX_COUNT || !file_path) {
         SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, 
                      "Parameter tidak valid untuk memuat efek suara!");
         return SDL_FALSE;
     }
     
     // Bebaskan chunk efek suara sebelumnya jika ada
     if (sfx_manager->sounds[sfx_type]) {
         Mix_FreeChunk(sfx_manager->sounds[sfx_type]);
     }
     
     // Muat chunk efek suara baru
     sfx_manager->sounds[sfx_type] = Mix_LoadWAV(file_path);
     if (!sfx_manager->sounds[sfx_type]) {
         SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, 
                      "Gagal memuat efek suara %d: %s", 
                      sfx_type, Mix_GetError());
         return SDL_FALSE;
     }
     
     return SDL_TRUE;
 }
 
 /**
  * @brief Memainkan efek suara
  * @param sfx_manager Pointer ke SfxManager
  * @param sfx_type Tipe efek suara yang akan dimainkan
  * @param loops Jumlah pengulangan (0 untuk sekali, -1 untuk loop tak terbatas)
  * @return Nomor channel yang digunakan untuk memainkan efek suara, atau -1 jika gagal
  */
 int sfx_manager_play(SfxManager* sfx_manager, 
                      SfxType sfx_type, 
                      int loops) {
     // Periksa parameter input
     if (!sfx_manager || sfx_type < 0 || sfx_type >= SFX_COUNT) {
         SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, 
                      "Parameter tidak valid untuk memainkan efek suara!");
         return -1;
     }
     
     // Periksa apakah efek suara di-mute
     if (sfx_manager->settings->sfx_mute) return -1;
     
     // Periksa apakah chunk efek suara tersedia
     if (!sfx_manager->sounds[sfx_type]) {
         SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, 
                      "Efek suara %d belum dimuat!", sfx_type);
         return -1;
     }
     
     // Mainkan efek suara di channel bebas
     int channel = Mix_PlayChannel(-1, sfx_manager->sounds[sfx_type], loops);
     
     // Periksa apakah pemutaran berhasil
     if (channel == -1) {
         SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, 
                      "Gagal memainkan efek suara: %s", Mix_GetError());
     }
     
     return channel;
 }
 
 /**
  * @brief Berhenti memainkan semua efek suara
  * @param sfx_manager Pointer ke SfxManager
  */
 void sfx_manager_stop_all(SfxManager* sfx_manager) {
     // Periksa parameter input
     if (!sfx_manager) return;
     
     // Hentikan semua channel efek suara
     Mix_HaltChannel(-1);
 }
 
 /**
  * @brief Mengatur volume efek suara
  * @param sfx_manager Pointer ke SfxManager
  * @param volume Volume baru (0.0 - 1.0)
  */
 void sfx_manager_set_volume(SfxManager* sfx_manager, float volume) {
     // Periksa parameter input
     if (!sfx_manager) return;
     
     // Konversi volume ke skala SDL_mixer (0-128)
     int sdl_volume = (int)(volume * MIX_MAX_VOLUME * 
                      sfx_manager->settings->master_volume);
     
     // Atur volume untuk semua channel
     Mix_Volume(-1, sdl_volume);
 }
 
 /**
  * @brief Memeriksa apakah suatu efek suara sudah dimuat
  * @param sfx_manager Pointer ke SfxManager
  * @param sfx_type Tipe efek suara yang akan diperiksa
  * @return SDL_TRUE jika efek suara sudah dimuat, SDL_FALSE jika belum
  */
 SDL_bool sfx_manager_is_loaded(SfxManager* sfx_manager, SfxType sfx_type) {
     // Periksa parameter input
     if (!sfx_manager || sfx_type < 0 || sfx_type >= SFX_COUNT) {
         return SDL_FALSE;
     }
     
     // Periksa apakah chunk efek suara tersedia
     return sfx_manager->sounds[sfx_type] != NULL;
 }