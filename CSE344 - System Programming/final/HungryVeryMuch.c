#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <signal.h>
#include <string.h>
#include <time.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <fcntl.h>


#define LOG_FILE "HungryVeryMuch.log"

typedef struct {
    int order_id;
    int customer_id;
    int status; // 0: placed, 1: prepared, 2: cooked, 3: delivering
    int x;
    int y;
    int distance;
} Order;

void handle_signal(int signal);
void log_activity(const char *activity);

int main(int argc, char *argv[]) {
    if (argc != 6) {
        fprintf(stderr, "Usage: %s [ipAddress] [portnumber] [numberOfClients] [p] [q]\n", argv[0]);
        exit(1);
    }

    char *ipAddress = argv[1];
    int port = atoi(argv[2]);
    int numClients = atoi(argv[3]);
    int p = atoi(argv[4]);
    int q = atoi(argv[5]);

    // Signal handling for client-side cancellation
    struct sigaction sa;
    memset(&sa, 0, sizeof(sa));
    sa.sa_handler = handle_signal;
    sigaction(SIGINT, &sa, NULL);
    sigaction(SIGTERM, &sa, NULL);

    // Connect to server
    int sock = 0;
    struct sockaddr_in serv_addr;

    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        printf("\n Socket creation error \n");
        return -1;
    }

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(port);

    if (inet_pton(AF_INET, ipAddress, &serv_addr.sin_addr) <= 0) {
        printf("\nInvalid address/ Address not supported \n");
        return -1;
    }

    if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) {
        printf("\nConnection Failed \n");
        return -1;
    }

    printf("Connected to server...\n");
    log_activity("Connected to server...\n");

    // Send PID to server
    int pid = getpid();
    send(sock, &pid, sizeof(int), 0);
    printf("Client PID: %d\n", pid);

    srand(time(0));
    for (int i = 0; i < numClients; i++) {
        Order order;
        order.order_id = i;
        order.customer_id = i;
        order.status = 0; // Placed
        order.x = rand() % p;
        order.y = rand() % q;
        order.distance = abs(order.x) + abs(order.y);

        send(sock, &order, sizeof(Order), 0);
        printf("Order %d placed at (%d, %d)\n", order.order_id, order.x, order.y);
        char activity[256];
        sprintf(activity, "Order %d placed at (%d, %d)\n", order.order_id, order.x, order.y);
        log_activity(activity);
    }

    int delivered_orders = 0;
    char buffer[256];
    while (read(sock, buffer, sizeof(buffer)) > 0) {
        printf("Server: %s", buffer);
        delivered_orders++;
        if (delivered_orders == numClients) {
            break;
        }
    }

    close(sock);
    return 0;
}

void handle_signal(int signal) {
    if (signal == SIGINT || signal == SIGTERM) {
        printf("Client shutting down...\n");
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