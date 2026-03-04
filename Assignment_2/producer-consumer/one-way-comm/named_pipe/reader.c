#include <stdio.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <unistd.h>

int main() {

    int fd;
    char *fifo = "mypipe";
    char buffer[100];

    fd = open(fifo, O_RDONLY);

    read(fd, buffer, sizeof(buffer));

    printf("Consumer consumed data: %s\n", buffer);

    close(fd);

    return 0;
}