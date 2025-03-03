# Simplified Tetris Makefile (main_menu only)

# Compiler and flags
CC = gcc
CFLAGS = -Wall -Wextra -pedantic -std=c11 -O2
DEBUG_FLAGS = -g -DDEBUG

# Directories
SRC_DIR = src
BUILD_DIR = build
BIN_DIR = $(BUILD_DIR)/bin
OBJ_DIR = $(BUILD_DIR)/obj
INCLUDE_DIR = include

# SDL2 configuration
SDL_CFLAGS = $(shell sdl2-config --cflags)
SDL_LIBS = $(shell sdl2-config --libs) -lSDL2_ttf

# Source files - just main_menu for now
SRCS = $(SRC_DIR)/ui/menu/main_menu.c

# Demo main file (temporary)
DEMO_MAIN = demo_main.c

# Generate corresponding object file paths
OBJS = $(patsubst $(SRC_DIR)/%.c,$(OBJ_DIR)/%.o,$(SRCS))
DEMO_OBJ = $(OBJ_DIR)/$(DEMO_MAIN:.c=.o)

# Create a list of directories for object files
OBJ_DIRS = $(sort $(dir $(OBJS))) $(OBJ_DIR)

# Main target
TARGET = $(BIN_DIR)/tetris_menu_demo

# Phony targets
.PHONY: all clean debug run dirs

# Default target
all: dirs $(TARGET)

# Debug build
debug: CFLAGS += $(DEBUG_FLAGS)
debug: all

# Create necessary directories
dirs:
	@mkdir -p $(OBJ_DIRS)
	@mkdir -p $(BIN_DIR)

# Create a simple demo main file
$(DEMO_MAIN):
	@echo "Creating demo main file..."
	@echo "#include \"src/ui/menu/main_menu.h\"" > $(DEMO_MAIN)
	@echo "#include <stdio.h>" >> $(DEMO_MAIN)
	@echo "int main(int argc, char* argv[]) {" >> $(DEMO_MAIN)
	@echo "    (void)argc; (void)argv; // Unused" >> $(DEMO_MAIN)
	@echo "    printf(\"Starting Tetris Menu Demo\\n\");" >> $(DEMO_MAIN)
	@echo "    MenuState state = runMainMenu();" >> $(DEMO_MAIN)
	@echo "    printf(\"Menu exited with state: %d\\n\", state);" >> $(DEMO_MAIN)
	@echo "    return 0;" >> $(DEMO_MAIN)
	@echo "}" >> $(DEMO_MAIN)

# Link the executable
$(TARGET): $(OBJS) $(DEMO_OBJ)
	$(CC) $(CFLAGS) $(SDL_CFLAGS) -o $@ $^ $(SDL_LIBS)
	@echo "Build complete: $@"

# Compile source files
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) $(SDL_CFLAGS) -I$(INCLUDE_DIR) -I$(SRC_DIR) -c $< -o $@
	@echo "Compiled: $<"

# Compile demo main
$(DEMO_OBJ): $(DEMO_MAIN)
	$(CC) $(CFLAGS) $(SDL_CFLAGS) -I$(INCLUDE_DIR) -I$(SRC_DIR) -c $< -o $@
	@echo "Compiled: $<"

# Clean build files
clean:
	rm -rf $(BUILD_DIR) $(DEMO_MAIN)
	@echo "Clean complete"

# Run the game
run: all
	$(TARGET)