#include <stdio.h>
#include <stdlib.h>

#define MAX_NUM_PROCESSES 16
#define QUEUE_SIZE 32
#define BUFFER_SIZE 1024

typedef struct Processes {
    int arrivalTime;
    int priority;
    int end;
    int *times; //even:CPU, odd:IO.
}Process;

typedef struct Queues {
    int size;
    int front;
    int back;
    Process *slots;
}Queue;

void removeNewline(char ***, int);
void populateQueue(Queue *);
void populateProcess(Process*);
void resizeBuffer(char ***, int *);
void freeStrArray(char **, int);
void print2dIntArray(int **, int);
void readInput(char ***);
void convertStrArr(char **, int, int **);
int doubleSizeOfQueue(Queue*);
int *convertStringToInt(char *, int*);
Queue initializeQueue(int);

