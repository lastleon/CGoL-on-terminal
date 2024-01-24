CC=gcc
CFLAGS=-I.
DEPS = conways_gol.h utils.h

cgol: board.o main.o
	$(CC) -o cgol board.o main.o

%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

