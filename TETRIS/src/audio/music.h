/**
//music.h
//Definisi struktur dan fungsi untuk manajemen musik permainan
//Varian Abidarma Syuhada
 */

 #ifndef MUSIC_H
 #define MUSIC_H
 
 #include <SDL3/SDL.h>
 #include <SDL3/SDL_mixer.h>
 #include "../settings/settings.h"
 
 /**
  * @brief Enum untuk jenis musik dalam permainan
  */
 typedef enum {
     MUSIC_MENU,      /< Musik menu utama */
     MUSIC_GAMEPLAY,  /< Musik selama permainan */
     MUSIC_GAMEOVER,  /< Musik game over */
     MUSIC_HIGHSCORE, /< Musik layar skor tinggi */
     MUSIC_COUNT      /< Jumlah total musik */
 } MusicType;
 
 /**
  * @brief Struktur untuk manajemen musik permainan
  */
 typedef struct {
     Mix_Music* tracks[MUSIC_COUNT];  /< Array track musik */
     MusicType current_track;         /< Track musik saat ini */
     GameSettings* settings;          /< Pengaturan permainan */
 } MusicManager;
 
 /**
  * @brief Inisialisasi manajer musik
  * @param settings Pointer ke pengaturan permainan
  * @return Pointer ke MusicManager yang baru diinisialisasi
  */
 MusicManager* music_manager_init(GameSettings* settings);
 
 /**
  * @brief Membebaskan sumber daya manajer musik
  * @param music_manager Pointer ke MusicManager
  */
 void music_manager_free(MusicManager* music_manager);
 
 /**
  * @brief Memuat musik dari file
  * @param music_manager Pointer ke MusicManager
  * @param music_type Tipe musik yang akan dimuat
  * @param file_path Path file musik
  * @return SDL_TRUE jika berhasil, SDL_FALSE jika gagal
  */
 SDL_bool music_manager_load(MusicManager* music_manager, 
                              MusicType music_type, 
                              const char* file_path);
 
 /**
  * @brief Memainkan musik
  * @param music_manager Pointer ke MusicManager
  * @param music_type Tipe musik yang akan dimainkan
  * @param loops Jumlah pengulangan (-1 untuk loop tak terbatas)
  */
 void music_manager_play(MusicManager* music_manager, 
                         MusicType music_type, 
                         int loops);
 
 /**
  * @brief Berhenti memainkan musik
  * @param music_manager Pointer ke MusicManager
  */
 void music_manager_stop(MusicManager* music_manager);
 
 /**
  * @brief Menjeda musik
  * @param music_manager Pointer ke MusicManager
  */
 void music_manager_pause(MusicManager* music_manager);
 
 /**
  * @brief Melanjutkan musik yang dijeda
  * @param music_manager Pointer ke MusicManager
  */
 void music_manager_resume(MusicManager* music_manager);
 
 /**
  * @brief Mengatur volume musik
  * @param music_manager Pointer ke MusicManager
  * @param volume Volume baru (0.0 - 1.0)
  */
 void music_manager_set_volume(MusicManager* music_manager, float volume);
 
 #endif // MUSIC_H