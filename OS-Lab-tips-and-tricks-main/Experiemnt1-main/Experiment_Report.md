# Experiment Report: Process Forking in C

## OBJECTIVE

The objective of this experiment is to demonstrate and understand the concept of process creation in Unix-like systems using the `fork()` system call in C. Specifically, the program creates a child process that executes the `whoami` command, while the parent process prints its process ID and waits for the child to complete.

## EQUATIONS, GENERAL CONCEPTS AND THEORETICAL BACKGROUND

### General Concepts
- **Process**: A process is an instance of a running program. Each process has a unique Process ID (PID) assigned by the operating system.
- **Fork System Call**: `fork()` is a system call that creates a new process (child) by duplicating the calling process (parent). It returns:
  - 0 in the child process.
  - The child's PID in the parent process.
  - -1 on failure.
- **Parent and Child Processes**: After `fork()`, both processes continue execution from the point of the call. They share the same code but have separate memory spaces.
- **Process Synchronization**: The parent can wait for the child using `wait()` or `waitpid()` to ensure the child finishes before the parent exits.
- **Executing Programs**: `execlp()` replaces the current process image with a new program. In this case, the child executes `whoami` to display the current user.

### Theoretical Background
In Unix, processes are fundamental for multitasking. Forking allows creating concurrent processes, enabling parallelism. The parent-child relationship ensures controlled execution, with the parent often managing resources and synchronization.

No mathematical equations are directly applicable, as this is a programming demonstration rather than a computational experiment.

## OBSERVATION AND DISCUSSION

During execution, the program successfully demonstrates process forking:

- The parent process prints its PID (e.g., "Process ID is: 17905").
- The child process executes `whoami`, outputting the current user (e.g., "codespace").
- The parent waits for the child to complete using `wait(NULL)`, ensuring proper synchronization.

Key observations:
- Each run generates a unique PID for the parent, as expected (e.g., 11661, 12815, 15785, 17905), confirming that processes are distinct instances.
- The child inherits the parent's environment but runs independently.
- Error handling for `fork()` failure is included, though not triggered in these runs.
- The output order may vary due to concurrent execution, but the logic ensures the parent waits.

This aligns with theoretical expectations: `fork()` creates a duplicate process, and `execlp()` overlays the child's memory with `whoami`. The experiment validates process isolation and synchronization in C.

## CONCLUSION

From this experiment, the main lessons learned include understanding how `fork()` enables process creation and concurrency in Unix systems, the importance of PIDs for process identification, and the role of synchronization primitives like `wait()` to manage parent-child relationships. Additionally, `execlp()` demonstrates how to replace a process image with a new executable, highlighting the flexibility of process management in C programming.

## APPENDIX I

### C Code (Exp1.C)
```c
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

int main(int argc, char *arg[])
{
    int pid;
    pid = fork();
    if (pid < 0)
    {
        printf("Fork Failed");
        exit(1);
    }
    else if (pid == 0)
    {
        execlp("whoami", "1s", NULL);
        exit(0);
    }
    else
    {
        printf("\n Process ID is: %d\n", getpid());
        wait(NULL);
        exit(0);
    }
}
```

## APPENDIX II

### Sample Run Outputs
- Run 1:  
  ```
   Process ID is: 11661
  codespace
  ```
- Run 2:  
  ```
   Process ID is: 12815
  codespace
  ```
- Run 3:  
  ```
  Initial PID: 15785
  Parent process ID: 15785
  Child process ID: 15786
  Child process ID: 15786
  Parent process ID: 15785
  ```
  (From the modified version in Exp1(fixed).C)

These outputs confirm the program's behavior across multiple executions.