#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <unistd.h>


#define GREEN "\033[1;32m"
#define BLUE "\033[1;34m"
#define YELLOW "\033[1;33m"
#define RED "\033[1;31m"
#define BRIGHT_GREEN "\033[0;92m"
#define MAGENTA "\033[1;35m"
#define RESET "\033[0m"
#define MAX_SIZE 50
#define BACKUP_FILENAME "backup.dat"
#define PASSWORD_FILE "password.dat"
#define DEFAULT_PASSWORD "admin123"
#define MAX_CONTACTS 100

typedef struct {
    char name[50];
    char email[30];
    char phone[15];
} Contact;


Contact contacts[MAX_CONTACTS];  // Global array
int contactCount = 0;  

int isDuplicatePhone(Contact contacts[], int contactCount, char phone[]);
void backupContacts();
void restoreBackup();
int authenticate();
void addContact();
void changePassword();
void viewContacts();
void searchContacts();
void updateContacts();
void deleteContacts();
void displayMenu();
int loadContacts(Contact contacts[], int *count);
void mergeSort(Contact contacts[], int left, int right, int sortBy);
void merge(Contact contacts[], int left, int mid, int right, int sortBy);


int main() {
    int choice;
    do {
        displayMenu();
        printf("Enter your choice: ");
        scanf("%d", &choice);
        while (getchar() != '\n'); // Clear input buffer

        switch (choice) {
            case 1:
                addContact();
                break;
            case 2:
                if(authenticate()) {
                    viewContacts();
                  }
                break;
            case 3:
                searchContacts();
                break;
            case 4:
                updateContacts();
                break;
            case 5:
                deleteContacts();
                break;
            case 6:
                changePassword();
                break;
            case 7:
                printf("Exiting...\n");
            default:
                printf("Invalid choice! Please try again.\n");
        }
    } while (choice != 7);
    return 0;
}

void displayMenu() {
    printf("\n========================================\n");
    printf("       " GREEN "CONTACT MANAGEMENT SYSTEM" RESET "\n");
    printf("========================================\n");
    printf(""RED"1."RESET" " BRIGHT_GREEN "Contact"RESET "\n");
    printf(""RED"2."RESET" " BRIGHT_GREEN "View Contacts"RESET "\n");
    printf(""RED"3."RESET" " BRIGHT_GREEN "Search Contact"RESET "\n");
    printf(""RED"4."RESET" " BRIGHT_GREEN "Update Contact"RESET "\n");
    printf(""RED"5."RESET" " BRIGHT_GREEN "Delete Contact"RESET "\n");
    printf(""RED"6."RESET" " BRIGHT_GREEN "Change Password"RESET "\n");
    printf(""RED"7."RESET" " BRIGHT_GREEN "Exit\n"RESET "");
    printf("========================================\n");
}

void addContact() {  // No need to pass contactCount
  FILE *file = fopen("contacts.dat", "ab");
  if (file == NULL) {
      printf(RED "Error opening file!\n" RESET);
      return;
  }
  int contactCount;

  int loaded = loadContacts(contacts, &contactCount);

  if (contactCount >= MAX_CONTACTS) {
      printf("Contact list is full. Cannot add more contacts.\n");
      fclose(file);
      return;
  }

  Contact c;
  printf("\nEnter Name: ");
  fgets(c.name, sizeof(c.name), stdin);
  c.name[strcspn(c.name, "\n")] = '\0';

  printf("Enter Phone Number: ");
  fgets(c.phone, sizeof(c.phone), stdin);
  c.phone[strcspn(c.phone, "\n")] = '\0';

  if (isDuplicatePhone(contacts, loaded, c.phone) == 1) {
    printf(""RED"Phone number already exists!"RESET"\n");
    fclose(file);
    return;
  }

  printf("Enter Email: ");
  fgets(c.email, sizeof(c.email), stdin);
  c.email[strcspn(c.email, "\n")] = '\0';

  if (isDuplicatePhone(contacts, loaded, c.email) == 1) {
    printf(""RED"Email already exists!"RESET"\n");
    fclose(file);
    return;
  }

  fwrite(&c, sizeof(Contact), 1, file);
  fclose(file);
  contactCount++;  // 🔥 Directly increment global variable
  printf("\n"MAGENTA"Contact added successfully!"RESET"\n");
}

