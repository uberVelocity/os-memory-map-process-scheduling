#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>

#include "fcfs.h"

/**
 * Convert string input into int array.
 */
int *convertStringToInt(char *string) {
    int i;
    int *array = calloc(strlen(string), sizeof(int));
    assert(array != NULL);
    for (i = 0; i < strlen(string); i++) {
        array[i] = string[i];
    }
    return array;
}

/**
 * Remove newline from input.
 */
void removeNewline(char **buffer, int inputSize) {
    for (int i = 0; i < inputSize; i++) {
        buffer[i][strlen(buffer[i]) - 1] = 0;
    }
}

/**
 * Resizes the buffer from MAX_NUM_PROCESSES to inputSize.
 */
void resizeBuffer(char **buffer, int *inputSize) {
    int i;
    *inputSize = 0;
    for (i = 0; i < MAX_NUM_PROCESSES; i++) {
        if (buffer[i][0] == 0) {
            printf("newline encountered at %d\n", i);
            *inputSize = i;
            break;
        }
    }
    buffer = realloc(buffer, *inputSize * sizeof(char*));
    assert(buffer != NULL);
    removeNewline(buffer, *inputSize);
}

/**
 * Reads the input as a string.
 */
char **readInput(int *inputSize) {
    int i = 0;
    char **buffer = calloc(MAX_NUM_PROCESSES, sizeof(char*));
    assert(buffer != NULL);
    for (i = 0; i < MAX_NUM_PROCESSES; i++) {
        buffer[i] = calloc(BUFFER_SIZE, sizeof(char));
        assert(buffer[i] != NULL);
        fgets(buffer[i], BUFFER_SIZE, stdin);
    }
    resizeBuffer(buffer, inputSize);
    return buffer;
}

/**
 * Sets the time values of the process.
 */
void populateProcess(Process *process) {

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
    int inputSize;
    char **input = readInput(&inputSize);
    printf("inputSize = %d\n", inputSize);
    
}