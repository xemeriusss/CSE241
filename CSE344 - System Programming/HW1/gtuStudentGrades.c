
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <time.h>

typedef struct {
    char name[50];
    char grade[5];
} Student;

// Write the message to the log file with time information
void write_to_log(const char *message) {
    
    pid_t pid = fork();

    if(pid == -1) {
        perror("Fork failed");
        exit(EXIT_FAILURE);
    }

    else if (pid == 0) {
        // Child process

        // printf("Child process ID (write_to_log): %d\n", getpid());

        // Open the file for appending
        int fd = open("log.txt", O_WRONLY | O_APPEND | O_CREAT, 0666);
        if (fd == -1) {
            perror("Error opening file");
            exit(EXIT_FAILURE);
        }

        // Append the message to the file with time information
        time_t now;
        time(&now);
        char *time_str = ctime(&now);
        time_str[strlen(time_str) - 1] = '\0'; // Remove the newline character from the end of the time string

        char combined_message[1024];
        snprintf(combined_message, sizeof(combined_message), "%s %s", time_str, message);

        if (write(fd, combined_message, strlen(combined_message)) == -1) {
            perror("Error writing to file");
            close(fd);
            exit(EXIT_FAILURE);
        }

        // Close the file
        close(fd);

        // Exit the child process
        exit(EXIT_SUCCESS);
    } 
    
    else {
        // Parent process

        // printf("Parent process ID (write_to_log): %d\n", getpid());

        // Wait for the child process to finish
        int status;
        waitpid(pid, &status, 0);

        // Check if the child process terminated successfully
        if (WIFEXITED(status) && WEXITSTATUS(status) == EXIT_SUCCESS) {
            //printf("Message written to log. Child process finished.\n");
            
        } else {
            // printf("Failed to write message to log.\n");
        }
    }
}

// Compare function for sorting students by name in ascending order
int compare_students_by_name(const void *a, const void *b) {
    
    // Cast the void pointers to Student pointers
    const Student *student_a = (const Student *)a;
    const Student *student_b = (const Student *)b;

    // Compare the names
    return strcmp(student_a->name, student_b->name);
}

// Compare function for sorting students by name in descending order
int compare_students_by_name_desc(const void *a, const void *b) {

    // Cast the void pointers to Student pointers
    const Student *student_a = (const Student *)a;
    const Student *student_b = (const Student *)b;

    // Compare the names in descending order
    return strcmp(student_b->name, student_a->name);
}

// Compare function for sorting students by grade in ascending order
int compare_students_by_grade(const void *a, const void *b) {

    // Cast the void pointers to Student pointers
    const Student *student_a = (const Student *)a;
    const Student *student_b = (const Student *)b;

    // Compare the grades
    return strcmp(student_a->grade, student_b->grade);
}

// Compare function for sorting students by grade in descending order
int compare_students_by_grade_desc(const void *a, const void *b) {

    // Cast the void pointers to Student pointers
    const Student *student_a = (const Student *)a;
    const Student *student_b = (const Student *)b;

    // Compare the grades in descending order
    return strcmp(student_b->grade, student_a->grade);
}

