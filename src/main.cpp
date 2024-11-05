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

using namespace std;

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
    queue<int> fifoQueue;
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
    queue<int> fifoQueue;
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

void runAlgorithmOnFile(string filePath) {

    // First, read from the file and load the data into a 2D array
    try {
        ifstream inFile(filePath);
        string fileLine;
        int id;
        int timeSlice;
        int numProcesses;
        getline(inFile, fileLine);

        // Find the algorithm type (also get time slice if RR)
        if (fileLine.substr(0, 2) == "RR") {
            id = 0;
            timeSlice = stoi(fileLine.substr(3));
        } else if (fileLine.substr(0, 3) == "SJF") {
            id = 2;
        } else if (fileLine.substr(0, 11) == "PR_noPREMP") {
            id = 3;
        } else if (fileLine.substr(0, 13) == "PR_withPREMP") {
            id = 4;
        } else {
            throw runtime_error("Invalid algorithm type or file not found");
        }
        
        // Get the number of processes
        if (getline(inFile, fileLine)) {
            numProcesses = stoi(fileLine);
        }

        // Load the process info into a 2D array
        string processes[numProcesses][4];
        int row = 0;  
        while (getline(inFile, fileLine)) {
            char prev  = ' ';
            int col = 0;
            for(char& c : fileLine) {
                if (prev == ' ' && c != ' ') {
                    processes[row][col] = c;
                    col++;
                    prev = c;
                } else if (prev != ' ' && c != ' ') {
                    processes[row][col-1] += c;
                    prev = c;
                } else if (prev != ' ' && c == ' ') {
                    prev = c;
                }
            }
            row++;
        }

        // Run the algorithm based on the id now that we have the data
        switch (id) {
            case 0:
                roundRobin();
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

    } catch (exception& e) {
        cerr << e.what() << endl;
    }

}

int main() {
    string filePath = "./test_cases/input4.txt";
    runAlgorithmOnFile(filePath);
    return 0;
}