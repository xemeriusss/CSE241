#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>
#include <signal.h>

sem_t newPickup, inChargeforPickup, newAutomobile, inChargeforAutomobile;

int mFree_automobile = 8; // 8 places for automobiles
int mFree_pickup = 4; // 4 places for pickups

pthread_t carOwnerThread, carAttendantThread;

void sigint_handler() {
    printf("\nSIGINT received. Cleaning threads.\n");
    pthread_cancel(carAttendantThread);
    pthread_cancel(carOwnerThread);
    sem_destroy(&newPickup);
    sem_destroy(&inChargeforPickup);
    sem_destroy(&newAutomobile);
    sem_destroy(&inChargeforAutomobile);
    exit(0);
}

void* carOwner() {

    // Car owners arrive and park their vehicles
    while (1) {
        int vehicleType = rand() % 2; // 0 for automobile, 1 for pickup

        if (vehicleType == 1) { // Pickup
            if (mFree_pickup > 0) {
                sem_wait(&inChargeforPickup);
                mFree_pickup--; // Free places for pickups decreased
                printf("Pickup owner parks its vehicle in temporary lot. Free places for pickups: %d\n", mFree_pickup);
                sem_post(&newPickup);
                sem_post(&inChargeforPickup);
            } else {
                printf("No space left for pickups in temporary lot. Pickup owner leaves.\n");
            }
        } 
        
        else if(vehicleType == 0) { // Automobile
            if (mFree_automobile > 0) {
                sem_wait(&inChargeforAutomobile);
                mFree_automobile--; // Free places for automobiles decreased
                printf("Automobile owner parks its vehicle in temporary lot. Free places for automobiles: %d\n", mFree_automobile);
                sem_post(&newAutomobile);
                sem_post(&inChargeforAutomobile);
            } else {
                printf("No space left for automobiles in temporary lot. Automobile owner leaves.\n");
            }
        }

        sleep(1); // Wait for 1 second before next vehicle arrives
    }
    return NULL;
}

void* carAttendant() {

    // Attendant parks vehicles
    while (1) {
        sem_wait(&newPickup);
        sem_wait(&inChargeforPickup);
        mFree_pickup++; // Increase the number of free places for pickups
        printf("Attendant parked pickup. Free places for pickups: %d\n", mFree_pickup);
        sem_post(&inChargeforPickup);

        sem_wait(&newAutomobile);
        sem_wait(&inChargeforAutomobile);
        mFree_automobile++; // Increase the number of free places for automobiles
        printf("Attendant parked automobile. Free places for automobiles: %d\n", mFree_automobile);
        sem_post(&inChargeforAutomobile);
    }
    return NULL;
}

int main() {

    srand(time(NULL)); // Seed for random number generator

    // Initialize semaphores
    sem_init(&newPickup, 0, 0);   
    sem_init(&newAutomobile, 0, 0);  

    sem_init(&inChargeforPickup, 0, 1);
    sem_init(&inChargeforAutomobile, 0, 1);

    // Initialize signal handler
    struct sigaction sa;
    sa.sa_handler = sigint_handler;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = 0;
    sigaction(SIGINT, &sa, NULL);

    // Create threads
    pthread_create(&carAttendantThread, NULL, carAttendant, NULL);
    pthread_create(&carOwnerThread, NULL, carOwner, NULL);

    // Wait for threads to finish
    pthread_join(carAttendantThread, NULL);
    pthread_join(carOwnerThread, NULL);

    return 0;
}