void viewContacts() {
  Contact contacts[MAX_SIZE];
  int count;

  // Load contacts from file/storage
  int loaded = loadContacts(contacts, &count);
  if (!loaded || count == 0) {
      printf("No contacts found!\n");
      return;
  }
  int sortBy;
  printf("\nSort by: 1. Name  2. Phone  3. Email\n");

  while (1) {  // Infinite loop until valid input is received
    printf("Enter sorting choice (1-3): ");
    if (scanf("%d", &sortBy) == 1 && (sortBy >= 1 && sortBy <= 3)) {
        break; // Valid input, exit loop
    } else {
        printf(""RED"Invalid input! Please enter a number between 1 and 3."RESET"\n");
        while (getchar() != '\n'); // Clear input buffer
    }
  }
  // Sort only if more than one contact exists
  if (count > 1) {
      mergeSort(contacts, 0, count - 1, sortBy);
  }

  // Display sorted contacts
  printf("\n=======================================================================\n");
  printf("                           SAVED CONTACTS (Sorted)\n");
  printf("=======================================================================\n");
  printf("No   | Name                | Phone       | Email\n");
  printf("-----------------------------------------------------------------------\n");

  for (int i = 0; i < count; i++) {
      printf(" " RED "%-4d" RESET " | " GREEN "%-20s" RESET " | " BLUE "%s" RESET " | " YELLOW "%-50s" RESET "\n", i + 1, contacts[i].name, contacts[i].phone, contacts[i].email);
  }

  printf("=======================================================================\n");
}

void searchContacts() {
    Contact contacts[MAX_SIZE];
    int count;
    char search[50];
    int found = 0;

    // Load contacts
    int loaded = loadContacts(contacts, &count);
    if (!loaded) {
        printf(RED "No contacts found!\n" RESET);
        return;
    }

    printf("\nEnter Name / Phone / Email to search: ");
    fgets(search, sizeof(search), stdin);
    search[strcspn(search, "\n")] = '\0'; // Remove newline character

    printf("\n=======================================================================\n");
    printf("                           " GREEN "SEARCH RESULTS" RESET "\n");
    printf("=======================================================================\n");
    printf("No   | Name              | Phone       | Email\n");
    printf("-----------------------------------------------------------------------\n");

    // Convert search query to lowercase
    for (int i = 0; search[i]; i++) {
        search[i] = tolower(search[i]);
    }

    // Check for partial matches in name, phone, or email
    for (int i = 0; i < count; i++) {
        char nameLower[50], phoneLower[20], emailLower[50];

        // Convert contact fields to lowercase for case-insensitive search
        strcpy(nameLower, contacts[i].name);
        strcpy(phoneLower, contacts[i].phone);
        strcpy(emailLower, contacts[i].email);

        for (int j = 0; nameLower[j]; j++) nameLower[j] = tolower(nameLower[j]);
        for (int j = 0; phoneLower[j]; j++) phoneLower[j] = tolower(phoneLower[j]);
        for (int j = 0; emailLower[j]; j++) emailLower[j] = tolower(emailLower[j]);

        // Check for partial match
        if (strstr(nameLower, search) || strstr(phoneLower, search) || strstr(emailLower, search)) {
            printf("%-4d | " GREEN "%-20s" RESET " | " BLUE "%s" RESET " | " YELLOW "%-50s" RESET "\n",
                   i + 1, contacts[i].name, contacts[i].phone, contacts[i].email);
            found = 1;
        }
    }

    if (!found) {
        printf(RED "No matching contacts found!\n" RESET);
    }

    printf("=======================================================================\n");
}

