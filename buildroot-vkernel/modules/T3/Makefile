LIBC = -lpthread -Wall
CC = gcc

# BUILDROOT_DIR := ../..
# KDIR := $(BUILDROOT_DIR)/output/build/linux-custom
# COMPILER := $(BUILDROOT_DIR)/output/host/bin/i686-buildroot-linux-uclibc-gcc


all:
	$(CC) -o thread_runner thread_runner.c $(LIBC)
	$(CC) -o deadline deadline.c $(LIBC)

# Use this with valgrind
# make debug
# valgrind --leak-check=full ./thread_runner 3 2 3 3
debug:
	$(CC) -g -o thread_runner thread_runner.c $(LIBC)
	$(CC) -o deadline deadline.c $(LIBC)

clean:
	rm thread_runner deadline

