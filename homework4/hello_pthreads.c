#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <string.h>

int thread_count;
int message_available;
int consumer;
char* message;
pthread_mutex_t mutex;

void *Hello(void* rank);


int main(int argc, char* argv[]) {
    long thread;
    pthread_t* thread_handles;

    thread_count = strtol(argv[1], NULL, 10);
    message_available = 0;
    // consumer = 0;

    thread_handles = malloc(thread_count*sizeof(pthread_t));
    message = malloc(200*sizeof(char));
    pthread_mutex_init(&mutex, NULL);
    for (thread = 0; thread < thread_count; thread++)
        pthread_create(&thread_handles[thread], NULL, Hello, (void*) thread);

    // printf("Hello from the main thread\n");

    for (thread = 0; thread < thread_count; thread++)
        pthread_join(thread_handles[thread], NULL);

    free(thread_handles);
    return 0;
    
}

void *Hello(void* rank) {
    long my_rank = (long) rank;

    // printf("Hello from thread %ld of %d\n", my_rank, thread_count);

    while (1) {
        pthread_mutex_lock(&mutex);
        if (my_rank % 2 == 0) {
            if (message_available == 0) {
                printf("This is the producer thread rank %ld...\n", my_rank);
                sprintf(message, "Producing message for consumer from rank %ld\n", my_rank);
                message_available = 1;
                pthread_mutex_unlock(&mutex);
                break;
            }
        }
        else {
            if (message_available == 1)
            {
                printf("\n");
                printf("This is the consumer thread rank %ld...\n", my_rank);
                printf("Printing message from producer...\n");
                printf("%s", message);
                printf("\n");
                message_available = 0;
                pthread_mutex_unlock(&mutex);
                break;
            }
        }
        pthread_mutex_unlock(&mutex);
    }

    return NULL;    
}