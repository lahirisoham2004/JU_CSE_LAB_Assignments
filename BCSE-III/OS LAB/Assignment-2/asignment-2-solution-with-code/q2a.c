#include <stdio.h>       // For input/output functions (e.g., printf, perror)
#include <stdlib.h>      // For `rand`, `srand`, and `exit`
#include <unistd.h>      // For `fork`, `sleep`, and `getpid`
#include <sys/types.h>   // For `pid_t` type
#include <time.h>        // For seeding random number generation
#include <sys/wait.h>    // For `wait` to wait for child processes

#define NUM_ITERATIONS 10

void child_process(char *name) {
    srand(getpid()); // Seed random number generator uniquely for each process
    for (int i = 0; i < NUM_ITERATIONS; i++) {
        // Display process name and iteration number
        printf("%s: Iteration %d\n", name, i + 1);

        // Generate a random sleep duration between 1 to 3 seconds
        int sleep_duration = rand() % 10 + 1; // Random number between 1 and 3
        // Display sleep duration
        printf("%s: Sleeping for %d seconds\n", name, sleep_duration);
        sleep(sleep_duration);
    }
}

int main() {
    // srand(time(NULL)); // Seed for random number generation

    // Create child process X
    pid_t pidX = fork();

    if (pidX < 0) {
        perror("Failed to create child process X");
        return 1;
    } else if (pidX == 0) {
        // Child process X
        child_process("Child X");
        return 0;
    }

    // Create child process Y
    pid_t pidY = fork();

    if (pidY < 0) {
        perror("Failed to create child process Y");
        return 1;
    } else if (pidY == 0) {
        // Child process Y
        child_process("Child Y");
        return 0;
    }

    // Parent process waits for both children to finish
    wait(NULL); // Wait for child X
    wait(NULL); // Wait for child Y

    printf("Both child processes have completed.\n");
    return 0;
}
