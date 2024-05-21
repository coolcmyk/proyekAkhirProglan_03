#include "header.h"
#include <omp.h>
#include <stdlib.h>
#include <stdio.h> 

int main() {
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
                        process_file(filepath);
                        break;
                    default:
                        printf("Invalid choice. Please try again.\n");
                }
                break;
            case 4:
                printf("Exiting...\n");
                break;
            default:
                printf("Invalid choice. Please try again.\n");
        }
    } while (choice != 4);

    return 0;
}