void updateContacts() {
    FILE *file = fopen("contacts.dat", "r+b");
    if (file == NULL) {
        printf("No contacts found!\n");
        return;
    }

    Contact c;
    char search[50];
    int found = 0;

    printf("\nEnter Name or Phone or Email to update: ");
    fgets(search, sizeof(search), stdin);
    search[strcspn(search, "\n")] = '\0';

    while (fread(&c, sizeof(Contact), 1, file) == 1) {
        if (strcmp(search, c.name) == 0 || strcmp(search, c.phone) == 0 || strcmp(search, c.email) == 0) {
            printf("\nEnter New Name: ");
            fgets(c.name, sizeof(c.name), stdin);
            c.name[strcspn(c.name, "\n")] = '\0';

            printf("Enter New Phone: ");
            fgets(c.phone, sizeof(c.phone), stdin);
            c.phone[strcspn(c.phone, "\n")] = '\0';

            printf("Enter New Email: ");
            fgets(c.email, sizeof(c.email), stdin);
            c.email[strcspn(c.email, "\n")] = '\0';

            fseek(file, -sizeof(Contact), SEEK_CUR);
            fwrite(&c, sizeof(Contact), 1, file);
            found = 1;
            printf("\nContact updated successfully!\n");
            break;
        }
    }

    fclose(file);
    if (!found) {
        printf("Contact not found!\n");
    }
}

void deleteContacts() {

  FILE *file = fopen("contacts.dat", "rb");
  if (file == NULL) {
      printf(RED "No contacts found!\n" RESET);
      return;
  }

  FILE *temp = fopen("temp.dat", "wb");
  if (temp == NULL) {
      printf(RED "Error opening file!\n" RESET);
      fclose(file);
      return;
  }

  Contact c;
  char search[50];
  int found = 0;

  if (authenticate()) {
      viewContacts();
  } else {
      fclose(file);
      fclose(temp);
      remove("temp.dat");
      return;
  }

  while (getchar() != '\n'); // Clear input buffer

  printf(YELLOW "\nEnter Name or Phone to delete: " RESET);
  fgets(search, sizeof(search), stdin);
  search[strcspn(search, "\n")] = '\0';

  while (fread(&c, sizeof(Contact), 1, file) == 1) {
      if (strcmp(search, c.name) != 0 && strcmp(search, c.phone) != 0) {
          fwrite(&c, sizeof(Contact), 1, temp);
      } else {
          found = 1;
      }
  }

  fclose(file);
  fclose(temp);

  if (found) {
      remove("contacts.dat");
      rename("temp.dat", "contacts.dat");
      printf(GREEN "\nContact deleted successfully!\n" RESET);
  } else {
      remove("temp.dat");
      printf(RED "\nContact not found!\n" RESET);
  }
}

int loadContacts(Contact contacts[], int *count) {
    FILE *file = fopen("contacts.dat", "rb");
    if (file == NULL) {
        printf("No contacts found.\n");
        *count = 0;
        return 0;
    }

    int i = 0;
    while (i < MAX_SIZE && fread(&contacts[i], sizeof(Contact), 1, file) == 1) {
        i++;
    }
    fclose(file);

    *count = i;
    return i;
}

void mergeSort(Contact contacts[], int left, int right, int sortBy) {
  if(left < right) {
    int mid = left + (right - left) / 2;
        mergeSort(contacts, left, mid, sortBy);
        mergeSort(contacts, mid + 1, right, sortBy);
        merge(contacts, left, mid, right, sortBy);
  }

}

void merge(Contact contacts[], int left, int mid, int right, int sortBy) {
  int n1 = mid - left + 1;
  int n2 = right - mid;

  Contact leftArr[n1], rightArr[n2];

  // Copy data to temp arrays
  for (int i = 0; i < n1; i++)
      leftArr[i] = contacts[left + i];
  for (int j = 0; j < n2; j++)
      rightArr[j] = contacts[mid + 1 + j];

  int i = 0, j = 0, k = left;

  while (i < n1 && j < n2) {
      int cmp = 0;
      
      // Compare based on user choice (sortBy: 1 = Name, 2 = Phone, 3 = Email)
      if (sortBy == 1)
          cmp = strcmp(leftArr[i].name, rightArr[j].name);
      else if (sortBy == 2)
          cmp = strcmp(leftArr[i].phone, rightArr[j].phone);
      else if (sortBy == 3)
          cmp = strcmp(leftArr[i].email, rightArr[j].email);

      if (cmp <= 0) {
          contacts[k] = leftArr[i];
          i++;
      } else {
          contacts[k] = rightArr[j];
          j++;
      }
      k++;
  }

  // Copy remaining elements of leftArr[]
  while (i < n1) {
      contacts[k] = leftArr[i];
      i++;
      k++;
  }

  // Copy remaining elements of rightArr[]
  while (j < n2) {
      contacts[k] = rightArr[j];
      j++;
      k++;
  }
}

