#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define MAX_INPUT 100

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

        // Exit command
        if (strcmp(input, "exit") == 0) {
            printf("Exiting ShellForge...\n");
            break;
        }

        // Built-in cd command
        if (strncmp(input, "cd ", 3) == 0) {

            // Extract directory name
            char *directory = input + 3;

            // Change directory
            if (chdir(directory) == 0) {
                printf("Directory changed to: %s\n", directory);
            } else {
                perror("cd");
            }

            continue;
        }

        // Handle "cd" without argument
        if (strcmp(input, "cd") == 0) {
            char *home = getenv("HOME");

            if (home != NULL) {
                if (chdir(home) == 0) {
                    printf("Changed to home directory: %s\n", home);
                } else {
                    perror("cd");
                }
            }

            continue;
        }

        // Unknown command
        printf("Unknown command: %s\n", input);
    }

    return 0;
}
