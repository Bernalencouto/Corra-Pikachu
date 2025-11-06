CC = gcc

TARGET = corra_pikachu

VCPKG_ROOT = C:/Users/joaoc/vcpkg/installed/x64-mingw-static

INCLUDE_PATH = -I$(VCPKG_ROOT)/include

LIB_PATH = -L$(VCPKG_ROOT)/lib

SRC_FILES = $(wildcard src/*.c)

CFLAGS = -Wall -g $(INCLUDE_PATH)

LIBS = $(LIB_PATH) -lraylib -lglfw3 -lopengl32 -lgdi32 -lwinmm -pthread

all:
	@echo "Compilando com GCC..."
	$(CC) $(CFLAGS) $(SRC_FILES) -o $(TARGET) $(LIBS)
	@echo "Feito! Executável: $(TARGET).exe"

run: all
	@echo "Executando o jogo..."
	./$(TARGET).exe

clean:
	@echo "Limpando arquivos..."
	rm -f $(TARGET).exe