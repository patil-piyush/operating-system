#include <iostream>
#include <vector>
#include <queue>
#include <unordered_map>
using namespace std;

// Function to print frames
void printFrames(vector<int> &frames, int capacity)
{
    for (int i = 0; i < capacity; i++)
    {
        if (i < frames.size())
            cout << frames[i] << " ";
        else
            cout << "- ";
    }
}

// ================= FCFS =================
int fcfs(vector<int> pages, int capacity)
{
    vector<int> frames(capacity, -1);
    queue<int> q;
    int faults = 0;

    cout << "\nFCFS:\n";

    for (int page : pages)
    {
        bool hit = false;

        for (int f : frames)
            if (f == page)
                hit = true;

        if (!hit)
        {
            faults++;
            if (q.size() < capacity)
            {
                q.push(page);
                frames[q.size() - 1] = page;
            }
            else
            {
                int old = q.front();
                q.pop();

                for (int i = 0; i < capacity; i++)
                {
                    if (frames[i] == old)
                    {
                        frames[i] = page;
                        break;
                    }
                }
                q.push(page);
            }

            cout << "Page " << page << " -> ";
            printFrames(frames, capacity);
            cout << " *\n";
        }
        else
        {
            cout << "Page " << page << " -> ";
            printFrames(frames, capacity);
            cout << "\n";
        }
    }

    int total = pages.size();
    int hits = total - faults;

    cout << "Total Page Faults: " << faults << endl;
    cout << "Total Hits: " << hits << endl;
    cout << "Hit Ratio: " << (float)hits / total << endl;
    cout << "Miss Ratio: " << (float)faults / total << endl;

    return faults;
}

// ================= LRU =================
int lru(vector<int> pages, int capacity)
{
    vector<int> frames;
    unordered_map<int, int> recent;
    int faults = 0;

    cout << "\nLRU:\n";

    for (int i = 0; i < pages.size(); i++)
    {
        int page = pages[i];
        bool hit = false;

        for (int f : frames)
            if (f == page)
                hit = true;

        if (!hit)
        {
            faults++;

            if (frames.size() < capacity)
            {
                frames.push_back(page);
            }
            else
            {
                int lruPage = frames[0], minIndex = recent[lruPage];

                for (int f : frames)
                {
                    if (recent[f] < minIndex)
                    {
                        minIndex = recent[f];
                        lruPage = f;
                    }
                }

                for (int i = 0; i < capacity; i++)
                {
                    if (frames[i] == lruPage)
                    {
                        frames[i] = page;
                        break;
                    }
                }
            }

            cout << "Page " << page << " -> ";
            printFrames(frames, capacity);
            cout << " *\n";
        }
        else
        {
            cout << "Page " << page << " -> ";
            printFrames(frames, capacity);
            cout << "\n";
        }

        recent[page] = i;
    }

    int total = pages.size();
    int hits = total - faults;

    cout << "Total Page Faults: " << faults << endl;
    cout << "Total Hits: " << hits << endl;
    cout << "Hit Ratio: " << (float)hits / total << endl;
    cout << "Miss Ratio: " << (float)faults / total << endl;

    return faults;
}

// ================= OPTIMAL =================
int optimal(vector<int> pages, int capacity)
{
    vector<int> frames;
    int faults = 0;

    cout << "\nOptimal:\n";

    for (int i = 0; i < pages.size(); i++)
    {
        int page = pages[i];
        bool hit = false;

        for (int f : frames)
            if (f == page)
                hit = true;

        if (!hit)
        {
            faults++;

            if (frames.size() < capacity)
            {
                frames.push_back(page);
            }
            else
            {
                int idx = -1, farthest = i;

                for (int j = 0; j < frames.size(); j++)
                {
                    int k;
                    for (k = i + 1; k < pages.size(); k++)
                    {
                        if (frames[j] == pages[k])
                            break;
                    }

                    if (k == pages.size())
                    {
                        idx = j;
                        break;
                    }

                    if (k > farthest)
                    {
                        farthest = k;
                        idx = j;
                    }
                }
                frames[idx] = page;
            }

            cout << "Page " << page << " -> ";
            printFrames(frames, capacity);
            cout << " *\n";
        }
        else
        {
            cout << "Page " << page << " -> ";
            printFrames(frames, capacity);
            cout << "\n";
        }
    }

    int total = pages.size();
    int hits = total - faults;

    cout << "Total Page Faults: " << faults << endl;
    cout << "Total Hits: " << hits << endl;
    cout << "Hit Ratio: " << (float)hits / total << endl;
    cout << "Miss Ratio: " << (float)faults / total << endl;

    return faults;
}

// ================= MAIN =================
int main()
{
    vector<int> pages;
    int n, capacity, choice;

    cout << "Enter number of pages: ";
    cin >> n;

    cout << "Enter reference string: ";
    for (int i = 0; i < n; i++)
    {
        int x;
        cin >> x;
        pages.push_back(x);
    }

    cout << "Enter number of frames: ";
    cin >> capacity;

    do
    {
        cout << "\n===== MENU =====\n";
        cout << "1. FCFS\n";
        cout << "2. LRU\n";
        cout << "3. Optimal\n";
        cout << "4. Compare All\n";
        cout << "5. Belady's Anomaly Check\n";
        cout << "6. Exit\n";
        cout << "Enter your choice: ";
        cin >> choice;

        switch (choice)
        {
        case 1:
            fcfs(pages, capacity);
            break;

        case 2:
            lru(pages, capacity);
            break;

        case 3:
            optimal(pages, capacity);
            break;

        case 4:
        {
            int f1 = fcfs(pages, capacity);
            int f2 = lru(pages, capacity);
            int f3 = optimal(pages, capacity);

            cout << "\n--- Comparison ---\n";
            cout << "FCFS Faults: " << f1 << " | Hit Ratio: " << (float)(pages.size()-f1)/pages.size() << endl;
            cout << "LRU Faults: " << f2 << " | Hit Ratio: " << (float)(pages.size()-f2)/pages.size() << endl;
            cout << "Optimal Faults: " << f3 << " | Hit Ratio: " << (float)(pages.size()-f3)/pages.size() << endl;
            break;
        }

        case 5:
        {
            int f_small, f_large;

            cout << "Enter smaller frame size: ";
            cin >> f_small;

            cout << "Enter larger frame size: ";
            cin >> f_large;

            cout << "\nRunning FCFS for " << f_small << " frames...\n";
            int faults_small = fcfs(pages, f_small);

            cout << "\nRunning FCFS for " << f_large << " frames...\n";
            int faults_large = fcfs(pages, f_large);

            cout << "\n--- Belady's Anomaly Result ---\n";
            cout << "Faults with " << f_small << " frames: " << faults_small << endl;
            cout << "Faults with " << f_large << " frames: " << faults_large << endl;

            if (faults_large > faults_small)
                cout << "Belady's Anomaly detected!\n";
            else
                cout << "No Belady's Anomaly.\n";

            break;
        }

        case 6:
            cout << "Exiting program...\n";
            break;

        default:
            cout << "Invalid choice!\n";
        }

    } while (choice != 6);

    return 0;
}