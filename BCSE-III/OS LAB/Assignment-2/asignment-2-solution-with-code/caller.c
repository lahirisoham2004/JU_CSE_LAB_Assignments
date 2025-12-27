#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <errno.h>
#include <fcntl.h>
#include <time.h>

int main(int argc, char* argv[]) {

    if (mkfifo("/tmp/fifo3b", 0777) == -1) {
        if (errno != EEXIST) {
            perror("mkfifo failed");
            exit(EXIT_FAILURE);
        }
    }

    int fd = open("/tmp/fifo3b", O_WRONLY);
    if (fd == -1) {
        perror("open failed");
        exit(EXIT_FAILURE);
    }
    srand(time(NULL)^getpid());

    int a[5];

    for(int i = 0; i< 5; i++) {
        a[i] = rand() % 100;
    }

    for(int i = 0; i< 5; i++) {
        write(fd, &a[i], sizeof(int));
        printf("Wrote %d.\n", a[i]);
    }
    
    printf("Closing sender.\n");
    close(fd);
    return 0;
}