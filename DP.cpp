#include <iostream>
#include <vector>
#include <algorithm>
#include <climits>

using namespace std;

struct Task
{
    int duration;
    vector<int> dependencies; // List of task indices this task depends on
};

// Function to calculate earliest start time for a task based on dependencies
int CET(int taskIndex, const vector<Task>& tasks, const vector<vector<int>>& dp, int numMachines)
{
    int earliestStart = 0;

    for (int j = 0; j < tasks[taskIndex].dependencies.size(); j++)
    {
        int dep = tasks[taskIndex].dependencies[j];
        int depCompletionTime = INT_MAX;
        for (int m = 0; m < numMachines; m++)
        {
            depCompletionTime = min(depCompletionTime, dp[dep][m]);
        }
        earliestStart = max(earliestStart, depCompletionTime);
    }

    return earliestStart;
}

// Function to solve the task scheduling problem using DP
pair<int, vector<int>> TaskScheduling(const vector<Task>& tasks, int numMachines, vector<vector<int>>& dp)
{
    int numTasks = tasks.size();
    dp.assign(numTasks, vector<int>(numMachines, INT_MAX)); // Resize and initialize DP table
    vector<int> machineTime(numMachines, 0);               // Machine availability times
    vector<int> taskAssignment(numTasks, -1);             // Task to machine assignment

    // Base case: Assign the first task to all machines
    for (int m = 0; m < numMachines; m++)
    {
        dp[0][m] = tasks[0].duration;
        machineTime[m] = tasks[0].duration;
    }

    // Fill the DP table
    for (int i = 1; i < numTasks; i++)
    {
        for (int m = 0; m < numMachines; m++)
        {
            int earliestStart = CET(i, tasks, dp, numMachines);
            int startTime = max(earliestStart, machineTime[m]);
            dp[i][m] = startTime + tasks[i].duration;
            machineTime[m] = dp[i][m];
        }
    }

    // Find the minimum 
    int minimum = INT_MAX;
    for (int m = 0; m < numMachines; m++) {
        minimum = min(minimum, dp[numTasks - 1][m]);
    }

    // Assign tasks to machines
    for (int i = 0; i < numTasks; i++)
    {
        for (int m = 0; m < numMachines; m++)
        {
            if (dp[i][m] < INT_MAX) {
                if (taskAssignment[i] == -1 || dp[i][m] < dp[i][taskAssignment[i] - 1])
                {
                    taskAssignment[i] = m + 1; // Machines are 1-indexed
                }
            }
        }
    }

    return { minimum, taskAssignment };
}

// Function to print task distribution and timing
void printTaskDistribution(const vector<Task>& tasks, const vector<vector<int>>& dp, const vector<int>& taskAssignment, int numMachines)
{
    cout << "\nTask Distribution and Timing:" << endl;
    for (int i = 0; i < tasks.size(); i++) {
        int machine = taskAssignment[i] - 1;
        int endTime = dp[i][machine];
        int startTime = endTime - tasks[i].duration;
        cout << "Task " << i + 1 << " on Machine " << taskAssignment[i] << ": ";
        cout << "Start Time: " << startTime << " hours, ";
        cout << "End Time: " << endTime << " hours." << endl;
    }
}

// Main function
int main() {
    // Input: Task durations and dependencies
    vector<Task> tasks = {
        {2, {}},       // Task 1: Etch the board
        {6, {0}},      // Task 2: Solder components (depends on Task 1)
        {3, {1}},      // Task 3: Test the board (depends on Task 2)
        {2, {2}}       // Task 4: Package the board (depends on Task 3)
    };

    int numMachines = 2;

    // Create DP table
    vector<vector<int>> dp;

    // Solve the scheduling problem
    auto result = TaskScheduling(tasks, numMachines, dp);
    int minim = result.first;
    vector<int> taskAssignment = result.second;

    // Print results
    cout << "Minimum : " << minim << " hours" << endl;

    // Print task distribution and timing
    printTaskDistribution(tasks, dp, taskAssignment, numMachines);

    return 0;
}
