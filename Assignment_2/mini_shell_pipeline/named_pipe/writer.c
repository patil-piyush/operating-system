#include <iostream>
#include <fcntl.h>
#include <unistd.h>
#include <cstring>

using namespace std;

int main() {

    int fd;
    char message[100];

    cout << "Writer started\n";

    while(true) {

        cout << "Enter message: ";
        cin.getline(message, 100);

        fd = open("mypipe", O_WRONLY);

        if (fd < 0) {
            perror("open");
            return 1;
        }

        write(fd, message, strlen(message) + 1);

        close(fd);

        if(strcmp(message, "exit") == 0)
            break;
    }

    return 0;
}