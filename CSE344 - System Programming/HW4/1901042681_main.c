#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <dirent.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/time.h>
#include <signal.h>

#define MAX_PATH 1024
#define MAX_BUFFER 100

typedef struct {
    int src_fd;                     // Source file descriptor
    int dest_fd;                    // Destination file descriptor
    char src_name[MAX_PATH];        // Source file path
    char dest_name[MAX_PATH];       // Destination file path
} file_info_t;

typedef struct {
    file_info_t buffer[MAX_BUFFER]; // Buffer to store file info
    int in;                         // Index to insert a file info
    int out;                        // Index to remove a file info
    int count;                      // Number of file info in the buffer
    pthread_mutex_t mutex;          // Mutex to protect the buffer
    pthread_cond_t not_empty;       // Condition variable to signal that the buffer is not empty
    pthread_cond_t not_full;        // Condition variable to signal that the buffer is not full
    int done;                       // Flag to indicate that the manager thread has finished
} buffer_t;

// Global variables
buffer_t buffer;
int num_workers;
int buffer_size;
unsigned long total_bytes_copied = 0;
int num_regular_files = 0;
int num_fifo_files = 0;
int num_directories = 0;
int sigint_received = 0;
struct timeval start_time, end_time;

// Signal handler for SIGINT
void sigint_handler() {
    printf("\nSIGINT received\n");
    
    sigint_received = 1;
    pthread_mutex_lock(&buffer.mutex);
    buffer.done = 1;
    pthread_cond_broadcast(&buffer.not_empty);
    pthread_mutex_unlock(&buffer.mutex);
}

void copy_directory(const char *src_dir, const char *dest_dir) {
    DIR *dp = opendir(src_dir);
    if (dp == NULL) {
        perror("opendir");
        return;
    }

    struct dirent *entry; // Pointer to the directory entry

    // Iterate over the directory entries
    while ((entry = readdir(dp)) != NULL && !sigint_received) {

        // Skip the current and parent directory entries
        if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0) {
            continue;
        }

        char src_path[MAX_PATH];
        char dest_path[MAX_PATH];
        snprintf(src_path, MAX_PATH, "%s/%s", src_dir, entry->d_name); // Construct the source path
        snprintf(dest_path, MAX_PATH, "%s/%s", dest_dir, entry->d_name); // Construct the destination path

        // Check if the entry is a directory
        if (entry->d_type == DT_DIR) {
            num_directories++;
            mkdir(dest_path, 0755);
            copy_directory(src_path, dest_path); // Recursive call to copy subdirectory
        } 
        
        // Check if the entry is a regular file
        else if (entry->d_type == DT_REG) {
            num_regular_files++;

            int src_fd = open(src_path, O_RDONLY); // Open the source file to read
            if (src_fd == -1) {
                perror("open src");
                continue;
            }

            int dest_fd = open(dest_path, O_WRONLY | O_CREAT | O_TRUNC, 0644); // Open the destination file to write
            if (dest_fd == -1) {
                perror("open dest");
                close(src_fd);
                continue;
            }

            pthread_mutex_lock(&buffer.mutex); // Lock the mutex 

            // Wait while the buffer is full
            while (buffer.count == buffer_size && !sigint_received) {
                pthread_cond_wait(&buffer.not_full, &buffer.mutex); // Wait for the buffer to be not full
            }

            // Check if the sigint is received
            if (sigint_received) {
                close(src_fd);
                close(dest_fd);
                pthread_mutex_unlock(&buffer.mutex);
                break;
            }

            // If the buffer is not full, insert the file info to the buffer
            buffer.buffer[buffer.in].src_fd = src_fd;
            buffer.buffer[buffer.in].dest_fd = dest_fd;
            strncpy(buffer.buffer[buffer.in].src_name, src_path, MAX_PATH); // Copy the source path to the buffer struct
            strncpy(buffer.buffer[buffer.in].dest_name, dest_path, MAX_PATH); // Copy the destination path to the buffer struct

            buffer.in = (buffer.in + 1) % buffer_size; // Update the index to insert a file info
            buffer.count++; // Increment the number of file info in the buffer

            pthread_cond_signal(&buffer.not_empty); // Signal that the buffer is not empty
            pthread_mutex_unlock(&buffer.mutex); // Unlock the mutex
        } 
        
        // Check if the entry is a FIFO file
        else if (entry->d_type == DT_FIFO) {
            num_fifo_files++;
            mkfifo(dest_path, 0644);  // Create FIFO in the destination directory
        }
    }

    closedir(dp);
}

void *manager_thread(void *arg) {
    char *src_dir = ((char **)arg)[0]; 
    char *dest_dir = ((char **)arg)[1];

    // Create the destination directory if it doesn't exist
    struct stat st;
    if (stat(dest_dir, &st) == -1) {
        mkdir(dest_dir, 0755);
    }

    // Copy the source directory to the destination directory
    copy_directory(src_dir, dest_dir);

    pthread_mutex_lock(&buffer.mutex);
    buffer.done = 1;
    pthread_cond_broadcast(&buffer.not_empty);
    pthread_mutex_unlock(&buffer.mutex);

    return NULL;
}

