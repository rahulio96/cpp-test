/*
CSC139
Fall 2024
Second Assignment
Gupta, Rahul
Section #5
OSs Tested on: Linux
*/

// g++ src/main.cpp -o bin/main
// bin/main

#include <iostream>
#include <queue>

int main()
{
    std::cout << "Hello, World!!!!!" << std::endl;

    return 0;
}

/*
 Input file structure
    RR 4        <- algorithm (time slice of 4)
    3           <- number of processes
    1 0 24 1    <- process #, arrival time, burst time, priority
    2 0 3 1
    3 0 3 1

 Output file structure
    RR 4        <- algorithm (time slice of 4)
    0 1         <- time process STARTED executing on CPU, and process #
    4 2
    7 3
    10 1
    14 1
    18 1
    22 1
    26 1
 */

void roundRobin() {
    // TODO: read from a file path and load into queue

    // pop process from queue, "run it" and add time slice to total
    // subtract time slice from the process's burst time
    // if process > 0, push it back into the queue
    // if process == 0, pop it from the queue
    std::queue<int> fifoQueue;
}

void shortestJobFirst() {
    // TODO: read from a file path and load into queue
    // TODO: simulate time based on an integer counter

    // this queue will have values based on arrival time
    // check head of queue to see if it matches current time
    // if it does, pop it and push it into the priority queue
    // priority queue will be sorted based on burst time
    // pop from priority queue and run the process

    // HANDLE TIES
    // if burst time is the same, compare arrival time (smaller arrival time first)
    // if arrival time is the same, compare process number (smaller process number first)
    std::queue<int> fifoQueue;
}

void priorityScheduling() {
    // basically the same as shortestJobFirst, but instead of sorting by burst time, sort by priority

    // HANDLE TIES
    // if priority is the same, compare process number (smaller process number first)
}

void prioritySchedulingWithPreemption() {
    // basically the same as priorityScheduling, but if a process with a higher priority arrives, preempt the current process
    // to preempt, push the current process back into the priority queue and pop the new process

    // HANDLE TIES
    // if priority is the same, compare process number (smaller process number first)
}