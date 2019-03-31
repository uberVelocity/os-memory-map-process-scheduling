# Operating Systems Lab 3: Scheduling and page replacement
# Scheduling
## Problem description
The task of this exercise is to implement virtual process schedulers using two algorithms: FCFS (First Come First Served - non-preemptive) and Round-Robin (pre-emptive).

A **non-preemptive** algorithm allows a process to run until it blocks (either on I/O or waiting for another process) or voluntarily releases the CPU. A **preemptive** algorithm picks a process and lets it run for a fixed amount of time, often called a **quantum**. In this exercise, the quantum is set  to ```10ms```. Both algorithms make use of queues in which they are stored.

The exercise will be parted into two: FCFS and Round-Robin.

## Representation of processes
A process will be represented through a string of numbers given through ```stdin```. The first value represents the arrival time of the process in the queue. The second value represents its priority. Every value until -1 represents alternating processing times in the form of CPU time and I/O time, starting from CPU time. A -1 indicates the end of the process.

An example of an input:
```
0 3 15 30 20 42 40 -1
3 1 9 41 33 90 160 10 -1
```
The first row represents one process which arrives in the system at time ```T=0```, with a priority of 3, CPU times: {15, 20, 40} and I/O times: {30, 42}. The last time is always a CPU time.
Using both algorithms, we are interested in the average turn-around time of the processes and drawing a comparison between the two. The turn-around time is defined as the time between the process entering the queue and the completion time of the process.

## First-come-first-served (FCFS)
FCFS is a non-preemptive algorithm that is usually used in batch systems. Its descriptive name accentuates that the execution of the processes is done based on their arrival time in the queue, regadless of any priority. As such, for the input given above, even though the second process has a higher priority, the first process arrives before it, so the processor starts executing the first process before executing the second process. Execution is done until the process blocks or finishes. In this simulation, the only form of blocking is initiated by I/O. Whenever the processor finished a burst of CPU time from a processor, it can either encounter a block in the form of I/O or the end of the process. If the process has finished its execution, the completion time is defined for that specific process and the turnaround time is computed by subtracting the arrival time from the completion time. If the process has to do I/O, the processor computes the new arrival time of the process by adding its I/O time to the global time of the system.

### Implementation
In order to compute the average turn-around time of all processes, we must first compute the turn-around time of each process. Simply put, the turn-around time of a process is the difference between completion time and arrival time.

```
turn_around_time = completion_time - arrival_time
```
Because it is first come first serve, the queue is populated with the processes given in their ascending order of arrival times. In order to store the processes based on their arrival times, a priority queue has been built in which the next process is chosen based on its arrival time. The arrival time of a process is given by stdin, however its new arrival time is computed and set by adding the global time of the system to the process' I/O time. The priority queue has an internal clock, referred to as ```time``` in the code, which keeps track of how much time has elapsed since the beginning of the simulation. Every time the processor needs to do a context switch, it takes the process which has the next minimal arrival time. The clock is updated by adding the burst time of the process to it. If the new arrival time is lower than the current time when the processor needs to do a context switch, the time remains the same. Otherwise, if the next minimal new arrival time of process is higher than the current time, the current time becomes the new arrival time of that process.

Should two processes arrive at the same time, the first one in the priority queue is taken.

As such, computing the average turn-around time for this algorithm implies going through the turn-around times of each process, summing each one per process, and dividing the result by the number of processes.

Example:
```
0 0 21 -1
1 0 3 -1
2 0 6 -1
3 0 2 -1
```

### Demo mode
The program can be run with the ```demo``` flag, showcasing the statistics of each process after each context switch - useful when debugging or as a general wonderment.
```bash
valgrind ./fcfs demo < input.in
```
## Round-Robin with priority scheduling
Round-robin is an attempt at making the system fair in regards to how much attention the processor gives to each process. It is a preemptive algorithm in which each process is assigned a time interval called `quantum` which defines the amount of the it is allowed to run. In this exercise, the `quantum` is fixed at 10ms for all processes. When a process blocks before its `quantum` has finished, the CPU context switches to the next runnable process in the list. This type of algorithm assumes that all processes are equal. In a real system, certain processes are more important than others, so significance needs to be taken into account when deciding which process to run next.

