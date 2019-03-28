#define MAX_FRAMES 10
#define MAX_PAGES 50
#define MAX_REFERENCES 100

typedef struct Queues {
    int front;
    int back;
    int *array;
    int size;
    int pageFaults;
}Queue;

typedef struct CircularLists {
    int *array;
    int *secondChances;
    int pointer;
    int size;
    int pageFaults;
}CList;

void clock(int);
void FIFO(int);
void printStateOfQueue(Queue *);
void addReference(CList *, int);
void printStateOfCList(CList *);
void enqueue(Queue *, int);
void replaceFirstPage(Queue *, int);
int referenceInClist(CList *, int);
int elementInQueue(Queue *, int);
int *convertInput(int *, int *);
CList initializeCList(int);
Queue initializeQueue(int);
