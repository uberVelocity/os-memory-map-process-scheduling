#include <stdio.h>
#include <stdlib.h>

#define QUEUE_SIZE 32

typedef struct Processes {
    int arrivalTime;
    int priority;
    int end;
    int *times; //even:CPU, odd:IO
    
}Process;

typedef struct Queues {
    int size;
    int front;
    int back;
    Process *slots;
}Queue;

void populateQueue(Queue*);
int doubleSizeOfQueue(Queue*);
Queue initializeQueue(int);

