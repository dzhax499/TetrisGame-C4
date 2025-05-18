//Nama file : blocks.c
//Deskripsi : Implementasi blok-blok game Tetris
//Oleh      : Dzakit Tsabit 241511071
#include <string.h>
#include "include/blocks.h"
#include <stdlib.h>
#include <stdio.h>
#include "raylib.h"   

// Warna untuk setiap jenis blok Tetris
// static const Color TETROMINO_COLORS[7] = {
//     {0, 255, 255, 255},   // Cyan (I)
//     {0, 0, 255, 255},     // Blue (J)
//     {255, 165, 0, 255},   // Orange (L)
//     {255, 255, 0, 255},   // Yellow (O)
//     {0, 255, 0, 255},     // Green (S)
//     {128, 0, 128, 255},   // Purple (T)
//     {255, 0, 0, 255}      // Red (Z)
// };

static const int WallKickTests[4][5][2] = {
    // 0 -> R (Dari posisi awal ke kanan)
    {{0,0}, {-1,0}, {-1,1}, {0,-2}, {-1,-2}},
    // R -> 2 (Dari kanan ke posisi terbalik)
    {{0,0}, {1,0}, {1,-1}, {0,2}, {1,2}},
    // 2 -> L (Dari posisi terbalik ke kiri)
    {{0,0}, {1,0}, {1,1}, {0,-2}, {1,-2}},
    // L -> 0 (Dari kiri kembali ke posisi awal)
    {{0,0}, {-1,0}, {-1,-1}, {0,2}, {-1,2}}
};


void InitBlocks0(void) {
    // Inisialisasi random seed
    srand(time(NULL));
}

TetrisBlock GenerateRandomBlock(void) {
    TetrisBlock block;
    block.type = rand() % 7;
    block.rotation = 0;
    
    // Sesuaikan posisi awal agar blok muncul di bagian atas papan
    block.x = BOARD_WIDTH / 2 - 2;
    block.y = -1;  // Mulai di atas papan
    
    // Salin bentuk blok dari template
    for (int y = 0; y < 4; y++) {
        for (int x = 0; x < 4; x++) {
            block.shape[y][x] = TETROMINO_SHAPES[block.type][0][y][x];
        }
    }
    
    // Tetapkan warna
    static const Color TETROMINO_COLORS[7] = {
        SKYBLUE,    // I
        DARKBLUE,   // J
        ORANGE,     // L
        YELLOW,     // O
        LIME,       // S
        PURPLE,     // T
        RED         // Z
    };
    block.color = TETROMINO_COLORS[block.type];
    
    return block;
}

// Fungsi untuk memutar blok dengan sistem wall kick
bool RotateBlockWithWallKick(TetrisBlock* block, TetrisBoard* board) {
    int initialRotation = block->rotation;
    int newRotation = (block->rotation + 1) % 4;  // Rotasi searah jarum jam

    // Coba rotasi normal tanpa wall kick
    if (IsValidBlockPosition(block, board, block->x, block->y, newRotation)) {
        block->rotation = newRotation;
        return true;
    }

    // Coba wall kick dengan 5 kemungkinan pergeseran
    for (int testIndex = 0; testIndex < 5; testIndex++) {
        int kickX = WallKickTests[initialRotation][testIndex][0];
        int kickY = WallKickTests[initialRotation][testIndex][1];

        // Jika posisi dengan wall kick valid, terapkan pergeseran
        if (IsValidBlockPosition(block, board, block->x + kickX, block->y + kickY, newRotation)) {
            block->x += kickX;
            block->y += kickY;
            block->rotation = newRotation;
            return true;
        }
    }

    return false; // Rotasi gagal dilakukan
}

// Fungsi untuk menghitung seberapa jauh blok bisa jatuh sebelum bertabrakan
int CalculateDropDistance(TetrisBlock *block, TetrisBoard *board) {
    int dropDistance = 0; // Inisialisasi jarak jatuh

    // Simulasikan pergerakan blok ke bawah satu per satu
    while (IsValidBlockPosition(block, board, block->x, block->y + dropDistance + 1, block->rotation)) {
        dropDistance++; // Tambahkan jarak jatuh jika posisi masih valid
    }

    return dropDistance; // Kembalikan total jarak jatuh yang memungkinkan
}

