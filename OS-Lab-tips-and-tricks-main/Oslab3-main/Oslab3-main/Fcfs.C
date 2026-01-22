#include <stdio.h>

int main() {
    int n = 10;
    int bt[] = {9, 8, 7, 8, 7, 8, 2, 1, 2, 5}; // Burst times
    int wt[10], tat[10];
    float avg_wt = 0, avg_tat = 0;
    
    printf("FCFS (First Come First Serve) Scheduling\n");
    printf("=========================================\n\n");
    
    // Calculate waiting time for each process
    wt[0] = 0; // First process has 0 waiting time
    
    for (int i = 1; i < n; i++) {
        wt[i] = wt[i-1] + bt[i-1];
    }
    
    // Calculate turnaround time for each process
    for (int i = 0; i < n; i++) {
        tat[i] = wt[i] + bt[i];
    }
    
    // Display process details
    printf("Process\tBurst Time\tWaiting Time\tTurnaround Time\n");
    printf("-------\t----------\t------------\t---------------\n");
    
    for (int i = 0; i < n; i++) {
        printf("P%d\t%d\t\t%d\t\t%d\n", i+1, bt[i], wt[i], tat[i]);
        avg_wt += wt[i];
        avg_tat += tat[i];
    }
    
    avg_wt /= n;
    avg_tat /= n;
    
    printf("\n=========================================\n");
    printf("Average Waiting Time (AVEWT): %.2f\n", avg_wt);
    printf("Average Turnaround Time (AVETT): %.2f\n", avg_tat);
    
    // Gantt Chart
    printf("\nGantt Chart:\n");
    printf("|");
    for (int i = 0; i < n; i++) {
        printf(" P%d |", i+1);
    }
    printf("\n");
    
    printf("0");
    int time = 0;
    for (int i = 0; i < n; i++) {
        time += bt[i];
        printf("    %d", time);
    }
    printf("\n");
    
    return 0;
}