void *worker_thread() {

    char buf[512]; // Buffer to read from the source file
    ssize_t n;

    while (1) {
        pthread_mutex_lock(&buffer.mutex);
        while (buffer.count == 0 && !buffer.done) {
            pthread_cond_wait(&buffer.not_empty, &buffer.mutex);
        }

        if (buffer.count == 0 && buffer.done) {
            pthread_mutex_unlock(&buffer.mutex);
            break;
        }

        file_info_t file = buffer.buffer[buffer.out]; // Get the file info from the buffer struct
        buffer.out = (buffer.out + 1) % buffer_size;
        buffer.count--;

        pthread_cond_signal(&buffer.not_full);
        pthread_mutex_unlock(&buffer.mutex);

        // Read from the source file and write to the destination file
        while ((n = read(file.src_fd, buf, sizeof(buf))) > 0) {
            if (write(file.dest_fd, buf, n) != n) {
                perror("write");
                break;
            }
            total_bytes_copied += n;
        }

        if (n == -1) {
            perror("read");
        }

        close(file.src_fd);
        close(file.dest_fd);

        fprintf(stdout, "Copied %s to %s\n", file.src_name, file.dest_name);
    }

    return NULL;
}


int main(int argc, char *argv[]) {
    if (argc != 5) {
        
        fprintf(stderr, "Usage: %s <buffer_size> <num_workers> <src_dir> <dest_dir>\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    buffer_size = atoi(argv[1]);
    num_workers = atoi(argv[2]);
    char *src_dir = argv[3];
    char *dest_dir = argv[4];

    if (buffer_size <= 0 || num_workers <= 0) {
        
        fprintf(stderr, "Buffer size and number of workers must be greater than 0\n");
        exit(EXIT_FAILURE);
    }

    struct sigaction sa;
    memset(&sa, 0, sizeof(sa));
    sa.sa_handler = sigint_handler;
    if (sigaction(SIGINT, &sa, NULL) == -1) {
        perror("sigaction");
        exit(EXIT_FAILURE);
    }

    buffer.in = 0;
    buffer.out = 0;
    buffer.count = 0;
    buffer.done = 0;
    pthread_mutex_init(&buffer.mutex, NULL); // Initialize the mutex
    pthread_cond_init(&buffer.not_empty, NULL); // Initialize the condition variable 
    pthread_cond_init(&buffer.not_full, NULL); // Initialize the condition variable

    gettimeofday(&start_time, NULL);

    pthread_t manager_thread_id; // Thread ID for the manager thread
    char *manager_args[] = { src_dir, dest_dir }; // Arguments for the manager thread

    // Create the manager thread
    if (pthread_create(&manager_thread_id, NULL, manager_thread, manager_args) != 0) {
        perror("pthread_create");
        exit(EXIT_FAILURE);
    }

    pthread_t worker_thread_id[num_workers]; // Thread IDs for the worker threads

    // Create the worker threads
    for (int i = 0; i < num_workers; i++) {
        if (pthread_create(&worker_thread_id[i], NULL, worker_thread, NULL) != 0) {
            perror("pthread_create");
            exit(EXIT_FAILURE);
        }
    }

    if (pthread_join(manager_thread_id, NULL) != 0) {
        perror("pthread_join");
        exit(EXIT_FAILURE);
    }

    for (int i = 0; i < num_workers; i++) {
        if (pthread_join(worker_thread_id[i], NULL) != 0) {
            perror("pthread_join");
            exit(EXIT_FAILURE);
        }
    }

    gettimeofday(&end_time, NULL);

    long seconds = end_time.tv_sec - start_time.tv_sec;
    long microseconds = end_time.tv_usec - start_time.tv_usec;
    long total_microseconds = (seconds * 1000000) + microseconds;
    long minutes = total_microseconds / 60000000;
    total_microseconds %= 60000000;
    seconds = total_microseconds / 1000000;
    long milliseconds = (total_microseconds % 1000000) / 1000;

    printf("\n---------------STATISTICS--------------------\n");
    printf("Consumers: %d - Buffer Size: %d\n", num_workers, buffer_size);
    printf("Number of Regular Files: %d\n", num_regular_files);
    printf("Number of FIFO Files: %d\n", num_fifo_files);
    printf("Number of Directories: %d\n", num_directories);
    printf("TOTAL BYTES COPIED: %lu\n", total_bytes_copied);
    printf("TOTAL TIME: %02ld:%02ld.%03ld (min:sec.mili)\n", minutes, seconds, milliseconds);

    pthread_mutex_destroy(&buffer.mutex);
    pthread_cond_destroy(&buffer.not_empty);
    pthread_cond_destroy(&buffer.not_full);

    return 0;
}
