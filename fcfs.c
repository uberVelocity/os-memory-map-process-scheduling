#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>

#include "fcfs.h"


/**
 * Remove newline from input.
 */
void removeNewline(char **buffer, int numProcesses) {
    for (int i = 0; i < numProcesses; i++) {
        buffer[i][strlen(buffer[i]) - 1] = 0;
    }
}

/**
 * Free char **array.
 */
void freeStrArray(char **array, int size) {
    for (int i = 0; i < size; i++) {
        free(array[i]);
    }
    free(array);
}

/**
 * Free int **array.
 */
void freeIntArray(int **array, int size) {
    for (int i = 0; i < size; i++) {
        free(array[i]);
    }
    free(array);
}

/**
 * Converts array of strings to array of ints.
 */
int **convertStrArr(char **stringArray, int numProcesses) {
    int i, j;
    int **intArray = calloc(numProcesses, sizeof(int*));
    assert(intArray != NULL);
    for (i = 0; i < numProcesses; i++) {
        char *cp = strtok(stringArray[i], " ");
        intArray[i] = calloc(BUFFER_SIZE, sizeof(int));
        assert(intArray[i] != NULL);
        j = 0;
        while (cp != NULL) {
            intArray[i][j] = atoi(cp);
            cp = strtok(NULL, " ");
            j++;
        }
    }
    freeStrArray(stringArray, MAX_NUM_PROCESSES);
    return intArray;
}

/**
 * Resizes the buffer from MAX_NUM_PROCESSES to inputSize.
 */
void resizeBuffer(char **buffer, int *numProcesses) {
    int i;
    *numProcesses = 0;
    for (i = 0; i < MAX_NUM_PROCESSES; i++) {
        if (buffer[i][0] == 0) {
            *numProcesses = i;
            break;
        }
    }
    assert(buffer != NULL);
}

/**
 * Reads the input as a string.
 */
char **readInput(int *numProcesses) {
    int i = 0;
    char **buffer = calloc(MAX_NUM_PROCESSES, sizeof(char*));
    for (i = 0; i < MAX_NUM_PROCESSES; i++) {
        buffer[i] = calloc(BUFFER_SIZE, sizeof(char));
        assert(buffer[i] != NULL);
        fgets(buffer[i], BUFFER_SIZE, stdin);
    }
    resizeBuffer(buffer, numProcesses);
    removeNewline(buffer, *numProcesses);
    return buffer;
}

/**
 * Print a 2D int array.
 */
void print2dIntArray(int **array, int numProcesses) {
    for (int i = 0; i < numProcesses; i++) {
        int j = 0;
        while (array[i][j] != -1) {
            printf("%d ", array[i][j]);
            j++;
        }
        printf("%d ", array[i][j]);
        printf("\n");
    }
}

/**
 * Sets the time values of the process.
 */
void populateProcess(Process *process, int *values) {
    int i = 0;
    while (values[i] != -1) {
        process->times[i] = values[i];
        i++;
    }
    process->times[i] = values[i];
    process->arrivalTime = values[0];
    process->priority = values[1];
    process->end = values[i];
    process->times = realloc(process->times, (i+1) * sizeof(int));
}

/**
 * Populates the queue with processes
 * given by the user.
 */
void populateQueue(Queue *queue, int numProcesses, int **intArray) {
    for (int i = 0; i < numProcesses; i++) {
        Process process = initializeProcess();
        populateProcess(&process, intArray[i]);
        insertIntoQueue(&process, queue);
    }
    freeIntArray(intArray, numProcesses);
}

/**
 * Inserts a process into the queue.
 */
void insertIntoQueue(Process *p, Queue *q) {
    if (q->back == q->size) {
        if (!doubleSizeOfQueue(q)) {
            perror("Error: failed to double size of queue...\n");
            abort();
        }
    }
    q->slots[q->back] = *p;
    q->back++;
}

/**
 * Initialization of queue of specific size.
 */
Queue initializeQueue(int size, int priority) {
    Queue q;
    q.priority = priority;
    q.size = size;
    q.front = 0;
    q.back = 0;
    q.slots = malloc(size * sizeof(Process));
    assert(q.slots != NULL);
    return q;
}

/**
 * Frees the slots of every process in the queue.
 */
void freeProcessesInQueue(Queue *q, int numProcesses) {
    for (int i = 0; i < numProcesses; i++) {
        free(q->slots[i].times);
    }
}

/**
 * Initializes a process.
 */
Process initializeProcess() {
    Process process;
    process.turnaroundTime = 0;
    process.times = calloc(MAX_NUM_TIMES, sizeof(int));
    return process;
}

/**
 * Prints the running times of each process in the queue.
 */
void printProcessesInQueue(Queue q, int numProcesses) {
    for (int i = 0; i < numProcesses; i++) {
        printf("process[%d]=", i);
        int j = 0;
        while (q.slots[i].times[j] != -1) {
            printf("%d ", q.slots[i].times[j]);
            j++;
        }
        printf("%d", q.slots[i].times[j]);
        printf("\n");
    }
}

/**
 * Doubles the size of a queue. 
 */
int doubleSizeOfQueue(Queue *q) {
    int oldSize = q->size;
    int newSize = oldSize * 2;
    q->slots = realloc(q->slots, newSize * sizeof(int));
    assert(q->slots != NULL);
    if (q->slots != NULL) {
        q->size = newSize;
        return 1;
    }
    return 0;
}

/**
 * Computes average turnaround time for a queue of processes using the 
 * First-come-First-served strategy.
 */
void compTurnaroundFCFS(Queue *q, int numProcesses) {
    float average = 0.0;
    int i, j;
    for (i = 0; i < numProcesses; i++) {
        j = 2;
        q->slots[i].turnaroundTime -= q->slots[i].arrivalTime;
        if (i > 0) {
            q->slots[i].turnaroundTime += q->slots[i-1].turnaroundTime;
        }
        while (q->slots[i].times[j] != q->slots[i].end) {
            q->slots[i].turnaroundTime += q->slots[i].times[j];
            j++;
        }
        printf("Total time of process %d = %d\n", i, q->slots[i].turnaroundTime);
    }
    for (i = 0; i < numProcesses; i++) {
        average += q->slots[i].turnaroundTime;
    }
    average /= numProcesses;
    printf("Average of processes = %.2f\n", average);
}

/**
 * Simulates the First-come-first-served (FCFS) algorithm on a set of 
 * processes with running times given from stdin. 
 */
void FCFS() {
    char **input = calloc(MAX_NUM_PROCESSES, sizeof(char*));
    int **intArray, numProcesses, i, j;
    assert(input != NULL);
    input = readInput(&numProcesses);
    intArray = convertStrArr(input, numProcesses);
    Queue queue = initializeQueue(numProcesses, P_DEFAULT);

    print2dIntArray(intArray, numProcesses);
    populateQueue(&queue, numProcesses, intArray);
    printProcessesInQueue(queue, numProcesses);
    compTurnaroundFCFS(&queue, numProcesses);

    freeProcessesInQueue(&queue, numProcesses);
    free(queue.slots);
}

int main(int argc, char* argv[]) {
    FCFS();
}