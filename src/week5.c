#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/wait.h>

#define MAX_INPUT 200

int main() {

    char input[MAX_INPUT];

    while (1) {

        // Display shell prompt
        printf("shellforge> ");
        fflush(stdout);

        // Read command
        if (fgets(input, sizeof(input), stdin) == NULL) {
            break;
        }

        // Remove newline
        input[strcspn(input, "\n")] = '\0';

        // Exit
        if (strcmp(input, "exit") == 0) {
            printf("Exiting ShellForge...\n");
            break;
        }

        // Find output redirection >
        char *output_file = strchr(input, '>');

        // Find input redirection <
        char *input_file = strchr(input, '<');

        // ---------------- OUTPUT REDIRECTION ----------------
        if (output_file != NULL) {

            // Separate command and filename
            *output_file = '\0';

            output_file++;

            // Remove spaces before filename
            while (*output_file == ' ') {
                output_file++;
            }

            // Remove trailing spaces
            char *end = output_file + strlen(output_file) - 1;

            while (end > output_file && *end == ' ') {
                *end = '\0';
                end--;
            }

            // Remove trailing spaces from command
            end = input + strlen(input) - 1;

            while (end > input && *end == ' ') {
                *end = '\0';
                end--;
            }

            // Open file
            int fd = open(output_file,
                          O_WRONLY | O_CREAT | O_TRUNC,
                          0644);

            if (fd < 0) {
                perror("open");
                continue;
            }

            // Create child process
            pid_t pid = fork();

            if (pid == 0) {

                // Redirect stdout to file
                dup2(fd, STDOUT_FILENO);

                // Close original file descriptor
                close(fd);

                // Execute command
                execlp(input, input, NULL);

                // If execlp fails
                perror("execlp");
                exit(1);

            } else if (pid > 0) {

                // Parent waits
                wait(NULL);

                close(fd);

            } else {
                perror("fork");
                close(fd);
            }

            continue;
        }

        // ---------------- INPUT REDIRECTION ----------------
        if (input_file != NULL) {

            // Separate command and filename
            *input_file = '\0';

            input_file++;

            // Remove spaces before filename
            while (*input_file == ' ') {
                input_file++;
            }

            // Remove trailing spaces
            char *end = input_file + strlen(input_file) - 1;

            while (end > input_file && *end == ' ') {
                *end = '\0';
                end--;
            }

            // Remove trailing spaces from command
            end = input + strlen(input) - 1;

            while (end > input && *end == ' ') {
                *end = '\0';
                end--;
            }

            // Open input file
            int fd = open(input_file, O_RDONLY);

            if (fd < 0) {
                perror("open");
                continue;
            }

            // Create child process
            pid_t pid = fork();

            if (pid == 0) {

                // Redirect stdin from file
                dup2(fd, STDIN_FILENO);

                // Close original descriptor
                close(fd);

                // Execute command
                execlp(input, input, NULL);

                // If execlp fails
                perror("execlp");
                exit(1);

            } else if (pid > 0) {

                // Parent waits
                wait(NULL);

                close(fd);

            } else {
                perror("fork");
                close(fd);
            }
continue;
        }

        // ---------------- NORMAL COMMAND ----------------

        pid_t pid = fork();

        if (pid == 0) {

            execlp(input, input, NULL);

            perror("Command failed");
            exit(1);

        } else if (pid > 0) {

            wait(NULL);

        } else {

            perror("fork");
        }
    }

    return 0;
}
