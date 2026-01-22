#include <iostream>
using namespace std;

int main() {
    int n = 10;
    int bt[] = {9, 8, 7, 8, 7, 8, 2, 1, 2, 5}; // Burst times
    int at[] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9}; // Arrival times
    int wt[10], tat[10], ct[10];
    float avg_wt = 0, avg_tat = 0;
    
    cout << "FCFS (First Come First Serve) Scheduling\n";
    cout << "=========================================\n\n";
    
    // Calculate completion time and waiting time for each process
    ct[0] = at[0] + bt[0];
    wt[0] = 0; // First process has 0 waiting time
    
    for (int i = 1; i < n; i++) {
        ct[i] = ct[i-1] + bt[i];
        wt[i] = ct[i-1] - at[i];
        if (wt[i] < 0) wt[i] = 0; // If process arrives after completion, no wait
    }
    
    // Calculate turnaround time for each process
    for (int i = 0; i < n; i++) {
        tat[i] = ct[i] - at[i];
    }
    
    // Display process details
    cout << "Process\tArrival Time\tBurst Time\tWaiting Time\tTurnaround Time\n";
    cout << "-------\t------------\t----------\t------------\t---------------\n";
    
    for (int i = 0; i < n; i++) {
        cout << "P" << i+1 << "\t" << at[i] << "\t\t" << bt[i] << "\t\t" << wt[i] << "\t\t" << tat[i] << "\n";
        avg_wt += wt[i];
        avg_tat += tat[i];
    }
    
    avg_wt /= n;
    avg_tat /= n;
    
    cout << "\n=========================================\n";
    cout << "Average Waiting Time (AVEWT): " << fixed;
    cout.precision(2);
    cout << avg_wt << "\n";
    cout << "Average Turnaround Time (AVETT): " << avg_tat << "\n";
    
    // Gantt Chart
    cout << "\nGantt Chart:\n";
    cout << "|";
    for (int i = 0; i < n; i++) {
        cout << " P" << i+1 << " |";
    }
    cout << "\n";
    
    cout << at[0];
    for (int i = 0; i < n; i++) {
        cout << "    " << ct[i];
    }
    cout << "\n";
    
    return 0;
}
