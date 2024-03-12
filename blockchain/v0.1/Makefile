# Compiler
CC = gcc

# Compiler flags
CFLAGS = -Wall -Werror -Wextra -pedantic -Wno-deprecated-declarations -ggdb3 -std=gnu90

# Source files
LIBS = -I. -L../../crypto -lhblk_crypto -lllist -lssl -lcrypto -pthread

# Object files (generated from source files)
PROVIDED = provided/_blockchain_print.c provided/_blockchain_destroy.c provided/_endianness.c

# Default Target: Build the executable
TASK0 = blockchain_create
TASK1 = block_create
TASK2 = block_destroy
TASK3 = blockchain_destroy

# Rule to build the executable
blockchain_create-test: $(TASK0).c test/blockchain_create-main.c $(PROVIDED)
	$(CC) $(CFLAGS) -o $@ $^ $(LIBS)

block_create-test: $(TASK1).c $(TASK0).c test/block_create-main.c $(PROVIDED)
	$(CC) $(CFLAGS) -o $@ $^ $(LIBS)

block_destroy-test: $(TASK2).c $(TASK1).c $(TASK0).c test/block_destroy-main.c $(PROVIDED)
	$(CC) $(CFLAGS) -o $@ $^ $(LIBS)

blockchain_destroy-test: $(TASK3).c $(TASK2).c $(TASK1).c $(TASK0).c test/blockchain_destroy-main.c $(PROVIDED)
	$(CC) $(CFLAGS) -o $@ $^ $(LIBS)

# Clean up object files and the executable
clean:
	rm $(OBJ) blockchain_create-test block_create-test block_destroy-test blockchain_destroy-test

# Check for betty style
check:
	betty $(TASK0).c $(TASK1).c $(TASK2).c $(TASK3).c

# Declaration of phony targets
.PHONY: clean, check
