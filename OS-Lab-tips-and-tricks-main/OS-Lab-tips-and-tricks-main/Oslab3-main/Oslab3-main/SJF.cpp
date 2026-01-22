#include <iostream>
#include <algorithm>
using namespace std;

struct Process {
    int id;
    float bt;
    float wt;
    float tat;
};

bool compare(Process a, Process b) {
    return a.bt < b.bt;
}

int main() {
    int n = 6;
    Process p[6] = {
        {1, 99.99, 0, 0},
        {2, 107.9, 0, 0},
        {3, 143.77, 0, 0},
        {4, 20.24, 0, 0},
        {5, 50.55, 0, 0},
        {6, 66.66, 0, 0}
    };
    
    cout << "SJF (Shortest Job First) Scheduling\n";
    cout << "====================================\n\n";
    
    // Sort by burst time
    sort(p, p + n, compare);
    
    // Calculate waiting time
    p[0].wt = 0;
    for (int i = 1; i < n; i++) {
        p[i].wt = p[i-1].wt + p[i-1].bt;
    }
    
    // Calculate turnaround time
    for (int i = 0; i < n; i++) {
        p[i].tat = p[i].wt + p[i].bt;
    }
    
    float avg_wt = 0, avg_tat = 0;
    
    // Display process details
    cout << "Process\tBurst Time\tWaiting Time\tTurnaround Time\n";
    cout << "-------\t----------\t------------\t---------------\n";
    
    for (int i = 0; i < n; i++) {
        cout << "P" << p[i].id << "\t" << p[i].bt << "\t\t" << p[i].wt << "\t\t" << p[i].tat << "\n";
        avg_wt += p[i].wt;
        avg_tat += p[i].tat;
    }
    
    avg_wt /= n;
    avg_tat /= n;
    
    cout << "\n====================================\n";
    cout << "Average Waiting Time (AVEWT): " << fixed;
    cout.precision(2);
    cout << avg_wt << "\n";
    cout << "Average Turnaround Time (AVETT): " << avg_tat << "\n";
    
    // Gantt Chart
    cout << "\nGantt Chart:\n";
    cout << "|";
    for (int i = 0; i < n; i++) {
        cout << " P" << p[i].id << " |";
    }
    cout << "\n";
    
    cout << "0";
    float time = 0;
    for (int i = 0; i < n; i++) {
        time += p[i].bt;
        cout << "   " << time;
    }
    cout << "\n";
    
    return 0;
}
