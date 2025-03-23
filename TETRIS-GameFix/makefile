# Compiler dan flags
CC = gcc
CFLAGS = -Wall -Wextra -std=c99
LDFLAGS = -Lmingw64 -lraylib -lopengl32 -lm -lpthread -lwinpthread

# Direktori source dan include
SRC_DIR = ./src
INCLUDE_DIR = include
BUILD_DIR = build

# File source dan object
SRC_FILES = $(SRC_DIR)/main.c $(SRC_DIR)/blocks.c $(SRC_DIR)/rendering.c $(SRC_DIR)/board.c $(SRC_DIR)/main_menu.c $(SRC_DIR)/scoring.c $(SRC_DIR)/game_sound.c
OBJ_FILES = $(patsubst $(SRC_DIR)/%.c,$(BUILD_DIR)/%.o,$(SRC_FILES))

# Nama output executable
TARGET = tetris_game

# Target utama
all: $(TARGET)

# Rule untuk membuat executable
$(TARGET): $(OBJ_FILES)
	$(CC) $(OBJ_FILES) -o $(TARGET) $(LDFLAGS)

# Rule untuk mengkompilasi file source ke object file
$(BUILD_DIR)/%.o: $(SRC_DIR)/%.c
	$(CC) $(CFLAGS) -I$(INCLUDE_DIR) -c $< -o $@

# Membersihkan file object dan executable
clean:
	rm -f $(OBJ_FILES) $(TARGET)

# Phony targets
.PHONY: all clean