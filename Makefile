CC = gcc
CFLAGS = -Wall -I include/ -g
LDFLAGS =

SRC = src/
OBJ = obj/
BIN = bin/

SRC_CONTENT = $(wildcard $(SRC)*.c)
OBJ_CONTENT = $(patsubst $(SRC)%.c, $(OBJ)%.o, $(SRC_CONTENT))

PROG = $(BIN)program

all: $(PROG)

$(PROG): $(OBJ_CONTENT)
	$(CC) $(LDFLAGS) $^ -o $@

$(OBJ)%.o: $(SRC)%.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJ)*.o $(PROG) || del /s "*.o" "$(PROG)";
