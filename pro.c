#include <stdio.h>
#include <stdlib.h>
#include <time.h>

typedef struct {
    int pid;
    int arrival_time;
    int burst_time;
    int remaining_time;
    int waiting_time;
    int turnaround_time;
} Process;

void generate_processes(Process processes[], int num_processes);
void round_robin(Process processes[], int num_processes, int quantum);
void calcs(Process processes[], int num_processes, float *average_waiting_time, float *average_turnaround_time);
void show_results(Process processes[], int num_processes, float average_waiting_time, float average_turnaround_time);

int main() {
    int num_processes, quantum;
    printf("Enter the number of processes: ");
    scanf("%d", &num_processes);
    printf("Enter the time quantum: ");
    scanf("%d", &quantum);

    Process processes[num_processes];
    float average_waiting_time, average_turnaround_time;

    srand(time(NULL));
    generate_processes(processes, num_processes);
    round_robin(processes, num_processes, quantum);
    calcs(processes, num_processes, &average_waiting_time, &average_turnaround_time);
    show_results(processes, num_processes, average_waiting_time, average_turnaround_time);

    return 0;
}

void generate_processes(Process processes[], int num_processes) {
    for (int i = 0; i < num_processes; i++) {
        processes[i].pid = i + 1;
        processes[i].arrival_time = rand() % 10;
        processes[i].burst_time = (rand() % 10) + 1;
        processes[i].remaining_time = processes[i].burst_time;
        processes[i].waiting_time = 0;
        processes[i].turnaround_time = 0;
    }
}

void round_robin(Process processes[], int num_processes, int quantum) {
    int time = 0;
    int completed_processes = 0;

    while (completed_processes < num_processes) {
        for (int i = 0; i < num_processes; i++) {
            if (processes[i].arrival_time <= time && processes[i].remaining_time > 0) {
                int time_slice = (processes[i].remaining_time > quantum) ? quantum : processes[i].remaining_time;
                time += time_slice;
                processes[i].remaining_time -= time_slice;

                if (processes[i].remaining_time == 0) {
                    completed_processes++;
                    processes[i].turnaround_time = time - processes[i].arrival_time;
                    processes[i].waiting_time = processes[i].turnaround_time - processes[i].burst_time;
                }
            } else {
                time++;
            }
        }
    }
}

void calcs(Process processes[], int num_processes, float *average_waiting_time, float *average_turnaround_time) {
    int total_waiting_time = 0;
    int total_turnaround_time = 0;

    for (int i = 0; i < num_processes; i++) {
        total_waiting_time += processes[i].waiting_time;
        total_turnaround_time += processes[i].turnaround_time;
    }

    *average_waiting_time = (float)total_waiting_time / num_processes;
    *average_turnaround_time = (float)total_turnaround_time / num_processes;
}

void show_results(Process processes[], int num_processes, float average_waiting_time, float average_turnaround_time) {
    printf("Process\tArrival Time\tBurst Time\tWaiting Time\tTurnaround Time\n");
    for (int i = 0; i < num_processes; i++) {
        printf("P%d\t%d\t\t%d\t\t%d\t\t%d\n", processes[i].pid, processes[i].arrival_time, processes[i].burst_time, processes[i].waiting_time, processes[i].turnaround_time);
    }
    printf("\nAverage Waiting Time: %.2f\n", average_waiting_time);
    printf("Average Turnaround Time: %.2f\n", average_turnaround_time);
}

