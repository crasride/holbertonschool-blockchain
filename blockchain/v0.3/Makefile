# Compiler and flags
CC = gcc

# Compiler flags
CFLAGS = -Wall -Werror -Wextra -pedantic -std=gnu90 -ggdb3 -c -I. -Itransaction/ -Iprovided/ -I../../crypto -L../../crypto -lhblk_crypto -lllist -lssl -lcrypto -pthread

# Source files and object files
SRC := $(wildcard *.c) $(wildcard transaction/*.c)  # Lista de todos los archivos .c en el directorio actual y en transaction/
OBJ := $(patsubst %.c, %.o, $(filter-out provided/%.c, $(SRC)))  # Lista de archivos .o, excluyendo los de provided/
NAME := libhblk_blockchain.a  # Nombre del archivo de la biblioteca

# Phony targets
.PHONY: clean oclean fclean libhblk_blockchain.a

# Clean targets
clean:
	$(RM) *~ $(NAME0)  # Elimina archivos temporales y el archivo de la biblioteca

oclean:
	$(RM) $(OBJ0)  # Elimina los archivos .o

fclean: clean oclean  # Limpia tanto los archivos temporales como los .o

# Target to build the library
libhblk_blockchain.a: blockchain.h transaction/transaction.h $(OBJ)
	ar rcs $(NAME) $(OBJ)  # Crea la biblioteca utilizando los archivos .o
