#include <iostream>
#include <vector>
#include <climits>
using namespace std;
/*
    1. FCFS                                             done
    2. SJF – Preemptive and Non-preemptive              done
    3. Round Robin                                      done
    4. Priority – Preemptive and Non-preemptive         done
*/

class process
{
private:
    int p;
    int *BT;           // burst time
    int *AT;           // arrival time
    int *WT;           // waiting time
    int *CT;           // completion time
    int *RT;           // remaining time
    int *TAT;          // turn around time
    bool *isCompleted; // to check if process is completed
    int *priority;     // priority of processes (for priority scheduling)

public:
    process()
    {
        p = 0;
    }

    void displayData()
    {
        cout << "\n================ Process Table ================\n";
        cout << "PID\tAT\tBT\tPt\tCT\tTAT\tWT\n";
        cout << "----------------------------------------------\n";

        for (int i = 0; i < p; i++)
        {
            cout << i + 1 << "\t"
                 << AT[i] << "\t"
                 << BT[i] << "\t"
                 << priority[i] << "\t"
                 << CT[i] << "\t"
                 << TAT[i] << "\t"
                 << WT[i] << "\n";
        }

        cout << "==============================================\n\n";
    }

    void inputData()
    {
        cout << "How many Processes are there to be executed?\n";
        cin >> p;
        AT = new int[p];
        BT = new int[p];
        WT = new int[p];
        CT = new int[p];
        RT = new int[p];
        TAT = new int[p];
        isCompleted = new bool[p];
        priority = new int[p];
        for (int i = 0; i < p; i++)
        {
            CT[i] = 0;
            TAT[i] = 0;
            WT[i] = 0;
            priority[i] = 0;
        }
        cout << "Enter the Arrival time for each process\n";
        for (int i = 0; i < p; i++)
        {
            cin >> AT[i];
        }
        cout << "Enter the Burst time for each process\n";
        for (int i = 0; i < p; i++)
        {
            cin >> BT[i];
        }
        cout<<"Is there any priority for the processes? (1 for Yes, 0 for No): ";
        int hasPriority;
        cin >> hasPriority;
        if (hasPriority) {
            cout << "Enter the Priority for each process\n";
            for (int i = 0; i < p; i++)
            {
                cin >> priority[i];
            }
        }
        cout << endl
             << endl;
        cout<<"Entered Data:\n";
        displayData();
    }

    void FCFS()
    {
        WT[0] = 0;
        cout << "ProcessID\t" << "WT Time\n";
        cout << "   " << 1 << "\t\t" << 0 << endl;
        for (int i = 1; i < p; i++)
        {
            WT[i] = BT[i - 1] + WT[i - 1];
            // condition to check if the cpu is in ideal mode
            if (AT[i] > WT[i])
            {
                WT[i] = AT[i];
            }
            cout << "   " << i + 1 << "\t\t" << WT[i] << endl;
        }
        cout << endl
             << endl;
    }

    void NPSJFScheduling()
    {
        for (int i = 0; i < p; i++)
        {
            isCompleted[i] = false;
        }

        int time = 0, completed = 0;

        while (completed < p)
        {
            int index = -1;
            int minBT = INT_MAX;

            for (int i = 0; i < p; i++)
            {
                if (AT[i] <= time && !isCompleted[i] && BT[i] < minBT)
                {
                    minBT = BT[i];
                    index = i;
                }
            }

            if (index == -1)
            {
                time++;
            }
            else
            {
                time += BT[index];
                CT[index] = time;
                TAT[index] = CT[index] - AT[index];
                WT[index] = TAT[index] - BT[index];

                isCompleted[index] = true;
                completed++;
            }
        }

        displayData();
    }

    void PSJFScheduling()
    {
        for (int i = 0; i < p; i++)
        {
            RT[i] = BT[i]; // Remaining Time
            isCompleted[i] = false;
        }

        int time = 0, completed = 0;

        while (completed < p)
        {
            int index = -1;
            int minRT = INT_MAX;

            // Find process with shortest remaining time
            for (int i = 0; i < p; i++)
            {
                if (AT[i] <= time && !isCompleted[i] && RT[i] < minRT)
                {
                    minRT = RT[i];
                    index = i;
                }
            }

            // CPU idle
            if (index == -1)
            {
                time++;
            }
            else
            {
                RT[index]--; // execute for 1 unit
                time++;

                if (RT[index] == 0)
                {
                    CT[index] = time;
                    TAT[index] = CT[index] - AT[index];
                    WT[index] = TAT[index] - BT[index];

                    isCompleted[index] = true;
                    completed++;
                }
            }
        }

        displayData();
    }

