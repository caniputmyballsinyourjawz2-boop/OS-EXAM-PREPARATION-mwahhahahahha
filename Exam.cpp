#include <iostream>
#include <algorithm>
#include <vector>
#include <iomanip>
#include <cmath>
using namespace std;

struct Process {
    int id;
    float at;  // Arrival Time
    float bt;  // Burst Time
    float ct;  // Completion Time
    float wt;  // Waiting Time
    float tat; // Turnaround Time
    float remaining_bt;
};

// SJF Scheduling
void SJF(vector<Process> processes) {
    int n = processes.size();
    vector<Process> p = processes;
    
    // Sort by arrival time first, then by burst time
    sort(p.begin(), p.end(), [](Process a, Process b) {
        if (a.at == b.at) return a.bt < b.bt;
        return a.at < b.at;
    });
    
    float current_time = 0;
    vector<bool> completed(n, false);
    
    for (int i = 0; i < n; i++) {
        int idx = -1;
        float min_bt = 999999;
        
        // Find process with minimum burst time among arrived processes
        for (int j = 0; j < n; j++) {
            if (!completed[j] && p[j].at <= current_time && p[j].bt < min_bt) {
                idx = j;
                min_bt = p[j].bt;
            }
        }
        
        // If no process arrived, advance time
        if (idx == -1) {
            for (int j = 0; j < n; j++) {
                if (!completed[j]) {
                    current_time = p[j].at;
                    idx = j;
                    break;
                }
            }
        }
        
        current_time += p[idx].bt;
        p[idx].ct = current_time;
        p[idx].tat = p[idx].ct - p[idx].at;
        p[idx].wt = p[idx].tat - p[idx].bt;
        completed[idx] = true;
    }
    
    // Sort by ID for display
    sort(p.begin(), p.end(), [](Process a, Process b) {
        return a.id < b.id;
    });
    
    cout << "\n============================================\n";
    cout << "SJF (Shortest Job First) SCHEDULING\n";
    cout << "============================================\n\n";
    
    cout << "PID\tAT\tBT\tCT\tWT\tTAT\n";
    cout << "---\t---\t---\t---\t---\t---\n";
    
    float avg_wt = 0, avg_tat = 0;
    for (int i = 0; i < n; i++) {
        cout << "P" << p[i].id << "\t" << fixed << setprecision(2)
             << p[i].at << "\t" << p[i].bt << "\t" << p[i].ct << "\t"
             << p[i].wt << "\t" << p[i].tat << "\n";
        avg_wt += p[i].wt;
        avg_tat += p[i].tat;
    }
    
    avg_wt /= n;
    avg_tat /= n;
    
    cout << "\nAverage Waiting Time (AVEWT): " << fixed << setprecision(2) << avg_wt << "\n";
    cout << "Average Turnaround Time (AVETT): " << avg_tat << "\n";
}

// SRTF Scheduling
void SRTF(vector<Process> processes) {
    int n = processes.size();
    vector<Process> p = processes;
    
    for (int i = 0; i < n; i++) {
        p[i].remaining_bt = p[i].bt;
        p[i].ct = 0;
    }
    
    float current_time = 0;
    int completed = 0;
    
    while (completed < n) {
        int idx = -1;
        float min_remaining = 999999;
        
        // Find process with minimum remaining burst time
        for (int i = 0; i < n; i++) {
            if (p[i].at <= current_time && p[i].remaining_bt > 0 && p[i].remaining_bt < min_remaining) {
                idx = i;
                min_remaining = p[i].remaining_bt;
            }
        }
        
        if (idx == -1) {
            // Advance time to next arrival
            float next_arrival = 999999;
            for (int i = 0; i < n; i++) {
                if (p[i].remaining_bt > 0 && p[i].at > current_time) {
                    next_arrival = min(next_arrival, p[i].at);
                }
            }
            current_time = next_arrival;
            continue;
        }
        
        current_time += 1;
        p[idx].remaining_bt -= 1;
        
        if (p[idx].remaining_bt == 0) {
            p[idx].ct = current_time;
            p[idx].tat = p[idx].ct - p[idx].at;
            p[idx].wt = p[idx].tat - p[idx].bt;
            completed++;
        }
    }
    
    // Sort by ID for display
    sort(p.begin(), p.end(), [](Process a, Process b) {
        return a.id < b.id;
    });
    
    cout << "\n============================================\n";
    cout << "SRTF (Shortest Remaining Time First) SCHEDULING\n";
    cout << "============================================\n\n";
    
    cout << "PID\tAT\tBT\tCT\tWT\tTAT\n";
    cout << "---\t---\t---\t---\t---\t---\n";
    
    float avg_wt = 0, avg_tat = 0;
    for (int i = 0; i < n; i++) {
        cout << "P" << p[i].id << "\t" << fixed << setprecision(2)
             << p[i].at << "\t" << p[i].bt << "\t" << p[i].ct << "\t"
             << p[i].wt << "\t" << p[i].tat << "\n";
        avg_wt += p[i].wt;
        avg_tat += p[i].tat;
    }
    
    avg_wt /= n;
    avg_tat /= n;
    
    cout << "\nAverage Waiting Time (AVEWT): " << fixed << setprecision(2) << avg_wt << "\n";
    cout << "Average Turnaround Time (AVETT): " << avg_tat << "\n";
}

