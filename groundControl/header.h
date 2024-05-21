#include <stdio.h>
#include <omp.h>
#include <dirent.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>


#define MAX_LINE_LENGTH 256

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

    printf("%s\n", filePath);
    char name[256];
    char timestamp[256];
    char status[256];

    while (fgets(name, sizeof(name), file) && fgets(timestamp, sizeof(timestamp), file) && fgets(status, sizeof(status), file)) {
        // Remove trailing newline characters
        name[strcspn(name, "\n")] = 0;
        timestamp[strcspn(timestamp, "\n")] = 0;
        status[strcspn(status, "\n")] = 0;

        // Split the name by comma
        char* token = strtok(name, ",");
        while (token != NULL) {
            printf("Name: %s", token);
            token = strtok(NULL, ",");
        }

        printf("Timestamp: %s", timestamp);
        printf("Status: %s", status);
        printf("\n");
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
    printf("1. User's Login / Register\n");
    printf("2. User's guide\n");
    printf("3. View status\n");
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
