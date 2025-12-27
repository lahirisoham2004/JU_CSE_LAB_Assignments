#include <stdio.h>
#include <stdlib.h>

#define MAX_JOBS 20
#define TIME_SLICE 25  // Time slice for Round Robin

// Job structure to store details
struct Job {
    int id;
    int arrival_time;
    int cpu_bursts[10]; // Assuming max 10 CPU/IO bursts
    int burst_count;
    int waiting_time;
    int turnaround_time;
    int remaining_time; // For SRTN and RR
    int completion_time;
};

// Function prototypes
void fcfs(struct Job jobs[], int n);
void srtn(struct Job jobs[], int n);
void round_robin(struct Job jobs[], int n, int time_slice);
void calculate_metrics(struct Job jobs[], int n);
void reset_metrics(struct Job jobs[], int n);

// Main function
int main() {
    struct Job jobs[MAX_JOBS];
    int n = 0;

    // Read job profiles from file
    FILE *file = fopen("job_profiles.txt", "r");
    if (!file) {
        printf("Error opening file!\n");
        return 1;
    }

    // Parse the input file
    while (fscanf(file, "%d", &jobs[n].id) != EOF) {
        fscanf(file, "%d", &jobs[n].arrival_time);
        jobs[n].burst_count = 0;
        jobs[n].remaining_time = 0;
        while (1) {
            int burst;
            fscanf(file, "%d", &burst);
            if (burst == -1) break;
            jobs[n].cpu_bursts[jobs[n].burst_count++] = burst;
            jobs[n].remaining_time += burst;  // Total CPU bursts are used as remaining time
        }
        n++;
    }
    fclose(file);

    // Run each scheduling algorithm
    printf("\n--- Running FCFS ---\n");
    fcfs(jobs, n);
    calculate_metrics(jobs, n);

    printf("\n--- Running SRTN ---\n");
    reset_metrics(jobs, n);
    srtn(jobs, n);
    calculate_metrics(jobs, n);

    printf("\n--- Running Round Robin ---\n");
    reset_metrics(jobs, n);
    round_robin(jobs, n, TIME_SLICE);
    calculate_metrics(jobs, n);

    return 0;
}

// First Come First Serve (FCFS) implementation
void fcfs(struct Job jobs[], int n) {
    int current_time = 0;
    
    for (int i = 0; i < n; i++) {
        if (jobs[i].arrival_time > current_time) {
            current_time = jobs[i].arrival_time;
        }
        jobs[i].waiting_time = current_time - jobs[i].arrival_time;
        jobs[i].turnaround_time = jobs[i].waiting_time + jobs[i].remaining_time; // Corrected
        current_time += jobs[i].remaining_time;
        jobs[i].completion_time = current_time;
    }
}

// Shortest Remaining Time Next (SRTN) implementation
void srtn(struct Job jobs[], int n) {
    int current_time = 0, completed = 0;
    int min_remaining_time = 100000, shortest = -1;
    int finish_time;
    int check = 0;

    while (completed != n) {
        for (int i = 0; i < n; i++) {
            if (jobs[i].arrival_time <= current_time && jobs[i].remaining_time > 0 && jobs[i].remaining_time < min_remaining_time) {
                min_remaining_time = jobs[i].remaining_time;
                shortest = i;
                check = 1;
            }
        }

        if (check == 0) {
            current_time++;
            continue;
        }

        jobs[shortest].remaining_time--;

        min_remaining_time = jobs[shortest].remaining_time;
        if (min_remaining_time == 0) {
            min_remaining_time = 100000;
        }

        if (jobs[shortest].remaining_time == 0) {
            completed++;
            check = 0;
            finish_time = current_time + 1;

            // Corrected waiting time calculation
            jobs[shortest].waiting_time = finish_time - jobs[shortest].arrival_time - jobs[shortest].cpu_bursts[jobs[shortest].burst_count - 1];
            jobs[shortest].turnaround_time = finish_time - jobs[shortest].arrival_time;
            jobs[shortest].completion_time = finish_time;

            if (jobs[shortest].waiting_time < 0) {
                jobs[shortest].waiting_time = 0;
            }
        }
        current_time++;
    }
}

// Round Robin implementation
void round_robin(struct Job jobs[], int n, int time_slice) {
    int current_time = 0;
    int completed = 0;
    int rem_burst[MAX_JOBS];

    for (int i = 0; i < n; i++) {
        rem_burst[i] = jobs[i].remaining_time;
    }

    while (completed != n) {
        int done = 1; // Check if all jobs are completed
        for (int i = 0; i < n; i++) {
            if (jobs[i].arrival_time <= current_time && rem_burst[i] > 0) {
                done = 0; // There are jobs that need processing
                if (rem_burst[i] > time_slice) {
                    current_time += time_slice;
                    rem_burst[i] -= time_slice;
                } else {
                    current_time += rem_burst[i];
                    jobs[i].completion_time = current_time;
                    jobs[i].turnaround_time = jobs[i].completion_time - jobs[i].arrival_time;
                    jobs[i].waiting_time = jobs[i].turnaround_time - jobs[i].remaining_time;
                    rem_burst[i] = 0;
                    completed++;
                }
            }
        }
        if (done) break; // Break if all jobs are completed
    }
}

// Reset waiting and turnaround times before running a new algorithm
void reset_metrics(struct Job jobs[], int n) {
    for (int i = 0; i < n; i++) {
        jobs[i].waiting_time = 0;
        jobs[i].turnaround_time = 0;
        jobs[i].completion_time = 0;
        jobs[i].remaining_time = 0;
        for (int j = 0; j < jobs[i].burst_count; j++) {
            jobs[i].remaining_time += jobs[i].cpu_bursts[j];
        }
    }
}

// Calculate metrics (waiting time and turnaround time)
void calculate_metrics(struct Job jobs[], int n) {
    int total_waiting_time = 0, total_turnaround_time = 0;
    
    for (int i = 0; i < n; i++) {
        total_waiting_time += jobs[i].waiting_time;
        total_turnaround_time += jobs[i].turnaround_time;
        printf("Job %d: Waiting Time = %d, Turnaround Time = %d\n", jobs[i].id, jobs[i].waiting_time, jobs[i].turnaround_time);
    }

    printf("Average Waiting Time: %.2f\n", (float)total_waiting_time / n);
    printf("Average Turnaround Time: %.2f\n", (float)total_turnaround_time / n);
}
