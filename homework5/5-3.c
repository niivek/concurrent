#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <limits.h>

// int thread_count;
// long total_darts = 0;

// long darts_in_circle = 0;

// pthread_mutex_t mutex;

// void Usage(char* prog_name);

// int main(int argc, char* argv[])
// {
//     return 0;
// }

// void Usage(char* prog_name);
// {
//     fprintf(stderr, "usage: %d <thread_count>\n", prog_name);
//     exit(0);
// }


int thread_count;               // global thread count
long total_darts = 0;           // global dart count
long darts_in_circle = 0;       // global dart tracker
double x, y, dist_sq;
double pi_est;
int number_in_circle = 0;
pthread_mutex_t mutex;

void* throw_darts(void* rank)
{
    long my_rank = (long) rank;

    long toss;

    int local_darts = total_darts / (int) thread_count;
    int first_thread = my_rank * local_darts;
    int last_thread = (my_rank + 1) * local_darts - 1;

    for (toss = first_thread; toss <= last_thread; toss++)
    {
        x = (double)rand()/RAND_MAX*2.0-1.0;
        y = (double)rand()/RAND_MAX*2.0-1.0;
        dist_sq = x*x + y*y;
        if (dist_sq <= 1)
        {
            pthread_mutex_lock(&mutex);
            number_in_circle++;
            pthread_mutex_unlock(&mutex);   
        }
    }

    return NULL;
}

int main(int argc, char* argv[])
{
    srand((unsigned)time(NULL));

    // check and parse command-line arguments
    if (argc != 3) {
        printf("Usage: %s <num-threads> <num-darts>\n", argv[0]);
        exit(EXIT_FAILURE);
    }
    thread_count = strtol(argv[1], NULL, 10);
    total_darts = strtoll(argv[2], NULL, 10);

    long thread;
    pthread_t* thread_handle;

    thread_handle = malloc(thread_count*sizeof(pthread_t));
    pthread_mutex_init(&mutex, NULL);

    // spawn worker thread
    for (thread = 0; thread < thread_count; thread++)
        pthread_create(&thread_handle[thread], NULL, 
            throw_darts, (void*) thread);

    // wait for thread to finish
    for (thread = 0; thread < thread_count; thread++)
        pthread_join(thread_handle[thread], NULL);

    // calculate pi
    pi_est = 4*number_in_circle/(double)total_darts;
    printf("Estimated pi: %e\n", pi_est);

    // clean up
    return EXIT_SUCCESS;
}