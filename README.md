# TetrisGame-C4
Tetris Game, made from C Language

Team from C4 :
- Dzakir Tsabit 241511071
- Ibnu Hilmi 241511079
- Rizky Satria 241511089
- Fatimah Hawwa 241511074
- Varian Abi 241511091

# Proyek Tetris

## Deskripsi
Proyek ini merupakan implementasi permainan Tetris dengan berbagai fitur modern seperti sistem penilaian, mode permainan yang bervariasi, dan antarmuka pengguna yang intuitif. Proyek ini dikembangkan menggunakan bahasa C dengan SDL3 sebagai framework grafis.

## Tim Pengembang
- Rizky Satria (Koordinator & Game Mode Manager)
- Varian Abidarma Syuhada (Audio & Settings Developer)
- Fatimah Hawwa Alkhansa (Game Logic & Scoring)
- Dzakit Tsabit Assyafiq (Block System & Preview)
- Ibnu Hilmi Athaillah (UI/UX & Player History)

## Struktur Proyek
```
src/
├── audio/
│   ├── audio_manager.c/h
│   ├── music.c/h
│   └── sfx.c/h
├── core/
│   ├── tetromino/
│   │   ├── blocks.c/h
│   │   └── rotation.c/h
│   └── collision/
│       └── collision.c/h
├── game/
│   ├── game.c/h
│   ├── modes/
│   │   ├── endless.c/h
│   │   └── level.c/h
│   └── scoring/
│       └── score.c/h
├── graphics/
│   └── animations/
│       └── particles.c/h
├── system/
│   └── sdl_init.c/h
├── ui/
│   ├── hud/
│   │   ├── next_preview.c/h
│   │   └── score_display.c/h
│   ├── menu/
│   │   ├── main_menu.c/h
│   │   └── pause_menu.c/h
│   └── settings/
│       └── settings_menu.c/h
└── utils/
    ├── save_load.c/h
    └── timer.c/h
```

## Tabel Pembagian Tugas

| Nama | Peran | Direktori Utama | File yang Dikerjakan | Detail Implementasi |
|------|-------|-----------------|----------------------|---------------------|
| Rizky Satria | Koordinator & Game Mode Manager | src/game/<br>src/game/modes/<br>src/system/ | game.c/h<br>endless.c/h<br>level.c/h<br>sdl_init.c/h | - Game state management<br>- Level progression system<br>- Koordinasi antar sistem<br>- Sistem leveling berbasis waktu/skor<br>- Peningkatan kecepatan progresif<br>- Target objectives per level<br>- Level unlocking system<br>- Inisialisasi sistem |
| Varian Abidarma Syuhada | Audio & Settings Developer | src/audio/<br>src/ui/settings/ | audio_manager.c/h<br>music.c/h<br>sfx.c/h<br>settings_menu.c/h | - Audio initialization<br>- Volume control<br>- Background music loading<br>- Music transitions<br>- Sound effects loading<br>- Effect triggering system<br>- Settings interface |
| Fatimah Hawwa Alkhansa | Game Logic & Scoring | src/game/scoring/<br>src/utils/<br>src/graphics/animations/ | score.c/h<br>timer.c/h<br>particles.c/h | - Score calculation<br>- Combo system<br>- Game time tracking<br>- Level time management<br>- Score popup animations<br>- "Tetris!" notifications<br>- Level up effects |
| Dzakit Tsabit Assyafiq | Block System & Preview | src/core/tetromino/<br>src/core/collision/<br>src/ui/hud/ | blocks.c/h<br>rotation.c/h<br>collision.c/h<br>next_preview.c/h | - Block definitions<br>- Hold system<br>- SRS implementation<br>- Wall kick system<br>- Collision detection<br>- Block locking system<br>- Next block preview |
| Ibnu Hilmi Athaillah | UI/UX & Player History | src/ui/menu/<br>src/ui/hud/<br>src/utils/ | main_menu.c/h<br>pause_menu.c/h<br>score_display.c/h<br>save_load.c/h | - Menu interface<br>- Navigation system<br>- Score display<br>- Timer display<br>- High score saving<br>- Statistics tracking<br>- Achievement system |

## Fitur Utama
- **Mode Permainan**: Mode Endless dan Level dengan tantangan spesifik
- **Sistem Penilaian**: Perhitungan skor dengan sistem combo
- **Sistem Blok**: Implementasi Super Rotation System (SRS) dan wall kick
- **Audio**: Musik latar dan efek suara
- **UI/UX**: Antarmuka menu utama, menu jeda, dan HUD informatif
- **Penyimpanan Data**: Sistem penyimpanan skor tinggi dan statistik pemain

## Petunjuk Kompilasi dan Menjalankan
*[Instruksi kompilasi dan running akan ditambahkan di sini]*

## Dependensi
- SDL3
- SDL3_mixer (untuk audio)
- SDL3_ttf (untuk rendering teks)
- SDL3_image (untuk loading gambar)

## Status Proyek
Dalam pengembangan aktif

## Kontak
Untuk pertanyaan lebih lanjut, silakan hubungi koordinator tim (Rizky) melalui [email koordinator].

**Project From Bandung Polytechnic**

**Informatics engineering department**


