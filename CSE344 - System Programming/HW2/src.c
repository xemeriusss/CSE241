#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/wait.h>
#include <errno.h>
#include <signal.h>
#include <string.h>

static volatile int child_count = 0;

void sigchld_handler() {
    int status;
    pid_t pid;

    printf("SIGCHLD received.\n");

    while ((pid = waitpid(-1, &status, WNOHANG)) > 0) {
        printf("Child process with PID %d terminated.\n", pid);
        child_count += 1;
    }

    if(pid == -1 && errno != ECHILD) {
        perror("waitpid");
    }

    printf("child_count: %d\n", child_count);
}

void sigint_handler() {
    printf("SIGINT received. Terminating all processes.\n");

    unlink("fifo1");
    unlink("fifo2");

    // Kill all child processes
    int s;
    s = kill(0, SIGKILL);
    if (s == -1) {
        perror("kill");
        exit(EXIT_FAILURE);
    }
    exit(EXIT_SUCCESS);
}

int main(int argc, char *argv[]) {

    // Set up signal handler for SIGCHLD
    struct sigaction sa;
    sa.sa_handler = sigchld_handler;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = SA_RESTART;
    if (sigaction(SIGCHLD, &sa, NULL) == -1) {
        perror("sigaction");
        exit(EXIT_FAILURE);
    }

    // Set up signal handler for SIGINT (Ctrl+C)
    struct sigaction sa_int;
    sa_int.sa_handler = sigint_handler;
    sigemptyset(&sa_int.sa_mask);
    sa_int.sa_flags = 0;
    if (sigaction(SIGINT, &sa_int, NULL) == -1) {
        perror("sigaction");
        exit(EXIT_FAILURE);
    }


    srand(time(NULL));

    if (argc != 2) {
        printf("Usage: %s <integer>\n", argv[0]);
        return 1;
    }

    int arr_len = atoi(argv[1]);

    // Create an array of random integers with the size of the argv[1]
    int *arr = (int *)malloc(arr_len * sizeof(int));
    for (int i = 0; i < arr_len; i++) {
        arr[i] = rand() % 100;
    }

    printf("The initial random array is: ");
    for (int i = 0; i < arr_len; i++) {
        printf("%d ", arr[i]);
    }
    printf("\n");

    // // Ask user for the command
    // printf("Choose the corresponding number for the command:\n");
    // printf("1. summation\n2. subtraction\n3. multiplication\n4. division\n");
    // int command;
    // scanf("%d", &command);

    // Ask user for the command as multiply
    printf("Write the command (multiply):\n");
    char command[10];
    scanf("%s", command);

    // Create FIFOs
    char *fifo1 = "fifo1";
    char *fifo2 = "fifo2";

    // unlink(fifo1);
    // unlink(fifo2);

    if(mkfifo(fifo1, 0666) == -1) {
        if(errno != EEXIST) {
            perror("mkfifo1");
            return EXIT_FAILURE;
        }
    }

    if(mkfifo(fifo2, 0666) == -1) {
        if(errno != EEXIST) {
            perror("mkfifo2");
            return EXIT_FAILURE;
        }
    }

    // CHILD 2 =========================================================================================================
    pid_t pid2 = fork();

    if(pid2 == -1) {
        perror("fork");
        return EXIT_FAILURE;
    }

    if(pid2 == 0) {

        printf("Child pid: %d\n", getpid());

        sleep(10);

        int fd2 = open(fifo2, O_RDONLY);
        if(fd2 == -1) {
            perror("open2");
            return 1;
        }

        // int sum;
        // if(read(fd2, &sum, sizeof(int)) == -1) {
        //     perror("read2");
        //     return 1;
        // }

        char command[10];
        if(read(fd2, command, 10) == -1) {
            perror("read2");
            return 1;
        }

        int arr2[arr_len];
        if(read(fd2, arr2, arr_len * sizeof(int)) == -1) {
            perror("read2");
            return 1;
        }

        sleep(5);
        int result_from_child1;
        if(read(fd2, &result_from_child1, sizeof(int)) == -1) {
            perror("read2");
            return 1;
        }

        printf("The random number array is received from the second FIFO\n");
        // printf("The array is: ");
        // for (int i = 0; i < arr_len; i++) {
        //     printf("%d ", arr2[i]);
        // }

        printf("The command is received from the second FIFO\n");
        // printf("The command is: %s\n", command);

        printf("The result from the first child is received from the second FIFO: %d\n", result_from_child1);

        // Perform the command
        int result2 = 1;

        if (strcmp(command, "multiply") == 0) {
            for (int i = 0; i < arr_len; i++) {
                result2 *= arr2[i];
            }
        }
        else {
            perror("Invalid command");
            return 1;
        }

        // switch (command) {
        //     case 1:
        //         result2 = 0;
        //         for (int i = 0; i < arr_len; i++) {
        //             result2 += arr2[i];
        //         }
        //         break;
        //     case 2:
        //         result2 = arr2[0];
        //         for (int i = 1; i < arr_len; i++) {
        //             result2 -= arr2[i];
        //         }
        //         break;
        //     case 3:
        //         result2 = 1;
        //         for (int i = 0; i < arr_len; i++) {
        //             result2 *= arr2[i];
        //         }
        //         break;
        //     case 4:
        //         result2 = arr2[0];
        //         for (int i = 1; i < arr_len; i++) {
        //             result2 /= arr2[i];
        //         }
        //         break;
        //     default:
        //         printf("Invalid command\n");
        //         break;
        // }

        printf("The result of the command (multiply) is: %d\n", result2);
        
        close(fd2);

        // Fİnal result
        int final_result = result2 + result_from_child1;
        printf("The final result is: %d\n", final_result);
        
        exit(EXIT_SUCCESS);

    }

    

    // // CHILD 1 =========================================================================================================
    pid_t pid1 = fork();

    if(pid1 == -1) {
        perror("fork");
        return EXIT_FAILURE;
    }

    if(pid1 == 0) {

        printf("Child pid: %d\n", getpid());

        sleep(10);

        int fd1 = open(fifo1, O_RDONLY);
        if(fd1 == -1) {
            perror("open1");
            return 1;
        }

        int arr1[arr_len];

        if(read(fd1, arr1, arr_len * sizeof(int)) == -1) {
            perror("read1");
            return 1;
        }

        printf("The random number array is received from the first FIFO\n");
        // printf("The array is: ");
        // for (int i = 0; i < arr_len; i++) {
        //     printf("%d ", arr1[i]);
        // }

        // Sum the array
        int sum = 0;
        for (int i = 0; i < arr_len; i++) {
            sum += arr1[i];
        }

        close(fd1);


        
        // Send the sum to the second FIFO
        int fd2 = open(fifo2, O_WRONLY);
        if(fd2 == -1) {
            perror("open2");
            return 1;
        }

        
        if(write(fd2, &sum, sizeof(int)) == -1) {
            perror("write2");
            return 1;
        }

        printf("The sum is sent to the second FIFO\n");
        printf("The sum of the array is: %d\n", sum);

        close(fd2);

        

        exit(EXIT_SUCCESS);

    }


    
    
    // write fifo2 command and random numbers
    printf("Parent pid: %d\n", getpid());

    int fd2 = open(fifo2, O_WRONLY);
    if(fd2 == -1) {
        perror("open1");
        return 1;
    }

    // Write the command to the second FIFO
    if(write(fd2, command, 10) == -1) {
        perror("write2");
        return 1;
    }

    // Write the random numbers to the second FIFO
    if(write(fd2, arr, arr_len * sizeof(int)) == -1) {
        perror("write2");
        return 1;
    }
    
    printf("The command and the array are sent to the second FIFO\n");

    close(fd2);

    // int status;
    // waitpid(pid2, &status, 0);

    // if(WIFEXITED(status)) {
    //     printf("Child process %d exited with status %d\n", pid2, WEXITSTATUS(status));
    // }

    // else {
    //     printf("Child process exited abnormally\n");
    // }


    // write fifo1 random numbers
    // printf("Parent pid: %d\n", getpid());

    int fd1 = open(fifo1, O_WRONLY);
    if(fd1 == -1) {
        perror("open1");
        return 1;
    }

    if(write(fd1, arr, arr_len * sizeof(int)) == -1) {
        perror("write1");
        return 1;
    }

    printf("The array is sent to the first FIFO\n");

    close(fd1);

    // int status;
    // waitpid(pid1, &status, 0);

    // if(WIFEXITED(status)) {
    //     printf("Child process %d exited with status %d\n", pid1, WEXITSTATUS(status));
    // }

    // else {
    //     printf("Child process exited abnormally\n");
    // }

    


    free(arr);

    // Parent process
    while (child_count < 2) {
        printf("Proceeding...\n");
        sleep(2);
    }

    printf("All child processes terminated. Exiting.\n");


    unlink(fifo1);
    unlink(fifo2);

    return 0;
    
}