
#include <stdio.h>
#include <time.h>
#include <ctype.h>
#include <stdlib.h>

int main() {
  int num, random, no_of_guess = 0;
  char choice;
  
  srand(time(NULL));  // Seed the random number generator
  random = rand() % 10 + 1;  // Generate a number between 1-10

  printf("----Welcome to Guess the Number Game!----\n");

  do {
    printf("\nDo you want to continue? [y/n]: ");
    scanf(" %c", &choice);
    choice = tolower(choice);

    if (choice == 'y') {
      printf("Enter your guessed number (between 1 - 10): ");
      scanf("%d", &num);

      // Validate the input range
      if (num < 1 || num > 10) {
        printf("Invalid input! Please enter a number between 1 and 10.\n");
        continue; // Skip further checks
      }

      // Count the number of attempts
      no_of_guess++;

      
      // Check if the guess is too high or low
      if (num < random) {
        printf("Your guess is too low!\n");
      } 
      else if (num > random) {
        printf("Your guess is too high!\n");
      } 
      else {
        printf("Congratulations! You guessed the number in %d attempts!\n", no_of_guess);
        break; // Exit the loop after the correct guess
      }
    }
    
  } while(choice != 'n');

  printf("\nThanks for playing!\n");
  printf("Developed By: Utsav Gupta\n");

  return 0;
}

