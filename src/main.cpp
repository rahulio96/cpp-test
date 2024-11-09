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
#include <fstream>
#include <string>
#include <cmath>
#include <array>
#include <unordered_map>

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

void roundRobin(std::queue<std::array<int, 4>> processes, int numProcesses, int timeSlice) {
    std::cout << "RR " << timeSlice << std::endl;
    std::queue<std::array<int, 4>> fifoQueue;

    // Map to keep track of process id and start times
    // Get the last associated time for each process (its start or end time)
    std::unordered_map<int, int> processTimes;

    // Load the first process into the queue
    fifoQueue.push(processes.front());
    processTimes.insert({processes.front()[0], processes.front()[1]});

    // Initialize curTime to the first process arrival time
    int curTime = processes.front()[1];
    double totalWaitTime = 0;

    processes.pop();

    while (!fifoQueue.empty()) {

        // Get the current process and pop it from the queue
        std::array<int, 4> curProcess = fifoQueue.front();
        fifoQueue.pop();

        // Get the time spent waiting and add it to the total wait time
        totalWaitTime += curTime - processTimes[curProcess[0]];

        // Print the current time (process start time) and the process id
        std::cout << curTime << "\t" << curProcess[0] << std::endl;

        // Get the minimum, if burst time is less than time slice, it means the process will finish
        // early and we need to add the remaining burst time to the current time. If time slice is
        // greater, then we just add the time slice to the current time.
        int runTime = std::min(curProcess[2], timeSlice);
        curTime += runTime;
        curProcess[2] -= runTime;

        // If the next process's arrival time is less than or equal to the current time it's ready to
        // be added to the FIFO queue
        while (!processes.empty() && processes.front()[1] <= curTime) {
            fifoQueue.push(processes.front());
            processTimes.insert({processes.front()[0], processes.front()[1]});
            processes.pop();
        }

        // If the process has remaining burst time, add to end of the FIFO queue
        if (curProcess[2] > 0) {
            fifoQueue.push(curProcess);
        }
   
        // Update the process time in the map to be its end time
        processTimes[curProcess[0]] = curTime;
    }

    double avgWaitTime = totalWaitTime / numProcesses;
    std::cout << "Average Waiting Time: " << avgWaitTime << std::endl;
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

void runAlgorithmOnFile(std::string filePath) {

    // First, read from the file and load the data into a 2D array
    try {
        std::ifstream inFile(filePath);
        std::string fileLine;
        int id;
        int timeSlice;
        int numProcesses;
        std::getline(inFile, fileLine);

        // Find the algorithm type (also get time slice if RR)
        if (fileLine.substr(0, 2) == "RR") {
            id = 0;
            timeSlice = stoi(fileLine.substr(3, fileLine.length()));
        } else if (fileLine.substr(0, 3) == "SJF") {
            id = 2;
        } else if (fileLine.substr(0, 11) == "PR_noPREMP") {
            id = 3;
        } else if (fileLine.substr(0, 13) == "PR_withPREMP") {
            id = 4;
        } else {
            throw std::runtime_error("Invalid algorithm type or file not found");
        }
        
        // Get the number of processes
        if (getline(inFile, fileLine)) {
            numProcesses = stoi(fileLine);
        }

        // Load the process info into a queue of processes
        // TODO: sort the processes based on arrival time JUST IN CASE!!!
        std::queue<std::array<int, 4>> processes;
        
        while (getline(inFile, fileLine)) {
            int start = 0; // Starting point of the number substring
            int index = 0; // Index of the process array 
            std::array<int, 4> process;
            for (int i = 0; i < fileLine.length(); i++) {
                if (i == fileLine.length()-1) {
                    // i-start gives the length of the number as a string
                    // Use i-start+1 since i is not a space and has the last number
                    process[index] = stoi(fileLine.substr(start, i-start+1));
                } else if (fileLine[i] == ' ') {
                    process[index] = stoi(fileLine.substr(start, i-start));
                    start = i+1;
                    index++;
                }
            }
            processes.push(process);
        }

        // Run the algorithm based on the id now that we have the data
        switch (id) {
            case 0:
                roundRobin(processes, numProcesses, timeSlice);
                break;
            case 1:
                shortestJobFirst();
                break;
            case 2:
                priorityScheduling();
                break;
            case 3:
                priorityScheduling();
                break;
            case 4:
                prioritySchedulingWithPreemption();
                break;
            default:
                break;
        }

    } catch (std::exception& e) {
        std::cerr << e.what() << std::endl;
    }

}

int main() {
    std::string filePath = "./test_cases/input1.txt";
    runAlgorithmOnFile(filePath);
    return 0;
}