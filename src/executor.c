typedef struct {
    char *name;
    char **args;
} Command;

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

void execute_command(Command cmd) {
    if (fork() == 0) { // Child process
        execvp(cmd.name, cmd.args);
        perror("execvp failed");
        exit(EXIT_FAILURE);
    } else { // Parent process
        wait(NULL); // Wait for the child process to finish
    }
}