# Directories
BIN_DIR = ./build/
SRC_DIR = ./src
# Sources
SOURCES = $(SRC_DIR)/main.c \
			$(SRC_DIR)/ui.c 
 # use \ filename to append another files
# OBJECT_NAMES = $(SOURCES:.c=" ")
# OBJECTS = $(BIN_DIR)/main.o #$(pathsubst %, $(OBJ_DIR)/%, $(OBJECT_NAMES))
TARGET = $(BIN_DIR)/main
# Flags
CFLAGS = -w $(shell sdl2-config --cflags) -lm -lpthread -g
LDFLAGS = -lSDL2 -lSDL2_image -lSDL2_ttf $(shell sdl2-config --libs)
CC = gcc
$(TARGET): $(SOURCES)
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)
.PHONY: run clean
# run: $(TARGET)
# 	$(TARGET)
run: $(TARGET)
	$(TARGET) input.txt
clean:
	@rm -r  $(BIN_DIR)/*