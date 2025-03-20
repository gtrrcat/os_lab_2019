CC=gcc

CFLAGS=-I.


new :  parallel_min_max zombie process_memory


parallel_min_max : parallel_min_max.c utils.o find_min_max.o utils.h find_min_max.h

    $(CC) -o parallel_min_max utils.o find_min_max.o parallel_min_max.c $(CFLAGS)


zombie : zombie.c

    $(CC) -o zombie -c zombie.c $(CFLAGS)


process_memory : process_memory.c

    $(CC) -o process_memory -c process_memory.c $(CFLAGS)


utils.o : utils.h

    $(CC) -o utils.o -c utils.c $(CFLAGS)


find_min_max.o : utils.h find_min_max.h

    $(CC) -o find_min_max.o -c find_min_max_.c $(CFLAGS)


clean :

    rm utils.o find_min_max.o parallel_min_max zombie process_memory