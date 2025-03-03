#include "particles.h"
#include <stdio.h>

void initParticles(ParticleEffect particles[], int maxParticles) {
    for (int i = 0; i < maxParticles; i++) {
        particles[i].active = false;
    }
}

void spawnScorePopup(ParticleEffect particles[], int x, int y, int score) {
    for (int i = 0; i < 10; i++) {
        if (!particles[i].active) {
            particles[i].x = x;
            particles[i].y = y;
            particles[i].lifetime = 30; // Animasi 30 frame
            particles[i].active = true;
            printf("Score Popup: +%d di (%d, %d)\n", score, x, y);
            break;
        }
    }
}

void spawnLevelUpEffect(ParticleEffect particles[], int x, int y) {
    for (int i = 0; i < 10; i++) {
        if (!particles[i].active) {
            particles[i].x = x;
            particles[i].y = y;
            particles[i].lifetime = 50; // Animasi 50 frame
            particles[i].active = true;
            printf("Level Up Effect di (%d, %d)\n", x, y);
            break;
        }
    }
}

void updateParticles(ParticleEffect particles[], int maxParticles) {
    for (int i = 0; i < maxParticles; i++) {
        if (particles[i].active) {
            particles[i].lifetime--;
            if (particles[i].lifetime <= 0) {
                particles[i].active = false;
            }
        }
    }
}
