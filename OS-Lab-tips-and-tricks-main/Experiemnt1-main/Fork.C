#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

int main() {
    pid_t pid1, pid2, pid3;
    int status;
    
    printf("Parent Process PID: %d\n\n", getpid());
    
    // Create first child process
    pid1 = fork();
    
    if (pid1 < 0) {
        perror("Fork 1 failed");
        exit(1);
    }
    else if (pid1 == 0) {
        // First child process
        printf("Child 1 - PID: %d, Parent PID: %d\n", getpid(), getppid());
        sleep(2);
        printf("Child 1 (PID: %d) exiting\n", getpid());
        exit(0);
    }
    
    // Create second child process
    pid2 = fork();
    
    if (pid2 < 0) {
        perror("Fork 2 failed");
        exit(1);
    }
    else if (pid2 == 0) {
        // Second child process
        printf("Child 2 - PID: %d, Parent PID: %d\n", getpid(), getppid());
        sleep(3);
        printf("Child 2 (PID: %d) exiting\n", getpid());
        exit(0);
    }
    
    // Create third child process
    pid3 = fork();
    
    if (pid3 < 0) {
        perror("Fork 3 failed");
        exit(1);
    }
    else if (pid3 == 0) {
        // Third child process
        printf("Child 3 - PID: %d, Parent PID: %d\n", getpid(), getppid());
        sleep(4);
        printf("Child 3 (PID: %d) exiting\n", getpid());
        exit(0);
    }
    
    // Parent process waits for all children
    printf("\nParent waiting for children to complete...\n\n");
    
    // Wait for first child
    waitpid(pid1, &status, 0);
    printf("Parent: Child 1 (PID: %d) has terminated\n", pid1);
    
    // Wait for second child
    waitpid(pid2, &status, 0);
    printf("Parent: Child 2 (PID: %d) has terminated\n", pid2);
    
    // Wait for third child (last process)
    waitpid(pid3, &status, 0);
    printf("Parent: Child 3 (PID: %d) has terminated\n", pid3);
    
    printf("\nAll child processes completed. Parent (PID: %d) exiting.\n", getpid());
    
    return 0;
}
