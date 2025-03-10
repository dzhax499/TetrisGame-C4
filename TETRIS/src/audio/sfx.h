/**
//sfx.h
//Definisi struktur dan fungsi untuk manajemen efek suara permainan
//Varian Abidarma Syuhada
 */

 #ifndef SFX_H
 #define SFX_H
 
 #include <SDL3/SDL.h>
 #include <SDL3/SDL_mixer.h>
 #include "../settings/settings.h"
 
 /**
  * @brief Enum untuk jenis efek suara dalam permainan
  */
 typedef enum {
     SFX_BLOCK_MOVE,      /< Efek suara saat blok bergerak */
     SFX_BLOCK_ROTATE,    /< Efek suara saat blok diputar */
     SFX_LINE_CLEAR,      /< Efek suara saat garis dihapus */
     SFX_TETRIS,          /< Efek suara saat tetris */
     SFX_HARD_DROP,       /< Efek suara saat hard drop */
     SFX_GAME_OVER,       /< Efek suara game over */
     SFX_LEVEL_UP,        /< Efek suara naik level */
     SFX_COUNT            /< Jumlah total efek suara */
 } SfxType;
 
 /**
  * @brief Struktur untuk manajemen efek suara permainan
  */
 typedef struct {
     Mix_Chunk* sounds[SFX_COUNT];  /< Array chunk efek suara */
     GameSettings* settings;         /< Pengaturan permainan */
 } SfxManager;
 
 /**
  * @brief Inisialisasi manajer efek suara
  * @param settings Pointer ke pengaturan permainan
  * @return Pointer ke SfxManager yang baru diinisialisasi
  */
 SfxManager* sfx_manager_init(GameSettings* settings);
 
 /**
  * @brief Membebaskan sumber daya manajer efek suara
  * @param sfx_manager Pointer ke SfxManager
  */
 void sfx_manager_free(SfxManager* sfx_manager);
 
 /**
  * @brief Memuat efek suara dari file
  * @param sfx_manager Pointer ke SfxManager
  * @param sfx_type Tipe efek suara yang akan dimuat
  * @param file_path Path file efek suara
  * @return SDL_TRUE jika berhasil, SDL_FALSE jika gagal
  */
 SDL_bool sfx_manager_load(SfxManager* sfx_manager, 
                            SfxType sfx_type, 
                            const char* file_path);
 
 /**
  * @brief Memainkan efek suara
  * @param sfx_manager Pointer ke SfxManager
  * @param sfx_type Tipe efek suara yang akan dimainkan
  * @param loops Jumlah pengulangan (0 untuk sekali, -1 untuk loop tak terbatas)
  * @return Nomor channel yang digunakan untuk memainkan efek suara, atau -1 jika gagal
  */
 int sfx_manager_play(SfxManager* sfx_manager, 
                      SfxType sfx_type, 
                      int loops);
 
 /**
  * @brief Berhenti memainkan semua efek suara
  * @param sfx_manager Pointer ke SfxManager
  */
 void sfx_manager_stop_all(SfxManager* sfx_manager);
 
 /**
  * @brief Mengatur volume efek suara
  * @param sfx_manager Pointer ke SfxManager
  * @param volume Volume baru (0.0 - 1.0)
  */
 void sfx_manager_set_volume(SfxManager* sfx_manager, float volume);
 
 /**
  * @brief Memeriksa apakah suatu efek suara sudah dimuat
  * @param sfx_manager Pointer ke SfxManager
  * @param sfx_type Tipe efek suara yang akan diperiksa
  * @return SDL_TRUE jika efek suara sudah dimuat, SDL_FALSE jika belum
  */
 SDL_bool sfx_manager_is_loaded(SfxManager* sfx_manager, SfxType sfx_type);
 
 #endif // SFX_H