// Sort student name or grade, in ascending or descending order. Then print the sorted list in the terminal.
void sortAll(const char *filename, int isStudent, int isAscending) {

    pid_t pid = fork();

    if(pid == -1) {
        perror("Fork failed");
        exit(EXIT_FAILURE);
    }

    else if (pid == 0) {
        // Child process

        // printf("Child process ID (sortAll): %d\n", getpid());

        // Open the file for reading
        int fd = open(filename, O_RDONLY);
        if (fd == -1) {
            perror("Error opening file");
            exit(EXIT_FAILURE);
        }

        char buffer[1024];
        char *line = buffer;
        int bytes_read;

        Student students[100];
        int student_count = 0;

        // Read the file byte by byte
        while ((bytes_read = read(fd, line, 1)) > 0) {
            // Check if a newline character is encountered
            if (*line == '\n') {
                *line = '\0'; // Null-terminate the string

                // Parse the line to extract the name and grade
                sscanf(buffer, "%[^,], %[^\n]", students[student_count].name, students[student_count].grade);
                student_count++;

                //printf("Name: %s, Grade: %s\n", students[student_count].name, students[student_count].grade);

                // Reset line buffer for the next line
                line = buffer;
            } else {
                // Move to the next character in the buffer
                line++;
            }
        }

        // Close the file
        close(fd);

        // Sort the students
        if (isStudent) {
            if (isAscending) {
                // printf("Sorting students by name in ascending order\n");
                qsort(students, student_count, sizeof(Student), compare_students_by_name);
            } else {
                // printf("Sorting students by name in descending order\n");
                qsort(students, student_count, sizeof(Student), compare_students_by_name_desc);
            }
        } 
        
        else {
            if (isAscending) {
                qsort(students, student_count, sizeof(Student), compare_students_by_grade);
            } else {
                qsort(students, student_count, sizeof(Student), compare_students_by_grade_desc);
            }
        }

        char buffer_print[1024];
        // Print the sorted students
        for (int i = 0; i < student_count; i++) {
            snprintf(buffer_print, sizeof(buffer_print), "%s, %s\n", students[i].name, students[i].grade); 
            if(write(STDOUT_FILENO, buffer_print, strlen(buffer_print)) == -1) {
                perror("Error writing to terminal");
                exit(EXIT_FAILURE);
            }   
        }

        // Exit the child process
        exit(EXIT_SUCCESS);
    }

    else {
        // Parent process

        // printf("Parent process ID (sortAll): %d\n", getpid());

        // Wait for the child process to finish
        int status;
        waitpid(pid, &status, 0);

        char *message = (char *)malloc(1024 * sizeof(char));
        if(message == NULL) {
            perror("Failed to allocate memory");
            exit(EXIT_FAILURE);
        }

        // Check if the child process terminated successfully
        if (WIFEXITED(status) && WEXITSTATUS(status) == EXIT_SUCCESS) {
            // printf("All student grades sorted. Child process finished.\n");
            snprintf(message, 1024, "Child process finished: %d, Students sorted: %s\n", pid, filename);
        } else {
            // printf("Failed to sort all student grades.\n");
            snprintf(message, 1024, "Child process failed: %d, Failed to sort students: %s\n", pid, filename);
        }

        write_to_log(message);
        free(message);
    }

}


// Command should append student and grade to the end of the file
void addStudentGrade(const char *filename, const char *studentName, const char *grade) {
    //printf("Adding student grade to file: %s\n", filename);
    //printf("Student name: %s\n", studentName);
    //printf("Grade: %s\n", grade);

    pid_t pid = fork();

    if(pid == -1) {
        perror("Fork failed");
        exit(EXIT_FAILURE);
    }

    else if (pid == 0) {
        // Child process

        // printf("Child process ID (addStudentGrade): %d\n", getpid());

        // Open the file for appending
        int fd = open(filename, O_WRONLY | O_APPEND);
        if (fd == -1) {
            perror("Error opening file");
            exit(EXIT_FAILURE);
        }

        // Append the student's name and grade to the file
        char buffer[1024];
        int len = snprintf(buffer, sizeof(buffer), "%s, %s\n", studentName, grade);

        if (write(fd, buffer, len) == -1) {
            perror("Error writing to file");
            exit(EXIT_FAILURE);
        }

        // Close the file
        close(fd);

        // Exit the child process
        exit(EXIT_SUCCESS);
    } 
    
    else {
        // Parent process

        // printf("Parent process ID (addStudentGrade): %d\n", getpid());

        // Wait for the child process to finish
        int status;
        waitpid(pid, &status, 0);

        char *message = (char *)malloc(1024 * sizeof(char));
        if(message == NULL) {
            perror("Failed to allocate memory");
            exit(EXIT_FAILURE);
        }

        // Check if the child process terminated successfully
        if (WIFEXITED(status) && WEXITSTATUS(status) == EXIT_SUCCESS) {
            // printf("Student grade added. Child process finished.\n");
            snprintf(message, 1024, "Child process finished: %d, Student grade added: %s, %s\n", pid, studentName, grade);

        } else {
            // printf("Failed to add student grade.\n");
            snprintf(message, 1024, "Child process failed: %d, Failed to add student grade: %s, %s\n", pid, studentName, grade);
        }

        write_to_log(message);
        free(message);
    }
}

