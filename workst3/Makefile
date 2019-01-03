LIBC = -lpthread -Wall
CC = gcc

all:
	$(CC) -o thread_runner thread_runner.c $(LIBC)
# Use this with valgrind
# make debug
# valgrind --leak-check=full ./thread_runner 3 2 3 3
debug:
	$(CC) -g -o thread_runner thread_runner.c $(LIBC)

clean:
	rm thread_runner

