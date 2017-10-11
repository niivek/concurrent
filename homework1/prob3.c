#include <stdio.h>
#include <mpi.h>

void print_input(int data_count, double data[]);
void add_bin(int local_d_count, double local_values[], double bin_maxes[], int local_bin_count[]);
void print_results(int local_d_count, double bin_maxes[], int bin_counts[], int my_rank);

int main()
{
	double data[] = {1.3, 2.9, 0.4, 0.3, 1.3, 4.4, 1.7, 0.4, 3.2, 0.3, //input values
						4.9, 2.4, 3.1, 4.4, 3.9, 0.4, 4.2, 4.5, 4.9, 0.9};
						
	int data_count, bin_count, sum, local_d_count, *local_bin_count;	//precalculated variables
	double min_meas, max_meas, bin_width;
	
	
	int *bin_counts;		//output variables
	double *bin_maxes, *local_values;
	
	int my_rank, comm_sz;
	MPI_Comm comm;
	
	MPI_Init(NULL, NULL);
   comm = MPI_COMM_WORLD;
   MPI_Comm_size(comm, &comm_sz);
   MPI_Comm_rank(comm, &my_rank);
	
	int i;
	if (my_rank == 0)
	{
		data_count = sizeof(data)/sizeof(data[0]);			//determines size of the array
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
		
		bin_count = (data_count/sum)+1;
		
		local_d_count = data_count/comm_sz;
		
		bin_width = (max_meas - min_meas)/bin_count;			//determines correct bin width 
	}
		
	MPI_Bcast(&data_count, 1, MPI_INT, 0, MPI_COMM_WORLD);
	MPI_Bcast(&bin_count, 1, MPI_INT, 0, MPI_COMM_WORLD);
	MPI_Bcast(&local_d_count, 1, MPI_INT, 0, MPI_COMM_WORLD);
	
	bin_counts = malloc(bin_count*sizeof(int));
	bin_maxes = malloc(bin_count*sizeof(double));
	local_bin_count = malloc(bin_count*sizeof(int));
	
	for (i=0;i<bin_count;i++)											//creates bin_maxes
	{
		bin_maxes[i] = min_meas+(bin_width*(i+1));
		bin_counts[i] = 0;
		local_bin_count[i] = 0;
	}
		
	MPI_Bcast(bin_maxes, local_d_count, MPI_DOUBLE, 0, MPI_COMM_WORLD);
	
	local_values = malloc(local_d_count*sizeof(double));
	
	if (my_rank == 0)
	{
		print_input(data_count, data);
		MPI_Scatter(data, local_d_count, MPI_DOUBLE, local_values, local_d_count, MPI_DOUBLE, 0, comm);
	}
	else
	{
		MPI_Scatter(data, local_d_count, MPI_DOUBLE, local_values, local_d_count, MPI_DOUBLE, 0, comm);
	}
	
	add_bin(local_d_count, local_values, bin_maxes, local_bin_count);
	
	MPI_Reduce(local_bin_count, bin_counts, local_d_count, MPI_INT, MPI_SUM, 0, comm);
	
	print_results(local_d_count, bin_maxes, bin_counts, my_rank);

	
	free(local_values);
	free(bin_counts);
	free(bin_maxes);
	MPI_Finalize();
	return 0;
}

void print_input(int data_count, double data[])
{
	int i;
	printf("-------------------------------------------------------\n");
	printf("Input Data: ");
	for (i=0;i<data_count;i++)
	{
		printf("%.2lf ", data[i]);
	}
	printf("\n-------------------------------------------------------\n");
}

void add_bin(int local_d_count, double local_values[], double bin_maxes[], int local_bin_count[])
{
	int i, j;
	for (i=0;i<local_d_count;i++)
	{
		for (j=0;j<local_d_count;j++)
		{
			if (local_values[i] <= bin_maxes[j])
			{
				local_bin_count[j] += 1;
				break;
			}
		}
	}
}

void print_results(int local_d_count, double bin_maxes[], int bin_counts[], int my_rank)
{
	int i;
	if (my_rank == 0)
	{	
		for (i=0;i<local_d_count;i++)
		{
			printf("Bin Maxes: %.2lf, Bin Counts: %d\n", bin_maxes[i], bin_counts[i]);
		}
		printf("-------------------------------------------------------\n");
	}
}




