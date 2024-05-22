#include <stdio.h>
#include <omp.h>
#include <dirent.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdbool.h>
#define MAX_LINE_LENGTH 1024
typedef struct {
    char* name;
    int NPM;
    int rank;
    float utsGrade;
    float uasGrade;
} Student;

void createStudent(Student* student, const char* name, int NPM, float utsGrade, float uasGrade) {
    student->name = strdup(name);
    student->NPM = NPM;
    student->rank = 0;
    student->utsGrade = utsGrade;
    student->uasGrade = uasGrade;
}

typedef struct Node {
    Student student;
    struct Node* next;
} Node;


typedef struct {
    Node* head;
    Node* tail;
} LinkedList;

void initializeList(LinkedList* list) {
    list->head = NULL;
    list->tail = NULL;
}

Node* createNode(Student student) {
    Node* newNode = (Node*)malloc(sizeof(Node));
    if (newNode == NULL) {
        printf("Failed to allocate memory for a new node.\n");
        return NULL;
    }
    newNode->student = student;
    newNode->next = NULL;
    return newNode;
}

void destroyNode(Node* node) {
    free(node->student.name);
    free(node);
}


void insertNode(LinkedList* list, Student student) {
    Node* newNode = createNode(student);
    if (newNode == NULL) {
        return;
    }
    if (list->head == NULL) {
        list->head = newNode;
        list->tail = newNode;
    } else {
        list->tail->next = newNode;
        list->tail = newNode;
    }
}

void destroyList(LinkedList* list) {
    Node* current = list->head;
    while (current != NULL) {
        Node* next = current->next;
        destroyNode(current);
        current = next;
    }
    list->head = NULL;
    list->tail = NULL;
}

void saveStudentsToFile(const char* filepath, LinkedList* list) {
    FILE* file = fopen(filepath, "a+");
    if (file == NULL) {
        printf("Failed to open the file for writing.\n");
        return;
    }
    Node* current = list->head;
    while (current != NULL) {
        fprintf(file, "%s,%d,%.2f,%.2f\n", current->student.name, current->student.NPM, current->student.utsGrade, current->student.uasGrade);
        current = current->next;
    }
    fclose(file);
}

void viewLoadedStudents(LinkedList* list) {
    // Print the loaded students
    Node* current = list->head;
    while (current != NULL) {
        printf("Name: %s, NPM: %d, UTS Grade: %.2f, UAS Grade: %.2f\n",
               current->student.name, current->student.NPM, current->student.utsGrade, current->student.uasGrade);
        current = current->next;
    }
    printf("Students loaded successfully.\n");
}

LinkedList loadStudentsFromFile(const char* filepath) {
    LinkedList list;
    initializeList(&list);
    FILE* file = fopen(filepath, "r");
    if (file == NULL) {
        printf("Failed to open the file for reading.\n");
        return list;
    }
    char line[MAX_LINE_LENGTH];
    while (fgets(line, sizeof(line), file)) {
        char* name = strtok(line, ",");
        char* npmStr = strtok(NULL, ",");
        char* utsGradeStr = strtok(NULL, ",");
        char* uasGradeStr = strtok(NULL, ",");
        if (name == NULL || npmStr == NULL || utsGradeStr == NULL || uasGradeStr == NULL) {
            printf("Invalid line format: %s\n", line);
            continue;
        }
        int npm = atoi(npmStr);
        float utsGrade = atof(utsGradeStr);
        float uasGrade = atof(uasGradeStr);
        Student student;
        createStudent(&student, name, npm, utsGrade, uasGrade);
        insertNode(&list, student);
    }
    fclose(file);
    return list;
}

