CC = gcc
CFLAGS = -I -g.
fcfsmake: fcfs.c
	$(CC) -o fcfs fcfs.c fcfs.h