Priority scheduling assigns each process a priority. The processor then chooses the runnable process with the highest priority. The book describes that it is convenient to group processes into priority classes and use priority scheduling among the classes but round-robin scheduling within each class. This is the implementation that we have opted for.

An easier implementation, however a less efficient one, is to firstly run in a Round-Robin fashion all of the processes in the highest priority queue until all of them have finished, then move on to the next-highest priority queue.
This approach is highly innefficient as the processor would be idle when other processes could be worked on.

Suppose there are only two processes that arrive at the same time, one with priority 1 that has 30ms CPU time followed by 100ms I/O time and one with priority 3 which has only 10ms CPU time. If we opt to work on all processes in the highest priority queue until the queue is empty, then  the process with priority 3 will have to wait 130ms until it gets a chance to run. Alternatively, if we switch to the next-highest priority runnable process, after 30ms of work on process 1, the other process will get a chance to be ran, decreasing the overall turn-around time.

### Implementation
Every process in the input is assigned, according to their priority, into three queues of respective priorities. The priority of a process can be either: {1, 2, 3}. Priority 1 is the highest priority.

Each queue has a pointer pointing to the last-ran process within the queue. The pointer implements a round-robin approach as the pointer gets incremented once a process has  ran for the given quantum, allowing the next process in the queue to run. Whenever the processor needs to do a context-switch, the queue with the highest-priority is checked for a runnable process at the position of the pointer. If a runnable process is found, that is chosen to run. Otherwise, the same rationale applies to the queues in descending order of priority (ascending order of numbers representing their priority).

Round-robin implies that a process cannot have two runs without the other processes from the queue having one run before it. As such, if all processes are blocking at every pointer location, the process waits until one becomes runnable. If two processes are runnable, the one with the highest priority will be chosen.

## Performance
In comparison to FCFS, Round-Robin does not run into starvation. This equates to a lower average turn-around time in general.

For the following input, FCFS yields an average turn-around time of 1350.75,while Round-Robin yields 1066.33.

```
0 1 100 25 100 20 50 20 100 10 200 -1
2 2 30 15 30 10 40 10 50 -1
15 3 10 300 20 20 20 30 10 5 30 100 250 -1
20 2 40 500 500 20 30 30 40 500 500 -1
```

Abstracting away from the exercise, multiple strategies exist which can improve the performance of this algorithm. Some of them make use of dynamically allocated priorities while others manipulate the quantum depending on the number of times a certain process has been visited. Such aspects are relevant in real systems since context switching comes with overhead. The type of system and the potential interactions that the system may be part of need to be taken into when choosing the quantum. 


# Page-replacement
## Problem description
The task of this exercise is to implement two page-replacement algorithms: The First-In, First-Out (FIFO) page-replacement algorithm and The Clock page-replacement algorithm. For both algorithms a demo mode is available showcasing the state of the queue during transitions between each reference. One can run the demo by running the program with the `demo` flag (`./page demo`).

## First-In, First-out (FIFO)
In the FIFO page-replacement algorithm, the operating system maintiains a list of all pages that are currently in memory. The order in which they are stored is based on the order in which they are initially stored, with newer pages appearing at the tail of the list, while older pages residing at the head of the list. Whenever a page fault occurs and there are no available frames, the oldest page in the list gets removed and the new page gets appended at the tail of the list. One drawback of this approach is the fact that old pages may still be useful and removing one such page may not be beneficial.

### Implementation
The input defines the amount of frames the operating system may use as well as a string of numbers representing references to pages. The program must compute the number of page faults that occur given a configuration of frames and references.

Example of input in which 4 frames are used to reference a list of pages:

`Input:`
```
4
1 2 3 4 7 2 5 2 3 1 7 6 4
```
`Output: page faults = 12`.

