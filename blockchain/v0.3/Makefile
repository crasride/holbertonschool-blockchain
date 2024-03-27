# Compiler and flags
CC = gcc
CFLAGS = -Wall -Werror -Wextra -pedantic -std=gnu90 -ggdb3 -c -I. -Itransaction/ -Iprovided/ -I../../crypto -L../../crypto -lhblk_crypto -lllist -lssl -lcrypto -pthread

# Source files and object files
SRC0 = $(wildcard *.c) $(wildcard transaction/*.c) $(wildcard provided/*.c)
OBJ0 = $(SRC0:.c=.o)
NAME0 = libhblk_blockchain.a

# Phony targets
.PHONY: clean oclean fclean libhblk_blockchain.a

# Clean targets
clean:
	$(RM) *~ $(NAME0)

oclean:
	$(RM) $(OBJ0)

fclean: clean oclean

# Target to build the library
libhblk_blockchain.a: blockchain.h transaction/transaction.h $(OBJ0)
	ar rcs $(NAME0) $(OBJ0)
