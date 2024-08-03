#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <signal.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <string.h>
#include <time.h>
#include <math.h>
#include <semaphore.h>
#include <complex.h>
#include <sys/time.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>

#define MAX_COOKS 100
#define MAX_DELIVERY 100
#define MAX_OVEN 6
#define QUEUE_SIZE 10
#define LOG_FILE "PideShop.log"
#define OVEN_APPARATUS 3 // Only 3 oven apparatus

// Structures for cooks, delivery personnel, and orders
typedef struct {
    int id;
    pthread_t thread;
    int capacity;
    int orders[3];
    int order_count;
    int delivery_count; // To find promoted delivery person
} DeliveryPerson;

typedef struct {
    int id;
    pthread_t thread;
} Cook;

typedef struct {
    int order_id;
    int customer_id;
    int status; // 0: placed, 1: prepared, 2: cooked, 3: delivering
    int x;
    int y;
    int distance;
} Order;

// Global variables
Cook cooks[MAX_COOKS];
DeliveryPerson delivery[MAX_DELIVERY];
Order orders[MAX_OVEN];
Order orderQueue[QUEUE_SIZE];
int queueSize = 0;
int cookPoolSize, deliveryPoolSize, deliverySpeed;
pthread_mutex_t lock, queueLock;
pthread_cond_t queueCond;
sem_t ovenSemaphore;
int client_socket;
int total_orders = 0;
int delivered_orders = 0;

// Function prototypes
void *cook_function(void *arg);
void *delivery_function(void *arg);
void handle_signal(int signal);
void log_activity(const char *activity);
void add_order(Order order);
Order get_order();
void notify_client(int order_id);
void* client_handler(void* arg);
double calculate_pseudo_inverse();
void promote_most_efficient_delivery_person();

int main(int argc, char *argv[]) {
    if (argc != 5) {
        fprintf(stderr, "Usage: %s [portnumber] [CookthreadPoolSize] [DeliveryPoolSize] [k]\n", argv[0]);
        exit(1);
    }

    int port = atoi(argv[1]);
    cookPoolSize = atoi(argv[2]);
    deliveryPoolSize = atoi(argv[3]);
    deliverySpeed = atoi(argv[4]);

    // Initialize server, thread pools, and signal handlers
    struct sigaction sa;
    memset(&sa, 0, sizeof(sa));
    sa.sa_handler = handle_signal;
    sigaction(SIGINT, &sa, NULL);
    sigaction(SIGTERM, &sa, NULL);

    // Initialize mutexes, condition variables, and semaphores for synchronization
    pthread_mutex_init(&lock, NULL);
    pthread_mutex_init(&queueLock, NULL);
    pthread_cond_init(&queueCond, NULL);
    sem_init(&ovenSemaphore, 0, OVEN_APPARATUS); // Initialize semaphore for 3 oven apparatus

    for (int i = 0; i < cookPoolSize; i++) {
        cooks[i].id = i;
        pthread_create(&cooks[i].thread, NULL, cook_function, (void *)&cooks[i]);
    }

    for (int i = 0; i < deliveryPoolSize; i++) {
        delivery[i].id = i;
        delivery[i].capacity = 3;
        delivery[i].order_count = 0;
        delivery[i].delivery_count = 0; 
        pthread_create(&delivery[i].thread, NULL, delivery_function, (void *)&delivery[i]);
    }

    // Server socket setup
    int server_fd;
    struct sockaddr_in address;
    int addrlen = sizeof(address);

    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
        perror("socket failed");
        exit(EXIT_FAILURE);
    }

    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(port);

    if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0) {
        perror("bind failed");
        exit(EXIT_FAILURE);
    }

    if (listen(server_fd, 3) < 0) {
        perror("listen");
        exit(EXIT_FAILURE);
    }

    // Print server ip and port
    char ip[INET_ADDRSTRLEN];
    inet_ntop(AF_INET, &address.sin_addr, ip, INET_ADDRSTRLEN);
    printf("Server listening on %s:%d\n", ip, port);
    
    printf("PideShop active waiting for connection...\n");
    log_activity("PideShop active waiting for connection...");

    while (1) {
        int new_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t*)&addrlen);
        if (new_socket < 0) {
            perror("accept");
            exit(EXIT_FAILURE);
        }
        printf("New connection accepted...\n");

        pthread_t client_thread;
        client_socket = new_socket;
        pthread_create(&client_thread, NULL, client_handler, &new_socket);
    }

    // Cleanup
    pthread_mutex_destroy(&lock);
    pthread_mutex_destroy(&queueLock);
    pthread_cond_destroy(&queueCond);
    sem_destroy(&ovenSemaphore); // Destroy semaphore
    close(server_fd);
    return 0;
}

