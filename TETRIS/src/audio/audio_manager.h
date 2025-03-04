// audio_manager.h
#ifndef AUDIO_MANAGER_H
#define AUDIO_MANAGER_H

#include <SDL2/SDL_mixer.h>

void init_audio();
void set_volume(int volume);
void mute_audio(int mute);
void close_audio();

#endif