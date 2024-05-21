#include "header.h"
#include <omp.h>
#include <stdlib.h>
#include <stdio.h> 

int main() {
    int studentCount;
    int choice;
    do {
        displayMenu();
        printf("Enter your choice (0 for Clear): ");
        scanf("%d", &choice);

        switch (choice) {
            case 0:
                system("clear");
                break;
            case 1:
                char path[100];
                char* resolved_path = realpath("../Silent-Face-Anti-Spoofing/main.py", path);
                if (resolved_path == NULL) {
                    perror("Error resolving path");
                } else {
                    runPythonScripts(resolved_path);
                }
                system("clear");
                break;
            case 2:
                showUserGuide();
                break;
            case 3:
                int choice;
                printf("1. View Registered Users :\n");
                printf("2. View Realtime Logs :\n");
                scanf("%d", &choice);
                switch (choice) {
                    case 1:
                        printf("Registered users:\n");
                        readFilesWithoutExtension("../groundControl/db");
                        break;
                    case 2:
                        system("clear");
                        printf("Recent Logs:\n");
                        const char *filepath = "../groundControl/log.txt";
                        display_csv_file(filepath);
                        break;
                    default:
                        printf("Invalid choice. Please try again.\n");
                }
                break;
            case 4:
                Student students;
                int studentCount = 0;
                printf("Enter the student's name (or 'q' to quit): ");
                char name[100];
                scanf("%s", name);
                while (strcmp(name, "q") != 0)
                {
                    printf("Enter the student's NPM: ");
                    int NPM;
                    scanf("%d", &NPM);
                    printf("Enter the student's UTS grade: ");
                    int UTSgrade;
                    scanf("%d", &UTSgrade);
                    printf("Enter the student's UAS grade: ");
                    int UASgrade;
                    scanf("%d", &UASgrade);
                    createStudent(&students, name, NPM, UTSgrade, UASgrade);
                    studentCount++;
                    printf("Enter the student's name (or 'q' to quit): ");
                    scanf("%s", name);
                }
                saveStudentsToFile("../groundControl/gradeData.csv", &students, studentCount);
                break;
            case 5:
                loadStudentsFromFile("../groundControl/gradeData.csv");
                break;

            case 6:
                viewLoadedStudents(&students, "../groundControl/gradeData.csv");
                break;

            case 7:
                printf("Exiting....\n");
                break;
            default:
                printf("Invalid choice. Please try again.\n");
        }
    } while (choice != 7);

    return 0;
}
