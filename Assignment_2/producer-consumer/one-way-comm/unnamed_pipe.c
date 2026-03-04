#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>

int main() {

    int pipefd[2];
    pid_t pid;

    char write_msg[] = "Hello from Producer";
    char read_msg[100];

    // create pipe
    pipe(pipefd);

    pid = fork();

    if(pid > 0) // Producer (Parent)
    {
        close(pipefd[0]); // close read end

        printf("Producer producing data...\n");
        write(pipefd[1], write_msg, strlen(write_msg)+1);

        close(pipefd[1]);
    }
    else // Consumer (Child)
    {
        close(pipefd[1]); // close write end

        read(pipefd[0], read_msg, sizeof(read_msg));
        printf("Consumer consumed data: %s\n", read_msg);

        close(pipefd[0]);
    }

    return 0;
}