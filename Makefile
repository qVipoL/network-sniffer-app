CC := gcc
FLAGS := -g -Wall
SRC := src
OBJ := obj
SRCS := $(wildcard $(SRC)/*.c)
OBJS := $(patsubst $(SRC)/%.c, $(OBJ)/%.o, $(SRCS))

BIN_DIR := bin
BIN := $(BIN_DIR)/sniffer-app

all: dirs $(BIN)

dirs:
	mkdir -p bin
	mkdir -p obj

$(BIN): $(OBJS)
	$(CC) $(FLAGS) $(OBJS) -o $@

$(OBJ)/%.o: $(SRC)/%.c
	$(CC) $(FLAGS) -c $< -o $@

run:
	sudo ./bin/

clean:
	$(RM) -r $(BIN_DIR)/* $(OBJ)/*