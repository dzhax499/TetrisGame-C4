/**
//settings_menu.c
//Implementasi manajemen pengaturan permainan
//Varian Abidarma Syuhada
 */

 #include <SDL3/SDL.h>
 #include <SDL3/SDL_filesystem.h>
 #include <stdio.h>
 #include "settings_menu.h"
 
 /** 
  * @brief Jalur file untuk menyimpan pengaturan 
  * Menggunakan SDL_GetPrefPath untuk mendapatkan lokasi yang sesuai
  */
 static const char* SETTINGS_FILE_PATH = "tetris_settings.cfg";
 
 /**
  * @brief Inisialisasi pengaturan permainan default
  * @return Pointer ke struktur GameSettings yang baru diinisialisasi
  */
 GameSettings* settings_init(void) {
     // Alokasikan memori untuk struktur pengaturan
     GameSettings* settings = SDL_malloc(sizeof(GameSettings));
     
     // Periksa apakah alokasi memori berhasil
     if (!settings) {
         SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, 
                      "Gagal mengalokasikan memori untuk pengaturan!");
         return NULL;
     }
     
     // Tetapkan nilai default untuk setiap pengaturan
     settings->master_volume = 1.0f;    // Volume master penuh
     settings->music_volume = 0.7f;     // Volume musik 70%
     settings->sfx_volume = 0.8f;       // Volume efek suara 80%
     settings->music_mute = SDL_FALSE;  // Musik tidak di-mute
     settings->sfx_mute = SDL_FALSE;    // Efek suara tidak di-mute
     
     return settings;
 }
 
 /**
  * @brief Membebaskan memori yang dialokasikan untuk pengaturan
  * @param settings Pointer ke struktur GameSettings yang akan dibebaskan
  */
 void settings_free(GameSettings* settings) {
     // Periksa apakah pointer tidak NULL sebelum membebaskan
     if (settings) {
         SDL_free(settings);
     }
 }
 
 /**
  * @brief Menyimpan pengaturan ke file
  * @param settings Pointer ke struktur GameSettings yang akan disimpan
  * @return SDL_TRUE jika berhasil, SDL_FALSE jika gagal
  */
 SDL_bool settings_save(const GameSettings* settings) {
     // Periksa input
     if (!settings) {
         SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, 
                      "Gagal menyimpan: Pointer pengaturan NULL!");
         return SDL_FALSE;
     }
     
     // Buka file untuk ditulis
     FILE* file = fopen(SETTINGS_FILE_PATH, "wb");
     if (!file) {
         SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, 
                      "Gagal membuka file untuk menyimpan pengaturan!");
         return SDL_FALSE;
     }
     
     // Tulis setiap pengaturan ke file
     size_t written = fwrite(settings, sizeof(GameSettings), 1, file);
     fclose(file);
     
     // Periksa apakah penulisan berhasil
     if (written != 1) {
         SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, 
                      "Gagal menulis pengaturan ke file!");
         return SDL_FALSE;
     }
     
     SDL_Log("Pengaturan berhasil disimpan!");
     return SDL_TRUE;
 }
 
 /**
  * @brief Memuat pengaturan dari file
  * @param settings Pointer ke struktur GameSettings yang akan dimuat
  * @return SDL_TRUE jika berhasil, SDL_FALSE jika gagal
  */
 SDL_bool settings_load(GameSettings* settings) {
     // Periksa input
     if (!settings) {
         SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, 
                      "Gagal memuat: Pointer pengaturan NULL!");
         return SDL_FALSE;
     }
     
     // Buka file untuk dibaca
     FILE* file = fopen(SETTINGS_FILE_PATH, "rb");
     if (!file) {
         SDL_LogWarning(SDL_LOG_CATEGORY_APPLICATION, 
                        "File pengaturan tidak ditemukan. Menggunakan pengaturan default.");
         // Inisialisasi ulang dengan pengaturan default jika file tidak ada
         *settings = *settings_init();
         return SDL_FALSE;
     }
     
     // Baca pengaturan dari file
     size_t read = fread(settings, sizeof(GameSettings), 1, file);
     fclose(file);
     
     // Periksa apakah pembacaan berhasil
     if (read != 1) {
         SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, 
                      "Gagal membaca pengaturan dari file!");
         return SDL_FALSE;
     }
     
     SDL_Log("Pengaturan berhasil dimuat!");
     return SDL_TRUE;
 }
