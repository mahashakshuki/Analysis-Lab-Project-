#include <iostream>
using namespace std;

// Function to schedule tasks
void scheduleTasks(int tasks[], int prerequisites[], int numTasks)
{   
    // Completion tracker
    int completed[4] = {0, 0, 0, 0};
    // Machine availability times
    int machine1_end_time = 0;
    int machine2_end_time = 0; 
    // Task counters for load balancing
    int machine1_task_count = 0;
    int machine2_task_count = 0; 

    for (int i = 0; i < numTasks; i++) 
    {
        // Wait for prerequisite to complete
        if (prerequisites[i] != -1)
        {
            while (!completed[prerequisites[i]]);
        }

        // Assign task based on availability and load balancing
        int start_time, end_time;
        if (machine1_end_time < machine2_end_time || (machine1_end_time == machine2_end_time && machine1_task_count <= machine2_task_count)) 
        {
            // Assign to Machine 1
            start_time = machine1_end_time;
            end_time = start_time + tasks[i];
            machine1_end_time = end_time;
            machine1_task_count++;
            cout << "Task " << i + 1 << " assigned to Machine 1 (Start: " 
                 << start_time << ", End: " << end_time << ")\n";
        } else {
            // Assign to Machine 2
            start_time = machine2_end_time;
            end_time = start_time + tasks[i];
            machine2_end_time = end_time;
            machine2_task_count++;
            cout << "Task " << i + 1 << " assigned to Machine 2 (Start: " 
                 << start_time << ", End: " << end_time << ")\n";
        }

        // Mark task as completed
        completed[i] = 1;
    }

   // int min_completion_time = max(machine1_end_time, machine2_end_time);
   // cout << "Minimum completion time: " << min_completion_time << endl;
}

int main() {
    // Task durations and prerequisites
    int tasks[4] = {2, 6, 3, 2};
    int prerequisites[4] = {-1, 0, 1, 2};
    int numTasks = 4;

    // Call the scheduling function
    scheduleTasks(tasks, prerequisites, numTasks);

    return 0;
}
