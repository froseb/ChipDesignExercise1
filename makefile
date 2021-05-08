OBJ_DIR=obj
BIN_DIR=bin
SRC_DIR=src
INCLUDE_DIR = include

SRC_FILES=$(wildcard $(SRC_DIR)/*.cpp)
OBJ_FILES=$(patsubst $(SRC_DIR)/%.cpp, $(OBJ_DIR)/%.o,$(SRC_FILES))

CC=g++
CFLAGS=-std=c++20 -O3 -Wall -Wextra -Wpedantic -Werror -I $(INCLUDE_DIR)

.PHONY: default clean

default: main

main: $(OBJ_FILES)
	[ -d $(BIN_DIR) ] || mkdir -p $(BIN_DIR)
	$(CC) $(CFLAGS) -o $(BIN_DIR)/main $(OBJ_FILES)

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp
	[ -d $(OBJ_DIR) ] || mkdir -p $(OBJ_DIR)
	$(CC) $(CFLAGS) -c -o $@ $<

clean:
	rm $(BIN_DIR)/*
	rm $(OBJ_DIR)/*
