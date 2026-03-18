#include <iostream>
#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>
#include <cstring>

using namespace std;

int main() {

    int fd;
    char buffer[100];

    // Create FIFO (ignore error if already exists)
    if (mkfifo("mypipe", 0666) == -1) {
        cout << "Pipe may already exist\n";
    }

    cout << "Reader started. Waiting for messages...\n";

    while(true) {

        fd = open("mypipe", O_RDONLY);

        if (fd < 0) {
            perror("open");
            return 1;
        }

        memset(buffer, 0, sizeof(buffer)); // clear buffer

        int n = read(fd, buffer, sizeof(buffer) - 1);

        if (n > 0) {
            buffer[n] = '\0'; // ensure null termination
            cout << "Received: " << buffer << endl;
        }

        close(fd);

        if(strcmp(buffer, "exit") == 0)
            break;
    }

    unlink("mypipe"); // delete pipe

    return 0;
}