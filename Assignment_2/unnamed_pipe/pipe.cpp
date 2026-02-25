
#include <iostream>
#include <unistd.h>
#include <cstring>

using namespace std;

int main()
{
    int fd[2];
    char write_msg[] = "Hello through pipe";
    char read_msg[50];

    // create pipe
    if (pipe(fd) == -1)
    {
        cout << "Pipe creation failed" << endl;
        return 1;
    }

    // write to pipe
    write(fd[1], write_msg, strlen(write_msg) + 1);

    // read from pipe
    read(fd[0], read_msg, sizeof(read_msg));

    cout << "Message written into pipe: " << write_msg << endl;
    cout << "Message read from pipe: " << read_msg << endl;

    close(fd[0]);
    close(fd[1]);

    return 0;
}