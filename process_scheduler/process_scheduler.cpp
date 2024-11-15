#include "process_scheduler.h"
#include <algorithm>
#include <string>
#include <sstream>

// FCFS Scheduling
void fcfs(std::vector<Process>& processes) {
    int currentTime = 0;
    for (auto& process : processes) {
        if (currentTime < process.arrivalTime) {
            currentTime = process.arrivalTime;
        }
        process.waitingTime = currentTime - process.arrivalTime;
        process.turnaroundTime = process.waitingTime + process.burstTime;
        currentTime += process.burstTime;
    }
}

// Round Robin Scheduling
void round_robin(std::vector<Process>& processes, int quantum) {
    int currentTime = 0;
    std::vector<int> remainingBurstTime(processes.size());
    for (size_t i = 0; i < processes.size(); i++) {
        remainingBurstTime[i] = processes[i].burstTime;
    }
    
    bool done;
    do {
        done = true;
        for (size_t i = 0; i < processes.size(); i++) {
            if (remainingBurstTime[i] > 0) {
                done = false;
                if (remainingBurstTime[i] > quantum) {
                    currentTime += quantum;
                    remainingBurstTime[i] -= quantum;
                } else {
                    currentTime += remainingBurstTime[i];
                    processes[i].waitingTime = currentTime - processes[i].burstTime - processes[i].arrivalTime;
                    processes[i].turnaroundTime = processes[i].waitingTime + processes[i].burstTime;
                    remainingBurstTime[i] = 0;
                }
            }
        }
    } while (!done);
}

// Shortest Job First Scheduling
void sjf(std::vector<Process>& processes) {
    std::sort(processes.begin(), processes.end(), [](Process& a, Process& b) {
        return a.burstTime < b.burstTime;
    });
    fcfs(processes);  // SJF is just sorted FCFS based on burst time
}

// Priority Scheduling
void priority_scheduling(std::vector<Process>& processes) {
    std::sort(processes.begin(), processes.end(), [](Process& a, Process& b) {
        return a.priority < b.priority;
    });
    fcfs(processes);  // Priority scheduling after sorting by priority
}

// Display the schedule as a string for ImGui
std::string display_schedule(const std::vector<Process>& processes) {
    std::ostringstream oss;
    oss << "PID\tArrival\tBurst\tPriority\tWaiting\tTurnaround\n";
    for (const auto& process : processes) {
        oss << process.pid << "\t" << process.arrivalTime << "\t"
            << process.burstTime << "\t" << process.priority << "\t"
            << process.waitingTime << "\t" << process.turnaroundTime << "\n";
    }
    return oss.str();
}