void process_file(const char *filepath) {
    FILE *file = fopen(filepath, "r");
    if (file == NULL) {
        perror("Error opening file");
        return;
    }

    char line[MAX_LINE_LENGTH];
    char name[MAX_LINE_LENGTH] = "";
    char date[MAX_LINE_LENGTH] = "";
    char status[MAX_LINE_LENGTH] = "";

    while (fgets(line, sizeof(line), file)) {
        // Remove newline character
        line[strcspn(line, "\n")] = 0;

        if (strlen(name) == 0) {
            strcpy(name, line);
        } else {
            // Process the date and status
            char *date_part = strtok(line, ",");
            char *status_part = strtok(NULL, ",");

            if (date_part && status_part) {
                strcpy(date, date_part);
                strcpy(status, status_part);

                // Print the complete entry
                printf("Name: %s, Date: %s, Status: %s\n", name, date, status);

                // Reset for the next entry
                name[0] = '\0';
                date[0] = '\0';
                status[0] = '\0';
            } else {
                fprintf(stderr, "Error parsing line: %s\n", line);
            }
        }
    }

    fclose(file);
}


void display_csv_file(const char *filepath) {
  // Open the CSV file
  FILE *fp = fopen(filepath, "r");
  if (fp == NULL) {
    printf("Error opening file: %s\n", filepath);
    return;
  }

  // Read the CSV file line by line
  char line[MAX_LINE_LENGTH];
  while (fgets(line, MAX_LINE_LENGTH, fp) != NULL) {
    char *field;

    // Get the first field
    field = strtok(line, ",");
    int field_count = 0;

    while (field != NULL) {
      // Extract data based on field position (assuming order)
      if (field_count == 0) {
        printf("name: %s ", field);
      } else if (field_count == 1) {
        printf("time: %s ", field);
      } else if (field_count == 2) {
        printf("status: %s", field);
      }

      // Remove trailing newline from field
      field[strcspn(field, "\n")] = '\0';

      field = strtok(NULL, ",");
      field_count++;
    }

    printf("\n"); // Add a newline after each formatted line
  }

  // Close the file
  fclose(fp);
}



void reorderLogData(const char* inputFilePath, const char* outputFilePath) {
    FILE* inputFile = fopen(inputFilePath, "r");
    FILE* outputFile = fopen(outputFilePath, "w");

    if (inputFile == NULL) {
        printf("Failed to open the input file.\n");
        return;
    }

    if (outputFile == NULL) {
        printf("Failed to open the output file.\n");
        fclose(inputFile);
        return;
    }

    char line[256];
    int lineCount = 0;
    char name[256], timestamp[256], status[256];

    while (fgets(line, sizeof(line), inputFile)) {
        line[strcspn(line, "\n")] = 0;  // Remove trailing newline

        switch (lineCount % 3) {
            case 0:
                strcpy(name, line);
                break;
            case 1:
                strcpy(timestamp, line);
                break;
            case 2:
                strcpy(status, line);
                fprintf(outputFile, "%s\n%s\n%s\n", name, timestamp, status);
                break;
        }

        lineCount++;
    }

    fclose(inputFile);
    fclose(outputFile);
}

void displayLogData(const char* filePath) {
    FILE* file = fopen(filePath, "r");
    if (file == NULL) {
        printf("Failed to open the file.\n");
        return;
    }

    char name[256];
    char timestamp[256];
    char status[256];

    while (fgets(name, sizeof(name), file) && fgets(timestamp, sizeof(timestamp), file) && fgets(status, sizeof(status), file)) {
        // Remove trailing newline characters
        // name[strcspn(name, "\n")] = 0;
        timestamp[strcspn(timestamp, "\n")] = 0;
        status[strcspn(status, "\n")] = 0;

        // Print the log entry in the desired format
        printf("%s, %s\n", timestamp, status);
    }

    fclose(file);
}

void readFilesWithoutExtension(const char* folderPath) {
    // Open the folder
    DIR* folder = opendir(folderPath);
    if (folder == NULL) {
        printf("Error opening folder.\n");
        return;
    }

    // Read the files in the folder
    struct dirent* entry;
    while ((entry = readdir(folder)) != NULL) {
        // Check if the entry is a regular file
        if (strchr(entry->d_name, '.') != NULL) {
            // Get the file name
            char* fileName = entry->d_name;
            // Check if the file name ends with ".pickle"
            size_t fileNameLength = strlen(fileName);
            if (fileNameLength > 7 && strcmp(fileName + fileNameLength - 7, ".pickle") == 0) {
                // Remove the extension ".pickle"
                fileName[fileNameLength - 7] = '\0';
                // Print the file name without the extension
                printf("%s\n", fileName);
            }
        }
    }

    // Close the folder
    closedir(folder);
}

