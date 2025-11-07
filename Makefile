CC = gcc
TARGET_NAME = corra_pikachu
SRC_FILES = $(wildcard src/*.c)
CFLAGS = -Wall -g

LIBS = -lraylib -lGL -lm -lpthread -ldl -lrt -lX11
TARGET_EXEC = $(TARGET_NAME)
RUN_CMD = ./$(TARGET_EXEC)
CLEAN_CMD = rm -f

ifeq ($(OS), Windows_NT)

    VCPKG_ROOT = C:/Users/joaoc/vcpkg/installed/x64-mingw-static
    
    CFLAGS += -I$(VCPKG_ROOT)/include
    
    LIBS = -L$(VCPKG_ROOT)/lib -lraylib -lglfw3 -lopengl32 -lgdi32 -lwinmm -pthread
    TARGET_EXEC = $(TARGET_NAME).exe
    RUN_CMD = $(TARGET_EXEC)
    CLEAN_CMD = del /F /Q $(TARGET_EXEC)
endif

all:
	@echo "Compilando para o sistema atual..."
	$(CC) $(CFLAGS) $(SRC_FILES) -o $(TARGET_EXEC) $(LIBS)
	@echo "Feito! Executável: $(TARGET_EXEC)"

run: all
	@echo "Executando o jogo..."
	$(RUN_CMD)

clean:
	@echo "Limpando arquivos..."
	$(CLEAN_CMD)