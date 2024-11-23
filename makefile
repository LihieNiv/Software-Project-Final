CC = gcc
CFLAGS = -ansi -Wall -Wextra -Werror -pedantic-errors

my_app: symnmf.o symnmf.h
	gcc -o symnmf symnmf.o

symnmf.o: symnmf.c
	$(CC) -c symnmf.c $(CFLAGS)