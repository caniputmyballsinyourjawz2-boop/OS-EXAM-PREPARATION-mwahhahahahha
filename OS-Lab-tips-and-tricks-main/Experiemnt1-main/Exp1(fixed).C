#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

int main() {
    pid_t pid;

    // 2. Declare pid and get the initial PID (though it will be overwritten by fork)
    pid = getpid();
    printf("Initial PID: %d\n", pid);

    // 3. Create a child process by calling fork()
    pid = fork();

    if (pid < 0) {
        printf("Fork failed\n");
        exit(1);
    } else if (pid == 0) {
        // 4. Child process: print child PID and parent PID
        printf("Child process ID: %d\n", getpid());
        printf("Parent process ID: %d\n", getppid());
    } else {
        // 4. Parent process: print parent PID and child PID
        printf("Parent process ID: %d\n", getpid());
        printf("Child process ID: %d\n", pid);
        wait(NULL);  // Wait for child to finish
    }

    // 5. Stop the program
    return 0;
}
