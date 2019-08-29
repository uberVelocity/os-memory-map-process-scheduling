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
    for (int i = 0; i < size; i++) {
        q.array[i] = -1;
    }
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
    if (!elementInQueue(q, element)) {
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

/**
 * Reads input from stdin and converts it from string to int.
 */
int *convertInput(int *frames, int *inputSize) {
    int i = 0, local;
    char *charPages = calloc(201, sizeof(char)), *cp;
    assert(charPages != NULL);
    int *final = calloc(MAX_REFERENCES, sizeof(int));
    scanf("%d\n", &local);
    *frames = local;
    fgets(charPages, 201, stdin);
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

/**
 * Prints the state of a queue.
 */
void printStateOfQueue(Queue *queue) {
    for (int i = 0; i < queue->size; i++) {
        printf("[%d] ", queue->array[i]);
    }
    printf("\n");
}

/**
 * Implements the first-in-first-out page-replacement algorithm.
 */
void FIFO(int demo_mode) {
    int frames, *pages, i = 0, inputSize;
    pages = convertInput(&frames, &inputSize);
    Queue queue = initializeQueue(frames);

    for (i = 0; i < inputSize; i++) {
        enqueue(&queue, pages[i]);
        if (demo_mode)  printStateOfQueue(&queue);
    }

    printf("page faults = %d\n", queue.pageFaults);

    free(pages);
    free(queue.array);
}

/**
 * Initializes a circular list.
 */
CList initializeCList(int size) {
    CList clist;
    clist.array = calloc(size, sizeof(int));
    clist.secondChances = calloc(size, sizeof(int));
    clist.pointer = 0;
    clist.pageFaults = 0;
    clist.size = size;
    for (int i = 0; i < size; i++) {
        clist.array[i] = -1;
    }
    return clist;
}

/**
 * Checks if a reference is already in the list. If it is, then it
 * gives it a second chance.
 */
int referenceInClist(CList *clist, int element) {
    for (int i = 0; i < clist->size; i++) {
        if (clist->array[i] == element) {
            clist->secondChances[i] = 1;
            return 1;
        }
    }
    return 0;
}

/**
 * Prints the state of a circular list.
 */
void printStateOfCList(CList *clist) {
    for (int i = 0; i < clist->size; i++) {
        printf("[%d] ", clist->array[i]);
    }
    printf("\n");
    // printf("P=%d\n", clist->pointer);
}

/**
 * Inserts a reference into a circular list.
 */
void addReference(CList *clist, int element) {
    if (!referenceInClist(clist, element)) {
        if (clist->secondChances[clist->pointer] == 0) {
            clist->array[clist->pointer] = element;
            if (clist->pointer == clist->size - 1) {
                clist->pointer = 0;
            }
            else {
                clist->pointer++;
            }
            clist->pageFaults++;
        }
        else {
            clist->secondChances[clist->pointer] = 0;
            if (clist->pointer == clist->size - 1) {
                clist->pointer = 0;
            }
            else {
                clist->pointer++;
            }
            addReference(clist, element);
        }
    }
}

/**
 * Implements the clock page-replacement algorithm.
 */
void clock(int demo_mode) {
    int frames, *pages, i, inputSize;
    pages = convertInput(&frames, &inputSize);
    CList clist = initializeCList(frames);
    for (i = 0; i < inputSize; i++) {
        addReference(&clist, pages[i]);
        if (demo_mode)  printStateOfCList(&clist);
    }
    printf("page faults: %d\n", clist.pageFaults);
    free(pages);
    free(clist.array);
    free(clist.secondChances);
}

/**
 * Initializes a page.
 */
Page initializePage(int pageNumber) {
    Page page;
    page.modified = 0;
    page.referenced = 0;
    page.pageNumber = pageNumber;
    return page;
}

/**
 * Stores an integer array as Pages in a TimedQueue
 */
void convertToTimedQueue(int *array) {
    
}

int main(int argc, char* argv[]) {
    int demo_mode = 0;
    if (argv[1] != NULL && strcmp(argv[1], "demo") == 0) {
        demo_mode = 1;
    }
    clock(demo_mode);
    return EXIT_SUCCESS;
}