// Calculate Recommended Quantum Time
float getRecommendedQuantum(vector<Process>& processes) {
    vector<float> burst_times;
    for (auto& p : processes) {
        burst_times.push_back(p.bt);
    }
    
    sort(burst_times.begin(), burst_times.end());
    
    float median;
    int n = burst_times.size();
    if (n % 2 == 0) {
        median = (burst_times[n/2 - 1] + burst_times[n/2]) / 2.0;
    } else {
        median = burst_times[n/2];
    }
    
    return median;
}

// Round Robin Scheduling
void RoundRobin(vector<Process> processes, float tq) {
    int n = processes.size();
    vector<Process> p = processes;
    
    for (int i = 0; i < n; i++) {
        p[i].remaining_bt = p[i].bt;
        p[i].ct = 0;
    }
    
    float current_time = 0;
    int completed = 0;
    int idx = 0;
    
    while (completed < n) {
        if (p[idx].remaining_bt > 0 && p[idx].at <= current_time) {
            float execute_time = min(p[idx].remaining_bt, tq);
            current_time += execute_time;
            p[idx].remaining_bt -= execute_time;
            
            if (p[idx].remaining_bt == 0) {
                p[idx].ct = current_time;
                p[idx].tat = p[idx].ct - p[idx].at;
                p[idx].wt = p[idx].tat - p[idx].bt;
                completed++;
            }
        } else if (p[idx].at > current_time) {
            current_time = p[idx].at;
        }
        
        idx = (idx + 1) % n;
    }
    
    // Sort by ID for display
    sort(p.begin(), p.end(), [](Process a, Process b) {
        return a.id < b.id;
    });
    
    cout << "\n============================================\n";
    cout << "ROUND ROBIN SCHEDULING\n";
    cout << "============================================\n";
    cout << "Recommended Quantum Time: " << fixed << setprecision(2) << tq << "\n\n";
    
    cout << "PID\tAT\tBT\tCT\tWT\tTAT\n";
    cout << "---\t---\t---\t---\t---\t---\n";
    
    float avg_wt = 0, avg_tat = 0;
    for (int i = 0; i < n; i++) {
        cout << "P" << p[i].id << "\t" << fixed << setprecision(2)
             << p[i].at << "\t" << p[i].bt << "\t" << p[i].ct << "\t"
             << p[i].wt << "\t" << p[i].tat << "\n";
        avg_wt += p[i].wt;
        avg_tat += p[i].tat;
    }
    
    avg_wt /= n;
    avg_tat /= n;
    
    cout << "\nAverage Waiting Time (AVEWT): " << fixed << setprecision(2) << avg_wt << "\n";
    cout << "Average Turnaround Time (AVETT): " << avg_tat << "\n";
}

int main() {
    // Process data
    vector<Process> processes = {
        {1, 1, 53, 0, 0, 0, 0},
        {2, 3, 43, 0, 0, 0, 0},
        {3, 8, 18, 0, 0, 0, 0},
        {4, 4, 16, 0, 0, 0, 0},
        {5, 6, 24, 0, 0, 0, 0},
        {6, 7, 73, 0, 0, 0, 0},
        {7, 2, 99, 0, 0, 0, 0},
        {8, 5, 27, 0, 0, 0, 0}
    };
    
    cout << "\n**** OS SCHEDULING ALGORITHMS ****\n";
    
    // Run SJF
    SJF(processes);
    
    // Run SRTF
    SRTF(processes);
    
    // Calculate recommended quantum time and run Round Robin
    float recommended_tq = getRecommendedQuantum(processes);
    RoundRobin(processes, recommended_tq);
    
    cout << "\n**** END OF SCHEDULING ****\n\n";
    
    return 0;
}
