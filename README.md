# Operating Systems Lab 3: Scheduling and page replacement
## Problem description
The task of this assignment is to implement virtual process schedulers using two algorithms: FCFS (First Come First Served - non-preemptive) and Round-Robin (pre-emptive).

A **non-preemptive** algorithm allows a process to run until it blocks (either on I/O or waiting for another process) or voluntarily releases the CPU. A **preemptive** algorithm picks a process and lets it run for a fixed amount of time, often called a **quantum**. In this exercise, the quantum is set  to ```10ms```. Both algorithms make use of queues in which they are stored.

The exercise will be parted into two: FCFS and Round-Robin.

## Representation of processes
A process will be represented through a string of numbers given through ```stdin```. The first value represents the arrival time of the process in the queue. The second value represents its priority. Every value until -1 represents alternating processing times in the form of CPU time and I/O time, starting from CPU time. A -1 indicates the end of the process.

An example of an input:
```
0 15 15 30 20 42 -1
3 1 9 41 33 90 160 10 -1
```

Using both algorithms, we are interested of the average turn-around time of the processes and drawing a comparison between the two. The turn-around time is defined as the time between the creation of the process (entering the queue) and finishing the process.

## First-come-first-served (FCFS)
FCFS is a non-preemptive algorithm that is usually used in batch systems. Its descriptive name accentuates that the execution of the processes is done based on their arrival time in the queue. As such, for the input given above, even though the second process has a higher priority, process 1 arrives before it, so the processor starts executing process 1 before process 2. Execution is done until the process blocks or finishes. In this simulation, the process never blocks, so the amount of time it runs for is the total sum of processing times.
### Implementation
In order to compute the average turn-around time of all processes, we must first compute the turn-around time of each process. Simply put, the turn-around time of a process is the difference between the time it took for it to finish execution and its arrival time.

```
turn_around_time = waiting_time + burst time
```

As such, computing the average turn-around time for this algorithm implies going through the execution times of each process, summing each one per process, and dividing the result by the number of processes. Processes that succeed a process must add the turn-around time of that process to their own turn-around time.

Example:
```
0 0 21 -1
1 0 3 -1
2 0 6 -1
3 0 2 -1

```

## Notes
Assumption is that the input is given in the order of the arrival in the queue (ascending order of first parameter).