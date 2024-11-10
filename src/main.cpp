/*
CSC139
Fall 2024
Second Assignment
Gupta, Rahul
Section #5
OSs Tested on: Linux
*/

#include <iostream>
#include <queue>
#include <fstream>
#include <string>
#include <cmath>
#include <array>
#include <unordered_map>
#include <iomanip>

// Sort by arrival time - used for processing the input file
// Use this just in case the input file is not in order
// If the arrival time is the same, sort by its original order from input file
struct compareArrivalTime {
    bool operator()(std::array<int, 5> const& p1, std::array<int, 5> const& p2) {
        if (p1[1] == p2[1]) {
            return p1[4] > p2[4];
        } else {
            return p1[1] > p2[1];
        }
    }
};

// Sort by burst time - used in SJF
// If the burst time and arrival time is the same, sort by process ID
struct compareBurstTime {
    bool operator()(std::array<int, 5> const& p1, std::array<int, 5> const& p2) {
        if (p1[2] == p2[2] && p1[1] == p2[1]) {
            return p1[0] > p2[0];
        } else {
            return p1[2] > p2[2];
        }
    }
};

// Sort by priority - used in priority scheduling
// If the burst time and arrival time is the same, sort by process ID
struct comparePriority {
    bool operator()(std::array<int, 5> const& p1, std::array<int, 5> const& p2) {
        if (p1[3] == p2[3]) {
            return p1[0] > p2[0];
        } else {
            return p1[3] > p2[3];
        }
    }
};

// Helper function to add the average wait time to the output
void outputAvgWaitTime(double avgWaitTime, std::vector<std::string>& output) {
    std::ostringstream oss;
    oss << "AVG Waiting Time: " << std::fixed << std::setprecision(2) << avgWaitTime;
    output.push_back(oss.str());
}

// Used for storing processes from the initial input file
using process_queue = std::priority_queue<std::array<int, 5>, std::vector<std::array<int, 5>>, compareArrivalTime>;

std::vector<std::string> roundRobin(process_queue processes, int numProcesses, int timeSlice) {
    std::vector<std::string> output;
    output.push_back("RR " + std::to_string(timeSlice));
    std::queue<std::array<int, 5>> fifoQueue;

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
        std::array<int, 5> curProcess = fifoQueue.front();
        fifoQueue.pop();

        // Get the time spent waiting and add it to the total wait time
        totalWaitTime += curTime - processTimes[curProcess[0]];

        // Output the current time (process start time) and the process id
        output.push_back(std::to_string(curTime) + "\t" + std::to_string(curProcess[0]));

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
    outputAvgWaitTime((totalWaitTime / numProcesses), output);
    return output;
}

std::vector<std::string> shortestJobFirst(process_queue processes, int numProcesses) {
    std::vector<std::string> output;
    output.push_back("SJF");

    // Use a priority queue to sort by burst time
    std::priority_queue<std::array<int, 5>, std::vector<std::array<int, 5>>, compareBurstTime> priorityQueue;

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
        std::array<int, 5> curProcess = priorityQueue.top();
        priorityQueue.pop();

        // Get the time spent waiting and add it to the total wait time
        totalWaitTime += curTime - curProcess[1];

        // Output the current time (process start time) and the process id
        output.push_back(std::to_string(curTime) + "\t" + std::to_string(curProcess[0]));

        // SJF processes execute until completion, so just add burst time to current time
        curTime += curProcess[2];

        // If the next process's arrival time is less than or equal to the current time it's ready to
        // be added to the priority queue
        while (!processes.empty() && processes.top()[1] <= curTime) {
            priorityQueue.push(processes.top());
            processes.pop();
        }
    }
    outputAvgWaitTime((totalWaitTime / numProcesses), output);
    return output;
}

std::vector<std::string> priorityScheduling(process_queue processes, int numProcesses) {
    std::vector<std::string> output;
    output.push_back("PR_noPREMP");

    // Use a priority queue to sort by priority
    std::priority_queue<std::array<int, 5>, std::vector<std::array<int, 5>>, comparePriority> priorityQueue;

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
        std::array<int, 5> curProcess = priorityQueue.top();
        priorityQueue.pop();

        // Get the time spent waiting and add it to the total wait time
        totalWaitTime += curTime - curProcess[1];

        // Output the current time (process start time) and the process id
        output.push_back(std::to_string(curTime) + "\t" + std::to_string(curProcess[0]));

        // In priority scheudling a processes executes until completion, so just add burst time to current time
        curTime += curProcess[2];

        // If the next process's arrival time is less than or equal to the current time it's ready to
        // be added to the priority queue
        while (!processes.empty() && processes.top()[1] <= curTime) {
            priorityQueue.push(processes.top());
            processes.pop();
        }
    }
    outputAvgWaitTime((totalWaitTime / numProcesses), output);
    return output;
}

