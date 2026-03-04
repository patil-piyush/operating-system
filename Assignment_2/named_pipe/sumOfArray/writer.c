#include <stdio.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <unistd.h>
#include <sys/types.h>

int main()
{
    //Create FIFO file (if it doesn't exist)
    mkfifo("mypipe", 0666);

    int fd;
    int arr[] = {1, 2, 3, 4, 5};
    fd = open("mypipe", O_WRONLY);
    write(fd, arr, sizeof(arr)); // +1 to include null terminator
    close(fd);
}