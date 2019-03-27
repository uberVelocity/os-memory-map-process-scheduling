#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

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
 * Returns element out of the queue.
 */
Process popElement(Queue *queue) {
    if (queue->front == queue->back) {
        perror("Queue is empty!\n");
        abort();
    }
    else {
        Process p = queue->slots[queue->front];
        for (int i = 0; i < queue->back - 1; i++) {
            queue->slots[i] = queue->slots[i + 1];
        }
        queue->slots[queue->back].id = 0;
        queue->back--;
        return p;
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
 * Initializes a process.
 */
Process initializeProcess(int id) {
    Process process;
    process.id = id;
    process.finished = 0;
    process.completionTime = 0;
    process.burstTime = 0;
    process.waitTime = 0;
    process.turnaroundTime = 0;
    process.pointer = 0;
    process.times = calloc(MAX_NUM_TIMES, sizeof(int));
    return process;
}

/**
 * Sorts a 2D array based off of priority.
 */
int **sort2DArray(int **array, int numProcesses) {
    int *copyPriorities = calloc(numProcesses, sizeof(int));
    assert(copyPriorities != NULL);
    int *orderArray = calloc(numProcesses, sizeof(int));
    assert(orderArray != NULL);
    int i, j, k = 0, l = 0, target, min = array[0][0];
    for (i = 0; i < numProcesses; i++) {
        copyPriorities[i] = array[i][0];
    }
    for (i = 0; i < numProcesses; i++) {
        min = copyPriorities[i];
        target = i;
        for (j = 0; j < numProcesses; j++) {
            if (min > copyPriorities[j]) {
                min = copyPriorities[j];
                target = j;
            }
        }
        copyPriorities[target] = MAX;
        orderArray[k] = min;
        k++;
    }
    int **sortedArray = calloc(numProcesses, sizeof(int*));
    assert(sortedArray != NULL);
    for (i = 0; i < numProcesses; i++) {
        sortedArray[i] = calloc(BUFFER_SIZE, sizeof(int));
        assert(sortedArray[i] != NULL);
    }
    j = 0;
    for (i = 0; i < numProcesses; i++) {
        target = orderArray[i];
        j = 0;
        while (array[j][0] != target) {
            j++;
        }
        k = 0;
        while (array[j][k] != -1) {
            sortedArray[l][k] = array[j][k];
            k++;
        }
        array[j][0] = MAX;
        sortedArray[l][k] = -1;
        l++;
    }
    freeIntArray(array, numProcesses);
    free(copyPriorities);
    free(orderArray);
    return sortedArray;
}

/**
 * Frees every process in the queue.
 */
void freeProcessesInQueue(Queue *q, int numProcesses) {
    for (int i = 0; i < numProcesses; i++) {
        free(q->slots[i].times);
    }
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
 * Initialization of queue of specific size.
 */
Queue initializeQueue(int size, int priority) {
    Queue q;
    q.priority = priority;
    q.size = size;
    q.time = 0;
    q.front = 0;
    q.back = 0;
    q.slots = malloc(size * sizeof(Process));
    assert(q.slots != NULL);
    return q;
}

/**
 * Sets the time values of the process.
 */
void populateProcess(Process *process, int *values) {
    int i = 0;
    while (values[i + 2] != -1) {
        process->times[i] = values[i + 2];
        printf("%d ", process->times[i]);
        i++;
    }
    process->times[i] = values[i + 2];
    printf("%d\n", process->times[i]);
    process->arrivalTime = values[0];
    process->priority = values[1];
    process->end = values[i + 2];
    process->times = realloc(process->times, (i+1) * sizeof(int));
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
 * Populates the process with the values from the sorted array and inserts them
 * into the queue. THE QUEUE IS NOT THE QUEUE WHICH SHOWCASES THE ORDER OF
 * THE PROCESSES. IT IS ONLY USED TO STORE THE PROCESSES.
 */
void populateProcessesIntoQueue(Queue *queue, int **sortedArray, int numProcesses) {
    int i, j;
    for (i = 0; i < numProcesses; i++) {
        Process process = initializeProcess(i);
        populateProcess(&process, sortedArray[i]);
        process.originalArrivalTime = process.arrivalTime;
        insertIntoQueue(&process, queue);
    }
    freeIntArray(sortedArray, numProcesses);
}

/**
 * Prints fields of ALL processes;
 */
void printAllProcessStats(Queue *q, int numProcesses) {
    int i;
    for (i = 0; i < numProcesses; i++) {
        printf("\n-------------\n");
        printProcessStats(q->slots[i]);
        printf("-------------\n");
    }
}

/**
 * Checks whether all pointers are at -1 (all processes have finished).
 */
int processesHaveFinished(Queue *q, int numProcesses) {
    int i;
    for (i = 0; i < numProcesses; i++) {
        if (q->slots[i].finished != 1) {
            return 0;
        }
    }
    return 1;
}

/**
 * Prints fields of process.
 */ 
void printProcessStats(Process p) {
    printf("id:%d\n", p.id);
    printf("arrival:%d\n", p.arrivalTime);
    printf("completion:%d\n", p.completionTime);
    printf("priority:%d\n", p.priority);
    printf("end:%d\n", p.end);
    
}

void printProcessTimes(Queue *queue, int numProcesses) {
    int i;
    for (i = 0; i < numProcesses; i++) {
        int j = 0;
        printf("process:%d\n", i);
        while (queue->slots[i].times[j] != -1) {
            printf("%d ", queue->slots[i].times[j]);
            j++;
        }
        printf("%d", queue->slots[i].times[j]);
        printf("\n");
    }
}

int returnIdOfMinArrivalTime(Queue *queue, int numProcesses) {
    int id = -1, i, min = MAX;
    for (i = numProcesses - 1; i >= 0; i--) {
        if (!queue->slots[i].finished && min > queue->slots[i].arrivalTime) {
            min = queue->slots[i].arrivalTime;
            id = queue->slots[i].id;
        }
    }
    return id;
}

void printAverage(Queue *queue, int numProcesses) {
    int i;
    float average = 0.0;
    for (i = 0; i < numProcesses; i++) {
        average += queue->slots[i].turnaroundTime;
    }
    average /= numProcesses;
    printf("average turnaround = %.2lf\n", average);
}

void printTurnarounds(Queue *queue, int numProcesses) {
    int i;
    for (i = 0; i < numProcesses; i++) {
        printf("turnaround of %d = %d\n", i, queue->slots[i].turnaroundTime);
    }
}

void printTable(Queue *queue, int numProcesses) {
    int i;
    for (i = 0; i < numProcesses; i++) {
        printf("ID:%d\tAT:%d\tCT:%d\tTIME:%d\n", queue->slots[i].id, queue->slots[i].arrivalTime, queue->slots[i].completionTime, queue->time);
    }
    printf("\n-------\n");
}

void FCFS(Queue *queue, int numProcesses) {
    int i, j, minArrivalTimeId;
    while (!processesHaveFinished(queue, numProcesses)) {
        minArrivalTimeId = returnIdOfMinArrivalTime(queue, numProcesses);
        if (queue->time < queue->slots[minArrivalTimeId].arrivalTime) {
            queue->time = queue->slots[minArrivalTimeId].arrivalTime;
        }
        queue->time += queue->slots[minArrivalTimeId].times[queue->slots[minArrivalTimeId].pointer];
        queue->slots[minArrivalTimeId].pointer++;
        if (queue->slots[minArrivalTimeId].times[queue->slots[minArrivalTimeId].pointer] == -1) {
            queue->slots[minArrivalTimeId].finished = 1;
            queue->slots[minArrivalTimeId].completionTime = queue->time;
            queue->slots[minArrivalTimeId].turnaroundTime = queue->slots[minArrivalTimeId].completionTime - queue->slots[minArrivalTimeId].originalArrivalTime;
        }
        if (queue->slots[minArrivalTimeId].finished != 1) {
            queue->slots[minArrivalTimeId].arrivalTime = queue->time + queue->slots[minArrivalTimeId].times[queue->slots[minArrivalTimeId].pointer];
            queue->slots[minArrivalTimeId].pointer++;
            if (queue->slots[minArrivalTimeId].times[queue->slots[minArrivalTimeId].pointer] == -1) {
                queue->slots[minArrivalTimeId].finished = 1;
                queue->slots[minArrivalTimeId].completionTime = queue->time;
                queue->slots[minArrivalTimeId].turnaroundTime = queue->slots[minArrivalTimeId].completionTime - queue->slots[minArrivalTimeId].originalArrivalTime;
            }
        }
        printTable(queue, numProcesses);
    }
    printTurnarounds(queue, numProcesses);
    printAverage(queue, numProcesses);
}

int main(int argc, char* argv[]) {
    char **input;
    int **intArray, numProcesses, i, j, **sortedArray;
    input = readInput(&numProcesses);
    intArray = convertStrArr(input, numProcesses);
    sortedArray = sort2DArray(intArray, numProcesses);
    Queue queue = initializeQueue(numProcesses, P_DEFAULT);
    populateProcessesIntoQueue(&queue, sortedArray, numProcesses);
    printProcessesInQueue(queue, numProcesses);
    printProcessTimes(&queue, numProcesses);
    printAllProcessStats(&queue, numProcesses);
    FCFS(&queue, numProcesses);
    // printAllProcessStats(&queue, numProcesses);

    freeProcessesInQueue(&queue, numProcesses);
    free(queue.slots);
}
