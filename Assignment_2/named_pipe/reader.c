#include <stdio.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <unistd.h>

int main()
{
    char buffer[100];
    int fd;

    fd = open("mypipe", O_RDONLY);
    int n = read(fd, buffer, sizeof(buffer));

    if(n > 0)
        printf("Received: %s\n", buffer);
    else
        printf("No data received\n");

    close(fd);
    return 0;
}