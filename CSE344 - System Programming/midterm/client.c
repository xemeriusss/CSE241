#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <signal.h>
#include <errno.h>


void handle_upload(char *second_word, int client_fd_write, char *client_fifo_write);
void handle_download(char *second_word, int client_fd_write, char *client_fifo_write, char *client_fifo_read);
void handle_readf(char *remaining_words, int client_fd_write, char *client_fifo_write, char *client_fifo_read);



void sigint_handler() {
    printf("SIGINT received. Terminating client.\n");

    char client_fifo_read[32];
    char client_fifo_write[32];
    int client_pid = getpid();
    sprintf(client_fifo_read, "/tmp/client_fifo_read_%d", client_pid);
    sprintf(client_fifo_write, "/tmp/client_fifo_write_%d", client_pid);

    unlink(client_fifo_read);
    unlink(client_fifo_write);

    exit(EXIT_SUCCESS);
}

int main(int argc, char *argv[]) {

    // SIGINT handler
    struct sigaction sa_int;
    sa_int.sa_handler = sigint_handler;
    sigemptyset(&sa_int.sa_mask);
    sa_int.sa_flags = 0;
    if (sigaction(SIGINT, &sa_int, NULL) == -1) {
        perror("sigaction");
        exit(EXIT_FAILURE);
    }



    if (argc != 3) {
        fprintf(stderr, "Usage: %s <Connect/tryConnect> <ServerPID>\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    char *mode = argv[1];
    int serverPID = atoi(argv[2]);

    char server_fifo[] = "/tmp/server_fifo";

    int server_fd = open(server_fifo, O_WRONLY);
    if (server_fd < 0) {
        perror("Failed to open server FIFO");
        exit(EXIT_FAILURE);
    }

    // Server message = connect pid
    char pid_msg[256];
    int client_pid = getpid();
    sprintf(pid_msg, "%d%s", client_pid, mode);

    if(write(server_fd, pid_msg, 256) < 0) {
        perror("Failed to write to server FIFO");
        exit(EXIT_FAILURE);
    }

    if (strcmp(mode, "connect") == 0) {
        // Implement continuous connection logic
        // printf("mode = connect\n");
        
    } else if (strcmp(mode, "tryConnect") == 0) {
        // Implement connection request logic
        // printf("mode = tryConnect\n");
    }

    // Create client FIFOs
    char client_fifo_read[32];
    char client_fifo_write[32];
    sprintf(client_fifo_read, "/tmp/client_fifo_read_%d", client_pid);
    sprintf(client_fifo_write, "/tmp/client_fifo_write_%d", client_pid);

    if(mkfifo(client_fifo_read, 0666) < 0) {
        if(errno != EEXIST) {
            perror("Failed to create client FIFO read");
            exit(EXIT_FAILURE);
        }
    }
    
    if(mkfifo(client_fifo_write, 0666) < 0) {
        if(errno != EEXIST) {
            perror("Failed to create client FIFO write");
            exit(EXIT_FAILURE);
        }
    }


    int client_fd_write;
    // int client_fd_read;

    printf("Waiting for Que.. Connection Established...\n");
    

    char *command = (char *)malloc(256);
    while (1) {
        printf("Enter command: ");
        
        if(fgets(command, 256, stdin) == NULL) {
            perror("Failed to read command");
            exit(EXIT_FAILURE);
        }

        // Remove newline character
        command[strcspn(command, "\n")] = 0;

        // Send command to server
        client_fd_write = open(client_fifo_write, O_WRONLY);
        if(client_fd_write < 0) {
            perror("Failed to open client FIFO write");
            exit(EXIT_FAILURE);
        }
        
        if(write(client_fd_write, command, 256) < 0) {
            perror("Failed to write to client FIFO");
            exit(EXIT_FAILURE);
        }

        // If command is "quit", send disconnect request to server
        if(strcmp(command, "quit") == 0) {
            printf("Sending disconnect request: PID %d\n", client_pid);
            printf("waiting for logfile...\n");
            printf("logfile write request granted\n");
            printf("bye\n");
            break;
        }
        

        char *first_word = NULL;
        char *second_word = NULL;
        char *remaining_words = NULL;

        char *space = strchr(command, ' ');
        if (space == NULL)
        {
            first_word = command;
            second_word = NULL;
            remaining_words = NULL;
        }
        else
        {
            first_word = command;
            *space = '\0';
            second_word = space + 1;
            remaining_words = strchr(second_word, ' ');
            if (remaining_words != NULL)
            {
                *remaining_words = '\0';
                remaining_words++;
            }
        }

        
        // Null terminate the first word
        first_word[strlen(first_word)] = '\0';

        // If upload or download, check if file exists, write to server success or fail
        if(strcmp(first_word, "upload") == 0) {
            // Check if file exists
            if(access(second_word, F_OK) == -1) {
                printf("File does not exist\n");

                // Send response to server
                client_fd_write = open(client_fifo_write, O_WRONLY);
                if(client_fd_write < 0) {
                    perror("Failed to open client FIFO write");
                    exit(EXIT_FAILURE);
                }

                char response[64];
                sprintf(response, "error");
                if(write(client_fd_write, response, 64) < 0) {
                    perror("Failed to write to client FIFO");
                    exit(EXIT_FAILURE);
                }

                close(client_fd_write);
                continue;
            }
            else {
                // Send response to server
                client_fd_write = open(client_fifo_write, O_WRONLY);
                if(client_fd_write < 0) {
                    perror("Failed to open client FIFO write");
                    exit(EXIT_FAILURE);
                }

                char response[64];
                sprintf(response, "success");
                if(write(client_fd_write, response, 64) < 0) {
                    perror("Failed to write to client FIFO");
                    exit(EXIT_FAILURE);
                }

                close(client_fd_write);

                handle_upload(second_word, client_fd_write, client_fifo_write);

                
            }
        }
        else if(strcmp(first_word, "download") == 0) {
            // Read success or fail message from server
            char msg[64];

            int client_fd_read = open(client_fifo_read, O_RDONLY);
            if(client_fd_read < 0) {
                perror("Failed to open client FIFO read");
                exit(EXIT_FAILURE);
            }

            if(read(client_fd_read, msg, 64) < 0) {
                perror("Failed to read from client FIFO");
                exit(EXIT_FAILURE);
            }

            if(strcmp(msg, "error") == 0) {
                printf("File does not exist on server\n");
                close(client_fd_read);
                continue;
            }
            else {
                close(client_fd_read);
                handle_download(second_word, client_fd_write, client_fifo_write, client_fifo_read);
            }
        }
        else if(strcmp(first_word, "readF") == 0) {
            // Read success or fail message from server
            char msg[64];

            int client_fd_read = open(client_fifo_read, O_RDONLY);
            if(client_fd_read < 0) {
                perror("Failed to open client FIFO read");
                exit(EXIT_FAILURE);
            }

            if(read(client_fd_read, msg, 64) < 0) {
                perror("Failed to read from client FIFO");
                exit(EXIT_FAILURE);
            }

            if(strcmp(msg, "error") == 0) {
                printf("File does not exist on server\n");
                close(client_fd_read);
                continue;
            }
            else {
                close(client_fd_read);
                handle_readf(remaining_words, client_fd_write, client_fifo_write, client_fifo_read);
            }
        }
        else {
    
            if(strcmp(first_word, "writeT") == 0) {
                // printf("writeT in client\n");
                continue;
            }

            else if(strcmp(first_word, "killServer") == 0) {
                // Sends a kill request to the Server
                // printf("killServer in client\n");

                break;

            }

            else if(strcmp(first_word, "list") == 0 || strcmp(first_word, "help") == 0 || strcmp(first_word, "archServer") == 0){
                int client_fd_read = open(client_fifo_read, O_RDONLY);
                if(client_fd_read < 0) {
                        perror("Failed to open client FIFO read");
                        exit(EXIT_FAILURE);
                }
                char *response = (char *)malloc(256);
                
                if(read(client_fd_read, response, 256) > 0) {
                    printf("%s\n", response);
                }
                free(response);

                close(client_fd_read);
            }

            else {
                printf("Invalid command\n");
            }

        }
                   
    }
    
    // printf("Sent connection request: PID %d\n", client_pid);

    close(server_fd);
    close(client_fd_write);
    free(command);
    //close(client_fd_read);
    // unlink(client_fifo_read);
    // unlink(client_fifo_write);
    return 0;
}

void handle_upload(char *second_word, int client_fd_write, char *client_fifo_write) {

    printf("file transfer request received. Beginning file transfer:\n");

    // Uploads a file to the Server
    char *filename = second_word;

    // Open the file
    int file = open(filename, O_RDONLY);
    if(file < 0) {
        perror("Failed to open file");
        exit(EXIT_FAILURE);
    }

    // Get file size
    struct stat st;
    stat(filename, &st);
    int file_size = st.st_size;

    // Read file content
    char *file_content = (char *)malloc(file_size);
    if(read(file, file_content, file_size) < 0) {
        perror("Failed to read file content");
        exit(EXIT_FAILURE);
    }

    // Close the file
    close(file);

    // Send file size and content to the Server
    client_fd_write = open(client_fifo_write, O_WRONLY);
    if(client_fd_write < 0) {
        perror("Failed to open client FIFO write");
        exit(EXIT_FAILURE);
    }

    if(write(client_fd_write, &file_size, sizeof(int)) < 0) {
        perror("Failed to write file size to client FIFO");
        exit(EXIT_FAILURE);
    }

    if(write(client_fd_write, file_content, file_size) < 0) {
        perror("Failed to write file content to client FIFO");
        exit(EXIT_FAILURE);
    }

    // Free memory
    free(file_content);

    printf("%d bytes transferred\n", file_size);

    // Close the client FIFO
    close(client_fd_write);
            
}

void handle_download(char *second_word, int client_fd_write, char *client_fifo_write, char *client_fifo_read) {

    printf("file download request received. Beginning file download:\n");
                
    // Downloads a file from the Server
    char *filename = second_word;

    // Write file content to a new file
    int file = open(filename, O_WRONLY | O_CREAT | O_TRUNC, 0666);
    if(file < 0) {
        perror("Failed to open file");
        exit(EXIT_FAILURE);
    }

    // Read file size and content from the Server
    int client_fd_read = open(client_fifo_read, O_RDONLY);
    if(client_fd_read < 0) {
        perror("Failed to open client FIFO read");
        exit(EXIT_FAILURE);
    }

    // Read file size
    int file_size;
    if(read(client_fd_read, &file_size, sizeof(int)) < 0) {
        perror("Failed to read file size from client FIFO");
        exit(EXIT_FAILURE);
    }

    // Allocate memory for file content 
    char *file_content = (char *)malloc(file_size);
    // Read file content
    if(read(client_fd_read, file_content, file_size) < 0) {
        perror("Failed to read file content from client FIFO");
        exit(EXIT_FAILURE);
    }

    // Write file content to the new file
    if(write(file, file_content, file_size) < 0) {
        perror("Failed to write file content to file");
        exit(EXIT_FAILURE);
    }

    // Free memory
    free(file_content);

    printf("%d bytes transferred\n", file_size);

    // Close the file
    close(file);

    // Close the client FIFO
    close(client_fd_read);


}

void handle_readf(char *remaining_words, int client_fd_write, char *client_fifo_write, char *client_fifo_read) {

    int client_fd_read = open(client_fifo_read, O_RDONLY);
    if(client_fd_read < 0) {
            perror("Failed to open client FIFO read");
            exit(EXIT_FAILURE);
    }
    // Check if line number is provided
    if(remaining_words != NULL) {
        char *line = (char *)malloc(256);
        //client_fd_read = open(client_fifo_read, O_RDONLY);
        
        if(read(client_fd_read, line, 256) > 0) {
            printf("Server: %s\n", line);
        }
        free(line);
    }
    // no line number 
    else {
        // First read the file size then read the file as 1d array                    
        // Read file size
        int file_size;
        if(read(client_fd_read, &file_size, sizeof(int)) < 0) {
            perror("Failed to read file size from client FIFO");
            exit(EXIT_FAILURE);
        }
        // Allocate memory for file content
        char *file_content = (char *)malloc(file_size);
        // Read file content
        if(read(client_fd_read, file_content, file_size) < 0) {
            perror("Failed to read file content from client FIFO");
            exit(EXIT_FAILURE);
        }
        // Print file content
        printf("File Content:\n%s\n", file_content);
        // Free memory
        free(file_content);
    }

    close(client_fd_read);
}