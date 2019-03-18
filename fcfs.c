#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#include "fcfs.h"

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
    while(in) {
        Process p = initializeProcess();
        populateProcess(in, p);
    }

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
    Queue queue = initializeQueue(QUEUE_SIZE);
    populateQueue(&queue);
    
}