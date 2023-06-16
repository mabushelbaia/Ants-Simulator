CC = gcc
BIN_DIR = ./build
SRC_DIR = ./src
SOURCES = $(SRC_DIR)/main.c \
			$(SRC_DIR)/ui.c \
			$(SRC_DIR)/ant.c 
TARGET = $(BIN_DIR)/main 
CFLAGS = -w $(shell sdl2-config --cflags) -g  -Wall -Wextra
LDFLAGS = -lSDL2 -lSDL2_image -lSDL2_ttf $(shell sdl2-config --libs) -lm -lpthread 
$(TARGET): $(SOURCES)
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

.PHONY: run clean
run: $(TARGET)
	$(TARGET) input.txt
clean:
	@rm -r  $(BIN_DIR)/*