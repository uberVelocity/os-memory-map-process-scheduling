#include <stdio.h>
#include <stdlib.h>

#define MAX_NUM_PROCESSES 16
#define MAX_NUM_TIMES 64
#define QUEUE_SIZE 32
#define BUFFER_SIZE 1024

#define P_DEFAULT 0
#define P_HIGH 1
#define P_MEDIUM 2
#define P_LOW 3

#define MAX 9999

typedef struct Processes {
    int id;
    int pointer;
    int finished;
    int completionTime;
    int burstTime;
    int waitTime;
    int originalArrivalTime;
    int turnaroundTime;
    int arrivalTime;
    int priority;
    int end;
    int *times; //even:CPU, odd:IO.
}Process;

typedef struct Queues {
    int priority;   // 0 - DEFAULT, 1 - HIGH, 2 - MEDIUM, 3 - LOW.
    int size;
    int front;
    int time;
    int back;
    Process *slots;
}Queue;

void FCFS();
void computeCompletionTime(Queue *, int);
void computeWaitTime(Queue *, int);
void computeBurst(Process *);
void printAverage(Queue *, int);
void compTurnaroundTime(Queue *, int);
void checkIfEntered(Queue *, int);
void freeProcessesInQueue(Queue *, int);
void removeNewline(char **, int);
void populateQueue(Queue *, int, int **);
void populateProcess(Process *, int *);
void insertIntoQueue(Process *, Queue *);
void resizeBuffer(char **, int *);
void freeStrArray(char **, int);
void freeIntArray(int **, int);
void print2dIntArray(int **, int);
void printAllProcessStats(Queue *, int);
void printProcessStats(Process);
void printProcessesInQueue(Queue, int);
char **readInput(int *);
int processesHaveFinished(Queue *, int);
int doubleSizeOfQueue(Queue *);
int *convertStringToInt(char *, int*);
int **convertStrArr(char **, int);
int **sort2DArray(int **, int);
Queue initializeQueue(int, int);
Process initializeProcess();
Process popElement(Queue *);
