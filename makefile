CC = gcc
CFLAGS = -g -I.
fcfsmake: fcfs.c
	$(CC) -o fcfs fcfs.c fcfs.h $(CFLAGS)
