#include <iostream>
#include <sstream>
#include <vector>
#include <unistd.h>
#include <sys/wait.h>
#include <cstring>

using namespace std;

// helper function to trim spaces
string trim(string s) {
    while (!s.empty() && s[0] == ' ') s.erase(0,1);
    while (!s.empty() && s[s.size()-1] == ' ') s.pop_back();
    return s;
}

int main() {

    string input;

    while (true) {

        cout << "myshell> ";
        getline(cin, input);

        if (input == "exit")
            break;

        // ---------- Split commands ----------
        vector<string> commands;
        stringstream ss(input);
        string temp;

        while (getline(ss, temp, '|')) {
            commands.push_back(trim(temp)); // remove extra spaces
        }

        int n = commands.size();

        // ---------- Create pipes ----------
        int pipes[n-1][2];

        for (int i = 0; i < n-1; i++) {
            if (pipe(pipes[i]) == -1) {
                perror("pipe");
                exit(1);
            }
        }

        // ---------- Create processes ----------
        for (int i = 0; i < n; i++) {

            pid_t pid = fork();

            if (pid == 0) {

                // input from previous pipe
                if (i > 0)
                    dup2(pipes[i-1][0], 0);

                // output to next pipe
                if (i < n-1)
                    dup2(pipes[i][1], 1);

                // close all pipes
                for (int j = 0; j < n-1; j++) {
                    close(pipes[j][0]);
                    close(pipes[j][1]);
                }

                // ---------- FIXED ARGUMENT PARSING ----------
                stringstream cmd(commands[i]);
                vector<string> temp_args;
                string word;

                while (cmd >> word) {
                    temp_args.push_back(word);
                }

                vector<char*> args;

                for (auto &arg : temp_args) {
                    args.push_back(&arg[0]);
                }

                args.push_back(NULL);

                // ---------- EXECUTE ----------
                execvp(args[0], args.data());

                perror("Execution failed");
                exit(1);
            }
        }

        // ---------- Parent closes pipes ----------
        for (int i = 0; i < n-1; i++) {
            close(pipes[i][0]);
            close(pipes[i][1]);
        }

        // ---------- Wait for children ----------
        for (int i = 0; i < n; i++)
            wait(NULL);
    }

    return 0;
}



// cat test.txt | grep name | wc -l
// cat test.txt | grep piyush test.txt
// cat test.txt | grep noo test.txt
// ls | wc