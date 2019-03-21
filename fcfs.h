#include <stdio.h>
#include <stdlib.h>

#define MAX_NUM_PROCESSES 16
#define MAX_NUM_TIMES 64
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

void freeProcessesInQueue(Queue *, int);
void removeNewline(char **, int);
void populateQueue(Queue *, int, int **);
void populateProcess(Process *, int *);
void insertIntoQueue(Process *, Queue *);
void resizeBuffer(char **, int *);
void freeStrArray(char **, int);
void freeIntArray(int **, int);
void print2dIntArray(int **, int);
void printProcessesInQueue(Queue, int);
char **readInput(int *);
int doubleSizeOfQueue(Queue*);
int *convertStringToInt(char *, int*);
int **convertStrArr(char **, int);
Queue initializeQueue(int);
Process initializeProcess();
