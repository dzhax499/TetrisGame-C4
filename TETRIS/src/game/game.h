// Nama program : game.h
// Deskripsi    : header untuk pengelolaan logika utama permainan 
// Pembuat      : [Rizky Satria Gunawan, 241511089, 24 February 2025, 11:01]

// game.h
#ifndef GAME_H
#define GAME_H

typedef enum { MENU, PLAYING, PAUSED } GameState;
extern GameState gameState;
void handleGameState();

#endif