CC = mpicc
SRC = src/paralel.c
BIN = par
CFLAGS = -lm -g
BUILD_FOLDER = build



all : $(SRC)
	@mkdir -p $(BUILD_FOLDER)
	@$(CC) -O3 $(SRC) $(CFLAGS) -o $(BUILD_FOLDER)/$(BIN).out

clean : 
	@rm -rf $(BUILD_FOLDER)/$(BIN).out

run : all
	@mpirun -np 4 $(BUILD_FOLDER)/$(BIN).out

time : all
	/usr/bin/time -p ./$(BUILD_FOLDER)/$(BIN).out