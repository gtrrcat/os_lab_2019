CC=gcc
CFLAGS=-I.
parallel_min_max : utils.o find_min_max.o utils.h find_min_max.h
	$(CC) -o parallel_min_max utils.o find_min_max.o parallel_min_max.c $(CFLAGS)
clean :
	rm utils.o find_min_max.o sequential_min_max parallel_min_max
