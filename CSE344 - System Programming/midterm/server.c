#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <dirent.h>
#include <signal.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <semaphore.h>
#include <errno.h>


#define SERVER_FIFO "/tmp/server_fifo" // Server FIFO path

// typedef struct
// {
//     int client_pid;
//     int client_num;
// } ClientInfo;

// ClientInfo *clients;

pid_t clientPids[10];
int gl_client_pid; // Global variable to store client PID to be used in signal handler
int numClients = 0;
int client_num = 0;

void handle_client(char *client_fifo_read, char *client_fifo_write, int client_pid, int log_fd, char *dirname, sem_t *serverSemIO, int forked_pid, int client_num);
void execute_upload(char *second_word, char *client_fifo_read, char *client_fifo_write, char *dirname, int client_fd_write);
void execute_download(char *second_word, char *client_fifo_read, char *client_fifo_write, char *dirname, int client_fd_write, int client_fd_read);
void execute_readf(char *second_word, char *remaining_words, char *client_fifo_read, char *client_fifo_write, int client_pid, int log_fd, int client_fd_read);


sem_t *makeSem(const char *name, int serverPID)
{
    char *semName = (char *)malloc(64);
    sprintf(semName, "%s%d", name, serverPID);
    sem_t *sem = sem_open(semName, O_CREAT, 0666, 1);
    if (sem == SEM_FAILED)
    {
        perror("Failed to create semaphore");
        return NULL;
    }
    return sem;
}


void sigint_handler(int signum)
{
    printf("Received SIGINT signal\n");

    // Unlink server FIFO
    unlink(SERVER_FIFO);

    // Kill all child processes
    int s;
    s = kill(0, SIGINT);
    if (s == -1)
    {
        perror("kill in sigint_handler");
        exit(EXIT_FAILURE);
    }

    exit(EXIT_SUCCESS);
}

void sigint_handler_ch(int signum)
{
    printf("Received SIGINT signal in child process\n");

    // Send kill signal to client
    int s;
    if(gl_client_pid > 0) {
        s = kill(gl_client_pid, SIGINT);
        if (s == -1)
        {
            perror("kill in sigint_handler_ch");
            exit(EXIT_FAILURE);
        }
    }
    exit(EXIT_SUCCESS);
}

void sigchld_handler(int signum)
{
    printf("Received SIGCHLD signal\n");

    // Decrease number of clients
    numClients--;
}


