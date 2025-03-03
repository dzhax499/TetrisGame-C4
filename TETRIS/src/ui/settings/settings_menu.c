// settings_menu.c
#include "settings_menu.h"
#include "audio_manager.h"
#include <stdio.h>

void open_settings_menu() {
    printf("1. Set Volume\n2. Mute\n3. Exit\n");
    int choice, volume;
    scanf("%d", &choice);
    switch (choice) {
        case 1:
            printf("Enter volume (0-128): ");
            scanf("%d", &volume);
            set_volume(volume);
            break;
        case 2:
            mute_audio(1);
            break;
        case 3:
            mute_audio(0);
            break;
    }
}