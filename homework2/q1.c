#include <stdio.h>
#include <mpi.h>
void Read_n(int* n_p, int* local_n_p, int my_rank, int comm_sz, MPI_Comm comm);
void Read_vector(double x_vector[], double y_vector[], int local_n, int my_rank, int comm_sz, MPI_Comm comm, MPI_Datatype type);
void Print_vector(double x_vector[], double y_vector[], int local_n, int n, int my_rank, int comm_sz, MPI_Comm comm, MPI_Datatype type);

int main()
{
	int my_rank, comm_sz, local_n;
	double *x_vector, *y_vector;
	int n;
	
	MPI_Init(NULL, NULL);
	MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);
	MPI_Comm_size(MPI_COMM_WORLD, &comm_sz);
	MPI_Comm comm;
	comm = MPI_COMM_WORLD;
	
	Read_n(&n, &local_n, my_rank, comm_sz, comm);
	
	MPI_Datatype type;
	MPI_Type_contiguous(local_n, MPI_DOUBLE, &type);
	MPI_Type_commit(&type);
	
	x_vector = malloc(local_n*sizeof(double));
	y_vector = malloc(local_n*sizeof(double));
	
	Read_vector(x_vector, y_vector, local_n, my_rank, comm_sz, comm, type);
	
	Print_vector(x_vector, y_vector, local_n, n, my_rank, comm_sz, comm, type);
	
	MPI_Type_free(&type);
	free(x_vector);
	free(y_vector);
	MPI_Finalize();
	return 0;
}

void Read_n(int* n_p, int* local_n_p, int my_rank, int comm_sz, 
      MPI_Comm comm) 
{
   int local_ok = 1;
   if (my_rank == 0)
   {    
      printf("How many values within the vector: ");
      scanf("%d", n_p);    
   }
   
   MPI_Bcast(n_p, 1, MPI_INT, 0, comm);
   
   if (*n_p < 0 || *n_p % comm_sz != 0)
      local_ok = 0;
      
   
   *local_n_p = *n_p/comm_sz;
}  /* Read_n */


void Read_vector(double x_vector[], double y_vector[], int local_n, int my_rank, int comm_sz, MPI_Comm comm, MPI_Datatype type)
{
   double* x = NULL;
   double* y = NULL;
   int i;
   
   if (my_rank == 0)
   {
      x = malloc(local_n * comm_sz * sizeof(double));
      y = malloc(local_n * comm_sz * sizeof(double));
      
      printf("Enter the x vector:\n");
      for (i = 0; i < local_n * comm_sz; i++) 
         scanf("%lf", &x[i]);
      
      MPI_Scatter(x, 1, type, x_vector, 1, type, 0, comm);
      
      printf("Enter the y vector:\n");
      for(i = 0; i < local_n * comm_sz; i++)
         scanf("%lf", &y[i]);
         
      MPI_Scatter(y, 1, type, y_vector, 1, type, 0, comm);
      
      free(x);
      free(y);
      
   } else 
   {
      MPI_Scatter(x, 1, type, x_vector, 1, type, 0, comm);
      MPI_Scatter(y, 1, type, y_vector, 1, type, 0, comm);
   }
}

void Print_vector(double x_vector[], double y_vector[], int local_n, int n, int my_rank, int comm_sz, MPI_Comm comm, MPI_Datatype type)
{
	double* x = NULL;
	double* y = NULL;
	int i;
	double sum;
	
	if (my_rank == 0) 
   {
      x = malloc(n * sizeof(double));
      y = malloc(n * sizeof(double));
      MPI_Gather(x_vector, 1, type, x, 1, type, 0, comm);
      MPI_Gather(y_vector, 1, type, y, 1, type, 0, comm);
      
      printf("x vector: ");
      for (i = 0; i < n; i++)
      {
         printf(" %.2f ", x[i]);         
      }
      printf("\n");
      
      printf("y vector: ");
      for (i = 0; i < n; i++)
      {
         printf(" %.2f ", y[i]);         
      }
      printf("\n");
      
      printf("x+y vector: ");
      for (i = 0; i < n; i++)
      {
         sum = x[i] + y[i];
         printf(" %.2f ", sum);         
      }
      printf("\n");
      
      free(x);
      free(y);
   } else 
   {
      MPI_Gather(x_vector, 1, type, x, 1, type, 0, comm);
      MPI_Gather(y_vector, 1, type, y, 1, type, 0, comm);
   }
}








