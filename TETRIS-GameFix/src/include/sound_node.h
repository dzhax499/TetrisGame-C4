// File: sound_node.h
// Deskripsi: Definisi struktur untuk linked list suara dan musik dalam game Tetris
// Oleh: Abi (Dimodifikasi)

#ifndef SOUND_NODE_H
#define SOUND_NODE_H

#include "raylib.h"
#include <stdbool.h>

/**
 * SoundNode - Struktur node untuk linked list efek suara
 * 
 * @sound: Objek Sound dari raylib untuk efek suara
 * @type: Jenis efek suara (enum SoundEffectType)
 * @name: Nama deskriptif untuk efek suara
 * @volume: Volume khusus untuk efek suara ini (0.0-1.0)
 * @next: Pointer ke node berikutnya dalam linked list
 */
typedef struct SoundNode {
    Sound sound;
    int type;          // Tipe enum SoundEffectType
    char* name;        // Nama deskriptif
    float volume;      // Volume spesifik
    struct SoundNode* next;
} SoundNode;

/**
 * MusicNode - Struktur node untuk linked list musik
 * 
 * @music: Objek Music dari raylib untuk trek musik
 * @type: Jenis trek musik (enum MusicTrackType)
 * @name: Nama deskriptif untuk trek musik
 * @volume: Volume khusus untuk trek musik ini (0.0-1.0)
 * @looping: Flag untuk menandakan apakah musik berulang
 * @next: Pointer ke node berikutnya dalam linked list
 */
typedef struct MusicNode {
    Music music;
    int type;          // Tipe enum MusicTrackType 
    char* name;        // Nama deskriptif
    float volume;      // Volume spesifik
    bool looping;      // Apakah diulang
    struct MusicNode* next;
} MusicNode;

#endif // SOUND_NODE_H