// Fungsi untuk memeriksa apakah blok dapat ditempatkan pada posisi tertentu tanpa menabrak batas papan atau blok lain
bool IsValidBlockPosition(TetrisBlock *block, TetrisBoard *board, int testX, int testY, int testRotation) {
    // Loop melalui matriks 4x4 yang merepresentasikan bentuk tetromino dalam rotasi tertentu
    for (int y = 0; y < 4; y++) {
        for (int x = 0; x < 4; x++) {
            // Lewati sel kosong (jika tidak ada bagian dari tetromino pada posisi ini)
            if (TETROMINO_SHAPES[block->type][testRotation][y][x] == 0) continue;

            // Hitung koordinat absolut di papan berdasarkan posisi uji
            int boardX = testX + x;
            int boardY = testY + y;

            // Cek apakah posisi berada di luar batas kiri atau kanan papan
            if (boardX < 0 || boardX >= BOARD_WIDTH) {
                return false; // Tidak valid jika keluar dari batas horizontal
            }

            // Cek apakah posisi berada di luar batas bawah papan
            if (boardY >= BOARD_HEIGHT) {
                return false; // Tidak valid jika melewati bagian bawah papan
            }

            // Jika posisi masih di atas papan (sebelum mencapai baris 0), izinkan pergerakan
            // tetapi tidak perlu melakukan validasi tabrakan dengan grid papan
            if (boardY < 0) continue;

            // Cek apakah posisi bertabrakan dengan blok lain yang sudah ada di papan
            if (board->grid[boardY][boardX] != BLOCK_EMPTY) {
                return false; // Tidak valid jika menabrak blok lain
            }
        }
    }

    // Jika semua pengecekan lolos, maka posisi dianggap valid
    return true;
}


// Fungsi untuk memutar blok ke rotasi berikutnya jika memungkinkan
bool RotateBlock(TetrisBlock *block, TetrisBoard *board) {
    // Hitung rotasi baru (mod 4 untuk memastikan tetap dalam rentang 0-3)
    int newRotation = (block->rotation + 1) % 4;

    // Cek apakah rotasi baru valid di posisi saat ini
    if (IsValidBlockPosition(block, board, block->x, block->y, newRotation)) {
        // Jika valid, perbarui rotasi blok
        block->rotation = newRotation;

        // Salin bentuk baru sesuai rotasi yang diperbarui
        for (int y = 0; y < 4; y++) {
            for (int x = 0; x < 4; x++) {
                block->shape[y][x] = TETROMINO_SHAPES[block->type][newRotation][y][x];
            }
        }

        return true; // Rotasi berhasil
    }

    return false; // Rotasi gagal karena tidak valid
}


// Fungsi untuk menggerakkan blok secara horizontal (ke kiri atau kanan)
bool MoveBlockHorizontal(TetrisBlock *block, TetrisBoard *board, int dx) {
    // Hitung posisi baru setelah pergerakan horizontal
    int newX = block->x + dx;

    // Cek apakah posisi baru valid (tidak menabrak batas atau blok lain)
    if (IsValidBlockPosition(block, board, newX, block->y, block->rotation)) {
        block->x = newX; // Perbarui posisi X blok
        return true; // Gerakan berhasil
    }
    return false; // Gerakan gagal
}

// Fungsi untuk menggerakkan blok turun satu langkah
bool MoveBlockDown(TetrisBlock *block, TetrisBoard *board) {
    // Hitung posisi baru setelah pergerakan ke bawah
    int newY = block->y + 1;

    // Cek apakah posisi baru valid (tidak menabrak batas bawah atau blok lain)
    if (IsValidBlockPosition(block, board, block->x, newY, block->rotation)) {
        block->y = newY; // Perbarui posisi Y blok
        return true; // Gerakan berhasil
    }
    return false; // Gerakan gagal (blok menyentuh dasar atau blok lain)
}

// Fungsi untuk melakukan Hard Drop (langsung jatuh ke posisi terendah yang memungkinkan)
void HardDropBlock(TetrisBlock *block, TetrisBoard *board) {
    // Selama blok masih bisa turun, teruskan turun
    while (MoveBlockDown(block, board)) {}

    // Setelah mencapai dasar, tempatkan blok di papan
    PlaceBlock(block, board);
}

