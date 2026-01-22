#include <stdio.h>

// Structure to store process information
typedef struct {
    int pid;
    float bt;
    float wt;
    float tat;
} Process;

// Function to sort processes by burst time
void sortByBurstTime(Process proc[], int n) {
    Process temp;
    for (int i = 0; i < n - 1; i++) {
        for (int j = 0; j < n - i - 1; j++) {
            if (proc[j].bt > proc[j + 1].bt) {
                temp = proc[j];
                proc[j] = proc[j + 1];
                proc[j + 1] = temp;
            }
        }
    }
}

int main() {
    int n = 5;
    Process proc[5] = {
        {1, 50.54, 0, 0},
        {2, 10.11, 0, 0},
        {3, 33.32, 0, 0},
        {4, 2.5, 0, 0},
        {5, 101.1, 0, 0}
    };
    
    float avg_wt = 0, avg_tat = 0;
    
    printf("SJF (Shortest Job First) Scheduling\n");
    printf("====================================\n\n");
    
    printf("Before Sorting:\n");
    printf("Process\tBurst Time\n");
    printf("-------\t----------\n");
    for (int i = 0; i < n; i++) {
        printf("P%d\t%.2f\n", proc[i].pid, proc[i].bt);
    }
    
    // Sort processes by burst time
    sortByBurstTime(proc, n);
    
    printf("\n\nAfter Sorting (SJF Order):\n");
    printf("Process\tBurst Time\n");
    printf("-------\t----------\n");
    for (int i = 0; i < n; i++) {
        printf("P%d\t%.2f\n", proc[i].pid, proc[i].bt);
    }
    
    // Calculate waiting time
    proc[0].wt = 0;
    for (int i = 1; i < n; i++) {
        proc[i].wt = proc[i-1].wt + proc[i-1].bt;
    }
    
    // Calculate turnaround time
    for (int i = 0; i < n; i++) {
        proc[i].tat = proc[i].wt + proc[i].bt;
    }
    
    // Display results
    printf("\n\nScheduling Details:\n");
    printf("Process\tBurst Time\tWaiting Time\tTurnaround Time\n");
    printf("-------\t----------\t------------\t---------------\n");
    
    for (int i = 0; i < n; i++) {
        printf("P%d\t%.2f\t\t%.2f\t\t%.2f\n", 
               proc[i].pid, proc[i].bt, proc[i].wt, proc[i].tat);
        avg_wt += proc[i].wt;
        avg_tat += proc[i].tat;
    }
    
    avg_wt /= n;
    avg_tat /= n;
    
    printf("\n====================================\n");
    printf("Average Waiting Time: %.2f\n", avg_wt);
    printf("Average Turnaround Time: %.2f\n", avg_tat);
    
    // Gantt Chart
    printf("\nGantt Chart:\n");
    printf("|");
    for (int i = 0; i < n; i++) {
        printf(" P%d |", proc[i].pid);
    }
    printf("\n");
    
    printf("0");
    float time = 0;
    for (int i = 0; i < n; i++) {
        time += proc[i].bt;
        printf("   %.2f", time);
    }
    printf("\n");
    
    return 0;
}