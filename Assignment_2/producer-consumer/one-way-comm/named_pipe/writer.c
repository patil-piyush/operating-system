#include <stdio.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>

int main() {

    int fd;
    char *fifo = "mypipe";
    char data[] = "Hello from Producer";

    // create fifo
    mkfifo(fifo, 0666);

    fd = open(fifo, O_WRONLY);

    printf("Producer producing data...\n");

    write(fd, data, strlen(data)+1);

    close(fd);

    return 0;
}