#include <iostream>
#include <fcntl.h>
#include <sys/stat.h>
#include <unistd.h>
#include <sstream>
#include <vector>
#include <sys/wait.h>

using namespace std;

// trim function
string trim(string s) {
    while (!s.empty() && s[0] == ' ') s.erase(0,1);
    while (!s.empty() && s[s.size()-1] == ' ') s.pop_back();
    return s;
}

int main() {
    const char* fifo = "mypipe";

    mkfifo(fifo, 0666);

    char buffer[1024];

    while (true) {

        int fd = open(fifo, O_RDONLY);

        read(fd, buffer, sizeof(buffer));

        string input(buffer);

        close(fd);

        if (input == "exit")
            break;

        // ---------- Split commands ----------
        vector<string> commands;
        stringstream ss(input);
        string temp;

        while (getline(ss, temp, '|')) {
            commands.push_back(trim(temp));
        }

        int n = commands.size();

        int pipes[n-1][2];

        for (int i = 0; i < n-1; i++) {
            pipe(pipes[i]);
        }

        // ---------- Fork processes ----------
        for (int i = 0; i < n; i++) {

            if (fork() == 0) {

                if (i > 0)
                    dup2(pipes[i-1][0], 0);

                if (i < n-1)
                    dup2(pipes[i][1], 1);

                for (int j = 0; j < n-1; j++) {
                    close(pipes[j][0]);
                    close(pipes[j][1]);
                }

                stringstream cmd(commands[i]);
                vector<string> temp_args;
                string word;

                while (cmd >> word)
                    temp_args.push_back(word);

                vector<char*> args;

                for (auto &arg : temp_args)
                    args.push_back(&arg[0]);

                args.push_back(NULL);

                execvp(args[0], args.data());

                perror("Execution failed");
                exit(1);
            }
        }

        // close pipes in parent
        for (int i = 0; i < n-1; i++) {
            close(pipes[i][0]);
            close(pipes[i][1]);
        }

        for (int i = 0; i < n; i++)
            wait(NULL);
    }

    return 0;
}