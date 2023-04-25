CC = gcc
CFLAGS = -Wall -I include/
LDFLAGS =

SRC_DIR = src/
OBJ_DIR = obj/
BIN_DIR = bin/

SRC_FILES = $(wildcard $(SRC_DIR)*.c)
OBJ_FILES = $(patsubst $(SRC_DIR)%.c, $(OBJ_DIR)%.o, $(SRC_FILES))

TARGET = $(BIN_DIR)program

.PHONY: all clean

all: $(TARGET)

$(TARGET): $(OBJ_FILES)
	$(CC) $(LDFLAGS) $^ -o $@

$(OBJ_DIR)%.o: $(SRC_DIR)%.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJ_DIR)*.o $(TARGET) || del /s "*.o" "$(TARGET)";



