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
        {1, 0, 99.99, 99.99, 0, 0, 0},
        {2, 1, 107.9, 107.9, 0, 0, 0},
        {3, 2, 143.77, 143.77, 0, 0, 0},
        {4, 3, 20.24, 20.24, 0, 0, 0},
        {5, 4, 50.55, 50.55, 0, 0, 0},
        {6, 5, 66.66, 66.66, 0, 0, 0}
    };
    
    cout << "Round Robin (RR) Scheduling\n";
    cout << "===========================\n";
    cout << "Time Quantum: " << tq << "\n\n";
    
    vector<GanttEntry> gantt_entries;
    
    queue<int> q;
    float current_time = 0;
    int completed = 0;
    
    // Add processes to queue based on arrival time
    while (completed < n) {
        // Add all processes that have arrived
        for (int i = 0; i < n; i++) {
            if (p[i].at <= current_time && p[i].remaining_bt > 0) {
                bool already_in_queue = false;
                queue<int> temp = q;
                while (!temp.empty()) {
                    if (temp.front() == i) {
                        already_in_queue = true;
                        break;
                    }
                    temp.pop();
                }
                if (!already_in_queue) {
                    q.push(i);
                }
            }
        }
        
        if (q.empty()) {
            // Jump to next arrival time
            for (int i = 0; i < n; i++) {
                if (p[i].remaining_bt > 0 && p[i].at > current_time) {
                    current_time = p[i].at;
                    break;
                }
            }
            continue;
        }
        
        int idx = q.front();
        q.pop();
        
        float start_time = current_time;
        float execute_time = min(p[idx].remaining_bt, tq);
        current_time += execute_time;
        p[idx].remaining_bt -= execute_time;
        
        gantt_entries.push_back({p[idx].id, start_time, current_time});
        
        if (p[idx].remaining_bt > 0) {
            q.push(idx);
        } else {
            p[idx].ct = current_time;
            p[idx].tat = p[idx].ct - p[idx].at;
            p[idx].wt = p[idx].tat - p[idx].bt;
            completed++;
        }
    }
    
    // Display process details
    cout << "Process\tArrival Time\tBurst Time\tWaiting Time\tTurnaround Time\n";
    cout << "-------\t------------\t----------\t------------\t---------------\n";
    
    float avg_wt = 0, avg_tat = 0;
    for (int i = 0; i < n; i++) {
        cout << "P" << p[i].id << "\t" << fixed << setprecision(2) 
             << p[i].at << "\t\t" << p[i].bt << "\t\t" 
             << p[i].wt << "\t\t" << p[i].tat << "\n";
        avg_wt += p[i].wt;
        avg_tat += p[i].tat;
    }
    
    avg_wt /= n;
    avg_tat /= n;
    
    cout << "\n===========================\n";
    cout << "Average Waiting Time (AVEWT): " << fixed << setprecision(2) << avg_wt << "\n";
    cout << "Average Turnaround Time (AVETT): " << avg_tat << "\n";
    
    // Gantt Chart - Improved Display
    cout << "\nGantt Chart:\n";
    cout << "============\n";
    
    // Process boxes
    cout << "|";
    for (auto& entry : gantt_entries) {
        cout << setw(6) << "P" + to_string(entry.process_id) << "|";
    }
    cout << "\n";
    
    // Time line
    cout << setw(1) << " ";
    for (auto& entry : gantt_entries) {
        cout << setw(6) << fixed << setprecision(0) << entry.start_time << "|";
    }
    // Add final time
    cout << setw(6) << fixed << setprecision(0) << gantt_entries.back().end_time << "\n";
    
    // Detailed timeline
    cout << "\nDetailed Timeline:\n";
    cout << "==================\n";
    for (auto& entry : gantt_entries) {
        cout << "P" << entry.process_id << ": " << fixed << setprecision(2) 
             << entry.start_time << " - " << entry.end_time << "\n";
    }
    
    return 0;
}