int authenticate() {
  char password[20], storedPassword[20];
  FILE *file = fopen(PASSWORD_FILE, "r");
  
  // If password file doesn't exist, create it with default password
  if (!file) {
      file = fopen(PASSWORD_FILE, "w");
      fprintf(file, "%s", DEFAULT_PASSWORD);
      fclose(file);
      strcpy(storedPassword, DEFAULT_PASSWORD);
  } else {
      fscanf(file, "%s", storedPassword);
      fclose(file);
  }

  // Authentication with 3 attempts
  for (int attempt = 1; attempt <= 3; attempt++) {
      printf("Enter Password to Access Contacts: ");
      scanf("%s", password);  // Read input

      if (strcmp(password, storedPassword) == 0) {
          printf(BRIGHT_GREEN "Access Granted\n" RESET);
          return 1;
      } else {
          printf(RED "Incorrect Password! %d Tries Left\n" RESET, 3 - attempt);
      }

      if (attempt == 3) {
          printf(RED "Too Many Incorrect Attempts! Exiting...\n" RESET);
          return 0;  // Authentication failed
      }
  }
  return 0;
}

void backupContacts() {
  FILE *source = fopen("contacts.dat", "rb");
  FILE *backup = fopen(BACKUP_FILENAME, "wb");
  if (source && backup) {
      Contact contact;
      while (fread(&contact, sizeof(Contact), 1, source)) {
          fwrite(&contact, sizeof(Contact), 1, backup);
      }
      printf(""BRIGHT_GREEN "Backup created successfully."RESET"\n");
  }
  fclose(source);
  fclose(backup);
}

void restoreBackup() {
  FILE *mainFile = fopen("contacts.dat", "rb");
  if (!mainFile) {
      FILE *backup = fopen(BACKUP_FILENAME, "rb");
      if (backup) {
          FILE *restoreFile = fopen("contact.dat", "wb");
          Contact contact;
          while (fread(&contact, sizeof(Contact), 1, backup)) {
              fwrite(&contact, sizeof(Contact), 1, restoreFile);
          }
          fclose(restoreFile);
          printf("Backup restored successfully.\n");
      }
      fclose(backup);
  } else {
      fclose(mainFile);
  }
}

void changePassword() {
  char oldPassword[20], newPassword[20];
  
  printf("Enter current password: ");
  scanf("%s", oldPassword);
  
  FILE *file = fopen(PASSWORD_FILE, "r");
  if (!file) {
      printf(RED "Password file missing! Can't change password.\n" RESET);
      return;
  }

  char storedPassword[20];
  fscanf(file, "%s", storedPassword);
  fclose(file);

  if (strcmp(oldPassword, storedPassword) != 0) {
      printf(RED "Incorrect current password!\n" RESET);
      return;
  }

  printf("Enter new password: ");
  scanf("%s", newPassword);
  
  file = fopen(PASSWORD_FILE, "w");  // Overwrite with new password
  if (!file) {
      printf(RED "Error updating password!\n" RESET);
      return;
  }

  fprintf(file, "%s", newPassword);
  fclose(file);
  printf(BRIGHT_GREEN "Password changed successfully!\n" RESET);
}

int isDuplicatePhone(Contact contacts[], int contactCount, char phone[]) {
  for (int i = 0; i < contactCount; i++) {
      if (strcmp(contacts[i].phone, phone) == 0) {
          return 1; // Duplicate found
      }
  }
  return 0; // No duplicate found
}


// PASSWORD -> user123