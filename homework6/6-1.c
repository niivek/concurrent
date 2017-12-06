#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>

int thread_count;
int local_n;
double a, b, h, n, local_a, local_b;
double local_int;
double total_int = 0;
pthread_mutex_t mutex;
sem_t sem;

void Usage(char* prog_name);
void Read_data(double* a, double* b, double* n);
void *Pth_trap(void* rank);
double Trap(double left_endpt, double right_endpt, int trap_count, double base_len);

int main(int argc, char* argv[])
{
    long    thread;
    pthread_t* thread_handles;

    if (argc != 2) Usage(argv[0]);
    thread_count = atoi(argv[1]);

    thread_handles = malloc(thread_count*sizeof(pthread_t));
    sem_init(&sem, 0, 1);

    Read_data(&a, &b, &n);
    h = (b-a)/n;
    local_n = n/thread_count;

    for (thread = 0; thread < thread_count; thread++)
      pthread_create(&thread_handles[thread], NULL,
         Pth_trap, (void*) thread);

    for (thread = 0; thread < thread_count; thread++)
      pthread_join(thread_handles[thread], NULL);

    printf("With n=%lf trapazoids, our estimate\n", n);
    printf("of the integral from %f to %f = %.15e\n", a, b, total_int);

    printf("%d thread count\n", thread_count);
    
    sem_destroy(&sem);
    return 0;
}

void Usage (char* prog_name) {
   fprintf(stderr, "usage: %s <thread_count>\n", prog_name);
   exit(0);
}  

void Read_data(double* a, double* b, double* n)
{
    printf("Please enter value for a: ");
    scanf("%lf", a);

    printf("Please enter value for b: ");
    scanf("%lf", b);

    printf("How many parts: n=");
    scanf("%lf", n);
}

void *Pth_trap(void* rank) 
{
    long my_rank = (long) rank;

    local_a = a + my_rank*local_n*h;
    local_b = local_a + local_n*h;
    local_int = Trap(local_a, local_b, local_n, h);

    pthread_mutex_lock(&mutex);
    // sem_wait(&sem);
    total_int += local_int;
    pthread_mutex_unlock(&mutex);
    // sem_post(&sem);

   return NULL;
}  /* Pth_mat_vect */

double Trap(double left_endpt, double right_endpt, int trap_count, double base_len) 
{
	double estimate, x;
	int i;

	estimate = (left_endpt)*(left_endpt) + ((right_endpt)*(right_endpt))/2.0;
	for (i = 1; i <= trap_count-1; i++) {
		x = left_endpt + i*base_len;
		estimate += x*x;
	}
	estimate = estimate*base_len;
	
	return estimate;
}