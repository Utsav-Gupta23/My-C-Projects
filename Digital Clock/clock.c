#include <stdio.h>
#include <time.h>
#include <unistd.h>  // For sleep()
#include <stdlib.h>  // For system()

void display_clock(int);
void fill_time(char*, int, int);
void fill_date(char*, int);

int main() {
    int ch;

    printf("Choose the Time Format\n");
    printf("1. 24-Hour Format\n2. 12-Hour Format\n");
    printf("Enter Choice: ");
    scanf("%d", &ch);

    if (ch != 1 && ch != 2) {
        printf("Invalid choice! Defaulting to 12-Hour Format...\n");
        ch = 2;
        sleep(2);
    }

    display_clock(ch);
    return 0;
}

// Function to keep updating and displaying the clock
void display_clock(int format) {
    char time_buffer[50];
    char date_buffer[50];

    while (1) {
        fill_time(time_buffer, sizeof(time_buffer), format);
        fill_date(date_buffer, sizeof(date_buffer));

        // Clear the screen
        printf("\033[H\033[J");

        // Display digital clock
        
        printf("*    Digital Clock      *\n");
        printf("**************************\n");
        printf("Date: %s\n", date_buffer);
        printf("Time: %s\n", time_buffer);
        printf("**************************\n");

        sleep(1); // Refresh every second
    }
}

// Function to fill time in the buffer
void fill_time(char* buffer, int size, int choice) {
    time_t raw_time;
    struct tm *current_time;

    time(&raw_time);
    current_time = localtime(&raw_time);

    if (choice == 1) {
        strftime(buffer, size, "%H:%M:%S", current_time);  // 24-hour format
    } else {
        strftime(buffer, size, "%I:%M:%S %p", current_time); // 12-hour format
    }
}

// Function to fill date in the buffer
void fill_date(char* buffer, int size) {
    time_t raw_time;
    struct tm *current_date;

    time(&raw_time);
    current_date = localtime(&raw_time);

    strftime(buffer, size, "%A | %b | %d | %Y", current_date); 
}
