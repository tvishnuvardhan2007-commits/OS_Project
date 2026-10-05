#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_ARGS 100

int main()
{
    char *line = NULL;
    size_t len = 0;
    ssize_t nread;

    char *args[MAX_ARGS];
    int argc;

    while (1)
    {
        printf("shellforge> ");
        fflush(stdout);

        nread = getline(&line, &len, stdin);

        /* Ctrl+D */
        if (nread == -1)
        {
            printf("\nExiting shell...\n");
            break;
        }

        /* Remove newline */
        line[strcspn(line, "\n")] = '\0';

        /* Ignore empty input */
        if (strlen(line) == 0)
            continue;

        /* Exit command */
        if (strcmp(line, "exit") == 0)
        {
            printf("Exiting shell...\n");
            break;
        }

        /* Tokenize the command */
        argc = 0;

        char *token = strtok(line, " \t");

        while (token != NULL && argc < MAX_ARGS - 1)
        {
            args[argc] = token;
            argc++;

            token = strtok(NULL, " \t");
        }

        args[argc] = NULL;

        /* Display tokens */
        printf("Arguments:\n");

        for (int i = 0; i < argc; i++)
        {
            printf("args[%d] = %s\n", i, args[i]);
        }
    }

    free(line);

    return 0;
}
