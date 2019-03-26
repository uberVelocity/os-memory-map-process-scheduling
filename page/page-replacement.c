#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>

#include "page-replacement.h"


/**
 * Initializes queue that holds `size` amount of 
 * pages.
 */
Queue initializeQueue(int size) {
    Queue q;
    q.array = calloc(size, sizeof(int));
    assert(q.array != NULL);
    q.front = 0;
    q.back = 0;
    q.size = size;
    q.pageFaults = 0;
    return q;
}

/**
 * Removes the first page of the queue and adds
 * the `element` at the end of the queue.
 */
void replaceFirstPage(Queue *q, int element) {
    for (int i = 0; i < q->size - 1; i++) {
        q->array[i] = q->array[i + 1];
    }
    q->back--;
    q->array[q->back] = element;
    q->back++;
}

/**
 * Returns 1 if `element` is in queue, 0 otherwise.
 */
int elementInQueue(Queue *q, int element) {
    for (int i = 0; i < q->size; i++) {
        // printf("comparing %d with element %d\n", q->array[i], element);
        if (element == q->array[i]) {
            return 1;
        }
    }
    return 0;
}

/**
 * Enqueues an element into the queue. If the element
 * is in queue then it does not enqueue it.
 */
void enqueue(Queue *q, int element) {
    // printf("Attempting to enqueue: %d\n", element);
    if (!elementInQueue(q, element)) {
        // printf("element %d is not in queue\n", element);
        if (q->size == q->back) {
            replaceFirstPage(q, element);
        }
        else {
            q->array[q->back] = element;
            q->back++;
        }
        q->pageFaults++;
    }
    // Nothing happens if element is already in the queue

}

int *convertInput(int *frames, int *inputSize) {
    int i = 0, local;
    char *charPages = calloc(MAX_REFERENCES, sizeof(char)), *cp;
    assert(charPages != NULL);
    int *final = calloc(MAX_REFERENCES, sizeof(int));
    scanf("%d\n", &local);
    *frames = local;
    fgets(charPages, 100, stdin);
    printf("input = %s\n", charPages);
    cp = strtok(charPages, " ");
    while (cp != NULL) {
        final[i] = atoi(cp);
        cp = strtok(NULL, " ");
        i++;
    }
    *inputSize = i;
    free(charPages);
    return final;
}

void printStateOfQueue(Queue *queue) {
    for (int i = 0; i < queue->size; i++) {
        printf("[%d] ", queue->array[i]);
    }
    printf("\n");
}

int main(int argc, char* argv[]) {
    int frames, *pages, i = 0, inputSize;
    pages = convertInput(&frames, &inputSize);
    Queue queue = initializeQueue(frames);

    for (i = 0; i < inputSize; i++) {
        enqueue(&queue, pages[i]);
        printStateOfQueue(&queue);
    }

    printf("page faults = %d\n", queue.pageFaults);

    free(pages);
    free(queue.array);
    return EXIT_SUCCESS;
}