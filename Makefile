CC = gcc

# Path dasar
BASEDIR = "C:/Users/ymete/OneDrive - Politeknik Negeri Bandung/Matkul/Semester 2/Proyek 2 - Pa Yudi/PROYEK 2/TetrisGame-C4/TETRIS"

# Include paths yang lengkap 
CFLAGS = -I$(BASEDIR)/include -I$(BASEDIR)/src/ui/menu

# Library flags 
LDFLAGS = -L$(BASEDIR)/lib -lSDL3

# Lokasi file sumber
MAIN_SRC = $(BASEDIR)/src/main.c
MENU_SRC = $(BASEDIR)/src/ui/menu/main_menu.c
FONT_SRC = $(BASEDIR)/src/ui/menu/font.c

# Target dan objek files
TARGET = build/tetris.exe
OBJ = build/main.o build/main_menu.o build/font.o

all: build $(TARGET)

# Membuat direktori build
build:
	mkdir -p build

$(TARGET): $(OBJ)
	$(CC) $(OBJ) -o $(TARGET) $(LDFLAGS)

# Aturan kompilasi untuk masing-masing file
build/main.o: 
	$(CC) $(CFLAGS) -c $(MAIN_SRC) -o build/main.o

build/main_menu.o: 
	$(CC) $(CFLAGS) -c $(MENU_SRC) -o build/main_menu.o

build/font.o: 
	$(CC) $(CFLAGS) -c $(FONT_SRC) -o build/font.o

clean:
	rm -rf build/*.o build/tetris.exe

run: $(TARGET)
	./$(TARGET)