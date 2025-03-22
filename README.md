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
Game Tetris ini merupakan implementasi lengkap dari permainan klasik Tetris dengan beberapa fitur modern seperti:

Sistem hold block
Preview block berikutnya
Shadow drop (bayangan tempat block akan jatuh)
Wall kicks untuk rotasi yang lebih baik
Sistem scoring dan level
Menu interaktif

## Tim Pengembang
Dzakit Tsabit (241511071) - Implementasi blok Tetris
Ibnu Hilmi (241511079) - Logika papan permainan & menu
Rizky Satria Gunawan (241511089) - Logika papan permainan
Abi - (241511091)  Rendering (241511091)
Fatimah Hawwa (241511074) - Sistem skor

## Struktur Proyek
```
tetris/
├── assets/
│   ├── audio/           # File-file audio
│   ├── fonts/           # File-file font
│   ├── log/             # File log dan highscore
│   └── textures/        # Tekstur dan gambar
├── include/             # Header files
│   ├── blocks.h         # Definisi bentuk dan logika block 
│   ├── board.h          # Definisi papan permainan
│   ├── main_menu.h      # Deklarasi menu utama
│   ├── rendering.h      # Fungsi rendering
│   ├── scoring.h        # Sistem skor
│   ├── sound.h          # Sistem audio
│   └── tetris.h         # Definisi umum
├── src/
│   ├── blocks.c         # Implementasi logika block
│   ├── board.c          # Implementasi logika papan
│   ├── main.c           # Program utama
│   ├── main_menu.c      # Implementasi menu
│   ├── rendering.c      # Implementasi rendering
│   ├── scoring.c        # Implementasi sistem skor
│   └── sound.c          # Implementasi sistem audio
├── makefile             # Buildscript
└── README.md            # Dokumentasi
```

Deskripsi File-File Utama

**blocks.c/h**

Implementasi bentuk-bentuk blok Tetris (I, J, L, O, S, T, Z)
Rotasi blok dan logika wall-kick
Pergerakan blok (horizontal, vertikal, hard drop)
Fungsi hold block

**board.c/h**

Implementasi papan permainan 10x20
Pengecekan dan penghapusan baris yang penuh
Pengecekan game over
Pewarnaan blok

**main.c**

Program utama, game loop
Penanganan input pemain
Logika utama permainan
Perpindahan antar state (menu, bermain, game over)

**main_menu.c/h**

Menu utama dengan tombol interaktif
Navigasi antar menu (play, settings, credits, exit)
Rendering menu

**rendering.c/h**

Rendering papan permainan
Rendering blok aktif, next, dan hold
Rendering shadow drop
Rendering UI (skor, level)

**scoring.c/h**

Sistem perhitungan skor
Kenaikan level berdasarkan baris yang dihapus
Penyimpanan highscore
Pengaturan kecepatan jatuh blok berdasarkan level

**sound.c/h**

Pengelolaan sound effect dan musik latar
Fungsi untuk memainkan efek suara saat aksi tertentu

**tetris.h**

Definisi konstanta global
Struktur dasar blok Tetris
Definisi ukuran window dan papan

**Prasyarat**

Compiler C (GCC/MinGW)
Library Raylib (direkomendasikan versi 3.5+)

Kompilasi
Menggunakan Makefile:

```make```

`./tetris_game`

**Kontrol Permainan**
| Tombol  | Fungsi                            |
|---------|-----------------------------------|
| ←       | Gerak ke kiri                    |
| →       | Gerak ke kanan                   |
| ↓       | Percepat turun                   |
| Z       | Putar blok (90° searah jarum jam) |
| SPACE   | Hard drop (jatuh langsung)       |
| C       | Hold blok                        |
| R       | Restart game (saat game over)    |
| ESC     | Keluar/kembali ke menu           |

**Fitur Level & Kecepatan**

- Kecepatan jatuh blok meningkat seiring kenaikan level.
- Setiap **10 baris** yang dibersihkan akan menaikkan level.
- **Formula kecepatan:**
`fallDelay = 1.0f - ((level - 1) * 0.15f);`
Dengan batas minimum **0.1 detik**.

**Project From Bandung Polytechnic**

**Informatics engineering department**


