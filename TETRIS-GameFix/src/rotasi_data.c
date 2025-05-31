// FIXED: rotasi_data.c - Data rotasi Tetromino Tetris lengkap dan benar
// Nama file : rotasi_data.c
// Deskripsi : Implementasi data array bentuk dan rotasi unik untuk setiap blok Tetris
// Oleh      : Dzakit Tsabit 241511071 (github : dzhax4499)

#ifndef ROTASI_DATA_H
#define ROTASI_DATA_H

// I Tetromino (Cyan) - 2 posisi unik
const int I_TETROMINO[2][4][4] = {
    // Horizontal (default)
    {
        {0, 0, 0, 0},
        {1, 1, 1, 1},
        {0, 0, 0, 0},
        {0, 0, 0, 0}
    },
    // Vertical
    {
        {0, 0, 1, 0},
        {0, 0, 1, 0},
        {0, 0, 1, 0},
        {0, 0, 1, 0}
    }
};

// J Tetromino (Blue) - 4 posisi
const int J_TETROMINO[4][4][4] = {
    // Rotasi 0° (default)
    {
        {1, 0, 0, 0},
        {1, 1, 1, 0},
        {0, 0, 0, 0},
        {0, 0, 0, 0}
    },
    // Rotasi 90° (clockwise)
    {
        {0, 1, 1, 0},
        {0, 1, 0, 0},
        {0, 1, 0, 0},
        {0, 0, 0, 0}
    },
    // Rotasi 180°
    {
        {0, 0, 0, 0},
        {1, 1, 1, 0},
        {0, 0, 1, 0},
        {0, 0, 0, 0}
    },
    // Rotasi 270°
    {
        {0, 1, 0, 0},
        {0, 1, 0, 0},
        {1, 1, 0, 0},
        {0, 0, 0, 0}
    }
};

// L Tetromino (Orange) - 4 posisi
const int L_TETROMINO[4][4][4] = {
    // Rotasi 0° (default)
    {
        {0, 0, 1, 0},
        {1, 1, 1, 0},
        {0, 0, 0, 0},
        {0, 0, 0, 0}
    },
    // Rotasi 90°
    {
        {0, 1, 0, 0},
        {0, 1, 0, 0},
        {0, 1, 1, 0},
        {0, 0, 0, 0}
    },
    // Rotasi 180°
    {
        {0, 0, 0, 0},
        {1, 1, 1, 0},
        {1, 0, 0, 0},
        {0, 0, 0, 0}
    },
    // Rotasi 270°
    {
        {1, 1, 0, 0},
        {0, 1, 0, 0},
        {0, 1, 0, 0},
        {0, 0, 0, 0}
    }
};

// O Tetromino (Yellow) - 1 posisi 
const int O_TETROMINO[1][4][4] = {
    {
        {0, 1, 1, 0},
        {0, 1, 1, 0},
        {0, 0, 0, 0},
        {0, 0, 0, 0}
    }
};

// S Tetromino (Green) - 2 posisi unik
const int S_TETROMINO[2][4][4] = {
    // Horizontal (default)
    {
        {0, 1, 1, 0},
        {1, 1, 0, 0},
        {0, 0, 0, 0},
        {0, 0, 0, 0}
    },
    // Vertical
    {
        {0, 1, 0, 0},
        {0, 1, 1, 0},
        {0, 0, 1, 0},
        {0, 0, 0, 0}
    }
};

// T Tetromino (Purple) - 4 posisi
const int T_TETROMINO[4][4][4] = {
    // Rotasi 0° (default)
    {
        {0, 1, 0, 0},
        {1, 1, 1, 0},
        {0, 0, 0, 0},
        {0, 0, 0, 0}
    },
    // Rotasi 90°
    {
        {0, 1, 0, 0},
        {0, 1, 1, 0},
        {0, 1, 0, 0},
        {0, 0, 0, 0}
    },
    // Rotasi 180° 
    {
        {0, 0, 0, 0},
        {1, 1, 1, 0},
        {0, 1, 0, 0},
        {0, 0, 0, 0}
    },
    // Rotasi 270° 
    {
        {0, 1, 0, 0},
        {1, 1, 0, 0},
        {0, 1, 0, 0},
        {0, 0, 0, 0}
    }
};

// Z Tetromino (Red) - 2 posisi unik
const int Z_TETROMINO[2][4][4] = {
    // Horizontal (default)
    {
        {1, 1, 0, 0},
        {0, 1, 1, 0},
        {0, 0, 0, 0},
        {0, 0, 0, 0}
    },
    // Vertical
    {
        {0, 0, 1, 0},
        {0, 1, 1, 0},
        {0, 1, 0, 0},
        {0, 0, 0, 0}
    }
};

// Jumlah rotasi unik untuk setiap blok
const int ROTATION_COUNT[7] = {2, 4, 4, 1, 2, 4, 2};

#endif // ROTASI_DATA_H