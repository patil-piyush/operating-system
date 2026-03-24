#include <iostream>
#include <fcntl.h>
#include <sys/stat.h>
#include <unistd.h>
#include <cstring>

using namespace std;

int main() {
    const char* fifo = "mypipe";

    // create FIFO if not exists
    mkfifo(fifo, 0666);

    int fd;
    string input;

    while (true) {
        cout << "Writer> ";
        getline(cin, input);

        if (input == "exit")
            break;

        fd = open(fifo, O_WRONLY);

        write(fd, input.c_str(), input.size() + 1);

        close(fd);
    }

    return 0;
}