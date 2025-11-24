CC=gcc
CFLAGS=-Wall -Wextra -std=c99 -pthread
LDFLAGS=-pthread

all: psum

psum: parallel_sum.o utils.o sum.o
	$(CC) $(LDFLAGS) -o psum parallel_sum.o utils.o sum.o

parallel_sum.o: parallel_sum.c utils.h sum.h
	$(CC) $(CFLAGS) -c parallel_sum.c -o parallel_sum.o

utils.o: utils.c utils.h
	$(CC) $(CFLAGS) -c utils.c -o utils.o

sum.o: sum.c sum.h
	$(CC) $(CFLAGS) -c sum.c -o sum.o

clean:
	rm -f *.o psum

.PHONY: all clean