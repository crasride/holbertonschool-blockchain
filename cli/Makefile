# Compiler
CC = gcc

# Compiler flags
CFLAGS = -Wall -Werror -Wextra -pedantic -std=gnu90 -ggdb3 -Wno-deprecated-declarations -c \
		-I. -I../blockchain/v0.3/ -I../blockchain/v0.3/transaction/ -I../blockchain/v0.3/provided/ -I../crypto

# Linker flags
LIBS = -L../blockchain/v0.3 -lhblk_blockchain -L../crypto -lhblk_crypto -lllist -lssl -lcrypto -pthread -lreadline

# Source files
SRC0 = cli.c cleanup.c handle_exit.c handle_info.c handle_mine.c handle_send.c \
		handle_wallet_create.c handle_wallet_load.c handle_wallet_save.c tools.c \
		handle_load.c handle_save.c mine.c handle_help.c unspent_list.c handle_info_block.c \

# Object files
OBJ0 = $(SRC0:.c=.o)

# Name of the executable
NAME0 = cli

# Phony targets
.PHONY: clean oclean flcean cli

# Clean targets
clean:
	$(RM) *~ $(NAME0)

oclean:
	$(RM) $(OBJ0)

fclean: clean oclean

# Default target
cli: cli.h $(OBJ0)
	$(CC) $(OBJ0) -o $(NAME0) $(LIBS)
