# Compiler and flags
CC := gcc
CFLAGS := -Wall -Werror -Wextra -pedantic -std=gnu90 -ggdb3 -I. -Itransaction/ -Iprovided/ -I../../crypto
LDFLAGS := -L../../crypto -lhblk_crypto -lllist -lssl -lcrypto -pthread

# Source files and object files
SRC := $(wildcard *.c) $(wildcard transaction/*.c) $(wildcard provided/*.c)
OBJ := $(SRC:.c=.o)

# Target library
TARGET := libhblk_blockchain.a

# Phony targets
.PHONY: all clean oclean fclean

# Default target
all: $(TARGET)

# Compile source files into object files
%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

# Build target library
$(TARGET): $(OBJ)
	ar rcs $@ $^

# Clean rules
clean:
	$(RM) *~ $(TARGET)

oclean:
	$(RM) $(OBJ)

fclean: clean oclean

# Dependency rules
blockchain.o: blockchain.h
transaction.o: transaction.h
