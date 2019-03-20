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
    int i, j;
    int **intArray = calloc(numProcesses, sizeof(int*));
    assert(intArray != NULL);
    printf("WILL PRINT INT ARRAY!\n");
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
    for (i = 0; i < numProcesses; i++) {
        j = 0;
        while (intArray[i][j] != -1) {
            printf("%d ", intArray[i][j]);
            j++;
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
        // printf("buffer[%d] = %s\n", i, buffer[i]);
        if (buffer[i][0] == 0) {
            *numProcesses = i;
            break;
        }
    }
    printf("%d\n", *numProcesses);
    // buffer = realloc(buffer, *numProcesses * sizeof(char*));
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
    char **input = calloc(MAX_NUM_PROCESSES, sizeof(char*));
    assert(input != NULL);
    int **intArray = calloc(MAX_NUM_PROCESSES, sizeof(int*));
    int numProcesses, i, j;
    

    input = readInput(&numProcesses);
    printf("%d\n", numProcesses);
    for (i = 0; i < numProcesses; i++) {
        printf("%s\n", input[i]);
    }

    intArray = convertStrArr(input, numProcesses);
    printf("\n---------------\n");
    for (i = 0; i < numProcesses; i++) {
        j = 0;
        while (intArray[i][j] != -1) {
            printf("%d ", intArray[i][j]);
            j++;
        }
        printf("\n");
    }
    printf("\n---------------\n");
    print2dIntArray(intArray, numProcesses);
}