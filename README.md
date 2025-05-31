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

## Kontribusi Anggota & Tim Pengembang

| Nama                      | NIM         | Kontribusi Utama & File yang Dikerjakan                                                                               |
|---------------------------|-------------|-----------------------------------------------------------------------------------------------------------------------|
| *Dzakit Tsabit*           | 241511071   | blocks.c/h, linkedlist_block.c/h, rotasi_data.c/h, tetris.h, dokumentasi, logika rotasi, hold, wall kick, sistem blok |
| *Ibnu Hilmi*              | 241511079   | board.c/h, board_linkedlist.c/h, main_menu.c/h, logika papan, menu, pengelolaan linked list board                     |
| *Rizky Satria Gunawan*    | 241511089   | board.c/h, board_linkedlist.c/h, pengelolaan grid, logika penghapusan baris, validasi board                           |
| *Fatimah Hawwa*           | 241511074   | scoring.c/h, sistem skor, highscore, pengaturan level, dokumentasi skor                                               |
| *Varian Abidarma*         | 241511091   | rendering.c/h, sound.c/h, game_sound.c/h, rendering UI, efek suara, integrasi Raylib                                  |

## Struktur Proyek
```
TETRIS-GameFix/
├── assets/
│   ├── audio/           # File audio (musik, efek suara)
│   ├── fonts/           # File font (misal .ttf)
│   ├── log/             # File log, highscore, leaderboard
│   └── textures/        # Tekstur dan gambar (misal .png)
├── build/
├── libs/
├── src/                 # Source code (.c)
│   ├── include/             # Header files (.h)
│   │   ├── blocks.h             # Definisi bentuk & logika blok
│   │   ├── board.h              # Definisi papan permainan (array)
│   │   ├── board_linkedlist.h   # Header papan linked list
│   │   ├── game_sound.h         # Header sistem suara
│   │   ├── linkedlist_block.h   # Header rotasi circular linked list
│   │   ├── leaderboard.h        # Header leaderboard
│   │   ├── main_menu.h          # Deklarasi menu utama
│   │   ├── rendering.h          # Fungsi rendering
│   │   ├── rotasi_data.h        # Data rotasi Tetromino
│   │   ├── scoring.h            # Sistem skor
│   │   ├── sound.h              # Sistem audio
│   │   ├── timer.h              # sistem timer
│   │   └── tetris.h             # Definisi umum & struktur global
│   ├── blocks.c             # Implementasi logika blok
│   ├── board_linkedlist.c   # Implementasi papan linked list
│   ├── game_sound.c         # Implementasi sistem suara
│   ├── linkedlist_block.c   # Implementasi rotasi circular linked list
│   ├── leaderboard.c        # Implemantasi leaderboard
│   ├── main.c               # Program utama
│   ├── main_menu.c          # Implementasi menu utama
│   ├── rendering.c          # Implementasi rendering
│   ├── rotasi_data.c        # Data rotasi Tetromino
│   ├── scoring.c            # Implementasi sistem skor
│   ├── sound.c              # Implementasi sistem audio
│   └── timer.c              # sistem timer
├── Makefile             # Build script
└── README.md            # Dokumentasi proyek
```

## Deskripsi File-File Utama

### *blocks.c/h*
- ✅ Implementasi bentuk-bentuk blok Tetris (I, J, L, O, S, T, Z)
- 🔄 Rotasi blok dan logika wall-kick
- 📦 Pergerakan blok (horizontal, vertikal, hard drop)
- ✋ Fungsi hold block

### *board.c/h*
- 🎮 Implementasi papan permainan *10x20* berbasis array
- 🧹 Pengecekan dan penghapusan *baris yang penuh*
- ❌ Pengecekan *game over*
- 🎨 Pewarnaan blok

### *board_linkedlist.c/h*
- 🧩 Implementasi papan permainan menggunakan *doubly linked list*
- 🧹 Kompaksi baris kosong di atas board
- 🔄 Update indeks baris secara dinamis

### *linkedlist_block.c/h*
- 🔄 Sistem rotasi blok berbasis *circular linked list*
- 🪢 Manajemen node rotasi dan indeks rotasi saat ini
- 🛠 Menghindari bug auto-rotation

### *rotasi_data.c/h*
- 🗃 Data array bentuk dan rotasi unik untuk setiap blok Tetris
- 🔢 Jumlah rotasi unik tiap jenis blok

### *main.c*
- 🔁 Program utama, *game loop*
- 🎮 Penanganan *input pemain*
- 🧠 Logika utama permainan
- 🔄 Perpindahan antar state (*menu, bermain, game over*)

### *main_menu.c/h*
- 📜 Menu utama dengan tombol *interaktif*
- 🔄 Navigasi antar menu (*play, settings, credits, exit*)
- 🎨 Rendering menu

### *rendering.c/h*
- 🖼 Rendering *papan permainan*
- 🔲 Rendering *blok aktif, next, dan hold*
- 👥 Rendering *shadow drop*
- 🏆 Rendering UI (*skor, level*)

### *scoring.c/h*
- 🏆 Sistem perhitungan *skor*
- 📈 Kenaikan *level* berdasarkan baris yang dihapus
- 💾 Penyimpanan *highscore*
- ⏳ Pengaturan *kecepatan jatuh blok* berdasarkan level

### *sound.c/h*
- 🎵 Pengelolaan *sound effect dan musik latar*
- 🔊 Fungsi untuk *memainkan efek suara* saat aksi tertentu

### *game_sound.c/h*
- 🎼 Manajemen musik latar dan efek suara khusus
- 🔄 Transisi antar musik (misal: menu ke in-game)

### *tetris.h*
- 🔢 Definisi *konstanta global*
- 🔲 Struktur dasar *blok Tetris*
- 🖥 Definisi *ukuran window dan papan*

### *timer.c/h*
- ⏲ Pengelolaan *timer* untuk delay jatuh blok
- ⏳ Timer untuk animasi, efek, dan transisi menu
- 🕒 Fungsi reset, start, stop, dan cek waktu habis

### *leaderboard.c/h*
- 🏅 Pengelolaan *leaderboard*/daftar skor tertinggi
- 💾 Penyimpanan dan pembacaan skor dari file
- 📝 Fungsi menambah, mengurutkan, dan menampilkan skor pemain

**Prasyarat**

- Compiler C (GCC/MinGW)
- Library Raylib (direkomendasikan versi 3.5+)

Kompilasi
Menggunakan GCC:

```
gcc -Wall -Wextra -std=c99 -Iinclude src/main.c src/blocks.c src/rendering.c src/board.c src/main_menu.c src/scoring.c src/game_sound.c src/timer.c src/leaderboard.c src/rotasi_data.c src/linkedlist_block.c -o tetris_game -Lmingw64 -lraylib -lopengl32 -lgdi32 -lwinmm -lm -lwinpthread
```

```
./tetris_game
```

Menggunakan Makefile:

```
make
```

```
./tetris_game
```

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
```
fallDelay = 1.0f - ((level - 1) * 0.15f);
```
Dengan batas minimum **0.1 detik**.

**Project From Bandung Polytechnic**

**Informatics engineering department**


