#include <stdio.h>

#define MAX 50
#define INF 9999

struct Process {
    int id;
    int arrival;
    int burst;
    int remaining;
    int finish;
    int wait;
    int turnaround;
};

/* ---------- Function Prototypes ---------- */
void getInput(struct Process p[], int *n);
void roundRobinScheduling(struct Process p[], int n, int quantum);
void displayResults(struct Process p[], int n);

/* ---------- Main ---------- */
int main() {
    struct Process processes[MAX];
    int n = 0;
    int choice, quantum;

    printf("CPU Scheduling - Round Robin Algorithm (C)\n\n");

    do {
        printf("Menu:\n");
        printf("1. Enter processes\n");
        printf("2. Run Round Robin Scheduling\n");
        printf("3. Exit\n");
        printf("Enter choice: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1:
                getInput(processes, &n);
                break;

            case 2:
                if (n == 0) {
                    printf("No processes entered. Use option 1 first.\n\n");
                } else {
                    printf("Enter time quantum: ");
                    scanf("%d", &quantum);
                    roundRobinScheduling(processes, n, quantum);
                }
                break;

            case 3:
                printf("Goodbye!\n");
                break;

            default:
                printf("Invalid choice!\n");
        }
    } while (choice != 3);

    return 0;
}

/* ---------- Input ---------- */
void getInput(struct Process p[], int *n) {
    printf("\nHow many processes? ");
    scanf("%d", n);

    for (int i = 0; i < *n; i++) {
        p[i].id = i + 1;

        printf("\nProcess %d:\n", p[i].id);
        printf("Arrival time: ");
        scanf("%d", &p[i].arrival);
        printf("Burst time: ");
        scanf("%d", &p[i].burst);

        p[i].remaining = p[i].burst;
        p[i].finish = 0;
        p[i].wait = 0;
        p[i].turnaround = 0;
    }

    printf("\n%d processes entered successfully.\n\n", *n);
}

/* ---------- Round Robin Scheduling ---------- */
void roundRobinScheduling(struct Process p[], int n, int quantum) {
    int i, j;

    /* Reset values */
    for (i = 0; i < n; i++) {
        p[i].remaining = p[i].burst;
        p[i].finish = p[i].wait = p[i].turnaround = 0;
    }

    /* Sort by arrival time */
    for (i = 0; i < n - 1; i++) {
        for (j = i + 1; j < n; j++) {
            if (p[i].arrival > p[j].arrival) {
                struct Process temp = p[i];
                p[i] = p[j];
                p[j] = temp;
            }
        }
    }

    /* Ready queue (circular) */
    int queue[MAX], front = 0, rear = 0;
    int inQueue[MAX] = {0};
    int completed[MAX] = {0};

    int currentTime = 0;
    int completedCount = 0;
    int current = -1;

    printf("\nExecution Timeline:\n");
    printf("Time\tProcess\n");
    printf("----\t-------\n");

    /* Add processes arriving at time 0 */
    for (i = 0; i < n; i++) {
        if (p[i].arrival == 0) {
            queue[rear++] = i;
            inQueue[i] = 1;
        }
    }

    while (completedCount < n) {

        /* CPU idle */
        if (current == -1 && front == rear) {
            int nextArrival = INF;

            for (i = 0; i < n; i++) {
                if (!completed[i] && p[i].arrival > currentTime) {
                    if (p[i].arrival < nextArrival)
                        nextArrival = p[i].arrival;
                }
            }

            if (nextArrival != INF) {
                printf("%d-%d\tIDLE\n", currentTime, nextArrival);
                currentTime = nextArrival;

                for (i = 0; i < n; i++) {
                    if (!completed[i] && !inQueue[i] && p[i].arrival <= currentTime) {
                        queue[rear++] = i;
                        inQueue[i] = 1;
                    }
                }
            }
        }

        /* Reinsert previous process */
        if (current != -1 && p[current].remaining > 0) {
            queue[rear++] = current;
            inQueue[current] = 1;
        }

        /* Execute next process */
        if (front != rear) {
            current = queue[front++];
            inQueue[current] = 0;

            int execTime =
                (p[current].remaining < quantum) ? p[current].remaining : quantum;

            printf("%d-%d\tP%d\n",
                   currentTime,
                   currentTime + execTime,
                   p[current].id);

            currentTime += execTime;
            p[current].remaining -= execTime;

            if (p[current].remaining == 0) {
                p[current].finish = currentTime;
                p[current].turnaround = p[current].finish - p[current].arrival;
                p[current].wait = p[current].turnaround - p[current].burst;

                completed[current] = 1;
                completedCount++;
                current = -1;
            }

            /* Add newly arrived processes */
            for (i = 0; i < n; i++) {
                if (!completed[i] && !inQueue[i] && p[i].arrival <= currentTime) {
                    queue[rear++] = i;
                    inQueue[i] = 1;
                }
            }
        } else {
            current = -1;
        }
    }

    displayResults(p, n);
}

/* ---------- Display Results ---------- */
void displayResults(struct Process p[], int n) {
    float totalWait = 0, totalTurnaround = 0;

    /* Sort by process ID */
    for (int i = 0; i < n - 1; i++) {
        for (int j = i + 1; j < n; j++) {
            if (p[i].id > p[j].id) {
                struct Process temp = p[i];
                p[i] = p[j];
                p[j] = temp;
            }
        }
    }

    printf("\nResults:\n");
    printf("Process\tArrival\tBurst\tFinish\tWait\tTurnaround\n");
    printf("-------\t-------\t-----\t------\t----\t----------\n");

    for (int i = 0; i < n; i++) {
        printf("P%d\t%d\t%d\t%d\t%d\t%d\n",
               p[i].id,
               p[i].arrival,
               p[i].burst,
               p[i].finish,
               p[i].wait,
               p[i].turnaround);

        totalWait += p[i].wait;
        totalTurnaround += p[i].turnaround;
    }

    printf("\nAverage Waiting Time: %.2f\n", totalWait / n);
    printf("Average Turnaround Time: %.2f\n", totalTurnaround / n);

    int totalTime = 0;
    for (int i = 0; i < n; i++) {
        if (p[i].finish > totalTime)
            totalTime = p[i].finish;
    }

    printf("Throughput: %.2f processes/unit time\n\n",
           n / (float) totalTime);
}
