#include <stdio.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>

int main() {

    int fd1, fd2;
    char buffer[100];

    char *fifo1 = "fifo1";
    char *fifo2 = "fifo2";

    mkfifo(fifo1, 0666);
    mkfifo(fifo2, 0666);

    fd1 = open(fifo1, O_WRONLY);
    fd2 = open(fifo2, O_RDONLY);

    char message[] = "Item produced";

    printf("Producer sending: %s\n", message);
    write(fd1, message, strlen(message)+1);

    read(fd2, buffer, sizeof(buffer));
    printf("Producer received: %s\n", buffer);

    close(fd1);
    close(fd2);

    return 0;
}