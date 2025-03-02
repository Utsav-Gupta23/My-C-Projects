#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <time.h>

const int BAR_LENGTH = 50;
const int MAX_TASK = 5;

typedef struct {
    int id;
    int progress;
    int step;
} Task;

void clear_screen();
void printbar(Task task);
int get_random_step();
void initialize_tasks(Task tasks[], int size);
int update_tasks(Task tasks[], int size);

int main() {
    Task tasks[MAX_TASK];

    srand(time(NULL));  // Seed for randomness
    initialize_tasks(tasks, MAX_TASK);  // Initialize tasks

    while (update_tasks(tasks, MAX_TASK)) {  // Keep updating until all tasks are complete
        sleep(1);
    }

    printf("All tasks Completed\n");
    return 0;
}

// Function to generate a random step between 0.5 and 4
int get_random_step() {
    return (int)((rand() % 5) + 1);
}

// Function to initialize all tasks
void initialize_tasks(Task tasks[], int size) {
    for (int i = 0; i < size; i++) {
        tasks[i].id = i + 1;
        tasks[i].progress = 0;
        tasks[i].step = get_random_step();  // Assign random step
    }
}

// Function to update task progress and check if tasks are complete
int update_tasks(Task tasks[], int size) {
    int tasks_incomplete = 0;  // Assume all tasks are complete

    clear_screen();

    for (int i = 0; i < size; i++) {
        if (tasks[i].progress < 100) {
            tasks[i].progress += tasks[i].step;
            if (tasks[i].progress > 100) {
                tasks[i].progress = 100;  // Prevent exceeding 100%
            }
            tasks_incomplete = 1;  // At least one task is still incomplete
        }
        printbar(tasks[i]);
    }
    return tasks_incomplete;  // Return 1 if tasks are incomplete, else 0
}

// Clear the console screen
void clear_screen() {
    #ifdef _WIN32
        system("cls");
    #else 
        system("clear");
    #endif
}

// Function to print the progress bar
void printbar(Task task) {
  
    int bars_to_show = (task.progress * BAR_LENGTH) / 100;

    printf("Task %d: [", task.id);
    for (int i = 0; i < BAR_LENGTH; i++) {
        if (i < bars_to_show) {
            printf("=");
        } else {
            printf(" ");
        }
    }
    printf("] %d%%\n", task.progress);
}
