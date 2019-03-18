#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#include "fcfs.h"


Queue initializeQueue(int size) {
    Queue q;
    q.size = size;
    q.slots = malloc(size * sizeof(int));
    assert(q.slots != NULL);
    return q;
}

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
    printf("Size of queue = %d\n", queue.size);
    if (!doubleSizeOfQueue(&queue)) {
        printf("ALLOC FAILED!\n");
    }
    printf("Size of queue = %d\n", queue.size);
}