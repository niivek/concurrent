#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>
#include <limits.h>

void get_dart_count(int* dart_count, int my_rank, MPI_Comm comm);
int get_num(int throws, int my_rank, int comm_sz, MPI_Comm comm);

int main()
{
    int my_rank, comm_sz, local_n;
    double *vector, *matrix;
    double *result_vec;
    int number_in_circle = 0;
    int n, i, j, count;
    int dart_count;
    double pi_estimate;
    int total_circle;

    MPI_Init(NULL, NULL);
    MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);
    MPI_Comm_size(MPI_COMM_WORLD, &comm_sz);
    MPI_Comm comm;
    comm = MPI_COMM_WORLD;

    get_dart_count(&dart_count, my_rank, comm);

    number_in_circle = get_num(dart_count, my_rank, comm_sz, comm);

    printf("my_rank: %d, number_in_circle: %d\n", my_rank, number_in_circle);
    MPI_Reduce(&number_in_circle, &total_circle, 1, MPI_INT, MPI_SUM, 0, comm);

    if (my_rank == 0)
    {   
        printf("total circle %d\n", total_circle);
        pi_estimate = 4*total_circle/((double) dart_count);
        printf("Pi Estimate: %lf\n", pi_estimate);
    }
    
    
    MPI_Finalize();
    return 0;
}

void get_dart_count(int* dart_count, int my_rank, MPI_Comm comm)
{
    if (my_rank == 0)
    {
        printf("How many darts would you like to throw?");
        scanf("%d", dart_count);
    }

    MPI_Bcast(dart_count, 1, MPI_INT, 0, comm);
}

int get_num(int throws, int my_rank, int comm_sz, MPI_Comm comm)
{
    int data_count = throws/comm_sz;
    int first_thread = data_count*my_rank;
    int last_thread = (my_rank + 1) * data_count - 1;
    
    int in_circle = 0;
    int toss;
    double distance_squared;
    double x, y;

    for (toss = first_thread; toss <= last_thread; toss++)
    {
        x = (double)rand()/RAND_MAX*2.0-1.0;
        y = (double)rand()/RAND_MAX*2.0-1.0;

        distance_squared = x*x + y*y;
        if (distance_squared <= 1)
            in_circle++;
    }
    return in_circle;
}
