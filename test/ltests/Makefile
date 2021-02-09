# Makefile for compiling and testing queue

CC = gcc -Wall -Wextra -Wpedantic

.PHONY: all
all: ltests

ltests: list.o ltests.o
	$(CC) -g ltests.o list.o -o ltests

ltests.o: ltests.c
	$(CC) -g -c ltests.c -o ltests.o

list.o: ../list.h ../list.c
	$(CC) -g -c ../list.c -o list.o

.PHONY: clean
clean:
	rm -f *.o ltests

