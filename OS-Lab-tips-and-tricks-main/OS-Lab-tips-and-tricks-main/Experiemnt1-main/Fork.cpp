#include <iostream>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
using namespace std;

int main () {
    pid_t pid1, pid2, pid3, pid4;

    // First fork
    pid1 = fork();

    if (pid1 < 0) {
        cerr << "Fork failed" << endl;
        return 1;
    }

    if (pid1 == 0) {
        // Child process
        pid2 = fork();

        if (pid2 < 0) {
            cerr << "Fork failed" << endl;
            return 1;
        }

        if (pid2 == 0) {
            // Grandchild process
            cout << "Grandchild Process ID: " << getpid() << ", Parent Process ID: " << getppid() << endl;
        } else {
            // Child process
            wait(NULL); // Wait for grandchild to finish
            cout << "Child Process ID: " << getpid() << ", Parent Process ID: " << getppid() << endl;
        }
    } else {
        // Parent process
        wait(NULL); // Wait for child to finish

        // Second fork from parent
        pid3 = fork();

        if (pid3 < 0) {
            cerr << "Fork failed" << endl;
            return 1;
        }

        if (pid3 == 0) {
            // Second child process
            pid4 = fork();

            if (pid4 < 0) {
                cerr << "Fork failed" << endl;
                return 1;
            }

            if (pid4 == 0) {
                // Second grandchild process
                cout << "Second Grandchild Process ID: " << getpid() << ", Parent Process ID: " << getppid() << endl;
            } else {
                // Second child process
                wait(NULL); // Wait for second grandchild to finish
                cout << "Second Child Process ID: " << getpid() << ", Parent Process ID: " << getppid() << endl;
            }
        } else {
            // Original parent process
            wait(NULL); // Wait for second child to finish
            cout << "Parent Process ID: " << getpid() << endl;
        }
    }

    return 0;
}