CC = clang
CFLAGS = -Wall -pedantic -std=c99

all: tree.o
	clang -Wall -pedantic -std=c99 tree.o -o all

tree.o: tree.c tree.h
	clang -Wall -pedantic -std=c99 -c tree.c

clean:
	rm *.o all