/**
//settings_menu.h
//Definisi struktur dan tipe data untuk pengaturan permainan
//Varian Abidarma Syuhada
 */

 #ifndef SETTINGS_MENU_H
 #define SETTINGS_MENU_H
 
 #include <SDL3/SDL.h>
 
 /** 
  * @brief Enum untuk item-item pengaturan dalam permainan 
  * Mendefinisikan berbagai pilihan pengaturan yang dapat dimodifikasi pengguna
  */
 typedef enum {
     SETTINGS_ITEM_MASTER_VOLUME,  /**< Pengaturan volume master */
     SETTINGS_ITEM_MUSIC_VOLUME,   /**< Pengaturan volume musik */
     SETTINGS_ITEM_SFX_VOLUME,     /**< Pengaturan volume efek suara */
     SETTINGS_ITEM_MUSIC_MUTE,     /**< Toggle mute musik */
     SETTINGS_ITEM_SFX_MUTE,       /**< Toggle mute efek suara */
     SETTINGS_ITEM_BACK,           /**< Kembali ke menu utama */
     SETTINGS_ITEM_COUNT           /**< Jumlah total item pengaturan */
 } SettingsItem;
 
 /** 
  * @brief Struktur untuk menyimpan pengaturan permainan 
  * Menyimpan status dan nilai untuk setiap pengaturan
  */
 typedef struct {
     float master_volume;   /**< Volume master (0.0 - 1.0) */
     float music_volume;    /**< Volume musik (0.0 - 1.0) */
     float sfx_volume;      /**< Volume efek suara (0.0 - 1.0) */
     SDL_bool music_mute;   /**< Status mute musik */
     SDL_bool sfx_mute;     /**< Status mute efek suara */
 } GameSettings;
 
 /**
  * @brief Inisialisasi pengaturan permainan default
  * @return Pointer ke struktur GameSettings yang baru diinisialisasi
  */
 GameSettings* settings_init(void);
 
 /**
  * @brief Membebaskan memori yang dialokasikan untuk pengaturan
  * @param settings Pointer ke struktur GameSettings yang akan dibebaskan
  */
 void settings_free(GameSettings* settings);
 
 /**
  * @brief Menyimpan pengaturan ke file
  * @param settings Pointer ke struktur GameSettings yang akan disimpan
  * @return SDL_TRUE jika berhasil, SDL_FALSE jika gagal
  */
 SDL_bool settings_save(const GameSettings* settings);
 
 /**
  * @brief Memuat pengaturan dari file
  * @param settings Pointer ke struktur GameSettings yang akan dimuat
  * @return SDL_TRUE jika berhasil, SDL_FALSE jika gagal
  */
 SDL_bool settings_load(GameSettings* settings);
 
 #endif // SETTINGS_MENU_H
 