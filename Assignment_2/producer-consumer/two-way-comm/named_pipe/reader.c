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

    fd1 = open(fifo1, O_RDONLY);
    fd2 = open(fifo2, O_WRONLY);

    read(fd1, buffer, sizeof(buffer));
    printf("Consumer received: %s\n", buffer);

    char reply[] = "Item consumed";

    printf("Consumer sending acknowledgement\n");
    write(fd2, reply, strlen(reply)+1);

    close(fd1);
    close(fd2);

    return 0;
}