# Compiler
CC = gcc

# Compiler flags
CFLAGS = -Wall -Werror -Wextra -pedantic -Wno-deprecated-declarations -ggdb3 --std=gnu90

# Source files
LIBS = -I. -L../../crypto -lhblk_crypto -lllist -lssl -lcrypto -pthread

# Object files (generated from source files)
PROVIDED0 = provided/_blockchain_print.c provided/_blockchain_destroy.c

# Default Target: Build the executable
TASK0 = blockchain_create

# Rule to build the executable
blockchain_create-test: $(TASK0).c test/blockchain_create-main.c $(PROVIDED0)
	$(CC) $(CFLAGS) -o $@ $^ $(LIBS)

# Clean up object files and the executable
clean:
	rm $(OBJ) blockchain_create-test

# Check for betty style
check:
	betty $(TASK0).c

# Declaration of phony targets
.PHONY: clean, check
