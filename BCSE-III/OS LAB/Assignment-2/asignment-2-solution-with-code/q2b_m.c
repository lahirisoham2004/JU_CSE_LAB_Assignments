#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <errno.h>
#include <fcntl.h>
#include <semaphore.h>
#include <time.h>

sem_t *S;
sem_t *Q;

void cleanup() {
    if (S) {
        sem_close(S);
        sem_unlink("sem1");
    }
    if (Q) {
        sem_close(Q);
        sem_unlink("sem2");
    }
}

int main(int argc, char* argv[]) {
    int pidx, pidy;

    sem_unlink("sem1");  //these cleanup is necessary as if previously same name semaphores exits then it ensure to removed the same named semaphor
    sem_unlink("sem2");  // if this is not done then it give error as i'm attempting to recreate it without first unlinking will result in an error.

    // Initialize semaphores
    S = sem_open("sem1", O_CREAT | O_EXCL, 0644, 1);  //O_CREAT means Creates the semaphore if it does not already exist or open if exists.
    if (S == SEM_FAILED) {                             // O_EXCL means Ensures that sem_open fails if the semaphore already exists.
        perror("sem_open S");
        exit(EXIT_FAILURE);
    }

    Q = sem_open("sem2", O_CREAT | O_EXCL, 0644, 0);
    if (Q == SEM_FAILED) {
        perror("sem_open Q");
        cleanup(); // Cleanup before exiting
        exit(EXIT_FAILURE);
    }

    printf("Creating Child X...\n");
    pidx = fork(); // Creating child process X
    if (pidx < 0) {
        perror("fork X");
        cleanup();
        exit(EXIT_FAILURE);
    }

    if (pidx != 0) { // Parent process
        printf("Creating Child Y...\n");
        pidy = fork(); // Creating child Y
        if (pidy < 0) {
            perror("fork Y");
            cleanup();
            exit(EXIT_FAILURE);
        }
    }

    if (pidx == 0) { // Child X
        for (int i = 0; i < 10; i++) {
            sem_wait(S);
            printf("Child X. Iteration no.: %d\n", i + 1);
            srand(time(NULL) ^ getpid());
            int sleep_duration = rand() % 5 + 1;
            printf("Child X: Sleeping for %d seconds\n", sleep_duration);
            sleep(sleep_duration);
            sem_post(Q);
        }
    }

    if (pidx != 0 && pidy == 0) { // Child Y
        for (int i = 0; i < 10; i++) {
            sem_wait(Q);
            printf("Child Y. Iteration no.: %d\n", i + 1);
            srand(time(NULL) ^ getpid());
            int sleep_duration = rand() % 5 + 1;
            printf("Child Y: Sleeping for %d seconds\n", sleep_duration);
            sleep(sleep_duration);
            sem_post(S);        
        }
    }

    if( pidx != 0 && pidy != 0) {       // Main process
        while (wait(NULL) > 0);
    }

    // Cleanup semaphores
    cleanup();
    
    return 0;
}
