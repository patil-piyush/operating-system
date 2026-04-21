#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>
#include <climits>
using namespace std;

//  FCFS 
void runFCFS(vector<int> requests, int currentHead) {
    int totalMovement = 0;

    cout << "\n[FCFS Disk Scheduling]\n";
    cout << "Seek Sequence: " << currentHead;

    for (int request : requests) {
        totalMovement += abs(currentHead - request);
        currentHead = request;
        cout << " -> " << currentHead;
    }

    cout << "\nTotal Head Movement = " << totalMovement << "\n";
}

//  SSTF 
void runSSTF(vector<int> requests, int currentHead) {
    int totalMovement = 0;
    vector<bool> isServed(requests.size(), false);

    cout << "\n[SSTF Disk Scheduling]\n";
    cout << "Seek Sequence: " << currentHead;

    for (int i = 0; i < requests.size(); i++) {
        int shortestDistance = INT_MAX;
        int selectedIndex = -1;

        for (int j = 0; j < requests.size(); j++) {
            if (!isServed[j]) {
                int distance = abs(currentHead - requests[j]);
                if (distance < shortestDistance) {
                    shortestDistance = distance;
                    selectedIndex = j;
                }
            }
        }

        if (selectedIndex == -1) break;

        isServed[selectedIndex] = true;
        totalMovement += shortestDistance;
        currentHead = requests[selectedIndex];
        cout << " -> " << currentHead;
    }

    cout << "\nTotal Head Movement = " << totalMovement << "\n";
}

//  SCAN 
void runSCAN(vector<int> requests, int currentHead, int diskSize) {
    int totalMovement = 0;
    vector<int> leftSide, rightSide;

    for (int request : requests) {
        if (request < currentHead)
            leftSide.push_back(request);
        else
            rightSide.push_back(request);
    }

    sort(leftSide.begin(), leftSide.end());
    sort(rightSide.begin(), rightSide.end());

    cout << "\n[SCAN Disk Scheduling - Moving RIGHT]\n";
    cout << "Seek Sequence: " << currentHead;

    // Move Right
    for (int request : rightSide) {
        totalMovement += abs(currentHead - request);
        currentHead = request;
        cout << " -> " << currentHead;
    }

    // Move to end of disk
    totalMovement += abs(currentHead - (diskSize - 1));
    currentHead = diskSize - 1;
    cout << " -> " << currentHead;

    // Reverse direction (Left)
    for (int i = leftSide.size() - 1; i >= 0; i--) {
        totalMovement += abs(currentHead - leftSide[i]);
        currentHead = leftSide[i];
        cout << " -> " << currentHead;
    }

    cout << "\nTotal Head Movement = " << totalMovement << "\n";
}

//  C-SCAN 
void runCSCAN(vector<int> requests, int currentHead, int diskSize) {
    int totalMovement = 0;
    vector<int> leftSide, rightSide;

    for (int request : requests) {
        if (request < currentHead)
            leftSide.push_back(request);
        else
            rightSide.push_back(request);
    }

    sort(leftSide.begin(), leftSide.end());
    sort(rightSide.begin(), rightSide.end());

    cout << "\n[C-SCAN Disk Scheduling - Moving RIGHT]\n";
    cout << "Seek Sequence: " << currentHead;

    // Move Right
    for (int request : rightSide) {
        totalMovement += abs(currentHead - request);
        currentHead = request;
        cout << " -> " << currentHead;
    }

    // Move to end
    totalMovement += abs(currentHead - (diskSize - 1));
    currentHead = diskSize - 1;
    cout << " -> " << currentHead;

    // Jump to beginning
    totalMovement += abs(currentHead - 0);
    currentHead = 0;
    cout << " -> " << currentHead;

    // Continue Right
    for (int request : leftSide) {
        totalMovement += abs(currentHead - request);
        currentHead = request;
        cout << " -> " << currentHead;
    }

    cout << "\nTotal Head Movement = " << totalMovement << "\n";
}

//  MAIN 
int main() {
    int numberOfRequests, initialHead, diskLimit;

    cout << "===== Disk Scheduling Simulator =====\n";

    cout << "Enter total number of disk requests: ";
    cin >> numberOfRequests;

    vector<int> requestQueue(numberOfRequests);
    cout << "Enter disk request values:\n";
    for (int i = 0; i < numberOfRequests; i++) {
        cout << "Request " << i + 1 << ": ";
        cin >> requestQueue[i];
    }

    cout << "Enter initial head position: ";
    cin >> initialHead;

    cout << "Enter total disk size: ";
    cin >> diskLimit;

    int userChoice;

    do {
        cout << "\n========== MENU ==========\n";
        cout << "1. FCFS\n";
        cout << "2. SSTF\n";
        cout << "3. SCAN\n";
        cout << "4. C-SCAN\n";
        cout << "5. Exit\n";
        cout << "Select an option: ";
        cin >> userChoice;

        switch (userChoice) {
            case 1:
                runFCFS(requestQueue, initialHead);
                break;
            case 2:
                runSSTF(requestQueue, initialHead);
                break;
            case 3:
                runSCAN(requestQueue, initialHead, diskLimit);
                break;
            case 4:
                runCSCAN(requestQueue, initialHead, diskLimit);
                break;
            case 5:
                cout << "Program terminated.\n";
                break;
            default:
                cout << "Invalid choice! Try again.\n";
        }

    } while (userChoice != 5);

    return 0;
}