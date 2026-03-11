#include <iostream>
#include <sstream>
#include <vector>
#include <unistd.h>
#include <sys/wait.h>

using namespace std;

int main() {

    string input;

    while (true) {

        cout << "myshell> ";
        getline(cin, input);

        if (input == "exit")
            break;

        vector<string> commands;
        stringstream ss(input);
        string temp;

        while (getline(ss, temp, '|')) {
            commands.push_back(temp);
        }

        int n = commands.size();

        int pipes[n-1][2];

        for (int i = 0; i < n-1; i++)
            pipe(pipes[i]);

        for (int i = 0; i < n; i++) {

            if (fork() == 0) {

                // Only redirect input if it's not the first command
                // redirect the output of the previous command to the input of the current command
                if (i > 0)
                    dup2(pipes[i-1][0], 0);
                
                // Only redirect output if it's not the last command
                // redirect the output of the current command to the input of the next command
                if (i < n-1)
                    dup2(pipes[i][1], 1);

                for (int j = 0; j < n-1; j++) {
                    close(pipes[j][0]);
                    close(pipes[j][1]);
                }

                stringstream cmd(commands[i]);
                vector<char*> args;
                string word;

                while (cmd >> word)
                    args.push_back(&word[0]);

                args.push_back(NULL);

                execvp(args[0], args.data());

                perror("Execution failed");
                exit(1);
            }
        }

        for (int i = 0; i < n-1; i++) {
            close(pipes[i][0]);
            close(pipes[i][1]);
        }

        for (int i = 0; i < n; i++)
            wait(NULL);
    }

    return 0;
}