The FIFO page-replacement algorithm can be easily implemented using a queue. The queue is implemented using an array. A page-fault occurs when a page that is not present in the queue gets referenced. When a page-fault occurs, the operating system must insert the page which has just been referenced into a frame. If the queue is full, the head of the queue is removed, every element is shifted one position in the queue, and the referenced page is placed at the tail of the queue. If the queue is not full (i.e. there are unused frames), the referenced page is simply inserted into the next-available frame.

For the input given above, the queue will undergo the following states ('-1' signifies an empty frame, '*' signifies a page fault):
```
[1] [-1] [-1] [-1] *
[1] [2] [-1] [-1] *
[1] [2] [3] [-1] *
[1] [2] [3] [4] *
[2] [3] [4] [7] *
[2] [3] [4] [7] 
[3] [4] [7] [5] *
[4] [7] [5] [2] *
[7] [5] [2] [3] *
[5] [2] [3] [1] *
[2] [3] [1] [7] *
[3] [1] [7] [6] *
[1] [7] [6] [4] *
```

## Clock
The clock algorithm is based off of the *Second chance* page-replacement algorithm which aims at reducing the amount of useful pages being thrown away for no good reason. In order to understand the clock algorithm, second chance needs to be introduced.

In the second-chance algorithm, each pages has a bit which acts like an extra life. When the operating system encounters a reference which is not in a frame, it inspects the R bit of the oldest page. If it is 0, the page is evicted from the frame, the list gets shifted, and the new
referenced page gets inserted. If it is 1, the bit is set to 0 and the next oldest page is inspected. The bit of a page is set to 1 when that page is referenced while the page is in a frame.

Second chance attempts at avoiding the removal of useful pages. However, it does unnecessary shifts every time a page fault occurs which is inefficient. The clock algorithm makes use of a pointer to the oldest page. When a page fault occurs, that page's R bit is inspected. If it is 0, the page is evicted and the newly referenced page is inserted in its place. Otherwise, the bit is cleared and the next oldest page is inspected until the newly referenced page has been added into a frame. Similarly to second chance, the bit of a page becomes 1 when the referenced page already exists within a frame.

### Implementation
A circular list makes for an clean implementation of the clock algorithm. The list has a pointer which initially starts at the oldest page of the list and which gets incremented when a new page needs to be inserted. Whenever a reference is given, the operating system checks whether the bit of the page found at the location of the pointer is 0. If it is, the newly referenced page gets inserted in its place in the array and the pointer gets incremented. If it is not, the bit of the pointed page becomes 0, the pointer gets incremented, and the process repeats until the page is inserted in the array. When the pointer reaches the end of the array, it resets itself at the beginning of the array. Since the problem description specifies the maximum number of frames and the maximum number of pages the input may have, resizing functions of the data structures have not been included in this exercise.

When comparing the clock algorithm to FIFO, we find that it yields a lower number of page-faults.

`Input:`
```
4
1 2 3 4 7 2 5 2 3 1 7 6 4
```

`Output: page faults = 11`

The transition states of the queue for the aforementioned input ('-1' signifies an empty frame, '*' signifies a page fault):
```
[1] [-1] [-1] [-1] 
[1] [2] [-1] [-1] 
[1] [2] [3] [-1] 
[1] [2] [3] [4] 
[7] [2] [3] [4] 
[7] [2] [3] [4] 
[7] [2] [5] [4] 
[7] [2] [5] [4] 
[7] [2] [5] [3] 
[1] [2] [5] [3] 
[1] [2] [7] [3] 
[1] [2] [7] [6] 
[4] [2] [7] [6]
```

## Performance
Since heavily used pages do not get thrown out as easily, a lower number of page faults is to be expected when using the clock algorithm. The following is a list of results for both FCFS and clock:

```
4
1 2 3 4 7 2 5 2 3 1 7 6 4
```
FCFS: 12 | clock: 11

```
3
7 0 1 2 0 3 0 4 2 3 0 3 2 1 2 0 1 7 0 1
```
FCFS: 15 | clock: 11