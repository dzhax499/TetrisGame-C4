#include "rotation.h"

void rotate_tetromino(Tetromino* tetromino, bool clockwise) {
    if (clockwise) {
        tetromino->rotation = (tetromino->rotation + 1) % 4;
    } else {
        tetromino->rotation = (tetromino->rotation + 3) % 4;
    }
}
