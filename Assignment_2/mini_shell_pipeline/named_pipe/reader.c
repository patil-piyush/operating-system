#include <iostream>
#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>
#include <cstring>

using namespace std;

int main() {

    int fd;
    char buffer[100];

    mkfifo("mypipe", 0666);

    cout << "Reader started. Waiting for messages...\n";

    while(true) {

        fd = open("mypipe", O_RDONLY);

        read(fd, buffer, sizeof(buffer));

        cout << "Received: " << buffer << endl;

        close(fd);

        if(strcmp(buffer, "exit") == 0)
            break;
    }

    unlink("mypipe");

    return 0;
}