// Command should return student name surname and grade
void searchStudent(const char *filename, const char *studentName) {
    //printf("Searching student grade in file: %s\n", filename);
    //printf("Student name: %s\n", studentName);

    pid_t pid = fork();

    if(pid == -1) {
        perror("Fork failed");
        exit(EXIT_FAILURE);
    }

    else if (pid == 0) {
        // Child process

        //printf("Child process ID (searchStudent): %d\n", getpid());

        // Open the file for reading
        int fd = open(filename, O_RDONLY);
        if (fd == -1) {
            perror("Error opening file");
            exit(EXIT_FAILURE);
        }

        char buffer[1024];
        char *line = buffer;
        int bytes_read;

        int found = 0;

        // Read the file byte by byte
        while ((bytes_read = read(fd, line, 1)) > 0) {
            // Check if a newline character is encountered
            if (*line == '\n') {
                *line = '\0'; // Null-terminate the string

                //printf("Line: %s\n", buffer);

                // Parse the line to extract the name and grade
                char *student_name = strtok(buffer, ",");
                char *grade = strtok(NULL, ",");

                if(student_name == NULL || grade == NULL) {
                    perror("Error parsing line");
                    close(fd);
                    exit(EXIT_FAILURE);
                }

                //printf("Full Student Name: %s\n", student_name);
                //printf("Grade: %s\n", grade);
                
                // Compare the name with the search query
                if (strcmp(student_name, studentName) == 0) {
                    printf("Student found! Student name: %s, Grade: %s\n", student_name, grade);
                    found = 1;
                    close(fd);
                    //return;
                    break;
                }

                // Reset line buffer for the next line
                line = buffer;
            } else {
                // Move to the next character in the buffer
                line++;
            }
        }

        // Close the file
        close(fd);

        // Exit the child process
        exit(found ? EXIT_SUCCESS : EXIT_FAILURE);
        
    } 
    
    else {
        // Parent process

        //printf("Parent process ID (searchStudent): %d\n", getpid());

        // Wait for the child process to finish
        int status;
        waitpid(pid, &status, 0);

        char *message = (char *)malloc(1024 * sizeof(char));
        if(message == NULL) {
            perror("Failed to allocate memory");
            exit(EXIT_FAILURE);
        }

        // Check if the child process terminated successfully
        if (WIFEXITED(status) && WEXITSTATUS(status) == EXIT_SUCCESS) {
            // printf("Student grade found. Child process finished.\n");
            snprintf(message, 1024, "Child process finished: %d, Student grade found: %s\n", pid, studentName);
        } else {
            // printf("Failed to find student grade.\n");
            snprintf(message, 1024, "Child process failed: %d, Failed to find student grade: %s\n", pid, studentName);
        }

        write_to_log(message);
        free(message);
    }
}

// Command should print all of the entries in the file
void displayStudentGrades(const char *filename, int isListGrades) {

    int listGradesFlag = 0;

    //printf("Showing all student grades in file: %s\n", filename);

    pid_t pid = fork();

    if(pid == -1) {
        perror("Fork failed");
        exit(EXIT_FAILURE);
    }

    else if (pid == 0) {
        // Child process

        //printf("Child process ID (showAll): %d\n", getpid());

        // Open the file for reading
        int fd = open(filename, O_RDONLY);
        if (fd == -1) {
            perror("Error opening file");
            exit(EXIT_FAILURE);
        }

        char buffer[1024];
        char *line = buffer;
        int bytes_read;

        // Read the file byte by byte
        while ((bytes_read = read(fd, line, 1)) > 0) {
            // Check if a newline character is encountered
            if (*line == '\n') {
                *line = '\0'; // Null-terminate the string

                //printf("%s\n", buffer);
                if(write(STDOUT_FILENO, buffer, strlen(buffer)) == -1) {
                    perror("Error writing to terminal");
                    exit(EXIT_FAILURE);
                }

                printf("\n");
                
                if(isListGrades == 1) {
                    if(listGradesFlag >= 4) {
                        break;
                    }
                }

                listGradesFlag++;
                //printf("List Grades Flag: %d\n", listGradesFlag);

                // Reset line buffer for the next line
                line = buffer;
            } else {
                // Move to the next character in the buffer
                line++;
            }
        }

        // Close the file
        close(fd);

        // Exit the child process
        exit(EXIT_SUCCESS);
    } 
    
    else {
        // Parent process

        // printf("Parent process ID (showAll): %d\n", getpid());

        // Wait for the child process to finish
        int status;
        waitpid(pid, &status, 0);

        char *message = (char *)malloc(1024 * sizeof(char));
        if(message == NULL) {
            perror("Failed to allocate memory");
            exit(EXIT_FAILURE);
        }

        // Check if the child process terminated successfully
        if (WIFEXITED(status) && WEXITSTATUS(status) == EXIT_SUCCESS) {
            // printf("All student grades shown. Child process finished.\n");
            if(isListGrades == 1) {
                snprintf(message, 1024, "Child process finished: %d, 5 student grades shown: %s\n", pid, filename);
            } else {
                snprintf(message, 1024, "Child process finished: %d, All student grades shown: %s\n", pid, filename);
            }
        } else {
            //printf("Failed to show all student grades.\n");
            snprintf(message, 1024, "Child process failed: %d, Failed to show student grades: %s\n", pid, filename);
        }

        write_to_log(message);
        free(message);
    }
}

