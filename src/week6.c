#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

int main() {

    int pipefd[2];

    // Create pipe
    if (pipe(pipefd) == -1) {
        perror("pipe");
        return 1;
    }

    // Create first child
    pid_t pid1 = fork();

    if (pid1 == -1) {
        perror("fork");
        return 1;
    }

    if (pid1 == 0) {

        // Child 1: ls

        // Close unused read end
        close(pipefd[0]);

        // Redirect stdout to pipe
        dup2(pipefd[1], STDOUT_FILENO);

        // Close original write descriptor
        close(pipefd[1]);

        // Execute ls
        execlp("ls", "ls", NULL);

        // If execlp fails
        perror("execlp");
        exit(1);
    }

    // Create second child
    pid_t pid2 = fork();

    if (pid2 == -1) {
        perror("fork");
        return 1;
    }

    if (pid2 == 0) {

        // Child 2: wc -l

        // Close unused write end
        close(pipefd[1]);

        // Redirect stdin from pipe
        dup2(pipefd[0], STDIN_FILENO);

        // Close original read descriptor
        close(pipefd[0]);

        // Execute wc -l
        execlp("wc", "wc", "-l", NULL);

        // If execlp fails
        perror("execlp");
        exit(1);
    }

    // Parent does not use the pipe
    close(pipefd[0]);
    close(pipefd[1]);

    // Wait for both children
    waitpid(pid1, NULL, 0);
    waitpid(pid2, NULL, 0);

    return 0;
}
