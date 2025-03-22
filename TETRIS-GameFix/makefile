# Compiler dan flags
CC = gcc
CFLAGS = -Wall -Wextra -std=c99
LDFLAGS = -Lmingw64 -lraylib -lopengl32 -lm -lpthread -lwinpthread

# Direktori source dan include
SRC_DIR = ./src
INCLUDE_DIR = include

# File source dan object
SRC_FILES = $(SRC_DIR)/main.c $(SRC_DIR)/blocks.c $(SRC_DIR)/rendering.c $(SRC_DIR)/board.c $(SRC_DIR)/main_menu.c $(SRC_DIR)/scoring.c
OBJ_FILES = $(SRC_FILES:.c=.o)

# Nama output executable
TARGET = tetris_game

# Target utama
all: $(TARGET)

# Rule untuk membuat executable
$(TARGET): $(OBJ_FILES)
	$(CC) $(OBJ_FILES) -o $(TARGET) $(LDFLAGS)

# Rule untuk mengkompilasi file source ke object file
%.o: %.c
	$(CC) $(CFLAGS) -I$(INCLUDE_DIR) -c $< -o $@

# Membersihkan file object dan executable
clean:
	rm -f $(OBJ_FILES) $(TARGET)

# Phony targets
.PHONY: all clean