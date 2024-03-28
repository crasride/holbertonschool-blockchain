CC = gcc
CFLAGS = -Wall -Werror -Wextra -pedantic -std=gnu90 -ggdb3 -c -I. -Itransaction/ -Iprovided/ -I../../crypto -L../../crypto -lhblk_crypto -lllist -lssl -lcrypto -pthread

# Source files and object files
SRC := $(wildcard *.c) $(wildcard transaction/*.c)
OBJ := $(patsubst %.c, %.o, $(filter-out provided/%.c, $(SRC)))
NAME := libhblk_blockchain.a

.PHONY: clean oclean flcean libhblk_blockchain.a

clean:
	$(RM) *~ $(NAME0)

oclean:
	$(RM) $(OBJ0)

fclean: clean oclean

# Target to build the library
libhblk_blockchain.a: blockchain.h transaction/transaction.h $(OBJ)
	ar rcs $(NAME) $(OBJ)
