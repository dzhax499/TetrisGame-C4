/**
 * @file collision.h
 * @brief Header file untuk deteksi collision tetromino
 * @author Dzakit Tsabit Assyafiq
 * 
 * File ini berisi fungsi-fungsi untuk mendeteksi collision antara
 * tetromino dengan grid, wall, dan floor.
 */

 #ifndef COLLISION_H
#define COLLISION_H

#include "blocks.h"

bool check_collision(Tetromino* tetromino, int grid[GRID_HEIGHT][GRID_WIDTH]);

#endif