#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>

#include "fcfs.h"

/**
 * Convert string input into int array.
 */
int *convertStringToInt(char *string, int *intArray) {
    int i;
    intArray = calloc(strlen(string), sizeof(int));
    assert(intArray != NULL);
    for (i = 0; i < strlen(string); i++) {
        intArray[i] = (int)string[i];
    }
    return intArray;
}

/**
 * Remove newline from input.
 */
void removeNewline(char **buffer, int numProcesses) {
    for (int i = 0; i < numProcesses; i++) {
        buffer[i][strlen(buffer[i]) - 1] = 0;
    }
}

/**
 * Free char *array.
 */
void freeStrArray(char **array, int size) {
    for (int i = 0; i < size; i++) {
        free(array[i]);
    }
    free(array);
}

/**
 * Converts array of strings to array of ints.
 */
int **convertStrArr(char **stringArray, int numProcesses) {
    printf("num processes!#!@#!@$ : %d\n", numProcesses);
    int **intArray = calloc(numProcesses, sizeof(int*)), i;
    for (i = 0; i < numProcesses; i++) {
        printf("string=%s\n", stringArray[i]);
    }
    assert(intArray != NULL);
    for (int i = 0; i < numProcesses; i++) {
        convertStringToInt(stringArray[i], intArray[i]);
        for (int j = 0; j < strlen(stringArray[i]); j++) {
            printf("%d ", intArray[i][j]);
        }
        printf("\n");
    }
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
            printf("newline encountered at %d\n", i);
            *numProcesses = i;
            break;
        }
    }
    buffer = realloc(buffer, *numProcesses * sizeof(char*));
    assert(&buffer != NULL);
    removeNewline(buffer, *numProcesses);
}

/**
 * Reads the input as a string.
 */
char **readInput(int *numProcesses) {
    int i = 0;
    char **buffer = calloc(MAX_NUM_PROCESSES, sizeof(char*));
    assert(buffer != NULL);
    for (i = 0; i < MAX_NUM_PROCESSES; i++) {
        buffer[i] = calloc(BUFFER_SIZE, sizeof(char));
        assert(buffer[i] != NULL);
        fgets(buffer[i], BUFFER_SIZE, stdin);
    }
    resizeBuffer(buffer, numProcesses);
    return buffer;
}

/**
 * Sets the time values of the process.
 */
void populateProcess(Process *process) {

}

void print2dIntArray(int **array, int numProcesses) {
    for (int i = 0; i < numProcesses; i++) {
        int j = 0;
        while (array[i][j] != -1) {
            printf("%d ", array[i][j]);
            j++;
        }
        printf("\n");
    }
}

/**
 * Populates the queue with processes
 * given by the user.
 */
void populateQueue(Queue *queue) {
    int i = 0;
    // while(in) {
    //     Process p = initializeProcess();
    //     populateProcess(in, p);
    // }

}

/**
 * Initialization of queue of specific size.
 */
Queue initializeQueue(int size) {
    Queue q;
    q.size = size;
    q.slots = malloc(size * sizeof(Process));
    assert(q.slots != NULL);
    return q;
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

int main(int argc, char* argv[]) {
    int numProcesses, i;
    char **input = readInput(&numProcesses);
    for (i = 0; i < numProcesses; i++) {
        printf("%s\n", input[i]);
    }
    printf("num processes = %d\n", numProcesses);
    int **arrayInt = convertStrArr(input, numProcesses);
    // print2dIntArray(convertedInput, numProcesses);
}