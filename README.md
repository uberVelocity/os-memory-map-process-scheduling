# Operating Systems Lab 3: Scheduling and page replacement
# Scheduling
## Problem description
The task of this assignment is to implement virtual process schedulers using two algorithms: FCFS (First Come First Served - non-preemptive) and Round-Robin (pre-emptive).

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

Should two processes arrive at the same time, the fisrt one in the priority queue is taken.

As such, computing the average turn-around time for this algorithm implies going through the execution times of each process, summing each one per process, and dividing the result by the number of processes.

Example:
```
0 0 21 -1
1 0 3 -1
2 0 6 -1
3 0 2 -1
```

## Notes
Input processes have to be orderedbased on their arrival time in order to put them into the queue accordingly. Testing cases where given in ascending order of their arrival time, however this may not always be the case.

# Page-replacement
## Problem description
