# Makefile for Tetris Game

# Compiler
CC = gcc

# Compiler flags
CFLAGS = -Wall -Wextra -std=c99 -g
LDFLAGS = 

# Raylib configuration
RAYLIB_FLAGS = -lraylib -lm -ldl -lpthread

# Directories
SRC_DIR = src
BUILD_DIR = build
INCLUDE_DIR = $(SRC_DIR)/include
ASSETS_DIR = assets

# Find all source files
SRCS = $(wildcard $(SRC_DIR)/*.c)
OBJS = $(patsubst $(SRC_DIR)/%.c,$(BUILD_DIR)/%.o,$(SRCS))

# Executable name
TARGET = $(BUILD_DIR)/tetris_game

# Default target
all: $(BUILD_DIR) $(TARGET)

# Create build directory
$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

# Link the executable
$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS) $(RAYLIB_FLAGS)

# Compile source files to object files
$(BUILD_DIR)/%.o: $(SRC_DIR)/%.c
	$(CC) $(CFLAGS) -I$(INCLUDE_DIR) -c $< -o $@

# Run the game
run: $(TARGET)
	./$(TARGET)

# Debug mode
debug: CFLAGS += -DDEBUG -g
debug: clean $(TARGET)

# Clean build files
clean:
	rm -rf $(BUILD_DIR)/*

# Install dependencies (example for Ubuntu/Debian)
install-deps:
	sudo apt-get update
	sudo apt-get install -y \
		build-essential \
		libraylib-dev \
		libasound2-dev \
		libx11-dev \
		libxrandr-dev \
		libxi-dev \
		libgl1-mesa-dev \
		libglu1-mesa-dev

# Create distribution package
dist: clean
	mkdir -p $(BUILD_DIR)/tetris_game
	cp -r $(SRC_DIR) $(ASSETS_DIR) Makefile README.md $(BUILD_DIR)/tetris_game/
	cd $(BUILD_DIR) && tar -czvf tetris_game.tar.gz tetris_game

# Help target
help:
	@echo "Tetris Game Makefile targets:"
	@echo "  all      - Build the game (default)"
	@echo "  run      - Build and run the game"
	@echo "  debug    - Build with debug symbols"
	@echo "  clean    - Remove build artifacts"
	@echo "  install-deps - Install build dependencies"
	@echo "  dist     - Create distribution package"
	@echo "  help     - Show this help message"

# Phony targets
.PHONY: all run debug clean install-deps dist help