int main(int argc, char *argv[])
{   

    // SIGINT handler
    struct sigaction sa;
    sa.sa_handler = sigint_handler;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = 0;
    if (sigaction(SIGINT, &sa, NULL) == -1)
    {
        perror("sigaction");
        exit(EXIT_FAILURE);
    }

    // SIGCHLD handler
    struct sigaction sa_chld;
    sa_chld.sa_handler = sigchld_handler;
    sigemptyset(&sa_chld.sa_mask);
    sa_chld.sa_flags = 0;
    if (sigaction(SIGCHLD, &sa_chld, NULL) == -1)
    {
        perror("sigaction");
        exit(EXIT_FAILURE);
    }
    
    // Check for correct number of arguments
    if (argc != 3)
    {
        fprintf(stderr, "Usage: %s <dirname> <max.#ofClients>\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    char *dirname = argv[1];
    int maxClients = atoi(argv[2]);
    // int client_num = 0;

    // Create server directory
    mkdir(dirname, 0755);
    chdir(dirname);

    // Open log file
    int log_fd = open("log.txt", O_WRONLY | O_CREAT, 0666);
    if (log_fd < 0)
    {
        perror("Failed to open log file");
        exit(EXIT_FAILURE);
    }

    // Create server FIFO
    if(mkfifo(SERVER_FIFO, 0666) < 0) {
        if(errno != EEXIST) {
            perror("Failed to create server FIFO");
            exit(EXIT_FAILURE);
        }
    }

    // Open server FIFO
    int server_fd = open(SERVER_FIFO, O_RDWR);
    if (server_fd < 0)
    {
        perror("Failed to open server FIFO");
        exit(EXIT_FAILURE);
    }

    int serverPID = getpid();

    // Create semaphore for IO operations
    sem_t *serverSemIO = makeSem("serverSemIO", serverPID);
    if(serverSemIO == NULL || serverSemIO < 0) {
        exit(EXIT_FAILURE);
    }

    printf("Server Started PID %d…\n", getpid());
    printf("waiting for clients...\n");

    // Write server PID to log file
    char *server_pid_log = (char *)malloc(256);
    sprintf(server_pid_log, "Server PID: %d\n", serverPID);
    if(write(log_fd, server_pid_log, strlen(server_pid_log)) < 0) {
        perror("Failed to write to log file");
        exit(EXIT_FAILURE);
    }
    free(server_pid_log);
    
    // I used a while loop to keep the server running and accepting clients
    while (1)
    {
        // char client_pid[32];
        char fifo_message[256];
        int client_pid = -1;

        if (read(server_fd, fifo_message, 256) > 0)
        {

            char *connect_mode = &fifo_message[32];
            sscanf(fifo_message, "%d%s", &client_pid, connect_mode);

            if (numClients < maxClients)
            {
                pid_t pid = fork();
                clientPids[numClients] = pid;
                client_num++;

                // Client PID is stored in global variable
                if(pid != 0 || pid != -1) {
                    gl_client_pid = client_pid;
                }

                // printf("forked pid: %d\n", pid);
                int forked_pid = pid;

                if (pid == 0)
                {
                    // Sigint handler for child process
                    struct sigaction sa_ch;
                    sa_ch.sa_handler = sigint_handler_ch;
                    sigemptyset(&sa_ch.sa_mask);
                    sa_ch.sa_flags = 0;
                    if (sigaction(SIGINT, &sa_ch, NULL) == -1)
                    {
                        perror("sigaction");
                        exit(EXIT_FAILURE);
                    }

                    // // sigusr1 handler for child process
                    // struct sigaction sa_ch2;
                    // sa_ch2.sa_handler = sigint_handler_ch2;
                    // sigemptyset(&sa_ch2.sa_mask);
                    // sa_ch2.sa_flags = 0;
                    // if (sigaction(SIGUSR1, &sa_ch2, NULL) == -1)
                    // {
                    //     perror("sigaction");
                    //     exit(EXIT_FAILURE);
                    // }
                    
                    char client_fifo_read[32];
                    char client_fifo_write[32];
                    sprintf(client_fifo_read, "/tmp/client_fifo_read_%d", client_pid);
                    sprintf(client_fifo_write, "/tmp/client_fifo_write_%d", client_pid);

                    // printf("client pid: %d\n", client_pid);

                    handle_client(client_fifo_read, client_fifo_write, client_pid, log_fd, dirname, serverSemIO, forked_pid, client_num);
                    
                    exit(EXIT_SUCCESS);
                }
                else
                {
                    // Parent process
                    printf("Client PID %d connected as client%d\n", client_pid, client_num);
                    numClients++;
                    
                    // Write client PID to log file
                    char *client_pid_log = (char *)malloc(256);
                    sprintf(client_pid_log, "Client %d connected.\n", client_pid);
                    if(write(log_fd, client_pid_log, strlen(client_pid_log)) < 0) {
                        perror("Failed to write to log file");
                        exit(EXIT_FAILURE);
                    }
                    free(client_pid_log);

                }
            }

            else
            {
                printf("Connection request PID %d... Que FULL\n", client_pid);
                
                // Write max number of clients reached to log file
                char *max_clients_log = (char *)malloc(256);
                sprintf(max_clients_log, "Max number of clients reached.\n");
                if(write(log_fd, max_clients_log, strlen(max_clients_log)) < 0) {
                    perror("Failed to write to log file");
                    exit(EXIT_FAILURE);
                }
                free(max_clients_log);

                // // Send signal to server to kill itself
                // int s;
                // s = kill(0, SIGINT);
                // if (s == -1)
                // {
                //     perror("kill in max clients");
                //     exit(EXIT_FAILURE);
                // }

                // break;
            }
        }
    }

    close(server_fd);
    unlink(SERVER_FIFO);
    close(log_fd);
    return 0;
}

void handle_client(char *client_fifo_read, char *client_fifo_write, int client_pid, int log_fd, char *dirname, sem_t *serverSemIO, int forked_pid, int client_num)
{

    int client_fd_read;
    int client_fd_write;

    char message[256];
    while (1)
    {
        // Read message from client
        client_fd_write = open(client_fifo_write, O_RDONLY);
        if (client_fd_write < 0)
        {
            perror("Failed to open client FIFO read");
            exit(EXIT_FAILURE);
        }
        if (read(client_fd_write, message, 256) > 0)
        {
            // printf("Client %d: %s\n", client_pid, message);
        }

        

        char *first_word;
        char *second_word;
        char *remaining_words;

        char *space = strchr(message, ' ');
        if (space == NULL)
        {
            first_word = message;
            second_word = NULL;
            remaining_words = NULL;
        }
        else
        {
            first_word = message;
            *space = '\0';
            second_word = space + 1;
            remaining_words = strchr(second_word, ' ');
            if (remaining_words != NULL)
            {
                *remaining_words = '\0';
                remaining_words++;
            }
        }

        // printf("First word: %s\n", first_word);
        // printf("Second word: %s\n", second_word);
        // printf("Remaining words: %s\n", remaining_words);

        // If upload, download or readF command is received, check if file exists in server directory
        if(strcmp(first_word, "upload") == 0) {
            // Read success or fail message from client
            char msg[64];

            if (read(client_fd_write, msg, 64) > 0)
            {
                // printf("Client %d: %s\n", client_pid, msg);
                if(strcmp(msg, "success") == 0) {
                    
                    // Write success message to log file
                    char *success_log = (char *)malloc(256);
                    sprintf(success_log, "Client %d successfully uploaded file %s\n", client_pid, second_word);
                    if(write(log_fd, success_log, strlen(success_log)) < 0) {
                        perror("Failed to write to log file");
                        exit(EXIT_FAILURE);
                    }
                    free(success_log);

                    // Execute upload
                    sem_wait(serverSemIO);
                    execute_upload(second_word, client_fifo_read, client_fifo_write, dirname, client_fd_write);
                    sem_post(serverSemIO);

                }
                else {
                    // Write fail message to log file
                    char *fail_log = (char *)malloc(256);
                    sprintf(fail_log, "Client %d failed to upload file %s\n", client_pid, second_word);
                    if(write(log_fd, fail_log, strlen(fail_log)) < 0) {
                        perror("Failed to write to log file");
                        exit(EXIT_FAILURE);
                    }
                    free(fail_log);
                    continue;
                }
            }
            
        }
        else if(strcmp(first_word, "download") == 0) {
            // Check if file exists in server directory
            if(access(second_word, F_OK) == -1) {
                printf("File does not exist\n");

                // Send response to server
                client_fd_read = open(client_fifo_read, O_WRONLY);
                if(client_fd_read < 0) {
                    perror("Failed to open client FIFO write");
                    exit(EXIT_FAILURE);
                }

                char response[64];
                sprintf(response, "error");
                if(write(client_fd_read, response, 64) < 0) {
                    perror("Failed to write to client FIFO");
                    exit(EXIT_FAILURE);
                }

                // Write error message to log file
                char *error_log = (char *)malloc(256);
                sprintf(error_log, "Client %d failed to download file %s\n", client_pid, second_word);
                if(write(log_fd, error_log, strlen(error_log)) < 0) {
                    perror("Failed to write to log file");
                    exit(EXIT_FAILURE);
                }
                free(error_log);

                close(client_fd_read);
                continue;
            }
            else {
                // Send response to client
                client_fd_read = open(client_fifo_read, O_WRONLY);
                if(client_fd_read < 0) {
                    perror("Failed to open client FIFO write");
                    exit(EXIT_FAILURE);
                }

                char response[64];
                sprintf(response, "success");
                if(write(client_fd_read, response, 64) < 0) {
                    perror("Failed to write to client FIFO");
                    exit(EXIT_FAILURE);
                }

                // Write success message to log file
                char *success_log = (char *)malloc(256);
                sprintf(success_log, "Client %d successfully downloaded file %s\n", client_pid, second_word);
                if(write(log_fd, success_log, strlen(success_log)) < 0) {
                    perror("Failed to write to log file");
                    exit(EXIT_FAILURE);
                }
                free(success_log);

                sem_wait(serverSemIO);
                execute_download(second_word, client_fifo_read, client_fifo_write, dirname, client_fd_write, client_fd_read);
                sem_post(serverSemIO);
                
            }
        }
        else if(strcmp(first_word, "readF") == 0) {
            // Check if file exists in server directory
            if(access(second_word, F_OK) == -1) {
                printf("File does not exist\n");

                // Send response to server
                client_fd_read = open(client_fifo_read, O_WRONLY);
                if(client_fd_read < 0) {
                    perror("Failed to open client FIFO write");
                    exit(EXIT_FAILURE);
                }

                char response[64];
                sprintf(response, "error");
                if(write(client_fd_read, response, 64) < 0) {
                    perror("Failed to write to client FIFO");
                    exit(EXIT_FAILURE);
                }

                // Write error message to log file
                char *error_log = (char *)malloc(256);
                sprintf(error_log, "Client %d failed to read file %s\n", client_pid, second_word);
                if(write(log_fd, error_log, strlen(error_log)) < 0) {
                    perror("Failed to write to log file");
                    exit(EXIT_FAILURE);
                }
                free(error_log);

                close(client_fd_read);
                continue;
            }
            else {
                // Send response to client
                client_fd_read = open(client_fifo_read, O_WRONLY);
                if(client_fd_read < 0) {
                    perror("Failed to open client FIFO write");
                    exit(EXIT_FAILURE);
                }

                char response[64];
                sprintf(response, "success");
                if(write(client_fd_read, response, 64) < 0) {
                    perror("Failed to write to client FIFO");
                    exit(EXIT_FAILURE);
                }

                // Write success message to log file
                char *success_log = (char *)malloc(256);
                sprintf(success_log, "Client %d successfully read file %s\n", client_pid, second_word);
                if(write(log_fd, success_log, strlen(success_log)) < 0) {
                    perror("Failed to write to log file");
                    exit(EXIT_FAILURE);
                }
                free(success_log);

                sem_wait(serverSemIO);
                execute_readf(second_word, remaining_words, client_fifo_read, client_fifo_write, client_pid, log_fd, client_fd_read);
                sem_post(serverSemIO);

                //void execute_readf(char *second_word, char *remaining_words, char *client_fifo_read, char *client_fifo_write, int client_pid, int log_fd, int client_fd_read)
                
            }
        }
        else {
            if (strcmp(first_word, "quit") == 0)
            {
                printf("Client %d disconnected\n", client_pid);
                
                // Write client disconnected message to log file
                char *client_disconnected_log = (char *)malloc(256);
                sprintf(client_disconnected_log, "Client %d disconnected.\n", client_pid);
                if(write(log_fd, client_disconnected_log, strlen(client_disconnected_log)) < 0) {
                    perror("Failed to write to log file");
                    exit(EXIT_FAILURE);
                }
                free(client_disconnected_log);

                numClients--;
                // printf("Number of clients: %d\n", numClients);

                // // Send signal to signal_handler_ch2
                // int s;
                // s = kill(forked_pid, SIGUSR1);
                // if (s == -1)
                // {
                //     perror("kill in quit");
                //     exit(EXIT_FAILURE);
                // }

                break;
            }

            else if (strcmp(first_word, "list") == 0)
            {
                sem_wait(serverSemIO);
                // Server sends list of files to client
                DIR *dir;
                struct dirent *ent;
                if ((dir = opendir(".")) != NULL)
                {
                    char file_names[256][256];
                    int num_files = 0;

                    while ((ent = readdir(dir)) != NULL)
                    {
                        if (ent->d_type == DT_REG)
                        {
                            strcpy(file_names[num_files], ent->d_name);
                            num_files++;
                        }
                    }
                    closedir(dir);

                    // Create a string with file names separated by newline
                    char file_names_string[256];
                    strcpy(file_names_string, file_names[0]);
                    for (int i = 1; i < num_files; i++)
                    {
                        strcat(file_names_string, "\n");
                        strcat(file_names_string, file_names[i]);
                    }

                    client_fd_read = open(client_fifo_read, O_WRONLY);
                    if (client_fd_read < 0)
                    {
                        perror("Failed to open client FIFO write");
                        exit(EXIT_FAILURE);
                    }

                    if (write(client_fd_read, file_names_string, 256) < 0)
                    {
                        perror("Failed to write to client FIFO");
                        exit(EXIT_FAILURE);
                    }

                    // Write success message to log file
                    char *success_log = (char *)malloc(256);
                    sprintf(success_log, "Client %d successfully listed files\n", client_pid);
                    if(write(log_fd, success_log, strlen(success_log)) < 0) {
                        perror("Failed to write to log file");
                        exit(EXIT_FAILURE);
                    }
                    free(success_log);
                }
                else
                {
                    perror("Failed to open directory");
                    exit(EXIT_FAILURE);
                }

                sem_post(serverSemIO);
            }

            else if (strcmp(first_word, "help") == 0)
            {
                client_fd_read = open(client_fifo_read, O_WRONLY);
                if (client_fd_read < 0) {
                    perror("Failed to open client FIFO write");
                    exit(EXIT_FAILURE);
                }
                
                if(second_word == NULL || second_word[0] == '\0') {
                    char commands[256] = "help, list, readF, writeT, upload, download, archServer, quit, killServer";
                    if (write(client_fd_read, commands, 256) < 0)
                    {
                        perror("Failed to write to client FIFO");
                        exit(EXIT_FAILURE);
                    }
                }

                else if(strcmp(second_word, "list") == 0) {
                    // Server sends list of commands to client
                    char commands[256] = "list\nsends a request to display the list of files in Servers directory (also displays the list received from the Server)";

                    if (write(client_fd_read, commands, 256) < 0)
                    {
                        perror("Failed to write to client FIFO");
                        exit(EXIT_FAILURE);
                    }
                }

                else if(strcmp(second_word, "readF") == 0) {
                    // Server sends list of commands to client
                    char commands[256] = "readF <file> <line #> \ndisplay the #th line of the <file>, returns with an error if <file> does not exists";
                    
                    if (write(client_fd_read, commands, 256) < 0)
                    {
                        perror("Failed to write to client FIFO");
                        exit(EXIT_FAILURE);
                    }
                }

                else if(strcmp(second_word, "writeT") == 0) {
                    // Server sends list of commands to client
                    char commands[256] = "writeT <file> <line #> <string>\nrequest to write the content of “string” to the #th line the <file>, \nif the line # is not given writes to the end of file. If the file does not exists in Servers directory creates and edits the file at the same time";

                    if (write(client_fd_read, commands, 256) < 0)
                    {
                        perror("Failed to write to client FIFO");
                        exit(EXIT_FAILURE);
                    }
                }

                else if(strcmp(second_word, "upload") == 0) {
                    // Server sends list of commands to client
                    char commands[256] = "upload <file>\nuploads the file from the current working directory of client to the Servers directory (beware of the cases no file in clients current working directory and file with the same name on Servers side)";

                    if (write(client_fd_read, commands, 256) < 0)
                    {
                        perror("Failed to write to client FIFO");
                        exit(EXIT_FAILURE);
                    }
                }

                else if(strcmp(second_word, "download") == 0) {
                    // Server sends list of commands to client
                    char commands[256] = "download <file>\nrequest to receive <file> from Servers directory to client side";

                    if (write(client_fd_read, commands, 256) < 0)
                    {
                        perror("Failed to write to client FIFO");
                        exit(EXIT_FAILURE);
                    }
                }

                else if(strcmp(second_word, "archServer") == 0) {
                    // Server sends list of commands to client
                    char commands[256] = "archServer <fileName>.tar\nUsing fork, exec and tar utilities create a child process that will collect all the files currently available on the the Server side and store them in the <filename>.tar archive";

                    if (write(client_fd_read, commands, 256) < 0)
                    {
                        perror("Failed to write to client FIFO");
                        exit(EXIT_FAILURE);
                    }
                }

                else if(strcmp(second_word, "quit") == 0) {
                    // Server sends list of commands to client
                    char commands[256] = "quit\nSend write request to Server side log file and quits";

                    if (write(client_fd_read, commands, 256) < 0)
                    {
                        perror("Failed to write to client FIFO");
                        exit(EXIT_FAILURE);
                    }
                }

                else if(strcmp(second_word, "killServer") == 0) {
                    // Server sends list of commands to client
                    char commands[256] = "killServer\nSends a kill request to the Server";

                    if (write(client_fd_read, commands, 256) < 0)
                    {
                        perror("Failed to write to client FIFO");
                        exit(EXIT_FAILURE);
                    }
                }

                // Write success message to log file
                char *success_log = (char *)malloc(256);
                sprintf(success_log, "Client %d successfully listed commands\n", client_pid);
                if(write(log_fd, success_log, strlen(success_log)) < 0) {
                    perror("Failed to write to log file");
                    exit(EXIT_FAILURE);
                }
                free(success_log);

            }

            else if(strcmp(first_word, "writeT") == 0){
                sem_wait(serverSemIO);
                // Server writes to file with given string writeT <file> <line #> <string>
                char *filename = second_word;
                char *first = remaining_words;
                char *remaining;

                // Parse the remaining words to get line number and string with strchr
                char *space2 = strchr(first, ' ');
                if (space2 != NULL)
                {
                    *space2 = '\0';
                    remaining = space2 + 1;
                }

                if(remaining == NULL || remaining[0] == '\0') {
                    // printf("No line number given\n");
                    // printf("string: %s\n", first);

                    // If line number is not given, write to the end of the file
                    // If file does not exist, create and write to the file
                    int file_fd = open(filename, O_WRONLY | O_APPEND);
                    if (file_fd < 0) {
                        // File does not exist, create and write to the file
                        file_fd = open(filename, O_WRONLY | O_CREAT | O_TRUNC, 0644);
                        if (file_fd < 0) {
                            perror("Failed to create file");
                            exit(EXIT_FAILURE);
                        }
                    }

                    if (write(file_fd, first, strlen(first)) < 0) {
                        perror("Failed to write to file");
                        exit(EXIT_FAILURE);
                    }
                    if (write(file_fd, "\n", 1) < 0) {
                        perror("Failed to write to file");
                        exit(EXIT_FAILURE);
                    }

                    close(file_fd);
                }

                else {
                    // printf("Line number given\n");
                    // printf("string: %s\n", remaining);
                    int line_num = atoi(first);

                    // if(line_num <= 0) {
                    //     perror("Invalid line number");
                    //     exit(EXIT_FAILURE);
                    // }
                    // else {
                    //     printf("Valid line number\n");
                    //     line_num--; // Convert to 0-based index
                    // }

                    // Write the content of “string” to the #th line the <file>
                    int file_fd = open(filename, O_RDONLY);
                    if (file_fd < 0) {
                        // File does not exist, create and read from the file
                        file_fd = open(filename, O_RDONLY | O_CREAT, 0644);
                        if (file_fd < 0) {
                            perror("Failed to open file");
                            exit(EXIT_FAILURE);
                        }
                    }

                    // Read the file contents
                    char *file_contents = (char *)malloc(256);
                    int bytes_read = 0;
                    if((bytes_read = read(file_fd, file_contents, 256)) < 0) {
                        perror("Failed to read file contents");
                        exit(EXIT_FAILURE);
                    }

                    // Close the file
                    close(file_fd);

                    // Open the file for writing
                    file_fd = open(filename, O_WRONLY | O_TRUNC);
                    if (file_fd < 0) {
                        perror("Failed to open file");
                        exit(EXIT_FAILURE);
                    }

                    // Write the file contents back to the file with the new line
                    int line_count = 1;
                    int i = 0;
                    while(i < bytes_read) {
                        if(file_contents[i] == '\n') {
                            line_count++;
                        }
                        if(line_count == line_num) {
                            break;
                        }
                        i++;
                    }

                    if(line_count < line_num) {
                        // Line number is greater than the number of lines in the file
                        // Write the file contents back to the file
                        if(write(file_fd, file_contents, bytes_read) < 0) {
                            perror("Failed to write to file");
                            exit(EXIT_FAILURE);
                        }
                        if(write(file_fd, "\n", 1) < 0) {
                            perror("Failed to write to file");
                            exit(EXIT_FAILURE);
                        }
                    }
                    else {
                        // Line number is less than the number of lines in the file
                        // Write the file contents back to the file until the line number
                        int j = 0;
                        while(j < i) {
                            if(write(file_fd, &file_contents[j], 1) < 0) {
                                perror("Failed to write to file");
                                exit(EXIT_FAILURE);
                            }
                            j++;
                        }
                        // Write the new line
                        if(write(file_fd, "\n", 1) < 0) {
                            perror("Failed to write to file");
                            exit(EXIT_FAILURE);
                        }
                        // Write the new string
                        if(write(file_fd, remaining, strlen(remaining)) < 0) {
                            perror("Failed to write to file");
                            exit(EXIT_FAILURE);
                        }
                        
                        // Write the rest of the file contents back to the file
                        j = i;
                        while(j < bytes_read) {
                            if(write(file_fd, &file_contents[j], 1) < 0) {
                                perror("Failed to write to file");
                                exit(EXIT_FAILURE);
                            }
                            j++;
                        }
                    }

                    close(file_fd);

                    free(file_contents);


                }
                sem_post(serverSemIO);

                // Write success message to log file
                char *success_log = (char *)malloc(256);
                sprintf(success_log, "Client %d successfully wrote to file %s\n", client_pid, second_word);
                if(write(log_fd, success_log, strlen(success_log)) < 0) {
                    perror("Failed to write to log file");
                    exit(EXIT_FAILURE);
                }
                free(success_log);
            }

            else if(strcmp(first_word, "archServer") == 0) {
                sem_wait(serverSemIO);
                char *filename = second_word;

                // Create a tar archive of the files in the server directory
                pid_t pid = fork();
                if(pid < 0) {
                    perror("Failed to fork");
                    exit(EXIT_FAILURE);
                }

                if(pid == 0) {
                    // Child process
                    pid_t pid2 = fork();
                    if(pid2 < 0) {
                        perror("Failed to fork");
                        exit(EXIT_FAILURE);
                    }
                    else if(pid2 == 0) {
                        char *tarBuffer = (char *)malloc(256);
                        sprintf(tarBuffer, "tar -czf /tmp/%s.tar.gz .", filename);

                        int ret = execl("/bin/sh", "sh", "-c", tarBuffer, (char *)0);
                        if(ret < 0) {
                            perror("Failed to execute tar command");
                            exit(EXIT_FAILURE);
                        }

                        free(tarBuffer);
                        exit(EXIT_SUCCESS);
                    }

                    else {
                        int status;
                        waitpid(pid2, &status, 0);
                        if(WIFEXITED(status)) {
                            if(WEXITSTATUS(status) == 0) {
                                char *moveBuffer = (char *)malloc(256);
                                sprintf(moveBuffer, "mv /tmp/%s.tar.gz .", filename);

                                int ret = execl("/bin/sh", "sh", "-c", moveBuffer, (char *)0);
                                if(ret < 0) {
                                    perror("Failed to execute move command");
                                    exit(EXIT_FAILURE);
                                }

                                free(moveBuffer);
                                exit(EXIT_SUCCESS);
                            }
                            else {
                                perror("Failed to create tar archive");
                                exit(EXIT_FAILURE);
                            }
                        }
                        
                    }
                }
                else {
                    int status;
                    waitpid(pid, &status, 0);
                    if(WIFEXITED(status)) {
                        if(WEXITSTATUS(status) == 0) {
                            // Send success message to client
                            client_fd_read = open(client_fifo_read, O_WRONLY);
                            if (client_fd_read < 0) {
                                perror("Failed to open client FIFO write");
                                exit(EXIT_FAILURE);
                            }

                            char *success_message = (char *)malloc(256);
                            sprintf(success_message, "Successfully created tar archive %s.tar.gz", filename);
                            if (write(client_fd_read, success_message, 256) < 0)
                            {
                                perror("Failed to write to client FIFO");
                                exit(EXIT_FAILURE);
                            }

                            if(write(log_fd, success_message, strlen(success_message)) < 0) {
                                perror("Failed to write to log file");
                                exit(EXIT_FAILURE);
                            }

                            free(success_message);
                            
                        }
                        else {
                            // Send error message to client
                            client_fd_read = open(client_fifo_read, O_WRONLY);
                            if (client_fd_read < 0) {
                                perror("Failed to open client FIFO write");
                                exit(EXIT_FAILURE);
                            }

                            char *error_message = (char *)malloc(256);
                            sprintf(error_message, "Failed to create tar archive %s.tar.gz", filename);
                            if (write(client_fd_read, error_message, 256) < 0)
                            {
                                perror("Failed to write to client FIFO");
                                exit(EXIT_FAILURE);
                            }

                            if(write(log_fd, error_message, strlen(error_message)) < 0) {
                                perror("Failed to write to log file");
                                exit(EXIT_FAILURE);
                            }

                            free(error_message);

                        }
                    }
                }
                sem_post(serverSemIO);
                
            }


            else if(strcmp(first_word, "killServer") == 0){
                // Server sends kill signal to all clients (server child processes) and kills itself

                printf("kill signal from client%d.. terminating server\n", client_num); 
                printf("bye"); 

                // Write kill message to log file
                char *kill_log = (char *)malloc(256);
                sprintf(kill_log, "Server killed\n");
                if(write(log_fd, kill_log, strlen(kill_log)) < 0) {
                    perror("Failed to write to log file");
                    exit(EXIT_FAILURE);
                }
                free(kill_log);

                // Send signal to all clients
                int s;
                s = kill(0, SIGINT);    
                if (s == -1)
                {
                    perror("kill inside killserver");
                    exit(EXIT_FAILURE);
                }

            }

            else {
                printf("Invalid command\n");
            }
        }

    }

    close(client_fd_read);
    close(client_fd_write);
    unlink(client_fifo_read);
    unlink(client_fifo_write);
}

void execute_upload(char *second_word, char *client_fifo_read, char *client_fifo_write, char *dirname, int client_fd_write) {
    // upload <file> uploads the file from the current working directory of client to the Servers directory

    char *filename = second_word;
    if(filename == NULL || filename[0] == '\0') {
        perror("No file name provided");
        exit(EXIT_FAILURE);
    }
    // printf("File name: %s\n", filename);

    char *file_path = (char *)malloc(sizeof(char) * 256);
    sprintf(file_path, "%s/%s", dirname, filename);

    // printf("File path: %s\n", file_path);

    // Open or create the file for writing
    int file_fd = open(filename, O_WRONLY | O_CREAT, 0666);
    if(file_fd < 0) {
        perror("Failed to open file");
        // exit(EXIT_FAILURE);

        // Send error message to client
    }

    // Read the file contents from the client
    int bytes_read = 0;

    client_fd_write = open(client_fifo_write, O_RDONLY);
    if (client_fd_write < 0) {
        perror("Failed to open client FIFO read");
        exit(EXIT_FAILURE);
    }

    // Read file size
    int file_size;
    if (read(client_fd_write, &file_size, sizeof(int)) < 0) {
        perror("Failed to read file size from client FIFO");
        exit(EXIT_FAILURE);
    }

    // Read file contents
    char *file_contents = (char *)malloc(file_size);
    if((bytes_read = read(client_fd_write, file_contents, file_size)) < 0) {
        perror("Failed to read file contents from client FIFO");
        exit(EXIT_FAILURE);
    }

    // Write the file contents to the file
    if(write(file_fd, file_contents, file_size) < 0) {
        perror("Failed to write file contents to file");
        exit(EXIT_FAILURE);
    }

    close(file_fd);
    free(file_contents);

    // Send success message to client    
}

void execute_download(char *second_word, char *client_fifo_read, char *client_fifo_write, char *dirname, int client_fd_write, int client_fd_read) {
                        
    // download <file> downloads the file from the Servers directory to the current working directory of client
    char *filename = second_word;
    if(filename == NULL || filename[0] == '\0') {
        perror("No file name provided");
        exit(EXIT_FAILURE);
    }

    char *file_path = (char *)malloc(sizeof(char) * 256);
    sprintf(file_path, "%s/%s", dirname, filename);


    // Open the file for reading
    int file_fd = open(filename, O_RDONLY);
    if(file_fd < 0) {
        perror("Failed to open file");
        // exit(EXIT_FAILURE);
        return;
    }

    // Get file size
    struct stat st;
    stat(filename, &st);
    int file_size = st.st_size;

    // Send file size to client
    client_fd_read = open(client_fifo_read, O_WRONLY);
    if (client_fd_read < 0) {
        perror("Failed to open client FIFO write");
        exit(EXIT_FAILURE);
    }

    if (write(client_fd_read, &file_size, sizeof(int)) < 0) {
        perror("Failed to write file size to client FIFO");
        exit(EXIT_FAILURE);
    }

    // Read file contents
    char *file_contents = (char *)malloc(file_size);
    if(read(file_fd, file_contents, file_size) < 0) {
        perror("Failed to read file contents from file");
        exit(EXIT_FAILURE);
    }

    // Write file contents to client
    if(write(client_fd_read, file_contents, file_size) < 0) {
        perror("Failed to write file contents to client FIFO");
        exit(EXIT_FAILURE);
    }

    close(file_fd);
    free(file_contents);
                        
}



void execute_readf(char *second_word, char *remaining_words, char *client_fifo_read, char *client_fifo_write, int client_pid, int log_fd, int client_fd_read) {

    //sem_wait(serverSemIO);
    char *filename = second_word;
    
    // No line number given
    if(remaining_words == NULL || remaining_words[0] == '\0') {
        // printf("No line number given\n");
        FILE *file = fopen(filename, "r");
        if(file == NULL) {
            perror("Failed to open file");
            exit(EXIT_FAILURE);
        }

        char *line = (char *)malloc(64);
        memset(line, 0, 64);
        char *file_contents = (char *)malloc(64); // Assuming initial file size of 64
        memset(file_contents, 0, 64);
        int line_count = 0;
        size_t  file_size = 64;
        size_t  file_length = 0;

        while(fgets(line, 64, file) != NULL) {
            size_t line_length = strlen(line);
            if (file_length + line_length + 1 >= file_size) { // Check if there is enough space for new line + '\n'
                size_t new_size = file_size * 2;
                char *new_ptr = realloc(file_contents, new_size);
                if (!new_ptr) {
                    perror("realloc failed");
                    free(line);
                    free(file_contents);
                    exit(EXIT_FAILURE);
                }
                file_contents = new_ptr;
                file_size = new_size;
            }
            strcat(file_contents, line);
            file_length += line_length;
            
        }

        fclose(file);
        
        client_fd_read = open(client_fifo_read, O_WRONLY);
        if (client_fd_read < 0) {
            perror("Failed to open client FIFO write");
            exit(EXIT_FAILURE);
        }

        if (write(client_fd_read, &file_size, sizeof(int)) < 0)
        {
            perror("Failed to write file size to client FIFO");
            exit(EXIT_FAILURE);
        }

        if (write(client_fd_read, file_contents, file_size) < 0)
        {
            perror("Failed to write file contents to client FIFO");
            exit(EXIT_FAILURE);
        }

        free(line);
        free(file_contents);
    }

    // Line number given
    else {
        // printf("Line number given\n");
        int line_num = atoi(remaining_words);
        line_num = line_num - 1;
        FILE *file = fopen(filename, "r");
        if(file == NULL) {
            perror("Failed to open file");
            exit(EXIT_FAILURE);
        }

        char line[256];
        int i = 0;

        memset(line, 0, 256);
        while(fgets(line, 256, file) != NULL) {
            if(i == line_num) {
                break;
            }
            i++;
        }

        if(i == line_num) {
            client_fd_read = open(client_fifo_read, O_WRONLY);
            if (client_fd_read < 0) {
                perror("Failed to open client FIFO write");
                exit(EXIT_FAILURE);
            }
            
            if (write(client_fd_read, line, 256) < 0)
            {
                perror("Failed to write to client FIFO");
                exit(EXIT_FAILURE);
            }
        }

        fclose(file);
    }
    //sem_post(serverSemIO);

    // Write success message to log file
    char *success_log = (char *)malloc(256);
    sprintf(success_log, "Client %d successfully read file %s\n", client_pid, second_word);
    if(write(log_fd, success_log, strlen(success_log)) < 0) {
        perror("Failed to write to log file");
        exit(EXIT_FAILURE);
    }
    free(success_log);
}