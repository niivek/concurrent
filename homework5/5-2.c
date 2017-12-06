#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

int thread_count;

double data[] = {1.3, 2.9, 0.4, 0.3, 1.3, 4.4, 1.7, 0.4, 3.2, 0.3, //input values
                4.9, 2.4, 3.1, 4.4, 3.9, 0.4, 4.2, 4.5, 4.9, 0.9};
int bin_count = 5;
int data_count, sum, local_d_count;
int *bin_counts, *local_bin_count;		//output variables

double min_meas, max_meas, bin_width;
double *bin_maxes, *local_values;
pthread_mutex_t mutex;

int i;

void Usage(char* prog_name);
void *printData(void* rank);

int main (int argc, char* argv[]) {
    long    thread;
    pthread_t* thread_handles;

    if (argc != 2) Usage(argv[0]);
    thread_count = atoi(argv[1]);
    thread_handles = malloc(thread_count*sizeof(pthread_t));

    data_count = sizeof(data)/sizeof(data[0]);

    min_meas = data[0];											//sets min_meas to first element in array
    max_meas = data[0];											//sets max_meas to first element in array
    
    for(i=1;i<data_count;i++)									//finds correct min_meas and max_meas in array
    {
        if(min_meas > data[i])
        {
            min_meas = data[i];
        }
        
        if(max_meas < data[i])
        {
            max_meas = data[i];
        }
    }
    sum = min_meas+max_meas;    
    local_d_count = data_count/thread_count;    
    bin_width = (max_meas - min_meas)/bin_count;
    
    bin_counts = malloc(bin_count*sizeof(int));
	bin_maxes = malloc(bin_count*sizeof(double));
    local_bin_count = malloc(bin_count*sizeof(int));
    
    for (i=0;i<bin_count;i++)											//creates bin_maxes
	{
		bin_maxes[i] = min_meas+(bin_width*(i+1));
		bin_counts[i] = 0;
        local_bin_count[i] = 0;
    }
    
    local_values = malloc(local_d_count*sizeof(double));

    // printf("min_meas: %lf\n", min_meas);
    // printf("max_meas: %lf\n", max_meas);
    // printf("bin_width: %lf\n", bin_width);
    // printf("local_d_count: %d\n", local_d_count);

    for (thread = 0; thread < thread_count; thread++)
        pthread_create(&thread_handles[thread], NULL,
            printData, (void*) thread);

    for (thread = 0; thread < thread_count; thread++)
        pthread_join(thread_handles[thread], NULL);

    printf("------------------------\n");
    for (i = 0; i < bin_count; i++) {
         printf("bin max: %lf\n", bin_maxes[i]);
         printf("bin count: %d\n", bin_counts[i]);
         printf("------------------------\n");
    }

    free(local_values);
	free(bin_counts);
	free(bin_maxes);
    return 0;
}

void Usage (char* prog_name) {
   fprintf(stderr, "usage: %s <thread_count>\n", prog_name);
   exit(0);
} 

void *printData(void* rank) {
    long my_rank = (long) rank;

    int local_data_count = data_count / (int) thread_count;
    int first_thread = my_rank * local_data_count;
    int last_thread = (my_rank + 1) * local_data_count - 1;
    double* local_data = malloc(local_data_count*sizeof(double));

    // printf("TESTING");
    int j = 0;
    for (i = first_thread; i <= last_thread; i++, j++) {
        local_data[j] = data[i];
    }

    // for (j = 0; j < local_data_count; j++) {
    //     printf("%lf from rank: %ld\n", local_data[j], my_rank);
    // }

    // pthread_mutex_lock(&mutex);
    for (i = 0; i < local_data_count; i++) {
        for (j = 0; j < bin_count; j++) {
            if (local_data[i] <= bin_maxes[j])
            {
                pthread_mutex_lock(&mutex);
                bin_counts[j]++;
                pthread_mutex_unlock(&mutex);
                break;
            }
        }
    }
    // pthread_mutex_unlock(&mutex);    
    return NULL;
}