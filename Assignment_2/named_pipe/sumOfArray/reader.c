#include <stdio.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <unistd.h>

int main()
{
    int arr[5];
    int fd;

    fd = open("mypipe", O_RDONLY);
    int n = read(fd, arr, sizeof(arr));
    int sum = 0;
    for(int i = 0; i < 5; i++)
        sum += arr[i];
    if(n > 0){
        printf("Received: ");
        for(int i = 0; i < 5; i++)
            printf("%d ", arr[i]);
        printf("\nSum: %d\n", sum);
    }
    else printf("No data received\n");

    close(fd);
    return 0;
}