void* client_handler(void* arg) {
    int new_socket = *((int*)arg);
    int order_count = 0;

    // Receive PID from client
    int pid;
    int bytes_read = read(new_socket, &pid, sizeof(int));
    if (bytes_read <= 0) {
        close(new_socket);
        return NULL;
    }
    printf("Client PID: %d\n", pid);

    // Receive orders from client
    while (1) {
        Order order;
        int bytes_read = read(new_socket, &order, sizeof(Order));
        if (bytes_read <= 0) break;
        add_order(order);
        order_count++;
    }

    //printf("%d new customers.. Serving\n", order_count);
    printf("Done serving client pid: %d\n", pid);
    
    // Write to log file
    char activity[256];
    snprintf(activity, sizeof(activity), "%d new customers served by client PID: %d\n", order_count, pid);
    log_activity(activity);

    close(new_socket);
    printf("PideShop active waiting for connection...\n");
    
    return NULL;
}

void *cook_function(void *arg) {
    Cook *cook = (Cook *)arg;
    while (1) {
        Order order = get_order();

        printf("Cook %d preparing order %d\n", cook->id, order.order_id);
        
        char activity[256];
        snprintf(activity, sizeof(activity), "Cook %d preparing order %d\n", cook->id, order.order_id);
        log_activity(activity);
        // sleep(1); // Simulate preparation time
        
        double preparation_time = calculate_pseudo_inverse();
        sleep((int)preparation_time); // Simulate preparation time
        //printf("Preparation time %f\n", preparation_time);

        sem_wait(&ovenSemaphore); // Wait for an oven apparatus to be available
        printf("Cook %d placing order %d in the oven\n", cook->id, order.order_id);
        
        char activity2[256];
        snprintf(activity2, sizeof(activity2), "Cook %d placing order %d in the oven\n", cook->id, order.order_id);
        log_activity(activity2);
        // sleep(1); // Simulate cooking time

        sleep((int)(preparation_time / 2)); // Simulate cooking time (half the preparation time)
        //printf("Cook time %f\n", preparation_time / 2);

        pthread_mutex_lock(&lock);
        order.status = 2; // Cooked
        orders[order.order_id % MAX_OVEN] = order; // Assign the order back to the global orders array
        pthread_mutex_unlock(&lock);

        printf("Cook %d completed order %d\n", cook->id, order.order_id);
        
        char activity3[256];
        snprintf(activity3, sizeof(activity3), "Cook %d completed order %d\n", cook->id, order.order_id);
        log_activity(activity3);
        sem_post(&ovenSemaphore); // Release the oven apparatus
    }
    return NULL;
}

void *delivery_function(void *arg) {

    DeliveryPerson *delivery = (DeliveryPerson *)arg; // Get the delivery person from the argument

    while (1) {
        pthread_mutex_lock(&lock);
        int found_order = 0;
        for (int i = 0; i < MAX_OVEN; i++) {
            if (orders[i].status == 2 && delivery->order_count < delivery->capacity) { // Order cooked and not yet delivering
                delivery->orders[delivery->order_count++] = orders[i].order_id;
                orders[i].status = 3; // Delivering
                found_order = 1;
            }
        }
        pthread_mutex_unlock(&lock);

        if (!found_order) {
            sleep(1); // Wait for orders
            continue;
        }

        printf("Delivery person %d delivering orders\n", delivery->id);
        
        char activity[256];
        snprintf(activity, sizeof(activity), "Delivery person %d delivering orders\n", delivery->id);
        log_activity(activity);

        // Calculate the sleep time based on the distance and delivery speed
        for (int i = 0; i < delivery->order_count; i++) {
            int order_id = delivery->orders[i];
            pthread_mutex_lock(&lock);
            Order order = orders[order_id % MAX_OVEN];
            pthread_mutex_unlock(&lock);

            double distance = sqrt(order.x * order.x + order.y * order.y); // Euclidean distance
            double sleep_time = distance / deliverySpeed;

            sleep((int)sleep_time); // Simulate delivery time
            //printf("Delivery time %.2f\n", sleep_time);

            printf("Order %d delivered by delivery person %d\n", order_id, delivery->id);
            
            char activity2[256];
            snprintf(activity2, sizeof(activity2), "Order %d delivered by delivery person %d\n", order_id, delivery->id);
            log_activity(activity2);

            notify_client(order_id);
            delivery->delivery_count++; // Increment the delivery count for this delivery person
        }

        pthread_mutex_lock(&lock);
        delivery->order_count = 0;
        pthread_mutex_unlock(&lock);
    }
    return NULL;
}