std::vector<std::string> prioritySchedulingWithPreemption(process_queue processes, int numProcesses) {
    std::vector<std::string> output;
    output.push_back("PR_withPREMP");

    // Map to keep track of process id and start times
    // Get the last associated time for each process (its start or end time)
    std::unordered_map<int, int> processTimes;

    // Use a priority queue to sort by priority
    std::priority_queue<std::array<int, 5>, std::vector<std::array<int, 5>>, comparePriority> priorityQueue;

    // Load the first process into the queue
    priorityQueue.push(processes.top());
    processTimes.insert({processes.top()[0], processes.top()[1]});

    // Initialize curTime to the first process arrival time
    int curTime = processes.top()[1];
    double totalWaitTime = 0;
    processes.pop();

    // Get all the other processes that have arrived
    while (!processes.empty() && processes.top()[1] <= curTime) {
        priorityQueue.push(processes.top());
        processTimes.insert({processes.top()[0], processes.top()[1]});
        processes.pop();
    }

    while (!priorityQueue.empty()) {

        // Get the current process and pop it from the queue
        std::array<int, 5> curProcess = priorityQueue.top();
        priorityQueue.pop();

        // Get the time spent waiting and add it to the total wait time
        totalWaitTime += curTime - processTimes[curProcess[0]];

        // Output the current time (process start time) and the process id
        output.push_back(std::to_string(curTime) + "\t" + std::to_string(curProcess[0]));

        // When time increases, check to see if more processes can be added to the
        // priority queue and check if the current process can be preempted
        while (curProcess[2] > 0) {
            curTime += 1;
            curProcess[2] -= 1;

            while (!processes.empty() && processes.top()[1] <= curTime) {
                priorityQueue.push(processes.top());
                processTimes.insert({processes.top()[0], processes.top()[1]});
                processes.pop();
            }

            // Preempt current process if it has not finished execution
            if (priorityQueue.top()[3] < curProcess[3] && curProcess[2] > 0) {
                priorityQueue.push(curProcess);
                break;
            }
        }
        processTimes[curProcess[0]] = curTime;
    }
    outputAvgWaitTime((totalWaitTime / numProcesses), output);
    return output;
}

std::vector<std::string> runAlgorithmOnFile(std::string filePath) {
    std::vector<std::string> output;

    // First, read from the file and load the data into a priority queue to ensure
    // that processes are sorted by arrival time in case they are not in order
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
        } else if (fileLine.substr(0, 10) == "PR_noPREMP") {
            id = 2;
        } else if (fileLine.substr(0, 12) == "PR_withPREMP") {
            id = 3;
        } else {
            throw std::runtime_error("Invalid algorithm type or file not found");
        }
        
        // Get the number of processes
        if (getline(inFile, fileLine)) {
            numProcesses = stoi(fileLine);
        }

        // Load the process info into a queue of processes
        // Queue with arrays: [processID, arrivalTime, burstTime, priority, originalOrder]
        process_queue processes;
        int count = 0; // Track original order from input file
        while (getline(inFile, fileLine)) {
            int start = 0; // Starting point of the number substring
            int index = 0; // Index of the process array 
            std::array<int, 5> process;
            for (int i = 0; i < fileLine.length(); i++) {
                if (i == fileLine.length()-1) {
                    // i-start gives the length of the number as a string
                    // Use i-start+1 since i is not a space and has the last number
                    process[index] = stoi(fileLine.substr(start, i-start+1));
                    process[index+1] = count;
                } else if (fileLine[i] == ' ') {
                    process[index] = stoi(fileLine.substr(start, i-start));
                    start = i+1;
                    index++;
                }
            }
            count++;
            processes.push(process);
        }

        // Run the algorithm based on the id now that we have the data
        switch (id) {
            case 0:
                output = roundRobin(processes, numProcesses, timeSlice);
                break;
            case 1:
                output = shortestJobFirst(processes, numProcesses);
                break;
            case 2:
                output = priorityScheduling(processes, numProcesses);
                break;
            case 3:
                output = prioritySchedulingWithPreemption(processes, numProcesses);
                break;
            default:
                break;
        }

    } catch (std::exception& e) {
        std::cerr << e.what() << std::endl;
    }
    return output;
}

// Test against all inputs/output files
bool testAlgorithms(int numTests) {
    try {
        bool allTestsPass = true;
        for (int i = 1; i <= numTests; i++) {
            std::string filePath = "./test_cases/input" + std::to_string(i) + ".txt";
            std::ifstream inFile("./test_cases/output" + std::to_string(i) + ".txt");
            std::string fileLine;
            std::vector<std::string> output = runAlgorithmOnFile(filePath);

            std::string testStatus = "PASS";
            bool pass = true;
            int index = 0;

            while(getline(inFile, fileLine)) {
                if (output[index] != fileLine) {
                    testStatus = "FAIL";
                    allTestsPass = false;
                    break;
                }
                index++;
            }
            std::cout << "TEST " << std::to_string(i) << "\t\t" << testStatus << std::endl;
        }
        return allTestsPass;
    } catch (std::exception& e) {
        std::cerr << e.what() << std::endl;
        return false;
    }
}

int main() {
    return !testAlgorithms(16);
}