    // non-preemptive priority scheduling
    void NPPriorityScheduling()
    {
        for (int i = 0; i < p; i++)
        {
            isCompleted[i] = false;
        }

        int time = 0, completed = 0;
        while (completed < p)
        {
            int index = -1;
            int highestPriority = INT_MAX;

            for (int i = 0; i < p; i++)
            {
                if (AT[i] <= time && !isCompleted[i] && priority[i] < highestPriority)
                {
                    highestPriority = priority[i];
                    index = i;
                }
            }

            if (index == -1)
            {
                time++;
            }
            else
            {
                time += BT[index];
                CT[index] = time;
                TAT[index] = CT[index] - AT[index];
                WT[index] = TAT[index] - BT[index];

                isCompleted[index] = true;
                completed++;
            }
        }
        displayData();
    }

    // preemptive priority scheduling
    void PPriorityScheduling()
    {
        for (int i = 0; i < p; i++)
        {
            RT[i] = BT[i]; // Remaining Time
            isCompleted[i] = false;
        }

        int time = 0, completed = 0;

        while (completed < p)
        {
            int index = -1;
            int highestPriority = INT_MAX;

            // Find process with highest priority
            for (int i = 0; i < p; i++)
            {
                if (AT[i] <= time && !isCompleted[i] && priority[i] < highestPriority)
                {
                    highestPriority = priority[i];
                    index = i;
                }
            }

            // CPU idle
            if (index == -1)
            {
                time++;
            }
            else
            {
                RT[index]--; // execute for 1 unit
                time++;

                if (RT[index] == 0)
                {
                    CT[index] = time;
                    TAT[index] = CT[index] - AT[index];
                    WT[index] = TAT[index] - BT[index];

                    isCompleted[index] = true;
                    completed++;
                }
            }
        }

        displayData();
    }

    // Round Robin scheduling
    void RoundRobinScheduling()
    {
            int timeQuantum;
            cout << "Enter Time Quantum: ";
            cin >> timeQuantum;
    
            for (int i = 0; i < p; i++)
            {
                RT[i] = BT[i]; // Remaining Time
                isCompleted[i] = false;
            }
    
            int time = 0, completed = 0;
    
            while (completed < p)
            {
                bool allIdle = true;
    
                for (int i = 0; i < p; i++)
                {
                    if (AT[i] <= time && !isCompleted[i])
                    {
                        allIdle = false;
    
                        if (RT[i] > timeQuantum)
                        {
                            RT[i] -= timeQuantum;
                            time += timeQuantum;
                        }
                        else
                        {
                            time += RT[i];
                            RT[i] = 0;
                            CT[i] = time;
                            TAT[i] = CT[i] - AT[i];
                            WT[i] = TAT[i] - BT[i];
    
                            isCompleted[i] = true;
                            completed++;
                        }
                    }
                }
    
                // If all processes are idle, move to the next time unit
                if (allIdle)
                {
                    time++;
                }
            }
    
            displayData();
    }
};

int main()
{
    int choice, subChoice;

    while (true)
    {
        cout << "\n===== CPU Scheduling Menu =====\n";
        cout << "1. FCFS (First Come First Serve)\n";
        cout << "2. SJF (Shortest Job First)\n";
        cout << "3. Round Robin\n";
        cout << "4. Priority\n";
        cout << "5. Exit\n";
        cout << "Enter your choice: ";
        cin >> choice;

        if (choice == 5)
        {
            cout << "Exiting program...\n";
            break;
        }

        process p;
        p.inputData();

        switch (choice)
        {
        case 1: // FCFS
            cout << "\n--- FCFS Scheduling ---\n";
            p.FCFS();
            break;

        case 2: // SJF
            cout << "\n--- SJF Scheduling ---\n";
            cout << "1. Non-Preemptive SJF\n";
            cout << "2. Preemptive SJF\n";
            cout << "Enter your choice: ";
            cin >> subChoice;

            if (subChoice == 1)
                p.NPSJFScheduling();
            else if (subChoice == 2)
                p.PSJFScheduling();
            else
                cout << "Invalid choice!\n";
            break;

        case 3: // Round Robin
            cout << "\n--- Round Robin Scheduling ---\n";
            p.RoundRobinScheduling();
            break;

        case 4: // Priority
            cout << "\n--- Priority Scheduling ---\n";
            cout << "1. Non-Preemptive Priority\n";
            cout << "2. Preemptive Priority\n";
            cout << "Enter your choice: ";
            cin >> subChoice;

            if (subChoice == 1)
                p.NPPriorityScheduling();
            else if (subChoice == 2)
                p.PPriorityScheduling();
            else
                cout << "Invalid choice!\n";
            break;

        default:
            cout << "Invalid choice! Try again.\n";
        }
    }

    return 0;
}
