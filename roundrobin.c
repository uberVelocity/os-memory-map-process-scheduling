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
 * Sets the time values of the process.
 */
void populateProcess(Process *process, int *values) {
    int i = 0;
    while (values[i + 2] != -1) {
        process->times[i] = values[i + 2];
        i++;
    }
    process->times[i] = values[i + 2];
    process->arrivalTime = values[0];
    process->priority = values[1];
    process->end = values[i + 2];
    // process->times = realloc(process->times, (i+1) * sizeof(int));
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
 * Initialization of queue of specific size.
 */
Queue initializeQueue(int size, int priority, int members) {
    Queue q;
    q.priority = priority;
    q.size = size;
    q.time = 0;
    q.members = members;
    q.processPointer = 0;
    q.front = 0;
    q.back = 0;
    if (size == 0)  q.slots = malloc(MAX_NUM_PROCESSES * sizeof(Process));
    else q.slots = malloc(size * sizeof(Process));
    assert(q.slots != NULL);
    return q;
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
 * Prints the average turn-around time of the processes.
 */
void printAverage(Queue *queue, int numProcesses) {
    int i;
    float average = 0.0;
    for (i = 0; i < numProcesses; i++) {
        average += queue->slots[i].turnaroundTime;
    }
    average /= numProcesses;
    printf("FCFS: average turnaround = %.2lf\n", average);
}

/**
 * Prints the processing times of all processes in the queue.
 */
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

/**
 * Checks whether all pointers are at -1 (all processes have finished).
 */
int processesHaveFinished(Queue *q, int numProcesses) {
    int i;
    // printf("numProcesses = %d\n", numProcesses);
    for (i = 0; i < numProcesses; i++) {
        if (q->slots[i].finished != 1) {
            return 0;
        }
    }
    return 1;
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
 * Populates all of the priority queues with the corresponding processes.
 */
void populateQueues(int **sortedArray, Queue *lowQueue, Queue *medQueue, Queue *highQueue, int numProcesses) {
    int i;
    for (i = 0; i < numProcesses; i++) {
        Process process = initializeProcess(i);
        populateProcess(&process, sortedArray[i]);
        process.originalArrivalTime = process.arrivalTime;
        switch (process.priority) {
            case P_HIGH:
                insertIntoQueue(&process, highQueue);
                break;
            case P_MEDIUM:
                insertIntoQueue(&process, medQueue);
                break;
            case P_LOW:
                insertIntoQueue(&process, lowQueue);
                break;
            default:
                insertIntoQueue(&process, lowQueue);
                break;
        }
    }
}

/**
 * Checks which process arrives first from the three queues.
 * Use the time at which it arrives to update the clock.
 * Used only when all processes at the pointers are doing I/O
 * so there are no runnable processes.
 */
int fastForward(Queue *lowQueue, Queue *medQueue, Queue *highQueue) {
    int min = MAX;
    if (lowQueue->members > 0 && min > lowQueue->slots[lowQueue->processPointer].arrivalTime) {
        min = lowQueue->slots[lowQueue->processPointer].arrivalTime;
    }
    if (medQueue->members > 0 && min > medQueue->slots[medQueue->processPointer].arrivalTime) {
        min = medQueue->slots[medQueue->processPointer].arrivalTime;
    }
    if (highQueue->members > 0 && min > highQueue->slots[highQueue->processPointer].arrivalTime) {
        min = highQueue->slots[highQueue->processPointer].arrivalTime;
    }
    return min;

}

int allQueuesFinished(Queue *lowQueue, Queue *medQueue, Queue *highQueue) {
    printf("l:%d m:%d h:%d\n", processesHaveFinished(lowQueue, lowQueue->members), processesHaveFinished(medQueue, medQueue->members), processesHaveFinished(highQueue, highQueue->members));
    return (processesHaveFinished(lowQueue, lowQueue->members) && processesHaveFinished(medQueue, medQueue->members) && processesHaveFinished(highQueue, highQueue->members));
}

void roundRobinPriority(Queue *lowQueue, Queue *medQueue, Queue *highQueue) {
    int pclock = 0;
    while (!allQueuesFinished(lowQueue, medQueue, highQueue)) {
        // Check if the high-priority process at pointer position
        // has arrived and is ready to be run.
        printf("highQueue->members = %d\n", highQueue->members);
        printf("highQueue->slots[highQueue->processPointer].arrivalTime = %d\n", highQueue->slots[highQueue->processPointer].arrivalTime);
        printf("pclock = %d\n", pclock);
        printf("!processesHaveFinished = %d\n\n", !processesHaveFinished(highQueue, highQueue->members));

        if (highQueue->members > 0 && highQueue->slots[highQueue->processPointer].arrivalTime <= pclock && !processesHaveFinished(highQueue, highQueue->members)) {
            printf("working on high - pv:%d\tclock:%d\n", highQueue->slots[highQueue->processPointer].pointer, pclock);
            // Update time.
            if (highQueue->slots[highQueue->processPointer].
            times[highQueue->slots[highQueue->processPointer].pointer] >= QUANTUM) {
                pclock += QUANTUM;
            }
            else {
                pclock += highQueue->slots[highQueue->processPointer].
                times[highQueue->slots[highQueue->processPointer].pointer];
            }
            highQueue->slots[highQueue->processPointer].
            times[highQueue->slots[highQueue->processPointer].pointer] -= QUANTUM;
            // If we finished a CPU burst, two options: finished or I/O.
            if (highQueue->slots[highQueue->processPointer].
            times[highQueue->slots[highQueue->processPointer].pointer] <= 0) {
                // Pointer of processor operation gets incremented.
                // (CAN LAND ON I/O OR -1).
                highQueue->slots[highQueue->processPointer].pointer++;
                // Process ended.
                printf("pointer value with which we compare = %d\n", highQueue->slots[highQueue->processPointer].times[highQueue->slots[highQueue->processPointer].pointer]);
                if (highQueue->slots[highQueue->processPointer].times[highQueue->slots[highQueue->processPointer].pointer] == -1) {
                    printf("SETTING PROCESS FROM HIGH QUEUE TO FINISHED\n");
                    highQueue->slots[highQueue->processPointer].finished = 1;
                    // Compute completionTime and turnaroundTime.
                    highQueue->slots[highQueue->processPointer].completionTime = pclock;
                    highQueue->slots[highQueue->processPointer].turnaroundTime = highQueue->slots[highQueue->processPointer].completionTime - highQueue->slots[highQueue->processPointer].originalArrivalTime;
                    highQueue->slots[highQueue->processPointer].arrivalTime = MAX;
                }
                // I/O time
                else {
                    highQueue->slots[highQueue->processPointer].arrivalTime = pclock + highQueue->slots[highQueue->processPointer].times[highQueue->slots[highQueue->processPointer].pointer];
                    highQueue->slots[highQueue->processPointer].pointer++;
                }
            }
            // Update process pointer position for round-robin.
            if (highQueue->processPointer == highQueue->members - 1) {
                highQueue->processPointer = 0;
            }
            else {
               
                highQueue->processPointer++;

            }
            // We are not interested in processes that have finished, so first we check
            // whether the whole queue has finished.
            if (!processesHaveFinished(highQueue, highQueue->members)) {
                // We go to the next process that has not finished within the queue.
                while (highQueue->slots[highQueue->processPointer].finished == 1) {
                    if (highQueue->processPointer == highQueue->members - 1) {
                        highQueue->processPointer = 0;
                    }
                    else {
                        highQueue->processPointer++;
                    }
                }
            }
        }

        // MEDIUM QUEUE
        else if (medQueue->members > 0 && medQueue->slots[medQueue->processPointer].arrivalTime <= pclock && !processesHaveFinished(medQueue, medQueue->members)) {
            printf("working on med - pv:%d\tclock:%d\n", medQueue->slots[medQueue->processPointer].pointer, pclock);
            // Update time.
            if (medQueue->slots[medQueue->processPointer].
            times[medQueue->slots[medQueue->processPointer].pointer] >= QUANTUM) {
                pclock += QUANTUM;
                medQueue->slots[medQueue->processPointer].
                times[medQueue->slots[medQueue->processPointer].pointer] -= QUANTUM;
            }
            else {
                pclock += medQueue->slots[medQueue->processPointer].
                times[medQueue->slots[medQueue->processPointer].pointer];
            }
            medQueue->slots[medQueue->processPointer].
            times[medQueue->slots[medQueue->processPointer].pointer] -= QUANTUM;
            // If we finished a CPU burst, two options: finished or I/O.
            if (medQueue->slots[medQueue->processPointer].
            times[medQueue->slots[medQueue->processPointer].pointer] <= 0) {
                // Pointer of processor operation gets incremented.
                // (CAN LAND ON I/O OR -1).
                medQueue->slots[medQueue->processPointer].pointer++;
                printf("pointer value with which we compare = %d\n", medQueue->slots[medQueue->processPointer].times[medQueue->slots[medQueue->processPointer].pointer]);

                // Check if process ended.
                if (medQueue->slots[medQueue->processPointer].
                times[medQueue->slots[medQueue->processPointer].pointer] == -1) {
                    printf("SETTING PROCESS FROM MEDIUM QUEUE TO FINISHED\n");
                    medQueue->slots[medQueue->processPointer].finished = 1;
                    // Compute completionTime and turnaroundTime.
                    medQueue->slots[medQueue->processPointer].completionTime = pclock;
                    medQueue->slots[medQueue->processPointer].turnaroundTime = medQueue->slots[medQueue->processPointer].completionTime - medQueue->slots[medQueue->processPointer].originalArrivalTime;
                    medQueue->slots[medQueue->processPointer].arrivalTime = MAX;

                }
                // I/O time
                else {
                    medQueue->slots[medQueue->processPointer].arrivalTime = pclock + medQueue->slots[medQueue->processPointer].times[medQueue->slots[medQueue->processPointer].pointer];
                    medQueue->slots[medQueue->processPointer].pointer++;                
                }
            }
            // Update process pointer position for round-robin.
            if (medQueue->processPointer == medQueue->members - 1) {
                medQueue->processPointer = 0;
            }
            else {
                medQueue->processPointer++;
            }
            // We are not interested in processes that have finished, so first we check
            // whether the whole queue has finished.
            if (!processesHaveFinished(medQueue, medQueue->members)) {
                // We go to the next process that has not finished within the queue.
                while (medQueue->slots[medQueue->processPointer].finished == 1) {
                    if (medQueue->processPointer == medQueue->members - 1) {
                        medQueue->processPointer = 0;
                    }
                    else {
                        medQueue->processPointer++;
                    }
                }
            }
        }

        // LOW QUEUE
        else if (lowQueue->members > 0 && lowQueue->slots[lowQueue->processPointer].arrivalTime <= pclock && !processesHaveFinished(lowQueue, lowQueue->members)) {
        // Update time.
            printf("working on low - pv:%d\tclock:%d\n", lowQueue->slots[lowQueue->processPointer].pointer, pclock);
            if (lowQueue->slots[lowQueue->processPointer].
            times[lowQueue->slots[lowQueue->processPointer].pointer] >= QUANTUM) {
                pclock += QUANTUM;
                lowQueue->slots[lowQueue->processPointer].
                times[lowQueue->slots[lowQueue->processPointer].pointer] -= QUANTUM;
            }
            else {
                pclock += lowQueue->slots[lowQueue->processPointer].
                times[lowQueue->slots[lowQueue->processPointer].pointer];
            }
            lowQueue->slots[lowQueue->processPointer].
            times[lowQueue->slots[lowQueue->processPointer].pointer] -= QUANTUM;
            // If we finished a CPU burst, two options: finished or I/O.
            if (lowQueue->slots[lowQueue->processPointer].
            times[lowQueue->slots[lowQueue->processPointer].pointer] <= 0) {
                // Pointer of processor operation gets incremented.
                // (CAN LAND ON I/O OR -1).
                lowQueue->slots[lowQueue->processPointer].pointer++;
                printf("pointer value with which we compare = %d\n", lowQueue->slots[lowQueue->processPointer].times[lowQueue->slots[lowQueue->processPointer].pointer]);

                // Check if process ended.
                if (lowQueue->slots[lowQueue->processPointer].
                times[lowQueue->slots[lowQueue->processPointer].pointer] == -1) {
                    printf("SETTING PROCESS FROM LOW QUEUE TO FINISHED\n");
                    lowQueue->slots[lowQueue->processPointer].finished = 1;
                    // Compute completionTime and turnaroundTime.
                    lowQueue->slots[lowQueue->processPointer].completionTime = pclock;
                    lowQueue->slots[lowQueue->processPointer].turnaroundTime = lowQueue->slots[lowQueue->processPointer].completionTime - lowQueue->slots[lowQueue->processPointer].originalArrivalTime;
                    lowQueue->slots[lowQueue->processPointer].arrivalTime = MAX;

                }
                // I/O time
                else {
                    lowQueue->slots[lowQueue->processPointer].arrivalTime = pclock + lowQueue->slots[lowQueue->processPointer].times[lowQueue->slots[lowQueue->processPointer].pointer];
                    lowQueue->slots[lowQueue->processPointer].pointer++;                
                }
            }
            // Update process pointer position for round-robin.
            if (lowQueue->processPointer == lowQueue->members - 1) {
                lowQueue->processPointer = 0;
            }
            else {
                lowQueue->processPointer++;
            }
            // We are not interested in processes that have finished, so first we check
            // whether the whole queue has finished.
            if (!processesHaveFinished(lowQueue, lowQueue->members)) {
                // We go to the next process that has not finished within the queue.
                while (lowQueue->slots[lowQueue->processPointer].finished == 1) {
                    if (lowQueue->processPointer == lowQueue->members - 1) {
                        lowQueue->processPointer = 0;
                    }
                    else {
                        lowQueue->processPointer++;
                    }
                }
            }
        }

        // All processes are doing I/O, so processor should wait for the
        // first runnable process. If there are multiple finishing at the same time,
        // they should be execited by priority.
        else {
            printf("IN THE ELSE\n");
            pclock = fastForward(lowQueue, medQueue, highQueue);
        }
    }
}

void countProcesses(int **sorted, int *l, int *m, int *h, int numProcesses) {
    int i;
    for (i = 0; i < numProcesses; i++) {
        if (sorted[i][1] == 1) {
            *h = *h + 1;
        }
        else if (sorted[i][1] == 2) {
            *m = *m + 1;
        }
        else if (sorted[i][1] == 3) {
            *l = *l + 1;
        }
    }
}

void printTotalAverage(Queue *low, Queue *med, Queue *high) {
    int i, j;
    float la = 0.0, ma = 0.0, ha = 0.0;
    int count = 0;
    if (low->members > 0) {
        for (i = 0; i < low->members; i++) {
            la += low->slots[i].turnaroundTime;
        }
        la /= low->members;
    }
    printf("la = %.2lf\n", la);
    if (med->members > 0) {
        for (i = 0; i < med->members; i++) {
            ma += med->slots[i].turnaroundTime;
        }
        ma /= med->members;
    }
    printf("ma = %.2lf\n", ma);
    
    if (high->members > 0) {
        for (i = 0; i < high->members; i++) {
            ha += high->slots[i].turnaroundTime;
        }
        ha /= high->members;
    }
    
    printf("ha = %.2lf\n", ha);

    if (low->members > 0)   count++;
    if (med->members > 0)   count++;
    if (high->members > 0)  count++;

    if (count > 0) {
        printf("Total average = %.2lf\n", (la + ma + ha)/count);
    }
}

int main(int argc, char* argv[]) {
    char **input;
    int **intArray, numProcesses, i, j, **sortedArray, demo_mode = 0, l = 0, m = 0, h = 0;
    if (argv[1] != NULL && strcmp(argv[1], "demo") == 0) {
        demo_mode = 1;
    }
    input = readInput(&numProcesses);
    intArray = convertStrArr(input, numProcesses);
    sortedArray = sort2DArray(intArray, numProcesses);
    countProcesses(sortedArray, &l, &m, &h, numProcesses);
    printf("COUNT PROCESSES: l = %d, m = %d, h = %d\n", l, m, h);
    Queue lowQueue = initializeQueue(numProcesses, P_LOW, l);
    Queue medQueue = initializeQueue(numProcesses, P_MEDIUM, m);
    Queue highQueue = initializeQueue(numProcesses, P_HIGH, h);
    populateQueues(sortedArray, &lowQueue, &medQueue, &highQueue, numProcesses);
    
    if (l > 0) {
        printf("lowQueue:\n");
        printProcessesInQueue(lowQueue, l);
    }
    if (m > 0) {
        printf("medQueue:\n");
        printProcessesInQueue(medQueue, m);
    }
    if (h > 0) {
        printf("highQueue:\n");
        printProcessesInQueue(highQueue, h);
    }

    roundRobinPriority(&lowQueue, &medQueue, &highQueue);

    printTotalAverage(&lowQueue, &medQueue, &highQueue);

    freeProcessesInQueue(&lowQueue, lowQueue.members);
    freeProcessesInQueue(&medQueue, medQueue.members);
    freeProcessesInQueue(&highQueue, highQueue.members);
    freeIntArray(sortedArray, numProcesses);
    free(lowQueue.slots);
    free(medQueue.slots);
    free(highQueue.slots);
}