// Fungsi ini memungkinkan pemain untuk menahan satu blok dan menukarnya dengan blok aktif saat ini
void HoldCurrentBlock(TetrisBoard* board) {
    // Jika belum pernah memegang blok sebelumnya
    if (!board->hold_block.hasHeld) {
        // Simpan blok saat ini ke hold_block
        board->hold_block.block = board->current_block;
        board->hold_block.hasHeld = true;

        // Reset posisi dan rotasi blok yang ditahan
        board->hold_block.block.rotation = 0;
        board->hold_block.block.x = BOARD_WIDTH / 2 - 2;
        board->hold_block.block.y = 0;

        // Perbarui bentuk blok yang di-hold berdasarkan rotasi awal (0)
        for (int y = 0; y < 4; y++) {
            for (int x = 0; x < 4; x++) {
                board->hold_block.block.shape[y][x] = 
                    TETROMINO_SHAPES[board->hold_block.block.type][0][y][x];
            }
        }

        // Ganti blok saat ini dengan blok berikutnya
        board->current_block = board->next_block;

        // Reset posisi blok baru agar muncul di bagian atas papan
        board->current_block.x = BOARD_WIDTH / 2 - 2;
        board->current_block.y = 0;

        // Generate blok berikutnya secara acak
        board->next_block = GenerateRandomBlock();
    } 
    // Jika sudah ada blok yang di-hold, lakukan pertukaran blok
    else {
        // Simpan blok saat ini ke dalam variabel sementara
        TetrisBlock tempBlock = board->current_block;

        // Ganti blok saat ini dengan blok yang sedang di-hold
        board->current_block = board->hold_block.block;

        // Reset rotasi dan posisi blok yang baru masuk ke permainan
        board->current_block.rotation = 0;
        board->current_block.x = BOARD_WIDTH / 2 - 2;
        board->current_block.y = 0;

        // Perbarui bentuk blok berdasarkan rotasi awal (0)
        for (int y = 0; y < 4; y++) {
            for (int x = 0; x < 4; x++) {
                board->current_block.shape[y][x] = 
                    TETROMINO_SHAPES[board->current_block.type][0][y][x];
            }
        }

        // Blok yang sebelumnya aktif sekarang menjadi blok yang di-hold
        board->hold_block.block = tempBlock;

        // Reset rotasi blok yang di-hold
        board->hold_block.block.rotation = 0;

        // Perbarui bentuk blok yang di-hold berdasarkan rotasi awal (0)
        for (int y = 0; y < 4; y++) {
            for (int x = 0; x < 4; x++) {
                board->hold_block.block.shape[y][x] = 
                    TETROMINO_SHAPES[board->hold_block.block.type][0][y][x];
            }
        }
    }
}


// Fungsi untuk menempatkan blok ke papan permainan setelah mencapai posisi final
// Blok akan ditempatkan di grid papan berdasarkan koordinat dan rotasinya
void PlaceBlock(TetrisBlock *block, TetrisBoard *board) {
    // Loop melalui setiap sel dalam matriks 4x4 dari bentuk tetromino
    for (int y = 0; y < 4; y++) {
        for (int x = 0; x < 4; x++) {
            // Hanya tempatkan sel yang tidak kosong dalam bentuk blok
            if (TETROMINO_SHAPES[block->type][block->rotation][y][x] != 0) {
                // Hitung posisi absolut dalam grid papan permainan
                int boardX = block->x + x;
                int boardY = block->y + y;
                
                // Pastikan koordinat berada dalam batas papan
                // boardY >= 0 → Mencegah penempatan di area negatif (di atas layar)
                // boardY < BOARD_HEIGHT → Mencegah keluar dari batas bawah papan
                // boardX >= 0 → Mencegah keluar dari batas kiri papan
                // boardX < BOARD_WIDTH → Mencegah keluar dari batas kanan papan
                if (boardY >= 0 && boardY < BOARD_HEIGHT && 
                    boardX >= 0 && boardX < BOARD_WIDTH) {
                    // Simpan tipe blok dalam grid papan
                    // Ditambah 1 karena indeks 0 biasanya digunakan untuk "kosong"
                    board->grid[boardY][boardX] = block->type + 1;
                }
            }
        }
    }
}
