#include <iostream>
#include <vector>
#include <climits>
using namespace std;



// Function to calculate the total time required to complete all tasks in a given schedule. It is the time when the last task finishes.
int time(const vector<int>& startTimes, const vector<int>& durations, int n)
{
    int maxTime = 0;
    for (int i = 0; i < n; i++)
    {
        maxTime = max(maxTime, startTimes[i] + durations[i]);
    }
    return maxTime;
}

// Function to check if the current schedule is valid
bool isValid(const vector<int>& startTimes, const vector<int>& durations, const vector<vector<int>>& dependencies, const vector<int>& machines, int task)
{
    // Check dependency constraints
    for (int i = 0; i < dependencies[task].size(); i++) 
    {
        int dependency = dependencies[task][i];
        if (startTimes[dependency] + durations[dependency] > startTimes[task]) 
        {
            return false; // Dependency not met
        }
    }

    // Check for overlap on the same machine
    for (int i = 0; i < task; i++) 
    {
        if (machines[i] == machines[task] && !(startTimes[i] + durations[i] <= startTimes[task] || startTimes[task] + durations[task] <= startTimes[i]))
        {
            return false; // Overlap on the same machine
        }
    }

    return true;
}

vector<int> bestStartTimes;
vector<int> bestMachines;
int calcTime = INT_MAX;

// Backtracking function to explore all possible schedules
void backtrack(vector<int>& startTimes, vector<int>& machines, const vector<int>& durations, const vector<vector<int>>& dependencies, int task, int n) 
{
    if (task == n) 
    {
        // All tasks are scheduled, calculate the time requried to finish the taskes 
        int BestT = time(startTimes, durations, n);
        if (BestT < calcTime)
        {
            calcTime = BestT;
            bestStartTimes = startTimes;
            bestMachines = machines;
        }
        return;
    }

    // Try all possible start times and machine assignments for the current task
    for (int startTime = 0; startTime <= calcTime; startTime++) 
    {
        for (int machine = 1; machine <= 2; machine++) { // Explicitly test both machines
            startTimes[task] = startTime;
            machines[task] = machine;

            if (isValid(startTimes, durations, dependencies, machines, task))
            {
                backtrack(startTimes, machines, durations, dependencies, task + 1, n);
            }
        }
    }
    calcTime;
}

int main() 
{
    
    const int n = 4;
    const vector<int> durations = {2, 6, 3, 2};
    const vector<vector<int>> dependencies = { {}, {0}, {1}, {1} };
    vector<int> startTimes(n, 0); // Start times for tasks
    vector<int> machines(n, 0);   // Machine assignments for tasks
    calcTime = INT_MAX;
    backtrack(startTimes, machines, durations, dependencies, 0, n);
    cout << "Minimum time : " << calcTime << " hours" << endl;
    cout << "Task Schedule (Start and End Times):" << endl;
    for (int i = 0; i < n; i++) {
        int endTime = bestStartTimes[i] + durations[i];
        cout << "Task " << i + 1 << ": Machine " << bestMachines[i]
             << " | Start: " << bestStartTimes[i] << " | End: " << endTime << endl;
    }

    return 0;
}
