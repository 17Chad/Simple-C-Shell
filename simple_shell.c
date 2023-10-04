#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

#define MAX_INPUT_SIZE 1024

int main() {
    char input[MAX_INPUT_SIZE];

    while (1) {
        printf("shell> ");
        fgets(input, sizeof(input), stdin);

        // Remove the newline character from the input
        size_t input_length = strlen(input);
        if (input_length > 0 && input[input_length - 1] == '\n') {
            input[input_length - 1] = '\0';
        }

        // Exit the shell if the user enters "exit" or "quit"
        if (strcmp(input, "exit") == 0 || strcmp(input, "quit") == 0) {
            break;
        }

        // Create a child process
        pid_t child_pid = fork();

        if (child_pid < 0) {
            perror("fork");
            exit(EXIT_FAILURE);
        } else if (child_pid == 0) {
            // This code is executed by the child process

            // Tokenize the input to separate command and arguments
            char *args[MAX_INPUT_SIZE];
            int num_args = 0;
            char *token = strtok(input, " ");

            while (token != NULL) {
                args[num_args++] = token;
                token = strtok(NULL, " ");
            }
            args[num_args] = NULL;

            // Use the system call to execute the command
            if (execvp(args[0], args) == -1) {
                perror("execvp");
                exit(EXIT_FAILURE);
            }

            // The child process will never reach this point
        } else {
            // This code is executed by the parent process
            // Wait for the child process to complete
            int status;
            waitpid(child_pid, &status, 0);


            //Troubleshooting output 
            //if (WIFEXITED(status)) {
            //    printf("Child process exited with status %d\n", WEXITSTATUS(status));
            //} else if (WIFSIGNALED(status)) {
            //    printf("Child process terminated by signal %d\n", WTERMSIG(status));
            //}
        }
    }

    return 0;
}