void handle_signal(int signal) {
    if (signal == SIGINT || signal == SIGTERM) {
        printf("Quiting...writing log file\n");
        log_activity("Server shutting down...");
        promote_most_efficient_delivery_person();

        // Cleanup
        pthread_mutex_destroy(&lock);
        pthread_mutex_destroy(&queueLock);
        sem_destroy(&ovenSemaphore); // Destroy semaphore
        close(client_socket);

        // Clean up threads
        for (int i = 0; i < cookPoolSize; i++) {
            pthread_cancel(cooks[i].thread);
        }

        for (int i = 0; i < deliveryPoolSize; i++) {
            pthread_cancel(delivery[i].thread);
        }

        // Exit the program
        exit(0);
    }
}

void log_activity(const char *activity) {
    int logFile = open(LOG_FILE, O_WRONLY | O_APPEND | O_CREAT, 0644);
    if (logFile == -1) {
        perror("Failed to open log file");
        return;
    }
    //char newline = '\n';
    write(logFile, activity, strlen(activity));
    //write(logFile, &newline, sizeof(newline));
    close(logFile);
}

void add_order(Order order) {
    pthread_mutex_lock(&queueLock);
    if (queueSize < QUEUE_SIZE) {
        orderQueue[queueSize++] = order;
        pthread_cond_signal(&queueCond);
    }
    pthread_mutex_unlock(&queueLock);
}

Order get_order() {
    pthread_mutex_lock(&queueLock);
    while (queueSize == 0) {
        pthread_cond_wait(&queueCond, &queueLock);
    }
    Order order = orderQueue[--queueSize];
    pthread_mutex_unlock(&queueLock);
    return order;
}

void notify_client(int order_id) {
    pthread_mutex_lock(&lock);
    delivered_orders++;
    pthread_mutex_unlock(&lock);

    char message[256];
    snprintf(message, sizeof(message), "Order %d has been delivered.\n", order_id);
    send(client_socket, message, strlen(message), 0);

    if (delivered_orders == total_orders) {
        close(client_socket);
        delivered_orders = 0; // Reset for the next client
    }
}

double calculate_pseudo_inverse() {
    int m = 30, n = 40;
    double complex A[m][n], A_conj_trans[n][m], A_pseudo_inv[n][m];
    struct timeval start, end;

    // Initialize matrix A with random complex numbers
    srand(time(0));
    for (int i = 0; i < m; i++) {
        for (int j = 0; j < n; j++) {
            A[i][j] = rand() % 10 + rand() % 10 * I;
        }
    }

    gettimeofday(&start, NULL);

    // Calculate conjugate transpose of A
    for (int i = 0; i < m; i++) {
        for (int j = 0; j < n; j++) {
            A_conj_trans[j][i] = conj(A[i][j]);
        }
    }

    // Simulate matrix multiplication and inversion 
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < m; j++) {
            A_pseudo_inv[i][j] = 0;
            for (int k = 0; k < m; k++) {
                A_pseudo_inv[i][j] += A_conj_trans[i][k] * A[k][j];
            }
            A_pseudo_inv[i][j] = 1.0 / A_pseudo_inv[i][j]; 
        }
    }

    sleep(1); // Simulate computation time

    gettimeofday(&end, NULL);
    double elapsed_time = (end.tv_sec - start.tv_sec) * 1.0 + (end.tv_usec - start.tv_usec) / 1000000.0;

    return elapsed_time;
}

void promote_most_efficient_delivery_person() {
    int max_deliveries = 0;
    int best_delivery_person = -1;

    for (int i = 0; i < deliveryPoolSize; i++) {
        if (delivery[i].delivery_count > max_deliveries) {
            max_deliveries = delivery[i].delivery_count;
            best_delivery_person = i;
        }
    }

    if (best_delivery_person != -1) {
        printf("Delivery person %d is promoted with %d deliveries.\n", best_delivery_person, max_deliveries);
    } else {
        printf("No deliveries were made today.\n");
    }
}

