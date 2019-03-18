#include <stdio.h>
#include <stdlib.h>

#define QUEUE_SIZE 32

typedef struct Queues {
    int size;
    int front;
    int back;
    int *slots;
}Queue;

Queue initializeQueue(int);
int doubleSizeOfQueue(Queue *q);


