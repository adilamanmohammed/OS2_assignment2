#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>

#define MAX_READERS 14 // Maximum number of readers allowed
#define TOTAL_READS 250000000 // Total number of times each reader will read the shared counter

/* Global variables */
int shared_counter = 0; // Shared counter
int in_cs = 0; // Flag indicating if a writer is in critical section
sem_t mutex, wrt; // Semaphore for mutual exclusion and one for writer preference

/* Writer thread function */
void* writer_thread(void* arg) {
    int i;
    for (i = 0; i < 25000; i++) {
        sem_wait(&wrt); // Wait for writer preference
        in_cs = 1; // Set flag to indicate writer is in critical section
        // Critical section
        shared_counter += 1;
        // End of critical section
        in_cs = 0; // Reset flag
        sem_post(&wrt); // Release writer preference
    }
    printf("Writer done.\n");
    pthread_exit(NULL);
}

/* Reader thread function */
void* reader_thread(void* arg) {
    int reader_id = (int) arg;
    int i;
    for (i = 0; i < TOTAL_READS; i++) {
        sem_wait(&mutex); // Wait for mutual exclusion
        if (in_cs == 1) { // Check if writer is in critical section
            printf("Error: Reader %d found writer in critical section.\n", reader_id);
            exit(1);
        }
        // Critical section
        int temp_counter = shared_counter; // Read shared counter
        // End of critical section
        sem_post(&mutex); // Release mutual exclusion
        relax_and_spend_time(); // Spend some time outside the critical section
    }
    printf("Reader %d done.\n", reader_id);
    pthread_exit(NULL);
}

/* Function to spend some time outside the critical section */
void relax_and_spend_time() {
    int i;
    for (i = 0; i < 250; i++) { // Can be adjusted to control how much time to spend outside the critical section
        i = i; // Dummy operation to spend some time
    }
}

/* Main function */
int main(int argc, char* argv[]) {
    if (argc != 2) { // Check if number of arguments is correct
        printf("Usage: ./program_name number_of_readers\n");
        exit(1);
    }
    int num_of_readers = atoi(argv[1]);
    if (num_of_readers < 1 || num_of_readers > MAX_READERS) { // Check if number of readers is within range
        printf("Error: Number of readers should be between 1 and %d.\n", MAX_READERS);
        exit(1);
    }

    /* Initialize semaphores */
    sem_init(&mutex, 0, 1);
    sem_init(&wrt, 0, 1);

    /* Create threads */
    pthread_t readers[num_of_readers], writer;
    int i, k = num_of_readers / 2;
    for (i = 0; i < k; i++) {
        pthread_create(&readers[i], NULL, reader_thread, (void*) i);
    }
    pthread_create(&writer, NULL, writer_thread, NULL);
    for (i = k; i < num_of_readers; i++) {
        pthread_create(&readers[i], NULL, reader_thread, (void*) i);
    }

    /* Join threads */
 for (i = 0; i < num_of_readers; i++) {
        pthread_join(readers[i], NULL);
    }
    pthread_join(writer, NULL);

    /* Destroy semaphores */
    sem_destroy(&mutex);
    sem_destroy(&wrt);

    return 0;
}