// Command should print numofEntries entries starting from pageNumber in the file
void displaySomeStudentGrades(const char *filename, int numofEntries, int pageNumber) {

    //printf("Showing some student grades in file: %s\n", filename);
    //printf("Number of entries: %d\n", numofEntries);
    //printf("Page number: %d\n", pageNumber);

    pid_t pid = fork();

    if(pid == -1) {
        perror("Fork failed");
        exit(EXIT_FAILURE);
    }

    else if (pid == 0) {
        // Child process

        //printf("Child process ID (showSome): %d\n", getpid());

        // Open the file for reading
        int fd = open(filename, O_RDONLY);
        if (fd == -1) {
            perror("Error opening file");
            exit(EXIT_FAILURE);
        }

        char buffer[1024];
        char *line = buffer;
        int bytes_read;

        int line_number = 1;

        // Read the file byte by byte
        while ((bytes_read = read(fd, line, 1)) > 0) {
            // Check if a newline character is encountered
            if (*line == '\n') {
                *line = '\0'; // Null-terminate the string

                // Calculate the line number and print the line if it is within the specified range
                if(line_number > (pageNumber - 1) * numofEntries && line_number <= pageNumber * numofEntries) {
                    //printf("%s\n", buffer);
                    if(write(STDOUT_FILENO, buffer, strlen(buffer)) == -1) {
                        perror("Error writing to terminal");
                        exit(EXIT_FAILURE);
                    }
                    printf("\n");
                }

                // Reset line buffer for the next line
                line = buffer;
                line_number++;
            } else {
                // Move to the next character in the buffer
                line++;
            }
        }

        // Close the file
        close(fd);

        // Exit the child process
        exit(EXIT_SUCCESS);
    } 
    
    else {
        // Parent process

        //printf("Parent process ID (showSome): %d\n", getpid());

        // Wait for the child process to finish
        int status;
        waitpid(pid, &status, 0);

        char *message = (char *)malloc(1024 * sizeof(char));
        if(message == NULL) {
            perror("Failed to allocate memory");
            exit(EXIT_FAILURE);
        }

        // Check if the child process terminated successfully
        if (WIFEXITED(status) && WEXITSTATUS(status) == EXIT_SUCCESS) {
            // printf("Some student grades shown. Child process finished.\n");
            snprintf(message, 1024, "Child process finished: %d, Some student grades shown: %s\n", pid, filename);
        } else {
            // printf("Failed to show some student grades.\n");
            snprintf(message, 1024, "Child process failed: %d, Failed to show some student grades: %s\n", pid, filename);
        }

        write_to_log(message);
        free(message);
    }
}

// Show usage information
void usage() {
    
    pid_t pid = fork();

    if(pid == -1) {
        perror("Fork failed");
        exit(EXIT_FAILURE);
    }

    else if (pid == 0) {
        // Child process

        //printf("Child process ID (usage): %d\n", getpid());

        printf("\nUsage: \n");
        printf("gtuStudentGrades grades.txt: Create file. \n");
        printf("addStudentGrade grades.txt studentName studentSurname grade: Add student grade to file. Grade should be between AA and FF.\n");
        printf("searchStudent grades.txt studentName studentSurname: Search student grade in file. \n");    
        printf("showAll grades.txt: Print all of the entries in the file. \n");
        printf("listGrades grades.txt: Print 5 entries in the beginning of the file. \n");
        printf("listSome numofEntries pageNumber grades.txt: Print numofEntries entries starting from pageNumber in the file. \n");
        printf("sortAll grades.txt: Sort student name or grade, in ascending or descending order. \n");
        printf("quit: Exit the program. \n\n");
        
        // Exit the child process
        exit(EXIT_SUCCESS);
    } 
    
    else {
        // Parent process

        //printf("Parent process ID (usage): %d\n", getpid());

        // Wait for the child process to finish
        int status;
        waitpid(pid, &status, 0);

        char *message = (char *)malloc(1024 * sizeof(char));
        if(message == NULL) {
            perror("Failed to allocate memory");
            exit(EXIT_FAILURE);
        }

        // Check if the child process terminated successfully
        if (WIFEXITED(status) && WEXITSTATUS(status) == EXIT_SUCCESS) {
            // printf("Usage shown. Child process finished.\n");
            snprintf(message, 1024, "Child process finished: %d, Usage shown\n", pid);
        } else {
            //printf("Failed to show usage.\n");
            snprintf(message, 1024, "Child process failed: %d, Failed to show usage\n", pid);
        }

        write_to_log(message);
        free(message);
    }
} 


