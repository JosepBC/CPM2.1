CC = gcc
SRC = src/prim2.c
BIN = par
CFLAGS = -fopenmp -g
BUILD_FOLDER = build



all : $(SRC)
	@$(CC) -O3 $(SRC) $(CFLAGS) -lm -o $(BUILD_FOLDER)/$(BIN).out

clean : 
	@rm -rf $(BUILD_FOLDER)/$(BIN).out

run : all
	@./$(BUILD_FOLDER)/$(BIN).out

time : all
	/usr/bin/time -p ./$(BUILD_FOLDER)/$(BIN).out