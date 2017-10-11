#include <stdio.h>
#include <mpi.h>


double Trap(double left_endpt, double right_endpt, int trap_count, double base_len);
void Read_data(double* a, double* b, double* n, int my_rank, MPI_Comm comm);

int main() 
{
	int my_rank, comm_sz, local_n;
	double a, b, h, n, local_a, local_b;
	double local_int, total_int;
	int source;

	MPI_Init(NULL, NULL);
	MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);
	MPI_Comm_size(MPI_COMM_WORLD, &comm_sz);
	MPI_Comm comm;
	comm = MPI_COMM_WORLD;
	
	Read_data(&a, &b, &n, my_rank, comm);
	
	h = (b-a)/n;
	local_n = n/comm_sz;

	local_a = a + my_rank*local_n*h;
	local_b = local_a + local_n*h;
	local_int = Trap(local_a, local_b, local_n, h);
	
	MPI_Reduce(&local_int, &total_int, 1, MPI_DOUBLE, MPI_SUM, 0, comm);
	
	if (my_rank == 0) {
		printf("With n=%lf trapazoids, our estimate\n", n);
		printf("of the integral from %f to %f = %.15e\n",
			a, b, total_int);
	}
	
	MPI_Finalize();
	return 0;

}

void Read_data(double* a, double* b, double* n, int my_rank, MPI_Comm comm)
{
	if (my_rank == 0)
	{
		printf("Please enter value for a: ");
		scanf("%lf", a);
	
		printf("Please enter value for b: ");
		scanf("%lf", b);
	
		printf("How many parts: n=");
		scanf("%lf", n);
	}
	
	MPI_Bcast(a, 1, MPI_DOUBLE, 0, comm);
	MPI_Bcast(b, 1, MPI_DOUBLE, 0, comm);
	MPI_Bcast(n, 1, MPI_DOUBLE, 0, comm);
}

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
