// sfx.h
#ifndef SFX_H
#define SFX_H

// Memuat efek suara dari file
void LoadSFX(const char *filename);

// Memainkan efek suara yang telah dimuat
void PlaySFX();

// Membersihkan efek suara yang dimuat
void FreeSFX();

#endif // SFX_H