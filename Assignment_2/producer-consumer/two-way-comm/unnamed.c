#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>

int main() {

    int pipe1[2]; // producer -> consumer
    int pipe2[2]; // consumer -> producer
    pid_t pid;

    char producer_msg[] = "Item produced";
    char consumer_msg[] = "Item consumed";
    char buffer[100];

    pipe(pipe1);
    pipe(pipe2);

    pid = fork();

    if(pid > 0) // Producer (Parent)
    {
        close(pipe1[0]); 
        close(pipe2[1]);

        printf("Producer sending: %s\n", producer_msg);
        write(pipe1[1], producer_msg, strlen(producer_msg)+1);

        read(pipe2[0], buffer, sizeof(buffer));
        printf("Producer received acknowledgement: %s\n", buffer);

        close(pipe1[1]);
        close(pipe2[0]);
    }
    else // Consumer (Child)
    {
        close(pipe1[1]);
        close(pipe2[0]);

        read(pipe1[0], buffer, sizeof(buffer));
        printf("Consumer received: %s\n", buffer);

        printf("Consumer sending acknowledgement\n");
        write(pipe2[1], consumer_msg, strlen(consumer_msg)+1);

        close(pipe1[0]);
        close(pipe2[1]);
    }

    return 0;
}