void displayMenu() {
    printf("Menu:\n");
    printf("1. Student's Login / Register\n");
    printf("2. User's guide\n");
    printf("3. View Student's status\n");
    printf("4. Input Student's Data\n");
    printf("5. LOad Student's Data from Database\n");
    printf("6. View Loaded Student's Data\n");
    printf("7. Reset Student's list\n");
    printf("8. Exit\n");
}



void runPythonScripts(const char* path) {
    // Code to run the Python script
    printf("Running Python script: %s\n", path);

    // Construct the command to run the Python script
    char command[100];
    snprintf(command, sizeof(command), "python %s", path);

    // Execute the command
    int result = system(command);

    // Check if the command executed successfully
    if (result == 0) {
        printf("Python script executed successfully.\n");
    } else {
        printf("Error executing Python script.\n");
    }
}



void showUserGuide() {
    // Code to display the user's guide
    printf("Displaying user's guide...\n");
    printf("Welcome to the Student Grade Management System!\n\n");

    printf("**Menu**\n");
    printf("1. Run Anti-Spoofing Script (Requires Python)\n");
    printf("2. View User Guide\n");
    printf("3. View Data\n");
    printf("   - 1. Registered Users\n");
    printf("   - 2. Recent Logs\n");
    printf("4. Add Student Grades\n");
    printf("5. Load Student Grades from File\n");
    printf("6. View Loaded Students\n");
    printf("7. Exit\n\n");

    printf("**How to Use**\n");

    printf("1. Run Anti-Spoofing Script (Requires Python):\n");
    printf("   - This option will attempt to run a Python script located at ../Silent-Face-Anti-Spoofing/main.py.\n");
    printf("   - Make sure you have Python installed and the script is located in the specified path.\n\n");

    printf("2. View User Guide:\n");
    printf("   - This option will display the user guide you're currently reading.\n\n");

    printf("3. View Data:\n");
    printf("   - This option allows you to view registered users and recent logs.\n");
    printf("     - 1. Registered Users: Shows a list of registered users from the 'groundControl/db' directory.\n");
    printf("     - 2. Recent Logs: Displays recent logs from 'groundControl/log.txt'.\n\n");

    printf("4. Add Student Grades:\n");
    printf("   - This option allows you to enter grades for multiple students.\n");
    printf("     - Enter the number of students you want to add.\n");
    printf("    - For each student, provide the following information:\n");
    printf("          - Name\n");
    printf("          - NPM (Student ID)\n");
    printf("          - UTS Grade\n");
    printf("          - UAS Grade\n");
    printf("   - After entering all student information, their grades will be saved to a file named 'gradeData.csv'.\n\n");

    printf("5. Load Student Grades from File:\n");
    printf("   - This option will load previously saved student grades from 'gradeData.csv'.\n\n");

    printf("6. View Loaded Students:\n");
    printf("   - This option will display the list of students whose grades were loaded from the file.\n\n");

    printf("7. Exit:\n");
    printf("   - This option will terminate the program.\n\n");

    printf("**Please note:**\n");
    printf(" - Some options require additional files or programs to function properly.\n");
    printf(" - Ensure you have the necessary files in the specified locations.\n\n");

    printf("**Have a questions?** Feel free to consult the developer for further assistance.\n");


}

void viewStatus() {
    // Code to read and display the status from a text file
    printf("Viewing status...\n");

    // Open the status file
    FILE *statusFile = fopen("status.txt", "r");
    if (statusFile == NULL) {
        printf("Error opening status file.\n");
        return;
    }

    // Read and display the status
    char status[100];
    while (fgets(status, sizeof(status), statusFile) != NULL) {
        printf("%s", status);
    }

    // Close the status file
    fclose(statusFile);
}