int main() {

    // printf("Main process ID: %d\n", getpid());
    char *message = (char *)malloc(1024 * sizeof(char));
    if(message == NULL) {
        perror("Failed to allocate memory");
        exit(EXIT_FAILURE);
    }
    snprintf(message, 1024, "Main process started. ID: %d\n", getpid());
    write_to_log(message);
    free(message);

    if(write(STDOUT_FILENO, "Enter a command, or type 'quit' to exit the program.\n", 53) == -1) {
        perror("Error writing to terminal");
        exit(EXIT_FAILURE);
    }

    // Take commands from terminal in loop
    while (1){

        char command[1024];
        ssize_t bytes_read;

        // Read a line from the terminal
        bytes_read = read(STDIN_FILENO, command, sizeof(command));

        if (bytes_read == -1) {
            perror("Error reading from terminal");
            exit(EXIT_FAILURE);
        }

        // Null-terminate the string
        command[bytes_read] = '\0';

        // Remove the newline character at the end
        if (command[bytes_read - 1] == '\n') {
            command[bytes_read - 1] = '\0';
        }

        // printf("Command: %s\n", command);

        // Tokenize the command
        char *token = strtok(command, " ");

        //printf("Token: %s\n", token);

        if(token == NULL) {
            usage();
        }
        else if(strcmp(token, "quit") == 0) {
            break;
        }   

        char *filename;
        char *studentName;
        char *studentSurname;
        char *grade;

        if (strcmp(token, "gtuStudentGrades") == 0) {

            filename = strtok(NULL, "\n");

            //printf("Filename: %s\n", filename);

            // gtuStudentGrades grades.txt
            if(filename != NULL) {
                // Create a child process
                pid_t pid = fork();

                if (pid == -1) {
                    // Error occurred
                    perror("Fork failed");
                    exit(EXIT_FAILURE);
                } 
                
                else if (pid == 0) {
                    // Child process

                    //printf("Child process ID: %d\n", getpid());

                    // Open the file for writing
                    int fd = open(filename, O_WRONLY | O_CREAT, 0666); // wronly çıkarılabilir??

                    if (fd == -1) {
                        perror("Error opening file");
                        exit(EXIT_FAILURE);
                    }
                    
                    // Close the file
                    close(fd);
                    
                    // Exit the child process
                    exit(EXIT_SUCCESS);
                } 
                
                else {
                    // Parent process

                    //printf("Parent process ID: %d\n", getpid());
                    
                    // Wait for the child process to finish
                    int status;
                    waitpid(pid, &status, 0);

                    // Write to log with filename
                    char *message = (char *)malloc(1024 * sizeof(char));
                    if(message == NULL) {
                        perror("Failed to allocate memory");
                        exit(EXIT_FAILURE);
                    }
                    
                    // Check if the child process terminated successfully
                    if (WIFEXITED(status) && WEXITSTATUS(status) == EXIT_SUCCESS) {
                        //printf("Child process finished. CREATE\n");
                        snprintf(message, 1024, "Child process finished: %d, File created: %s\n", pid, filename);
                    } 
                    else {
                        //printf("Child process failed. CREATE\n");
                        snprintf(message, 1024, "Child process failed: %d, Failed to create file: %s\n", pid, filename);
                    }

                    write_to_log(message);
                    free(message);
                }
            }

            else {
                usage();
            }

        } 

        else if(strcmp(token, "addStudentGrade") == 0) {
            filename = strtok(NULL, " ");
            studentName = strtok(NULL, " ");
            studentSurname = strtok(NULL, " ");
            grade = strtok(NULL, "\n");

            if(filename == NULL || studentName == NULL || studentSurname == NULL || grade == NULL) {
                usage();
                continue;
            }

            else if(strlen(grade) > 2 || (strcmp(grade, "AA") != 0 && strcmp(grade, "BA") != 0 && strcmp(grade, "BB") != 0 && strcmp(grade, "CB") != 0 && strcmp(grade, "CC") != 0 && strcmp(grade, "DC") != 0 && strcmp(grade, "DD") != 0 && strcmp(grade, "FF") != 0)) {
                usage();
                continue;
            }

            // Concatenate student name and surname
            char *full_student_name = malloc(strlen(studentName) + strlen(studentSurname) + 2);
            strcpy(full_student_name, studentName);
            strcat(full_student_name, " ");
            strcat(full_student_name, studentSurname);

            if(filename != NULL && full_student_name != NULL && grade != NULL) {
                addStudentGrade(filename, full_student_name, grade);
            }

            free(full_student_name);
        }

        else if(strcmp(token, "searchStudent") == 0) {
            filename = strtok(NULL, " ");
            studentName = strtok(NULL, " ");
            studentSurname = strtok(NULL, "\n");

            if(filename == NULL || studentName == NULL || studentSurname == NULL) {
                usage();
                continue;
            }

            // Concatenate student name and surname
            char *full_student_name = malloc(strlen(studentName) + strlen(studentSurname) + 2);
            strcpy(full_student_name, studentName);
            strcat(full_student_name, " ");
            strcat(full_student_name, studentSurname);

            // printf("Filename: %s\n", filename);
            // printf("Student Name: %s\n", full_student_name);

            if(filename != NULL && full_student_name != NULL) {
                searchStudent(filename, full_student_name);
            }

            free(full_student_name);
        }

        else if(strcmp(token, "showAll") == 0) {
            filename = strtok(NULL, "\n");

            if(filename == NULL) {
                usage();
                continue;
            }

            //printf("Filename: %s\n", filename);

            if(filename != NULL) {
                displayStudentGrades(filename, 0);
            }
        }

        else if(strcmp(token, "listGrades") == 0) {
            filename = strtok(NULL, "\n");

            if(filename == NULL) {
                usage();
                continue;
            }

            //printf("Filename: %s\n", filename);

            if(filename != NULL) {
                displayStudentGrades(filename, 1);
            }
        }

        else if(strcmp(token, "listSome") == 0) {
            int numofEntries = atoi(strtok(NULL, " "));
            int pageNumber = atoi(strtok(NULL, " "));
            filename = strtok(NULL, "\n");

            if(filename == NULL || numofEntries == 0 || pageNumber == 0) {
                usage();
                continue;
            }

            //printf("Filename: %s\n", filename);

            if(filename != NULL && numofEntries != 0 && pageNumber != 0) {
                displaySomeStudentGrades(filename, numofEntries, pageNumber);
            }
        }

        else if(strcmp(token, "sortAll") == 0) {
            filename = strtok(NULL, "\n");

            if(filename == NULL) {
                usage();
                continue;
            }

            // Ask if the user wants to sort by name or grade
            int isStudent = 0;
            int isAscending = 0;

            printf("Please enter 1 for name, 0 for grade: ");
            if(scanf("%d", &isStudent) != 1) {
                perror("Error reading from terminal");
                exit(EXIT_FAILURE);
            }

            printf("Please enter 1 for ascending, 0 for descending: ");
            if(scanf("%d", &isAscending) != 1) {
                perror("Error reading from terminal");
                exit(EXIT_FAILURE);
            }

            // printf("Filename: %s\n", filename);
            // printf("isStudent: %d\n", isStudent);
            // printf("isAscending: %d\n", isAscending);

            if(filename != NULL) {
                sortAll(filename, isStudent, isAscending);
            }
        }

        else {
            usage();
        }

    }

    // printf("Main process finished. ID: %d\n", getpid());
    char *message2 = (char *)malloc(1024 * sizeof(char));
    if(message2 == NULL) {
        perror("Failed to allocate memory");
        exit(EXIT_FAILURE);
    }
    snprintf(message2, 1024, "Main process finished: %d\n", getpid());
    write_to_log(message2);
    free(message2);

    return 0;

}