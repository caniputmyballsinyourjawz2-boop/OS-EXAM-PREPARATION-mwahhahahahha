#include <iostream>
#include <queue>
#include <iomanip>
#include <vector>
using namespace std;

struct Process {
    int id;
    float at;
    float bt;
    float remaining_bt;
    float wt;
    float tat;
    float ct;
    bool in_queue;
};

struct GanttEntry {
    int process_id;
    float start_time;
    float end_time;
};

int main() {
    int n = 6;
    float tq = 25;
    Process p[6] = {
        {1, 0, 99.99, 99.99, 0, 0, 0, false},
        {2, 1, 107.9, 107.9, 0, 0, 0, false},
        {3, 2, 143.77, 143.77, 0, 0, 0, false},
        {4, 3, 20.24, 20.24, 0, 0, 0, false},
        {5, 4, 50.55, 50.55, 0, 0, 0, false},
        {6, 5, 66.66, 66.66, 0, 0, 0, false}
    };
    
    cout << "Round Robin (RR) Scheduling - PREEMPTIVE\n";
    cout << "=========================================\n";
    cout << "Time Quantum: " << tq << "\n\n";
    
    vector<GanttEntry> gantt_entries;
    queue<int> ready_queue;
    float current_time = 0;
    int completed = 0;
    
    // Add first process if it arrives at time 0
    for (int i = 0; i < n; i++) {
        if (p[i].at <= current_time && !p[i].in_queue) {
            ready_queue.push(i);
            p[i].in_queue = true;
        }
    }
    
    while (completed < n) {
        if (ready_queue.empty()) {
            // Find next arriving process
            float next_arrival = 999999;
            for (int i = 0; i < n; i++) {
                if (p[i].remaining_bt > 0 && p[i].at > current_time) {
                    next_arrival = min(next_arrival, p[i].at);
                }
            }
            current_time = next_arrival;
            
            // Add newly arrived processes
            for (int i = 0; i < n; i++) {
                if (p[i].at <= current_time && p[i].remaining_bt > 0 && !p[i].in_queue) {
                    ready_queue.push(i);
                    p[i].in_queue = true;
                }
            }
            continue;
        }
        
        int idx = ready_queue.front();
        ready_queue.pop();
        p[idx].in_queue = false;
        
        float start_time = current_time;
        
        // PREEMPTIVE: Check if any process arrives during this time quantum
        float execute_time = min(p[idx].remaining_bt, tq);
        float next_arrival_time = 999999;
        
        // Find if any process will arrive during execution
        for (int i = 0; i < n; i++) {
            if (p[i].at > current_time && p[i].at < current_time + execute_time && p[i].remaining_bt > 0) {
                next_arrival_time = min(next_arrival_time, p[i].at);
            }
        }
        
        // If a process arrives during execution, preempt at that point
        if (next_arrival_time < current_time + execute_time) {
            execute_time = next_arrival_time - current_time;
        }
        
        current_time += execute_time;
        p[idx].remaining_bt -= execute_time;
        
        gantt_entries.push_back({p[idx].id, start_time, current_time});
        
        // Add processes that arrived during execution
        for (int i = 0; i < n; i++) {
            if (p[i].at <= current_time && p[i].remaining_bt > 0 && !p[i].in_queue && i != idx) {
                ready_queue.push(i);
                p[i].in_queue = true;
            }
        }
        
        // Re-add current process if not finished
        if (p[idx].remaining_bt > 0) {
            ready_queue.push(idx);
            p[idx].in_queue = true;
        } else {
            p[idx].ct = current_time;
            p[idx].tat = p[idx].ct - p[idx].at;
            p[idx].wt = p[idx].tat - p[idx].bt;
            completed++;
        }
    }
    
    // Display Arrival Time Timeline First
    cout << "\nArrival Timeline:\n";
    cout << "=================\n";
    for (int i = 0; i < n; i++) {
        cout << "<";
        int dashes = (int)p[i].at * 2;
        for (int j = 0; j < dashes; j++) cout << "-";
        cout << ">\n";
        
        cout << " ";
        for (int j = 0; j < dashes; j++) cout << " ";
        cout << "|\n";
        
        cout << " ";
        for (int j = 0; j < dashes; j++) cout << " ";
        cout << fixed << setprecision(2) << p[i].at << "\n";
        
        cout << " ";
        for (int j = 0; j < dashes; j++) cout << " ";
        cout << "P" << p[i].id << " arrives\n\n";
    }
    
    // Display process details
    cout << "\nProcess Details:\n";
    cout << "================\n";
    cout << "Process\tArrival Time\tBurst Time\tCompletion Time\tWaiting Time\tTurnaround Time\n";
    cout << "-------\t------------\t----------\t---------------\t------------\t---------------\n";
    
    float avg_wt = 0, avg_tat = 0;
    for (int i = 0; i < n; i++) {
        cout << "P" << p[i].id << "\t" << fixed << setprecision(2) 
             << p[i].at << "\t\t" << p[i].bt << "\t\t" 
             << p[i].ct << "\t\t"
             << p[i].wt << "\t\t" << p[i].tat << "\n";
        avg_wt += p[i].wt;
        avg_tat += p[i].tat;
    }
    
    avg_wt /= n;
    avg_tat /= n;
    
    cout << "\n===========================\n";
    cout << "Average Waiting Time (AVEWT): " << fixed << setprecision(2) << avg_wt << "\n";
    cout << "Average Turnaround Time (AVETT): " << avg_tat << "\n";
    
    // Gantt Chart Display
    cout << "\n\nGantt Chart (Execution Timeline):\n";
    cout << "==================================\n";
    
    // Top border
    for (auto& entry : gantt_entries) {
        cout << "+-------";
    }
    cout << "+\n";
    
    // Process IDs
    cout << "|";
    for (auto& entry : gantt_entries) {
        cout << "  P" << entry.process_id << "   |";
    }
    cout << "\n";
    
    // Bottom border
    for (auto& entry : gantt_entries) {
        cout << "+-------";
    }
    cout << "+\n";
    
    // Timeline
    cout << fixed << setprecision(2);
    cout << gantt_entries[0].start_time;
    for (auto& entry : gantt_entries) {
        int spaces = 7 - to_string((int)entry.end_time).length();
        for (int i = 0; i < spaces; i++) cout << " ";
        cout << entry.end_time;
    }
    cout << "\n";
    
    // Detailed Execution Steps
    cout << "\n\nDetailed Execution Steps:\n";
    cout << "=========================\n";
    for (size_t i = 0; i < gantt_entries.size(); i++) {
        auto& entry = gantt_entries[i];
        cout << "Step " << (i+1) << ": P" << entry.process_id 
             << " executes from " << fixed << setprecision(2) 
             << entry.start_time << " to " << entry.end_time 
             << " (Duration: " << (entry.end_time - entry.start_time) << ")\n";
    }
    
    return 0;
}