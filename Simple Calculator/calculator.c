#include <stdio.h>
#include <math.h>
#include <ctype.h>

int main() {
    int ch;
    char check;

    printf("\n--------------------------------\n");
    printf("Welcome to Simple Calculator\n");

    do {
        printf("\n--------------------------------\n");
        printf("Choose one of the following options: ");
        printf("\n1. Add\n2. Subtract\n3. Multiply\n4. Divide\n5. Modulus\n6. Power\n7. Exit");
        printf("\nNow, Enter Your Choice: ");
        scanf("%d", &ch);

        if (ch == 7) {
            printf("\nExited Successfully!!\n");
            break;  // Exit the loop immediately if the user chooses "Exit"
        }

        float a, b;  // Declare variables once
        printf("Enter number 1: ");
        scanf("%f", &a);
        printf("Enter number 2: ");
        scanf("%f", &b);

        switch (ch) {
            case 1:
                printf("The addition of %.4f and %.4f is: %.2f\n", a, b, a + b);
                break;
            case 2:
                printf("The Subtraction of %.4f and %.4f is: %.2f\n", a, b, a - b);
                break;
            case 3:
                printf("The Multiplication of %.4f x %.4f is: %.6f\n", a, b, a * b);
                break;
            case 4:
                if (b == 0) {
                    printf("Error: Division by zero is not allowed.\n");
                } else {
                    printf("The Division of %.4f / %.4f is: %.6f\n", a, b, a / b);
                }
                break;
            case 5:
                if (b != 0) {
                    printf("The Modulo of %.4f and %.4f is: %.4f\n", a, b, fmod(a, b));
                } else {
                    printf("Error: Modulus with zero is not allowed.\n");
                }
                break;
            case 6:
                printf("The power of %.4f ^ %.4f is: %.4f\n", a, b, pow(a, b));
                break;
            default:
                printf("Invalid choice! Please enter a number between 1 and 7.\n");
                break;
        }
        printf("\n--------------------------------\n");
        printf("Do you want to continue? [y/n]: ");
        scanf(" %c", &check);
        check = tolower(check);

    } while (check == 'y');

    printf("\nThanks for using the calculator!\n");
    printf("Developed By: Utsav Gupta\n");

    return 0;
}
