# Nama file output
TARGET = game

# Compiler
CC = gcc

# Flag kompilasi (menggunakan SDL3)
CFLAGS = -Wall -Wextra -std=c99 `sdl3-config --cflags`
LDFLAGS = `sdl3-config --libs`

# Semua file .c dalam direktori
SRC = $(wildcard *.c)
OBJ = $(SRC:.c=.o)

# Rule utama: build executable
$(TARGET): $(OBJ)
	$(CC) -o $(TARGET) $(OBJ) $(LDFLAGS)

# Rule untuk mengompilasi file .c menjadi .o
%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

# Bersihkan file hasil kompilasi
clean:
	rm -f $(OBJ) $(TARGET)
