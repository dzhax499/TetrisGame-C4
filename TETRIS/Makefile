# Compiler
CC = gcc

# Compiler flags
CFLAGS = -std=c11 -Iinclude -Iinclude/SDL3 -Isrc/ui/menu

# Linker flags
LDFLAGS = -Llib -lSDL3


# Source files
SOURCES = src/main.c \
          src/ui/menu/main_menu.c \
          src/ui/menu/font.c \
          src/ui/settings/settings_menu.c

# Object files
OBJECTS = $(SOURCES:.c=.o)

# Executable name
EXECUTABLE = tetris

# Default target
all: $(EXECUTABLE)

# Link the executable
$(EXECUTABLE): $(OBJECTS)
	$(CC) $(OBJECTS) -o $@ $(LDFLAGS)

# Compile source files into object files
%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

# Clean up build files
clean:
	rm -f $(OBJECTS) $(EXECUTABLE)
