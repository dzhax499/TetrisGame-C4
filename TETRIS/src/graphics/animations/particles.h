#ifndef PARTICLES_H
#define PARTICLES_H

#include <stdbool.h>

// Struktur data untuk efek visual
typedef struct {
    int x, y;
    int lifetime;
    bool active;
} ParticleEffect;

// Inisialisasi efek
void initParticles(ParticleEffect particles[], int maxParticles);

// Menampilkan animasi skor popup
void spawnScorePopup(ParticleEffect particles[], int x, int y, int score);

// Menampilkan animasi level up
void spawnLevelUpEffect(ParticleEffect particles[], int x, int y);

// Update efek animasi
void updateParticles(ParticleEffect particles[], int maxParticles);

#endif
