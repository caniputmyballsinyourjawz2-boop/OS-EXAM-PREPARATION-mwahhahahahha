#include <stdio.h>

#define MAX 20

struct Process {
    char id;
    int arrival;
    int burst;
    int priority;
    int finish;
    int wait;
    int turnaround;
    int done;   // 0 = false, 1 = true
};

void getInput(struct Process processes[], int *n) {
    printf("Enter number of processes: ");
    scanf("%d", n);

    for (int i = 0; i < *n; i++) {
        processes[i].id = 'A' + i;

        printf("\nProcess %c:\n", processes[i].id);
        printf("Arrival time: ");
        scanf("%d", &processes[i].arrival);
        printf("Burst time: ");
        scanf("%d", &processes[i].burst);
        printf("Priority (lower = higher priority): ");
        scanf("%d", &processes[i].priority);

        processes[i].done = 0;
    }
}

void runPriorityScheduling(struct Process processes[], int n) {
    printf("\n--- Non-Preemptive Priority Scheduling ---\n\n");

    int time = 0;
    int completed = 0;

    printf("Gantt Chart:\n");
    printf("Time\tProcess\n");
    printf("----\t-------\n");

    while (completed < n) {
        int selected = -1;
        int highestPriority = 9999;

        for (int i = 0; i < n; i++) {
            if (!processes[i].done && processes[i].arrival <= time) {
                if (processes[i].priority < highestPriority) {
                    highestPriority = processes[i].priority;
                    selected = i;
                }
            }
        }

        if (selected != -1) {
            printf("%d-%d\t%c\n",
                   time,
                   time + processes[selected].burst,
                   processes[selected].id);

            time += processes[selected].burst;
            processes[selected].finish = time;
            processes[selected].turnaround =
                processes[selected].finish - processes[selected].arrival;
            processes[selected].wait =
                processes[selected].turnaround - processes[selected].burst;
            processes[selected].done = 1;
            completed++;
        } else {
            time++; // CPU idle
        }
    }

    // Sort back by process ID
    for (int i = 0; i < n - 1; i++) {
        for (int j = i + 1; j < n; j++) {
            if (processes[i].id > processes[j].id) {
                struct Process temp = processes[i];
                processes[i] = processes[j];
                processes[j] = temp;
            }
        }
    }

    printf("\nResults:\n");
    printf("Process\tArrival\tBurst\tPriority\tFinish\tWait\tTurnaround\n");
    printf("-------\t-------\t-----\t--------\t------\t----\t----------\n");

    float totalWait = 0;
    float totalTurnaround = 0;

    for (int i = 0; i < n; i++) {
        printf("%c\t%d\t%d\t%d\t\t%d\t%d\t%d\n",
               processes[i].id,
               processes[i].arrival,
               processes[i].burst,
               processes[i].priority,
               processes[i].finish,
               processes[i].wait,
               processes[i].turnaround);

        totalWait += processes[i].wait;
        totalTurnaround += processes[i].turnaround;
    }

    printf("\nAverage Waiting Time: %.2f", totalWait / n);
    printf("\nAverage Turnaround Time: %.2f\n\n", totalTurnaround / n);
}

int main() {
    struct Process processes[MAX];
    int n = 0;
    int choice;

    printf("CPU Scheduling - Non-Preemptive Priority (C)\n\n");

    do {
        printf("Menu:\n");
        printf("1. Enter processes\n");
        printf("2. Run scheduling\n");
        printf("3. Exit\n");
        printf("Choice: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1:
                getInput(processes, &n);
                break;

            case 2:
                if (n == 0) {
                    printf("Enter processes first.\n");
                } else {
                    runPriorityScheduling(processes, n);
                }
                break;

            case 3:
                printf("Goodbye\n");
                break;

            default:
                printf("Invalid choice\n");
        }
    } while (choice != 3);

    return 0;
}
