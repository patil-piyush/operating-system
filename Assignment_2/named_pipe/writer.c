#include <stdio.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>

int main()
{
    //Create FIFO file (if it doesn't exist)
    mkfifo("mypipe", 0666);

    int fd;
    char msg[] = "Hello FIFO!";
    fd = open("mypipe", O_WRONLY);
    write(fd, msg, strlen(msg)+1); // +1 to include null terminator
    close(fd);
}