#ifndef PROCESS_SCHEDULER_H
#define PROCESS_SCHEDULER_H

#include <vector>
#include <string>

// Process structure
struct Process {
    int pid;
    int arrivalTime;
    int burstTime;
    int priority;
    int waitingTime;
    int turnaroundTime;
};

// Scheduler functions
void fcfs(std::vector<Process>& processes);
void round_robin(std::vector<Process>& processes, int quantum);
void sjf(std::vector<Process>& processes);
void priority_scheduling(std::vector<Process>& processes);

std::string display_schedule(const std::vector<Process>& processes); // Helper to display results

#endif // PROCESS_SCHEDULER_H
