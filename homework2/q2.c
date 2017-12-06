#include "mpi.h"
#include <stdio.h>

#define WCOMM MPI_COMM_WORLD
int main(int argc, char **argv)
{
    int my_rank, count;
    int *rand_array;
    double *comp_array;	
    
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(WCOMM, &my_rank);
    
	int sumforA = 0;
	int i;
	
	count = 10;

    rand_array = malloc(count*sizeof(int));
    comp_array = malloc(count*sizeof(double));

    int pdf_i = 1;
    int cdf_i = 1;

    pdf_i = my_rank;
    
    for (i=0;i<count;i++)
	{
		rand_array[i] = rand()%8;
		if (my_rank == 0)
		{
			printf("Array[%d]: %d\n", i, rand_array[i]);
		}
	}	
	
	
	
	if (my_rank == 0)
	{
		printf("Serial Program for a\n");
		for (i=0;i<count;i++)
		{
			sumforA += rand_array[i];
			printf("Serial Array[%d]: %d\n", i, sumforA);
		}
	}

/*    MPI_Scan(&pdf_i, &cdf_i, 1, MPI_INT, MPI_SUM, WCOMM);*/

/*    printf("process %d: cumulative sum = %d\n", my_rank, cdf_i);*/

    MPI_Finalize();
}
