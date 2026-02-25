#include <iostream>
#include <unistd.h>
#include <sys/types.h>

using namespace std;

int main()
{
    pid_t p;

    cout << "Before fork" << endl;

    p = fork();

    if (p < 0)
    {
        cout << "Fork failed" << endl;
        return 1;
    }
    else if (p == 0)
    {
        // Child process
        cout << "I am child having id: " << getpid() << endl;
        cout << "My parent's id is: " << getppid() << endl;
    }
    else
    {
        // Parent process
        cout << "I am parent having id: " << getpid() << endl;
        cout << "My child's id is: " << p << endl;
    }

    cout << "Common section executed by both processes" << endl;

    return 0;
}


