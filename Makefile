CC=gcc

CFLAGS=-Wall -Wextra -Werror -O0 -g -std=c11 -pthread
LDFLAGS=-lm

.PHONY: all

all: grade

nqueenst.o: nqueens.h nqueenst.c 
	$(CC) $(CFLAGS) -c nqueenst.c

test: nqueenst.o test.c
	$(CC) $(CFLAGS) -o test nqueenst.o test.c $(LDFLAGS) 



grade: test
	./test


clean:
	rm -rf *.o test
