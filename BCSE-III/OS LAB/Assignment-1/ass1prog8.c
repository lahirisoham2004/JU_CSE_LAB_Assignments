#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>

// Maximum length of a command
#define MAX_COMMAND_LENGTH 1024

// Function to parse the command line input
void parse_command(char *input, char **args) {
    int i = 0;
    char *token = strtok(input, " \n");
    while (token != NULL) {
        args[i++] = token;
        token = strtok(NULL, " \n");
    }
    args[i] = NULL; // End the argument list with NULL
}

// Function to execute built-in commands or external commands
int execute_command(char **args) {
    // If the command is empty, do nothing
    if (args[0] == NULL) {
        return 1;
    }

    // Built-in command: exit
    if (strcmp(args[0], "exit") == 0) {
        return 0; // Exit the shell
    }

    // Built-in command: cd
    if (strcmp(args[0], "cd") == 0) {
        if (args[1] == NULL) {
            fprintf(stderr, "Expected argument to \"cd\"\n");
        } else {
            if (chdir(args[1]) != 0) {
                perror("chdir failed");
            }
        }
        return 1;
    }

    // Built-in command: pwd
    if (strcmp(args[0], "pwd") == 0) {
        char cwd[MAX_COMMAND_LENGTH];
        if (getcwd(cwd, sizeof(cwd)) != NULL) {
            printf("%s\n", cwd);
        } else {
            perror("getcwd failed");
        }
        return 1;
    }

    // Built-in command: echo
    if (strcmp(args[0], "echo") == 0) {
        for (int i = 1; args[i] != NULL; i++) {
            printf("%s ", args[i]);
        }
        printf("\n");
        return 1;
    }

    // For other commands, fork a process and execute
    pid_t pid = fork();
    if (pid == 0) { // Child process
        if (execvp(args[0], args) == -1) {
            perror("execvp failed");
        }
        exit(EXIT_FAILURE);
    } else if (pid < 0) { // Forking error
        perror("fork failed");
    } else { // Parent process
        int status;
        waitpid(pid, &status, 0); // Wait for the child process to complete
    }
    return 1;
}

int main() {
    char input[MAX_COMMAND_LENGTH];
    char *args[MAX_COMMAND_LENGTH / 2 + 1]; // Command arguments

    while (1) {
        // Display the prompt
        printf("1234-shell> ");
        fflush(stdout);

        // Read input
        if (fgets(input, sizeof(input), stdin) == NULL) {
            perror("fgets failed");
            exit(EXIT_FAILURE);
        }

        // Parse the command line input
        parse_command(input, args);

        // Execute the command
        if (!execute_command(args)) {
            break; // Exit the loop and end the shell
        }
    }

    return 0;
}

