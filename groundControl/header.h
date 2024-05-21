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
    // Allocate memory for the name
    student->name = (char*)malloc(strlen(name) + 1);
    if (student->name == NULL) {
        printf("Failed to allocate memory for the name.\n");
        return;
    }
    // Copy the name
    strcpy(student->name, name);
    // Set the rank and grades
    student->NPM = NPM;
    student->utsGrade = utsGrade;
    student->uasGrade = uasGrade;
}

void destroyStudent(Student* student) {
    // Free the memory for the name
    free(student->name);
    // Reset the struct fields
    student->name = NULL;
    student->rank = 0;
    student->utsGrade = 0.0;
    student->uasGrade = 0.0;
}

void saveStudentsToFile(const char* filepath, Student* students, int numStudents) {
    FILE* file = fopen(filepath, "a+");
    if (file == NULL) {
        printf("Failed to open the file for writing.\n");
        return;
    }
    for (int i = 0; i < numStudents; i++) {
        fprintf(file, "%s,%d,%.2f,%.2f\n", students[i].name, students[i].NPM, students[i].utsGrade, students[i].uasGrade);
    }
    fclose(file);
}


void viewLoadedStudents(Student* students, const char* filepath) {
    FILE* file = fopen(filepath, "r");
    if (file == NULL) {
        printf("Failed to open the file for reading.\n");
    }
    // Count the number of lines in the file
    int count = 0;
    char line[256];
    while (fgets(line, sizeof(line), file)) {
        count++;
    }
    for (int i = 0; i < count; i++) {
        printf("Name: %s, NPM: %d, UTS Grade: %.2f, UAS Grade: %.2f\n",
               students[i].name, students[i].NPM, students[i].utsGrade, students[i].uasGrade);
    }
}


Student* loadStudentsFromFile(const char* filepath) {
    FILE* file = fopen(filepath, "r");
    if (file == NULL) {
        printf("Failed to open the file for reading.\n");
        return NULL;
    }
    // Count the number of lines in the file
    int count = 0;
    char line[256];
    while (fgets(line, sizeof(line), file)) {
        count++;
    }
    // Allocate memory for the students
    Student* students = (Student*)malloc(count * sizeof(Student));
    if (students == NULL) {
        fclose(file);
        return NULL;
    }
    printf("count: %d\n", count);
    // Reset the file pointer to the beginning of the file
    fseek(file, 0, SEEK_SET);
    // Read the students from the file
    for (int i = 0; i < count; i++) {
        char name[256];
        int rank;
        int NPM;
        float utsGrade;
        float uasGrade;
        if (fscanf(file, "%255[^,],%d,%f,%f\n", name, &NPM, &utsGrade, &uasGrade) == 4) {
            createStudent(&students[i], name, NPM, utsGrade, uasGrade);
        } else {
            printf("Error parsing line %d.\n", i + 1);
        }
    }
    fclose(file);
    // *numStudents = count;
    return students;
}

// int main() {
//     // Create an array of students
//     Student students[3];
//     createStudent(&students[0], "John Doe", 1, 85.5, 90.0);
//     createStudent(&students[1], "Jane Smith", 2, 90.0, 95.0);
//     createStudent(&students[2], "Bob Johnson", 3, 80.0, 85.0);

//     // Save the students to a file
//     saveStudentsToFile("students.csv", students, 3);

//     // Load the students from the file
//     int numStudents;
//     Student* loadedStudents = loadStudentsFromFile("students.csv", &numStudents);
//     if (loadedStudents != NULL) {
//         // Print the loaded students
//         for (int i = 0; i < numStudents; i++) {
//             printf("Name: %s, Rank: %d, UTS Grade: %.2f, UAS Grade: %.2f\n",
//                    loadedStudents[i].name, loadedStudents[i].rank, loadedStudents[i].utsGrade, loadedStudents[i].uasGrade);
//         }
//         // Free the memory for the loaded students
//         for (int i = 0; i < numStudents; i++) {
//             destroyStudent(&loadedStudents[i]);
//         }
//         free(loadedStudents);
//     }

//     // Destroy the students
//     for (int i = 0; i < 3; i++) {
//         destroyStudent(&students[i]);
//     }

//     return 0;
// }




// int main() {
//     // Create a student
//     Student student;
//     createStudent(&student, "John Doe", 1, 85.5, 90.0);
//     // Access the student's fields
//     printf("Name: %s\n", student.name);
//     printf("Rank: %d\n", student.rank);
//     printf("UTS Grade: %.2f\n", student.utsGrade);
//     printf("UAS Grade: %.2f\n", student.uasGrade);
//     // Destroy the student
//     destroyStudent(&student);
//     return 0;
// }









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



// void display_csv_file(const char *filepath) {
//   // Open the CSV file
//   FILE *fp = fopen(filepath, "r");
//   if (fp == NULL) {
//     printf("Error opening file: %s\n", filepath);
//     return;
//   }

//   // Read the CSV file line by line
//   char line[MAX_LINE_LENGTH];
//   while (fgets(line, MAX_LINE_LENGTH, fp) != NULL) {
//     char *field;

//     // Get the first field
//     field = strtok(line, ",");
//     while (field != NULL) {
//       // Print the field with a trailing space
//       printf("%s ", field);
//       field = strtok(NULL, ",");
//     }

//     // Print a newline after each line
//     printf("\n");
//   }

//   // Close the file
//   fclose(fp);
// }
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
    printf("4. Exit\n");
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
