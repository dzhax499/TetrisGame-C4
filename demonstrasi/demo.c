/**
 * TETRIS GAME
 * -----------
 * Implementasi permainan Tetris klasik menggunakan SDL2
 * 
 * Game ini memiliki fitur:
 * - Semua 7 tetromino standar (I, J, L, O, S, T, Z)
 * - Sistem rotasi Super Rotation System (SRS) dengan wall kicks
 * - Preview tetromino berikutnya
 * - Sistem hold piece
 * - Ghost piece untuk menunjukkan posisi landing
 * - Scoring dan level sistem
 */

 #include <SDL2/SDL.h>
 #include <stdbool.h>
 #include <stdlib.h>
 #include <time.h>
 #include <stdio.h>
 
 // Konstanta Game
 #define WINDOW_WIDTH 640      // Lebar jendela permainan
 #define WINDOW_HEIGHT 720     // Tinggi jendela permainan
 #define BOARD_WIDTH 10        // Lebar board dalam jumlah sel
 #define BOARD_HEIGHT 22       // Tinggi board (20 terlihat + 2 tersembunyi)
 #define BLOCK_SIZE 30         // Ukuran setiap blok dalam piksel
 #define PREVIEW_BLOCK_SIZE 20 // Ukuran blok preview dalam piksel
 #define NEXT_QUEUE_SIZE 3     // Jumlah tetromino selanjutnya yang ditampilkan
 
 // Posisi tampilan board permainan
 #define BOARD_X ((WINDOW_WIDTH - BOARD_WIDTH * BLOCK_SIZE) / 2)  // Posisi X board di tengah layar
 #define BOARD_Y 40                                               // Posisi Y board dari atas layar
 
 // Posisi area preview tetromino
 #define PREVIEW_X (BOARD_X + BOARD_WIDTH * BLOCK_SIZE + 40)
 #define PREVIEW_Y 100
 
 // Posisi area hold tetromino
 #define HOLD_X (BOARD_X - 120)
 #define HOLD_Y 100
 
 // Definisi warna untuk setiap tetromino
 static const SDL_Color TETROMINO_COLORS[7] = {
     {0, 255, 255, 255},   // I - Cyan (Biru Muda)
     {0, 0, 255, 255},     // J - Blue (Biru)
     {255, 165, 0, 255},   // L - Orange (Oranye)
     {255, 255, 0, 255},   // O - Yellow (Kuning)
     {0, 255, 0, 255},     // S - Green (Hijau)
     {128, 0, 128, 255},   // T - Purple (Ungu)
     {255, 0, 0, 255}      // Z - Red (Merah)
 };
 
 // Definisi bentuk 7 tetromino standar
 // 0 = kosong, 1 = terisi
 // Setiap tetromino memiliki 4 rotasi
 static const int TETROMINOS[7][4][4][4] = {
     // I - Cyan (Biru Muda)
     {
         {{0, 0, 0, 0}, {1, 1, 1, 1}, {0, 0, 0, 0}, {0, 0, 0, 0}},  // Rotasi 0°
         {{0, 0, 1, 0}, {0, 0, 1, 0}, {0, 0, 1, 0}, {0, 0, 1, 0}},  // Rotasi 90°
         {{0, 0, 0, 0}, {0, 0, 0, 0}, {1, 1, 1, 1}, {0, 0, 0, 0}},  // Rotasi 180°
         {{0, 1, 0, 0}, {0, 1, 0, 0}, {0, 1, 0, 0}, {0, 1, 0, 0}}   // Rotasi 270°
     },
     // J - Blue (Biru)
     {
         {{1, 0, 0, 0}, {1, 1, 1, 0}, {0, 0, 0, 0}, {0, 0, 0, 0}},  // Rotasi 0°
         {{0, 1, 1, 0}, {0, 1, 0, 0}, {0, 1, 0, 0}, {0, 0, 0, 0}},  // Rotasi 90°
         {{0, 0, 0, 0}, {1, 1, 1, 0}, {0, 0, 1, 0}, {0, 0, 0, 0}},  // Rotasi 180°
         {{0, 1, 0, 0}, {0, 1, 0, 0}, {1, 1, 0, 0}, {0, 0, 0, 0}}   // Rotasi 270°
     },
     // L - Orange (Oranye)
     {
         {{0, 0, 1, 0}, {1, 1, 1, 0}, {0, 0, 0, 0}, {0, 0, 0, 0}},  // Rotasi 0°
         {{0, 1, 0, 0}, {0, 1, 0, 0}, {0, 1, 1, 0}, {0, 0, 0, 0}},  // Rotasi 90°
         {{0, 0, 0, 0}, {1, 1, 1, 0}, {1, 0, 0, 0}, {0, 0, 0, 0}},  // Rotasi 180°
         {{1, 1, 0, 0}, {0, 1, 0, 0}, {0, 1, 0, 0}, {0, 0, 0, 0}}   // Rotasi 270°
     },
     // O - Yellow (Kuning)
     {
         {{0, 1, 1, 0}, {0, 1, 1, 0}, {0, 0, 0, 0}, {0, 0, 0, 0}},  // Rotasi 0° (semua sama untuk O)
         {{0, 1, 1, 0}, {0, 1, 1, 0}, {0, 0, 0, 0}, {0, 0, 0, 0}},  // Rotasi 90°
         {{0, 1, 1, 0}, {0, 1, 1, 0}, {0, 0, 0, 0}, {0, 0, 0, 0}},  // Rotasi 180°
         {{0, 1, 1, 0}, {0, 1, 1, 0}, {0, 0, 0, 0}, {0, 0, 0, 0}}   // Rotasi 270°
     },
     // S - Green (Hijau)
     {
         {{0, 1, 1, 0}, {1, 1, 0, 0}, {0, 0, 0, 0}, {0, 0, 0, 0}},  // Rotasi 0°
         {{0, 1, 0, 0}, {0, 1, 1, 0}, {0, 0, 1, 0}, {0, 0, 0, 0}},  // Rotasi 90°
         {{0, 0, 0, 0}, {0, 1, 1, 0}, {1, 1, 0, 0}, {0, 0, 0, 0}},  // Rotasi 180°
         {{1, 0, 0, 0}, {1, 1, 0, 0}, {0, 1, 0, 0}, {0, 0, 0, 0}}   // Rotasi 270°
     },
     // T - Purple (Ungu)
     {
         {{0, 1, 0, 0}, {1, 1, 1, 0}, {0, 0, 0, 0}, {0, 0, 0, 0}},  // Rotasi 0°
         {{0, 1, 0, 0}, {0, 1, 1, 0}, {0, 1, 0, 0}, {0, 0, 0, 0}},  // Rotasi 90°
         {{0, 0, 0, 0}, {1, 1, 1, 0}, {0, 1, 0, 0}, {0, 0, 0, 0}},  // Rotasi 180°
         {{0, 1, 0, 0}, {1, 1, 0, 0}, {0, 1, 0, 0}, {0, 0, 0, 0}}   // Rotasi 270°
     },
     // Z - Red (Merah)
     {
         {{1, 1, 0, 0}, {0, 1, 1, 0}, {0, 0, 0, 0}, {0, 0, 0, 0}},  // Rotasi 0°
         {{0, 0, 1, 0}, {0, 1, 1, 0}, {0, 1, 0, 0}, {0, 0, 0, 0}},  // Rotasi 90°
         {{0, 0, 0, 0}, {1, 1, 0, 0}, {0, 1, 1, 0}, {0, 0, 0, 0}},  // Rotasi 180°
         {{0, 1, 0, 0}, {1, 1, 0, 0}, {1, 0, 0, 0}, {0, 0, 0, 0}}   // Rotasi 270°
     }
 };
 
 // Data wall kick untuk SRS (Super Rotation System)
 // Data untuk JLOSTZ tetrominos (semua kecuali I dan O)
 static const int WALL_KICK_JLOSTZ[4][5][2] = {
     // 0>>1 (Rotasi dari 0° ke 90°)
     {{0, 0}, {-1, 0}, {-1, 1}, {0, -2}, {-1, -2}},
     // 1>>2 (Rotasi dari 90° ke 180°)
     {{0, 0}, {1, 0}, {1, -1}, {0, 2}, {1, 2}},
     // 2>>3 (Rotasi dari 180° ke 270°)
     {{0, 0}, {1, 0}, {1, 1}, {0, -2}, {1, -2}},
     // 3>>0 (Rotasi dari 270° ke 0°)
     {{0, 0}, {-1, 0}, {-1, -1}, {0, 2}, {-1, 2}}
 };
 
 // Data wall kick khusus untuk tetromino I
 static const int WALL_KICK_I[4][5][2] = {
     // 0>>1 (Rotasi dari 0° ke 90°)
     {{0, 0}, {-2, 0}, {1, 0}, {-2, -1}, {1, 2}},
     // 1>>2 (Rotasi dari 90° ke 180°)
     {{0, 0}, {-1, 0}, {2, 0}, {-1, 2}, {2, -1}},
     // 2>>3 (Rotasi dari 180° ke 270°)
     {{0, 0}, {2, 0}, {-1, 0}, {2, 1}, {-1, -2}},
     // 3>>0 (Rotasi dari 270° ke 0°)
     {{0, 0}, {1, 0}, {-2, 0}, {1, -2}, {-2, 1}}
 };
 
 // Struktur untuk merepresentasikan tetromino aktif
 typedef struct
 {
     int type;      // Tipe tetromino (0-6 untuk I, J, L, O, S, T, Z)
     int rotation;  // Rotasi tetromino (0-3 untuk empat rotasi)
     int x;         // Posisi X di papan permainan
     int y;         // Posisi Y di papan permainan
 } Tetromino;
 
 // Sistem blok untuk mengelola semua operasi terkait block
 typedef struct
 {
     Tetromino current;               // Tetromino aktif saat ini
     int next_queue[NEXT_QUEUE_SIZE]; // Antrian tetromino berikutnya
     int hold_block;                  // Blok yang ditahan (-1 jika tidak ada)
     bool can_hold;                   // Apakah pemain dapat menahan blok
 } BlockSystem;
 
 // Struktur papan permainan
 typedef struct
 {
     int cells[BOARD_HEIGHT][BOARD_WIDTH]; // 0 untuk kosong, 1-7 untuk tipe tetromino
 } GameBoard;
 
 // Prototipe fungsi
 void blocks_init(BlockSystem *system);                                                 // Inisialisasi sistem blok
 void collision_init_board(GameBoard *board);                                           // Inisialisasi papan permainan
 bool collision_check(const BlockSystem *system, const GameBoard *board, int offset_x, int offset_y); // Cek tabrakan
 void collision_lock_tetromino(const BlockSystem *system, GameBoard *board);            // Kunci tetromino di papan
 int collision_clear_lines(GameBoard *board);                                           // Hapus baris yang lengkap
 bool collision_move_left(BlockSystem *system, GameBoard *board);                       // Geser tetromino ke kiri
 bool collision_move_right(BlockSystem *system, GameBoard *board);                      // Geser tetromino ke kanan
 bool collision_soft_drop(BlockSystem *system, GameBoard *board);                       // Turunkan tetromino perlahan
 void collision_hard_drop(BlockSystem *system, GameBoard *board);                       // Jatuhkan tetromino langsung
 bool rotation_rotate_clockwise(BlockSystem *system, GameBoard *board);                 // Rotasi searah jarum jam
 bool rotation_rotate_counterclockwise(BlockSystem *system, GameBoard *board);          // Rotasi berlawanan jarum jam
 void blocks_hold(BlockSystem *system);                                                 // Tahan tetromino saat ini
 void blocks_spawn_new(BlockSystem *system);                                            // Munculkan tetromino baru
 void render_board(SDL_Renderer *renderer, const GameBoard *board);                     // Render papan permainan
 void render_current_tetromino(SDL_Renderer *renderer, const BlockSystem *system);      // Render tetromino aktif
 void render_ghost_piece(SDL_Renderer *renderer, const BlockSystem *system, const GameBoard *board); // Render ghost piece
 void render_next_preview(SDL_Renderer *renderer, const BlockSystem *system);           // Render preview tetromino berikutnya
 void render_hold_preview(SDL_Renderer *renderer, const BlockSystem *system);           // Render tetromino yang ditahan
 
 /**
  * Inisialisasi sistem blok tetromino
  * 
  * Fungsi ini menginisialisasi sistem blok, termasuk:
  * - Mengisi antrian next tetromino secara acak
  * - Mengatur state hold tetromino
  * - Memunculkan tetromino aktif pertama
  * 
  * @param system Pointer ke struktur BlockSystem yang akan diinisialisasi
  */
 void blocks_init(BlockSystem *system)
 {
     // Inisialisasi random seed
     srand(time(NULL));
 
     // Inisialisasi antrian next dengan blok acak
     for (int i = 0; i < NEXT_QUEUE_SIZE; i++)
     {
         system->next_queue[i] = rand() % 7;
     }
 
     // Set status hold ke kosong
     system->hold_block = -1;
     system->can_hold = true;
 
     // Inisialisasi blok pertama
     system->current.type = system->next_queue[0];
     system->current.rotation = 0;
     system->current.x = (BOARD_WIDTH - 4) / 2; // Posisikan di tengah
     system->current.y = 0;
 
     // Perbarui antrian
     for (int i = 0; i < NEXT_QUEUE_SIZE - 1; i++)
     {
         system->next_queue[i] = system->next_queue[i + 1];
     }
     system->next_queue[NEXT_QUEUE_SIZE - 1] = rand() % 7;
 }
 
 /**
  * Inisialisasi papan permainan
  * 
  * Fungsi ini menginisialisasi papan permainan dengan mengosongkan semua sel.
  * 
  * @param board Pointer ke struktur GameBoard yang akan diinisialisasi
  */
 void collision_init_board(GameBoard *board)
 {
     // Kosongkan papan (set semua sel ke kosong)
     for (int y = 0; y < BOARD_HEIGHT; y++)
     {
         for (int x = 0; x < BOARD_WIDTH; x++)
         {
             board->cells[y][x] = 0; // Kosong
         }
     }
 }
 
 /**
  * Memeriksa tabrakan tetromino dengan papan atau batas pada offset tertentu
  * 
  * Fungsi ini memeriksa apakah tetromino aktif akan bertabrakan dengan
  * blok lain atau batas papan jika digerakkan ke offset yang diberikan.
  * 
  * @param system Pointer ke BlockSystem (informasi tetromino aktif)
  * @param board Pointer ke GameBoard (informasi papan)
  * @param offset_x Offset X yang akan diperiksa
  * @param offset_y Offset Y yang akan diperiksa
  * @return true jika terjadi tabrakan, false jika tidak
  */
 bool collision_check(const BlockSystem *system, const GameBoard *board, int offset_x, int offset_y)
 {
     const int(*shape)[4] = TETROMINOS[system->current.type][system->current.rotation];
     int block_x = system->current.x + offset_x;
     int block_y = system->current.y + offset_y;
 
     // Periksa setiap sel dari tetromino
     for (int y = 0; y < 4; y++)
     {
         for (int x = 0; x < 4; x++)
         {
             // Jika sel ini terisi pada tetromino
             if (shape[y][x])
             {
                 int board_x = block_x + x;
                 int board_y = block_y + y;
 
                 // Periksa apakah melewati batas
                 if (board_x < 0 || board_x >= BOARD_WIDTH || board_y >= BOARD_HEIGHT)
                 {
                     return true; // Tabrakan dengan batas
                 }
 
                 // Izinkan tetromino melampaui batas atas papan
                 if (board_y < 0)
                 {
                     continue;
                 }
 
                 // Periksa apakah sel sudah terisi di papan
                 if (board->cells[board_y][board_x] != 0)
                 {
                     return true; // Tabrakan dengan blok yang ada
                 }
             }
         }
     }
 
     return false; // Tidak ada tabrakan
 }
 
 /**
  * Kunci tetromino aktif ke posisinya pada papan
  * 
  * Fungsi ini menambahkan tetromino aktif ke papan permainan,
  * mengubah status sel-sel papan yang sesuai.
  * 
  * @param system Pointer ke BlockSystem (informasi tetromino aktif)
  * @param board Pointer ke GameBoard yang akan diubah
  */
 void collision_lock_tetromino(const BlockSystem *system, GameBoard *board)
 {
     const int(*shape)[4] = TETROMINOS[system->current.type][system->current.rotation];
     int block_type = system->current.type;
     int block_x = system->current.x;
     int block_y = system->current.y;
 
     // Letakkan blok tetromino ke papan
     for (int y = 0; y < 4; y++)
     {
         for (int x = 0; x < 4; x++)
         {
             if (shape[y][x])
             {
                 int board_x = block_x + x;
                 int board_y = block_y + y;
 
                 // Pastikan kita berada dalam batas
                 if (board_x >= 0 && board_x < BOARD_WIDTH &&
                     board_y >= 0 && board_y < BOARD_HEIGHT)
                 {
                     board->cells[board_y][board_x] = block_type + 1; // +1 untuk menghindari 0 (kosong)
                 }
             }
         }
     }
 }
 
 /**
  * Memeriksa dan menghapus baris yang sudah lengkap
  * 
  * Fungsi ini memeriksa setiap baris di papan dan menghapus
  * baris yang sudah lengkap terisi, lalu menggeser baris-baris di atasnya.
  * 
  * @param board Pointer ke GameBoard yang akan diperiksa dan diubah
  * @return Jumlah baris yang dihapus
  */
 int collision_clear_lines(GameBoard *board)
 {
     int lines_cleared = 0;
 
     // Periksa setiap baris dari bawah ke atas
     for (int y = BOARD_HEIGHT - 1; y >= 0; y--)
     {
         bool is_line_full = true;
 
         // Periksa apakah setiap sel di baris ini terisi
         for (int x = 0; x < BOARD_WIDTH; x++)
         {
             if (board->cells[y][x] == 0)
             {
                 is_line_full = false;
                 break;
             }
         }
 
         if (is_line_full)
         {
             lines_cleared++;
 
             // Geser semua baris di atas ini ke bawah
             for (int move_y = y; move_y > 0; move_y--)
             {
                 for (int x = 0; x < BOARD_WIDTH; x++)
                 {
                     board->cells[move_y][x] = board->cells[move_y - 1][x];
                 }
             }
 
             // Kosongkan baris paling atas
             for (int x = 0; x < BOARD_WIDTH; x++)
             {
                 board->cells[0][x] = 0;
             }
 
             // Karena kita menggeser semua baris, kita perlu memeriksa baris ini lagi
             y++;
         }
     }
 
     return lines_cleared;
 }
 
 /**
  * Gerakkan tetromino ke kiri jika memungkinkan
  * 
  * @param system Pointer ke BlockSystem yang akan diubah
  * @param board Pointer ke GameBoard untuk memeriksa tabrakan
  * @return true jika berhasil bergerak, false jika tidak
  */
 bool collision_move_left(BlockSystem *system, GameBoard *board)
 {
     if (!collision_check(system, board, -1, 0))
     {
         system->current.x -= 1;
         return true;
     }
     return false;
 }
 
 /**
  * Gerakkan tetromino ke kanan jika memungkinkan
  * 
  * @param system Pointer ke BlockSystem yang akan diubah
  * @param board Pointer ke GameBoard untuk memeriksa tabrakan
  * @return true jika berhasil bergerak, false jika tidak
  */
 bool collision_move_right(BlockSystem *system, GameBoard *board)
 {
     if (!collision_check(system, board, 1, 0))
     {
         system->current.x += 1;
         return true;
     }
     return false;
 }
 
 /**
  * Lakukan soft drop (turunkan satu sel jika memungkinkan)
  * 
  * @param system Pointer ke BlockSystem yang akan diubah
  * @param board Pointer ke GameBoard untuk memeriksa tabrakan
  * @return true jika berhasil bergerak, false jika tidak
  */
 bool collision_soft_drop(BlockSystem *system, GameBoard *board)
 {
     if (!collision_check(system, board, 0, 1))
     {
         system->current.y += 1;
         return true;
     }
     return false;
 }
 
 /**
  * Temukan di mana tetromino akan mendarat jika dijatuhkan langsung ke bawah
  * 
  * @param system Pointer ke BlockSystem dengan tetromino aktif
  * @param board Pointer ke GameBoard untuk memeriksa tabrakan
  * @return Jarak drop (berapa sel ke bawah)
  */
 int collision_get_drop_position(const BlockSystem *system, const GameBoard *board)
 {
     int drop_distance = 0;
 
     // Terus periksa ke bawah sampai menemukan tabrakan
     while (!collision_check(system, board, 0, drop_distance + 1))
     {
         drop_distance++;
     }
 
     return drop_distance;
 }
 
 /**
  * Hard drop tetromino aktif (jatuhkan langsung ke posisi paling bawah)
  * 
  * @param system Pointer ke BlockSystem yang akan diubah
  * @param board Pointer ke GameBoard untuk memeriksa tabrakan
  */
 void collision_hard_drop(BlockSystem *system, GameBoard *board)
 {
     int drop_distance = collision_get_drop_position(system, board);
     system->current.y += drop_distance;
 }
 
 /**
  * Mencoba untuk merotasi tetromino, menggunakan SRS wall kicks jika perlu
  * 
  * Fungsi ini menerapkan Super Rotation System (SRS) untuk mencoba
  * merotasi tetromino bahkan jika ada rintangan, dengan menggeser posisi
  * tetromino untuk menghindari tabrakan.
  * 
  * @param system Pointer ke BlockSystem yang akan diubah
  * @param board Pointer ke GameBoard untuk memeriksa tabrakan
  * @param direction Arah rotasi (1 = searah jarum jam, -1 = berlawanan jarum jam)
  * @return true jika rotasi berhasil, false jika tidak
  */
 bool rotation_rotate(BlockSystem *system, GameBoard *board, int direction)
 {
     int original_rotation = system->current.rotation;
     int new_rotation = (original_rotation + direction + 4) % 4;
     int block_type = system->current.type;
 
     // Simpan posisi asli
     int original_x = system->current.x;
     int original_y = system->current.y;
 
     // Set rotasi baru (sementara)
     system->current.rotation = new_rotation;
 
     // Tes posisi dasar tanpa wall kicks
     if (!collision_check(system, board, 0, 0))
     {
         // Rotasi berhasil tanpa wall kick
         return true;
     }
 
     // Blok O tidak memerlukan wall kicks (rotasinya di sekeliling pusatnya)
     if (block_type == 3)
     {
         // Reset ke status awal
         system->current.rotation = original_rotation;
         return false;
     }
 
     // Tentukan data wall kick yang digunakan berdasarkan tipe tetromino
     const int(*wall_kick_data)[5][2];
     if (block_type == 0)
     { // Tetromino I
         wall_kick_data = WALL_KICK_I;
     }
     else
     { // Tetromino J, L, O, S, T, Z
         wall_kick_data = WALL_KICK_JLOSTZ;
     }
 
     // Coba setiap wall kick test
     int kick_index = direction == 1 ? original_rotation : new_rotation;
 
     for (int test = 0; test < 5; test++)
     {
         int kick_x = wall_kick_data[kick_index][test][0];
         int kick_y = wall_kick_data[kick_index][test][1];
 
         // Terapkan offset wall kick
         system->current.x = original_x + kick_x;
         system->current.y = original_y - kick_y; // Negatif karena sumbu y terbalik
 
         // Tes apakah posisi ini berhasil
         if (!collision_check(system, board, 0, 0))
         {
             // Kita menemukan posisi valid dengan wall kick
             return true;
         }
     }
 
     // Jika kita sampai di sini, tidak ada wall kick yang berhasil - kembalikan rotasi
     system->current.rotation = original_rotation;
     system->current.x = original_x;
     system->current.y = original_y;
     return false;
 }
 
 /**
  * Rotasi tetromino aktif searah jarum jam
  * 
  * @param system Pointer ke BlockSystem yang akan diubah
  * @param board Pointer ke GameBoard untuk memeriksa tabrakan
  * @return true jika rotasi berhasil, false jika tidak
  */
 bool rotation_rotate_clockwise(BlockSystem *system, GameBoard *board)
 {
     return rotation_rotate(system, board, 1);
 }
 
 /**
  * Rotasi tetromino aktif berlawanan arah jarum jam
  * 
  * @param system Pointer ke BlockSystem yang akan diubah
  * @param board Pointer ke GameBoard untuk memeriksa tabrakan
  * @return true jika rotasi berhasil, false jika tidak
  */
 bool rotation_rotate_counterclockwise(BlockSystem *system, GameBoard *board)
 {
     return rotation_rotate(system, board, -1);
 }
 
 /**
  * Fungsi untuk menahan (hold) tetromino saat ini dan mendapatkan
  * tetromino yang ditahan sebelumnya atau tetromino berikutnya
  * 
  * @param system Pointer ke BlockSystem yang akan diubah
  */
 void blocks_hold(BlockSystem *system)
 {
     if (!system->can_hold)
     {
         return; // Tidak bisa hold dua kali berturut-turut
     }
 
     int temp = system->current.type;
 
     if (system->hold_block == -1)
     {
         // Hold pertama, ambil blok baru dari antrian
         system->hold_block = temp;
         blocks_spawn_new(system);
     }
     else
     {
         // Tukar dengan blok yang ditahan
         system->current.type = system->hold_block;
         system->hold_block = temp;
 
         // Reset posisi dan rotasi
         system->current.rotation = 0;
         system->current.x = (BOARD_WIDTH - 4) / 2;
         system->current.y = 0;
     }
 
     // Tandai bahwa kita sudah menggunakan hold
     system->can_hold = false;
 }
 
 /**
  * Munculkan tetromino baru dari antrian next
  * 
  * @param system Pointer ke BlockSystem yang akan diubah
  */
 void blocks_spawn_new(BlockSystem *system)
 {
     // Ambil tetromino berikutnya dari antrian
     system->current.type = system->next_queue[0];
     system->current.rotation = 0;
     system->current.x = (BOARD_WIDTH - 4) / 2;
     system->current.y = 0;
 
     // Geser antrian ke depan
     for (int i = 0; i < NEXT_QUEUE_SIZE - 1; i++)
     {
         system->next_queue[i] = system->next_queue[i + 1];
     }
 
     // Isi slot terakhir dengan tetromino acak baru
     system->next_queue[NEXT_QUEUE_SIZE - 1] = rand() % 7;
 
     // Reset flag hold
     system->can_hold = true;
 }
 
 /**
  * Render papan permainan
  * 
  * @param renderer Renderer SDL yang digunakan
  * @param board Pointer ke GameBoard yang akan dirender
  */
 void render_board(SDL_Renderer *renderer, const GameBoard *board)
 {
     // Render latar belakang papan
     SDL_SetRenderDrawColor(renderer, 40, 40, 40, 255);
     SDL_Rect boardRect = {BOARD_X, BOARD_Y, BOARD_WIDTH * BLOCK_SIZE, (BOARD_HEIGHT - 2) * BLOCK_SIZE};
     SDL_RenderFillRect(renderer, &boardRect);
 
     // Render garis grid (opsional)
     SDL_SetRenderDrawColor(renderer, 60, 60, 60, 255);
     for (int x = 0; x <= BOARD_WIDTH; x++)
     {
         int pos_x = BOARD_X + x * BLOCK_SIZE;
         SDL_RenderDrawLine(renderer, pos_x, BOARD_Y, pos_x, BOARD_Y + (BOARD_HEIGHT - 2) * BLOCK_SIZE);
     }
     for (int y = 0; y <= BOARD_HEIGHT - 2; y++)
     {
         int pos_y = BOARD_Y + y * BLOCK_SIZE;
         SDL_RenderDrawLine(renderer, BOARD_X, pos_y, BOARD_X + BOARD_WIDTH * BLOCK_SIZE, pos_y);
     }
 
     // Render blok-blok yang sudah ditempatkan
     for (int y = 2; y < BOARD_HEIGHT; y++) // Mulai dari 2 untuk mengabaikan area tersembunyi
     {
         for (int x = 0; x < BOARD_WIDTH; x++)
         {
             if (board->cells[y][x] != 0)
             {
                 // Ambil warna berdasarkan tipe tetromino
                 int block_type = board->cells[y][x] - 1;
                 SDL_Color color = TETROMINO_COLORS[block_type];
 
                 // Set warna dan render blok
                 SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);
                 SDL_Rect blockRect = {
                     BOARD_X + x * BLOCK_SIZE + 1,
                     BOARD_Y + (y - 2) * BLOCK_SIZE + 1,
                     BLOCK_SIZE - 2,
                     BLOCK_SIZE - 2
                 };
                 SDL_RenderFillRect(renderer, &blockRect);
 
                 // Tambahkan highlight dan shadow untuk efek 3D
                 SDL_SetRenderDrawColor(renderer, 
                     (color.r + 80 > 255) ? 255 : color.r + 80, 
                     (color.g + 80 > 255) ? 255 : color.g + 80, 
                     (color.b + 80 > 255) ? 255 : color.b + 80, 
                     255);
                 
                 // Highlight (sisi atas dan kiri)
                 SDL_RenderDrawLine(renderer, blockRect.x, blockRect.y, blockRect.x + blockRect.w - 1, blockRect.y);
                 SDL_RenderDrawLine(renderer, blockRect.x, blockRect.y, blockRect.x, blockRect.y + blockRect.h - 1);
 
                 // Shadow (sisi bawah dan kanan)
                 SDL_SetRenderDrawColor(renderer, 
                     (color.r - 40 < 0) ? 0 : color.r - 40, 
                     (color.g - 40 < 0) ? 0 : color.g - 40, 
                     (color.b - 40 < 0) ? 0 : color.b - 40, 
                     255);
                 SDL_RenderDrawLine(renderer, blockRect.x, blockRect.y + blockRect.h - 1, blockRect.x + blockRect.w - 1, blockRect.y + blockRect.h - 1);
                 SDL_RenderDrawLine(renderer, blockRect.x + blockRect.w - 1, blockRect.y, blockRect.x + blockRect.w - 1, blockRect.y + blockRect.h - 1);
             }
         }
     }
 }
 
 /**
  * Render tetromino yang sedang aktif
  * 
  * @param renderer Renderer SDL yang digunakan
  * @param system Pointer ke BlockSystem dengan tetromino aktif
  */
 void render_current_tetromino(SDL_Renderer *renderer, const BlockSystem *system)
 {
     const int(*shape)[4] = TETROMINOS[system->current.type][system->current.rotation];
     SDL_Color color = TETROMINO_COLORS[system->current.type];
 
     // Set warna sesuai tipe tetromino
     SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);
 
     // Render setiap blok dari tetromino
     for (int y = 0; y < 4; y++)
     {
         for (int x = 0; x < 4; x++)
         {
             if (shape[y][x])
             {
                 int board_x = system->current.x + x;
                 int board_y = system->current.y + y;
 
                 // Hanya render jika dalam area yang terlihat
                 if (board_y >= 2)
                 {
                     SDL_Rect blockRect = {
                         BOARD_X + board_x * BLOCK_SIZE + 1,
                         BOARD_Y + (board_y - 2) * BLOCK_SIZE + 1,
                         BLOCK_SIZE - 2,
                         BLOCK_SIZE - 2
                     };
                     SDL_RenderFillRect(renderer, &blockRect);
 
                     // Tambahkan highlight dan shadow untuk efek 3D
                     SDL_SetRenderDrawColor(renderer, 
                         (color.r + 80 > 255) ? 255 : color.r + 80, 
                         (color.g + 80 > 255) ? 255 : color.g + 80, 
                         (color.b + 80 > 255) ? 255 : color.b + 80, 
                         255);
                     
                     // Highlight (sisi atas dan kiri)
                     SDL_RenderDrawLine(renderer, blockRect.x, blockRect.y, blockRect.x + blockRect.w - 1, blockRect.y);
                     SDL_RenderDrawLine(renderer, blockRect.x, blockRect.y, blockRect.x, blockRect.y + blockRect.h - 1);
 
                     // Shadow (sisi bawah dan kanan)
                     SDL_SetRenderDrawColor(renderer, 
                         (color.r - 40 < 0) ? 0 : color.r - 40, 
                         (color.g - 40 < 0) ? 0 : color.g - 40, 
                         (color.b - 40 < 0) ? 0 : color.b - 40, 
                         255);
                     SDL_RenderDrawLine(renderer, blockRect.x, blockRect.y + blockRect.h - 1, blockRect.x + blockRect.w - 1, blockRect.y + blockRect.h - 1);
                     SDL_RenderDrawLine(renderer, blockRect.x + blockRect.w - 1, blockRect.y, blockRect.x + blockRect.w - 1, blockRect.y + blockRect.h - 1);
                 }
             }
         }
     }
 }
 
 /**
  * Render ghost piece (bayangan di mana tetromino akan mendarat)
  * 
  * @param renderer Renderer SDL yang digunakan
  * @param system Pointer ke BlockSystem dengan tetromino aktif
  * @param board Pointer ke GameBoard untuk menentukan posisi landing
  */
 void render_ghost_piece(SDL_Renderer *renderer, const BlockSystem *system, const GameBoard *board)
 {
     // Salin BlockSystem saat ini
     BlockSystem ghost_system = *system;
     
     // Temukan posisi drop
     int drop_distance = collision_get_drop_position(system, board);
     ghost_system.current.y += drop_distance;
 
     const int(*shape)[4] = TETROMINOS[ghost_system.current.type][ghost_system.current.rotation];
     SDL_Color color = TETROMINO_COLORS[ghost_system.current.type];
 
     // Set warna transparan untuk ghost piece
     SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, 128);
 
     // Render setiap blok dari ghost tetromino
     for (int y = 0; y < 4; y++)
     {
         for (int x = 0; x < 4; x++)
         {
             if (shape[y][x])
             {
                 int board_x = ghost_system.current.x + x;
                 int board_y = ghost_system.current.y + y;
 
                 // Hanya render jika dalam area yang terlihat
                 if (board_y >= 2)
                 {
                     SDL_Rect blockRect = {
                         BOARD_X + board_x * BLOCK_SIZE + 1,
                         BOARD_Y + (board_y - 2) * BLOCK_SIZE + 1,
                         BLOCK_SIZE - 2,
                         BLOCK_SIZE - 2
                     };
                     
                     // Hanya render garis batas untuk ghost piece
                     SDL_RenderDrawRect(renderer, &blockRect);
                 }
             }
         }
     }
 }
 
 /**
  * Render preview tetromino selanjutnya di area next
  * 
  * @param renderer Renderer SDL yang digunakan
  * @param system Pointer ke BlockSystem dengan antrian next
  */
 void render_next_preview(SDL_Renderer *renderer, const BlockSystem *system)
 {
     // Render area preview
     SDL_SetRenderDrawColor(renderer, 40, 40, 40, 255);
     SDL_Rect previewRect = {PREVIEW_X, PREVIEW_Y, 6 * PREVIEW_BLOCK_SIZE, 15 * PREVIEW_BLOCK_SIZE};
     SDL_RenderFillRect(renderer, &previewRect);
 
     // Render teks "NEXT"
     // (Dalam implementasi lengkap, ini akan menggunakan SDL_ttf)
     
     // Render setiap tetromino dalam antrian next
     for (int i = 0; i < NEXT_QUEUE_SIZE; i++)
     {
         int block_type = system->next_queue[i];
         const int(*shape)[4] = TETROMINOS[block_type][0]; // Selalu gunakan rotasi 0 untuk preview
         SDL_Color color = TETROMINO_COLORS[block_type];
 
         // Set warna sesuai tipe tetromino
         SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);
 
         // Tentukan posisi offset untuk tetromino ini
         int offset_y = i * 5 * PREVIEW_BLOCK_SIZE;
         
         // Render setiap blok dari tetromino
         for (int y = 0; y < 4; y++)
         {
             for (int x = 0; x < 4; x++)
             {
                 if (shape[y][x])
                 {
                     SDL_Rect blockRect = {
                         PREVIEW_X + (x + 1) * PREVIEW_BLOCK_SIZE,
                         PREVIEW_Y + offset_y + (y + 1) * PREVIEW_BLOCK_SIZE,
                         PREVIEW_BLOCK_SIZE - 2,
                         PREVIEW_BLOCK_SIZE - 2
                     };
                     SDL_RenderFillRect(renderer, &blockRect);
 
                     // Tambahkan highlight dan shadow untuk efek 3D
                     SDL_SetRenderDrawColor(renderer, 
                         (color.r + 80 > 255) ? 255 : color.r + 80, 
                         (color.g + 80 > 255) ? 255 : color.g + 80, 
                         (color.b + 80 > 255) ? 255 : color.b + 80, 
                         255);
                     
                     // Highlight (sisi atas dan kiri)
                     SDL_RenderDrawLine(renderer, blockRect.x, blockRect.y, blockRect.x + blockRect.w - 1, blockRect.y);
                     SDL_RenderDrawLine(renderer, blockRect.x, blockRect.y, blockRect.x, blockRect.y + blockRect.h - 1);
 
                     // Shadow (sisi bawah dan kanan)
                     SDL_SetRenderDrawColor(renderer, 
                         (color.r - 40 < 0) ? 0 : color.r - 40, 
                         (color.g - 40 < 0) ? 0 : color.g - 40, 
                         (color.b - 40 < 0) ? 0 : color.b - 40, 
                         255);
                     SDL_RenderDrawLine(renderer, blockRect.x, blockRect.y + blockRect.h - 1, blockRect.x + blockRect.w - 1, blockRect.y + blockRect.h - 1);
                     SDL_RenderDrawLine(renderer, blockRect.x + blockRect.w - 1, blockRect.y, blockRect.x + blockRect.w - 1, blockRect.y + blockRect.h - 1);
                 }
             }
         }
     }
 }
 
 /**
  * Render tetromino yang disimpan di area hold
  * 
  * @param renderer Renderer SDL yang digunakan
  * @param system Pointer ke BlockSystem dengan informasi hold
  */
 void render_hold_preview(SDL_Renderer *renderer, const BlockSystem *system)
 {
     // Render area hold
     SDL_SetRenderDrawColor(renderer, 40, 40, 40, 255);
     SDL_Rect holdRect = {HOLD_X, HOLD_Y, 6 * PREVIEW_BLOCK_SIZE, 6 * PREVIEW_BLOCK_SIZE};
     SDL_RenderFillRect(renderer, &holdRect);
 
     // Render teks "HOLD"
     // (Dalam implementasi lengkap, ini akan menggunakan SDL_ttf)
     
     // Jika ada blok yang ditahan, render preview-nya
     if (system->hold_block != -1)
     {
         int block_type = system->hold_block;
         const int(*shape)[4] = TETROMINOS[block_type][0]; // Selalu gunakan rotasi 0 untuk preview
         SDL_Color color = TETROMINO_COLORS[block_type];
 
         // Terapkan efek abu-abu jika tidak bisa hold
         if (!system->can_hold)
         {
             color.r = color.r / 2;
             color.g = color.g / 2;
             color.b = color.b / 2;
         }
 
         // Set warna sesuai tipe tetromino
         SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);
 
         // Render setiap blok dari tetromino
         for (int y = 0; y < 4; y++)
         {
             for (int x = 0; x < 4; x++)
             {
                 if (shape[y][x])
                 {
                     SDL_Rect blockRect = {
                         HOLD_X + (x + 1) * PREVIEW_BLOCK_SIZE,
                         HOLD_Y + (y + 1) * PREVIEW_BLOCK_SIZE,
                         PREVIEW_BLOCK_SIZE - 2,
                         PREVIEW_BLOCK_SIZE - 2
                     };
                     SDL_RenderFillRect(renderer, &blockRect);
 
                     // Tambahkan highlight dan shadow untuk efek 3D
                     SDL_SetRenderDrawColor(renderer, 
                         (color.r + 80 > 255) ? 255 : color.r + 80, 
                         (color.g + 80 > 255) ? 255 : color.g + 80, 
                         (color.b + 80 > 255) ? 255 : color.b + 80, 
                         255);
                     
                     // Highlight (sisi atas dan kiri)
                     SDL_RenderDrawLine(renderer, blockRect.x, blockRect.y, blockRect.x + blockRect.w - 1, blockRect.y);
                     SDL_RenderDrawLine(renderer, blockRect.x, blockRect.y, blockRect.x, blockRect.y + blockRect.h - 1);
 
                     // Shadow (sisi bawah dan kanan)
                     SDL_SetRenderDrawColor(renderer, 
                         (color.r - 40 < 0) ? 0 : color.r - 40, 
                         (color.g - 40 < 0) ? 0 : color.g - 40, 
                         (color.b - 40 < 0) ? 0 : color.b - 40, 
                         255);
                     SDL_RenderDrawLine(renderer, blockRect.x, blockRect.y + blockRect.h - 1, blockRect.x + blockRect.w - 1, blockRect.y + blockRect.h - 1);
                     SDL_RenderDrawLine(renderer, blockRect.x + blockRect.w - 1, blockRect.y, blockRect.x + blockRect.w - 1, blockRect.y + blockRect.h - 1);
                 }
             }
         }
     }
 }
 
 // Main program
 int main(int argc, char *argv[])
 {
    (void)argc;
    (void)argv;
     // Inisialisasi SDL
     if (SDL_Init(SDL_INIT_VIDEO) < 0)
     {
         printf("SDL could not initialize! SDL_Error: %s\n", SDL_GetError());
         return 1;
     }
 
     // Buat window
     SDL_Window *window = SDL_CreateWindow("Tetris Game", 
                                          SDL_WINDOWPOS_UNDEFINED, 
                                          SDL_WINDOWPOS_UNDEFINED,
                                          WINDOW_WIDTH, 
                                          WINDOW_HEIGHT, 
                                          SDL_WINDOW_SHOWN);
     if (window == NULL)
     {
         printf("Window could not be created! SDL_Error: %s\n", SDL_GetError());
         SDL_Quit();
         return 1;
     }
 
     // Buat renderer
     SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
     if (renderer == NULL)
     {
         printf("Renderer could not be created! SDL_Error: %s\n", SDL_GetError());
         SDL_DestroyWindow(window);
         SDL_Quit();
         return 1;
     }
 
     // Inisialisasi game state
     GameBoard board;
     BlockSystem blocks;
 
     collision_init_board(&board);
     blocks_init(&blocks);
 
     // Game loop variables
     bool quit = false;
     SDL_Event e;
     
     // Game timing variables
     Uint32 last_drop_time = SDL_GetTicks();
     Uint32 drop_delay = 1000; // Awalnya 1 detik per drop
     int level = 1;
     int score = 0;
     int lines_cleared_total = 0;
 
     // Flag untuk menandai kapan blok perlu dikunci
     bool lock_block = false;
     Uint32 lock_delay_start = 0;
     const Uint32 LOCK_DELAY = 500; // 500ms delay sebelum mengunci
 
     // Game loop
     while (!quit)
     {
         // Handle events
         while (SDL_PollEvent(&e) != 0)
         {
             if (e.type == SDL_QUIT)
             {
                 quit = true;
             }
             else if (e.type == SDL_KEYDOWN)
             {
                 switch (e.key.keysym.sym)
                 {
                     case SDLK_LEFT:
                         collision_move_left(&blocks, &board);
                         break;
                     case SDLK_RIGHT:
                         collision_move_right(&blocks, &board);
                         break;
                     case SDLK_DOWN:
                         collision_soft_drop(&blocks, &board);
                         break;
                     case SDLK_UP:
                         rotation_rotate_clockwise(&blocks, &board);
                         break;
                     case SDLK_z:
                         rotation_rotate_counterclockwise(&blocks, &board);
                         break;
                     case SDLK_SPACE:
                         collision_hard_drop(&blocks, &board);
                         lock_block = true;
                         break;
                     case SDLK_c:
                         blocks_hold(&blocks);
                         break;
                     case SDLK_ESCAPE:
                         quit = true;
                         break;
                 }
             }
         }
 
         // Auto drop tetromino berdasarkan timer
         Uint32 current_time = SDL_GetTicks();
         if (current_time - last_drop_time > drop_delay)
         {
             if (!collision_soft_drop(&blocks, &board))
             {
                 // Jika tidak bisa turun lagi, mulai lock delay
                 if (!lock_block)
                 {
                     lock_block = true;
                     lock_delay_start = current_time;
                 }
             }
             last_drop_time = current_time;
         }
 
         // Check lock delay
         if (lock_block && current_time - lock_delay_start > LOCK_DELAY)
         {
             // Kunci tetromino
             collision_lock_tetromino(&blocks, &board);
             
             // Periksa baris yang lengkap
             int lines_cleared = collision_clear_lines(&board);
             lines_cleared_total += lines_cleared;
 
             // Update skor
             switch (lines_cleared)
             {
                 case 1: score += 100 * level; break;
                 case 2: score += 300 * level; break;
                 case 3: score += 500 * level; break;
                 case 4: score += 800 * level; break; // Tetris!
             }
 
             // Update level setiap 10 baris
             level = 1 + lines_cleared_total / 10;
             
             // Update kecepatan drop
             drop_delay = 1000 - (level - 1) * 50;
             if (drop_delay < 100) drop_delay = 100; // Minimal 0.1 detik
 
             // Spawn tetromino baru
             blocks_spawn_new(&blocks);
             
             // Reset lock flag
             lock_block = false;
             
             // Cek game over condition (jika tetromino baru tumpang tindih dengan blok lain)
             if (collision_check(&blocks, &board, 0, 0))
             {
                 printf("Game Over! Score: %d, Level: %d\n", score, level);
                 quit = true;
             }
         }
 
         // Render
         SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
         SDL_RenderClear(renderer);
 
         // Render semua elemen game
         render_board(renderer, &board);
         render_ghost_piece(renderer, &blocks, &board);
         render_current_tetromino(renderer, &blocks);
         render_next_preview(renderer, &blocks);
         render_hold_preview(renderer, &blocks);
 
         // TODO: Render skor dan level
         // (Dalam implementasi lengkap, ini akan menggunakan SDL_ttf)
 
         SDL_RenderPresent(renderer);
         
         // Batasi frame rate
         SDL_Delay(16); // Sekitar 60 FPS
     }
 
     // Cleanup
     SDL_DestroyRenderer(renderer);
     SDL_DestroyWindow(window);
     SDL_Quit();
 
     return 0;
 }