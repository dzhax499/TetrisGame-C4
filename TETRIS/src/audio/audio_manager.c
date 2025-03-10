/**
//audio_manager.c
//Implementasi manajemen sistem audio permainan
//Varian Abidarma Syuhada
 */

 #include <SDL3/SDL.h>
 #include <SDL3/SDL_mixer.h>
 #include "audio_manager.h"
 #include "settings.h"
 
 /** 
  * @brief Jumlah maksimum efek suara yang dapat dimuat 
  */
 #define MAX_SOUND_EFFECTS 64
 
 /**
  * @brief Inisialisasi sistem audio
  * @param settings Pointer ke pengaturan permainan
  * @return Pointer ke AudioManager yang baru diinisialisasi, atau NULL jika gagal
  */
 AudioManager* audio_manager_init(GameSettings* settings) {
     // Periksa apakah SDL_mixer sudah diinisialisasi
     if (Mix_Init(MIX_INIT_MP3 | MIX_INIT_OGG) == 0) {
         SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, 
                      "Gagal menginisialisasi SDL_mixer: %s", Mix_GetError());
         return NULL;
     }
     
     // Buka audio
     if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0) {
         SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, 
                      "Gagal membuka audio: %s", Mix_GetError());
         return NULL;
     }
     
     // Alokasikan memori untuk AudioManager
     AudioManager* audio_manager = SDL_malloc(sizeof(AudioManager));
     if (!audio_manager) {
         SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, 
                      "Gagal mengalokasikan memori untuk audio manager!");
         Mix_CloseAudio();
         Mix_Quit();
         return NULL;
     }
     
     // Inisialisasi properti AudioManager
     audio_manager->background_music = NULL;
     audio_manager->sound_effects = SDL_malloc(sizeof(Mix_Chunk*) * MAX_SOUND_EFFECTS);
     if (!audio_manager->sound_effects) {
         SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, 
                      "Gagal mengalokasikan memori untuk efek suara!");
         SDL_free(audio_manager);
         Mix_CloseAudio();
         Mix_Quit();
         return NULL;
     }
     
     // Inisialisasi semua pointer efek suara menjadi NULL
     for (int i = 0; i < MAX_SOUND_EFFECTS; i++) {
         audio_manager->sound_effects[i] = NULL;
     }
     audio_manager->num_sound_effects = 0;
     
     // Simpan pointer ke pengaturan
     audio_manager->settings = settings;
     
     // Atur volume awal
     audio_manager_set_music_volume(audio_manager, settings->music_volume);
     audio_manager_set_sfx_volume(audio_manager, settings->sfx_volume);
     
     return audio_manager;
 }
 
 /**
  * @brief Membebaskan sumber daya audio
  * @param audio_manager Pointer ke AudioManager yang akan dibebaskan
  */
 void audio_manager_free(AudioManager* audio_manager) {
     if (!audio_manager) return;
     
     // Hentikan dan bebaskan musik latar belakang
     if (audio_manager->background_music) {
         Mix_FreeMusic(audio_manager->background_music);
     }
     
     // Bebaskan semua efek suara
     for (int i = 0; i < audio_manager->num_sound_effects; i++) {
         if (audio_manager->sound_effects[i]) {
             Mix_FreeChunk(audio_manager->sound_effects[i]);
         }
     }
     
     // Bebaskan array efek suara
     SDL_free(audio_manager->sound_effects);
     
     // Tutup audio dan keluar dari SDL_mixer
     Mix_CloseAudio();
     Mix_Quit();
     
     // Bebaskan AudioManager
     SDL_free(audio_manager);
 }
 
 /**
  * @brief Memuat musik latar belakang
  * @param audio_manager Pointer ke AudioManager
  * @param file_path Path file musik
  * @return SDL_TRUE jika berhasil, SDL_FALSE jika gagal
  */
 SDL_bool audio_manager_load_music(AudioManager* audio_manager, const char* file_path) {
     if (!audio_manager || !file_path) return SDL_FALSE;
     
     // Bebaskan musik sebelumnya jika ada
     if (audio_manager->background_music) {
         Mix_FreeMusic(audio_manager->background_music);
     }
     
     // Muat musik baru
     audio_manager->background_music = Mix_LoadMUS(file_path);
     if (!audio_manager->background_music) {
         SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, 
                      "Gagal memuat musik: %s", Mix_GetError());
         return SDL_FALSE;
     }
     
     return SDL_TRUE;
 }
 
 /**
  * @brief Memuat efek suara
  * @param audio_manager Pointer ke AudioManager
  * @param file_path Path file efek suara
  * @return Indeks efek suara yang dimuat, atau -1 jika gagal
  */
 int audio_manager_load_sound_effect(AudioManager* audio_manager, const char* file_path) {
     if (!audio_manager || !file_path) return -1;
     
     // Periksa apakah masih ada ruang untuk efek suara
     if (audio_manager->num_sound_effects >= MAX_SOUND_EFFECTS) {
         SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, 
                      "Maksimum efek suara telah tercapai!");
         return -1;
     }
     
     // Muat efek suara
     Mix_Chunk* sound_effect = Mix_LoadWAV(file_path);
     if (!sound_effect) {
         SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, 
                      "Gagal memuat efek suara: %s", Mix_GetError());
         return -1;
     }
     
     // Simpan efek suara dan kembalikan indeksnya
     int index = audio_manager->num_sound_effects;
     audio_manager->sound_effects[index] = sound_effect;
     audio_manager->num_sound_effects++;
     
     return index;
 }
 
 /**
  * @brief Memainkan musik latar belakang
  * @param audio_manager Pointer ke AudioManager
  * @param loops Jumlah pengulangan (-1 untuk loop tak terbatas)
  */
 void audio_manager_play_music(AudioManager* audio_manager, int loops) {
     if (!audio_manager || !audio_manager->background_music) return;
     
     // Periksa apakah musik di-mute
     if (audio_manager->settings->music_mute) return;
     
     // Mainkan musik
     if (Mix_PlayMusic(audio_manager->background_music, loops) == -1) {
         SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, 
                      "Gagal memainkan musik: %s", Mix_GetError());
     }
 }
 
 /**
  * @brief Memainkan efek suara
  * @param audio_manager Pointer ke AudioManager
  * @param sound_index Indeks efek suara yang akan dimainkan
  * @param loops Jumlah pengulangan
  */
 void audio_manager_play_sound_effect(AudioManager* audio_manager, int sound_index, int loops) {
     if (!audio_manager || sound_index < 0 || 
         sound_index >= audio_manager->num_sound_effects) return;
     
     // Periksa apakah efek suara di-mute
     if (audio_manager->settings->sfx_mute) return;
     
     // Mainkan efek suara
     if (Mix_PlayChannel(-1, audio_manager->sound_effects[sound_index], loops) == -1) {
         SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, 
                      "Gagal memainkan efek suara: %s", Mix_GetError());
     }
 }
 
 /**
  * @brief Mengatur volume musik
  * @param audio_manager Pointer ke AudioManager
  * @param volume Volume baru (0.0 - 1.0)
  */
 void audio_manager_set_music_volume(AudioManager* audio_manager, float volume) {
     if (!audio_manager) return;
     
     // Konversi volume ke skala SDL_mixer (0-128)
     int sdl_volume = (int)(volume * MIX_MAX_VOLUME * 
                      audio_manager->settings->master_volume);
     
     // Atur volume musik
     Mix_VolumeMusic(sdl_volume);
 }
 
 /**
  * @brief Mengatur volume efek suara
  * @param audio_manager Pointer ke AudioManager
  * @param volume Volume baru (0.0 - 1.0)
  */
 void audio_manager_set_sfx_volume(AudioManager* audio_manager, float volume) {
     if (!audio_manager) return;
     
     // Konversi volume ke skala SDL_mixer (0-128)
     int sdl_volume = (int)(volume * MIX_MAX_VOLUME * 
                      audio_manager->settings->master_volume);
     
     // Atur volume untuk semua channel
     Mix_Volume(-1, sdl_volume);
 }
 
 /**
  * @brief Meng-mute atau meng-unmute musik
  * @param audio_manager Pointer ke AudioManager
  * @param mute SDL_TRUE untuk mute, SDL_FALSE untuk unmute
  */
 void audio_manager_mute_music(AudioManager* audio_manager, SDL_bool mute) {
     if (!audio_manager || !audio_manager->settings) return;
     
     // Atur status mute musik di pengaturan
     audio_manager->settings->music_mute = mute;
     
     // Jika mute, hentikan musik
     if (mute) {
         Mix_PauseMusic();
     } else {
         // Jika tidak mute, lanjutkan musik
         Mix_ResumeMusic();
     }
 }
 
 /**
  * @brief Meng-mute atau meng-unmute efek suara
  * @param audio_manager Pointer ke AudioManager
  * @param mute SDL_TRUE untuk mute, SDL_FALSE untuk unmute
  */
 void audio_manager_mute_sfx(AudioManager* audio_manager, SDL_bool mute) {
     if (!audio_manager || !audio_manager->settings) return;
     
     // Atur status mute efek suara di pengaturan
     audio_manager->settings->sfx_mute = mute;
     
     // Jika mute, hentikan semua channel
     if (mute) {
         Mix_HaltChannel(-1);
     }
 }