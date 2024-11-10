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
#include <iomanip>

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

// Sort by arrival time - used in all algorithms
// Use this just in case the input file is not in order
struct CompareArrivalTime {
    bool operator()(std::array<int, 4> const& p1, std::array<int, 4> const& p2) {
        return p1[1] > p2[1];
    }
};

// Sort by burst time - used in SJF
// If the burst time and arrival time is the same, sort by process ID
struct CompareBurstTime {
    bool operator()(std::array<int, 4> const& p1, std::array<int, 4> const& p2) {
        if (p1[2] == p2[2] && p1[1] == p2[1]) {
            return p1[0] > p2[0];
        } else {
            return p1[2] > p2[2];
        }
    }
};

void roundRobin(std::priority_queue<std::array<int, 4>, std::vector<std::array<int, 4>>, CompareArrivalTime> processes, int numProcesses, int timeSlice) {
    std::cout << "RR " << timeSlice << std::endl;
    std::queue<std::array<int, 4>> fifoQueue;

    // Map to keep track of process id and start times
    // Get the last associated time for each process (its start or end time)
    std::unordered_map<int, int> processTimes;

    // Load the first process into the queue
    fifoQueue.push(processes.top());
    processTimes.insert({processes.top()[0], processes.top()[1]});

    // Initialize curTime to the first process arrival time
    int curTime = processes.top()[1];
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
        while (!processes.empty() && processes.top()[1] <= curTime) {
            fifoQueue.push(processes.top());
            processTimes.insert({processes.top()[0], processes.top()[1]});
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
    std::cout << "Average Waiting Time: " << std::fixed << std::setprecision(2) << avgWaitTime << std::endl;
}

void shortestJobFirst(std::priority_queue<std::array<int, 4>, std::vector<std::array<int, 4>>, CompareArrivalTime> processes, int numProcesses) {
    std::cout << "SJF" << std::endl;

    // Use a priority queue to sort by burst time
    std::priority_queue<std::array<int, 4>, std::vector<std::array<int, 4>>, CompareBurstTime> priorityQueue;

    // Load the first process into the queue
    priorityQueue.push(processes.top());

    // Initialize curTime to the first process arrival time
    int curTime = processes.top()[1];
    double totalWaitTime = 0;
    processes.pop();

    // Get all the other processes that have arrived
    while (!processes.empty() && processes.top()[1] <= curTime) {
        priorityQueue.push(processes.top());
        processes.pop();
    }

    while (!priorityQueue.empty()) {

        // Get the current process and pop it from the queue
        std::array<int, 4> curProcess = priorityQueue.top();
        priorityQueue.pop();

        // Get the time spent waiting and add it to the total wait time
        totalWaitTime += curTime - curProcess[1];

        // Print the current time (process start time) and the process id
        std::cout << curTime << "\t" << curProcess[0] << std::endl;

        // SJF processes execute until completion, so just add burst time to current time
        curTime += curProcess[2];

        // If the next process's arrival time is less than or equal to the current time it's ready to
        // be added to the priority queue
        while (!processes.empty() && processes.top()[1] <= curTime) {
            priorityQueue.push(processes.top());
            processes.pop();
        }
    }

    double avgWaitTime = totalWaitTime / numProcesses;
    std::cout << "Average Waiting Time: " << std::fixed << std::setprecision(2) << avgWaitTime << std::endl;
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
            id = 1;
        } else if (fileLine.substr(0, 11) == "PR_noPREMP") {
            id = 2;
        } else if (fileLine.substr(0, 13) == "PR_withPREMP") {
            id = 3;
        } else {
            throw std::runtime_error("Invalid algorithm type or file not found");
        }
        
        // Get the number of processes
        if (getline(inFile, fileLine)) {
            numProcesses = stoi(fileLine);
        }

        // Load the process info into a queue of processes
        // Use a priority queue to ENSURE that the processes are sorted by arrival time
        // in case it's not in order in the input.txt file!
        std::priority_queue<std::array<int, 4>, std::vector<std::array<int, 4>>, CompareArrivalTime> processes;
        
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
                shortestJobFirst(processes, numProcesses);
                break;
            case 2:
                priorityScheduling();
                break;
            case 3:
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
    std::string filePath = "./test_cases/input13.txt";
    runAlgorithmOnFile(filePath);
    return 0;
}