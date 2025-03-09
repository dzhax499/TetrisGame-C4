# Compiler
CC = gcc

# Compiler flags
CFLAGS = -Wall -Wextra -g -Dmain=SDL_main

# Directories
SRC_DIR = .
INC_DIR = src/include
LIB_DIR = src/lib

# SDL2 configuration
SDL_CFLAGS = -I$(INC_DIR)
SDL_LDFLAGS = -L$(LIB_DIR) -lmingw32 -lSDL2main -lSDL2 -mwindows

# Target executable
TARGET = tetris1

# Source files
SRC = $(SRC_DIR)/demo.c

# Object files
OBJ = $(SRC:.c=.o)

# Default target
all: $(TARGET)

# Link the target executable
$(TARGET): $(OBJ)
	$(CC) $(CFLAGS) -o $@ $^ $(SDL_LDFLAGS)

# Compile source files
%.o: %.c
	$(CC) $(CFLAGS) $(SDL_CFLAGS) -c $< -o $@

# Clean target
clean:
	rm -f $(OBJ) $(TARGET)

# Run target
run: $(TARGET)
	./$(TARGET)

.PHONY: all clean run