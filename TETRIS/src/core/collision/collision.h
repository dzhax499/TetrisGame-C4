/**
 * collision.h
 * Header untuk deteksi tabrakan game Tetris
 * Oleh: Dzakit Tsabit Assyafiq
 */

 #ifndef COLLISION_H
 #define COLLISION_H
 
 #include "blocks.h"
 
 // Dimensi papan permainan
 #define GAME_BOARD_WIDTH 10
 #define GAME_BOARD_HEIGHT 20
 
 // Forward declaration untuk struktur Game
 typedef struct {
     int board[GAME_BOARD_HEIGHT][GAME_BOARD_WIDTH];  // Papan permainan
     TetrominoSystem* tetromino_system;              // Sistem Tetromino
     // Lainnya bisa ditambahkan sesuai kebutuhan game
 } Game;
 
 // Fungsi-fungsi untuk deteksi tabrakan
 int is_collision(Game* game, const Tetromino* tetromino);
 int can_move_down(Game* game, const Tetromino* tetromino);
 int can_move_left(Game* game, const Tetromino* tetromino);
 int can_move_right(Game* game, const Tetromino* tetromino);
 void lock_tetromino(Game* game, const Tetromino* tetromino);
 void calculate_ghost_position(Game* game, const Tetromino* tetromino, Tetromino* ghost);
 
 #endif /* COLLISION_H */