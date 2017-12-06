#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>

void Read_n(int* n, int* local_n, int my_rank, int comm_sz, MPI_Comm comm);
void Create_matrix(double matrix[], int n, int my_rank, MPI_Comm comm);
void Create_vector(double vector[], int n, int my_rank, int comm_sz, MPI_Comm comm);
void mat_vect_mult(double matrix[], double vector[], double result_vec[], int local_n, int n, int my_rank, int comm_sz, MPI_Comm comm);


int main()
{
      int my_rank, comm_sz, local_n;
      double *vector, *matrix;
      double *result_vec;
	int n, i, j, count;
	
	MPI_Init(NULL, NULL);
	MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);
	MPI_Comm_size(MPI_COMM_WORLD, &comm_sz);
	MPI_Comm comm;
	comm = MPI_COMM_WORLD;
      
      Read_n(&n, &local_n, my_rank, comm_sz, comm);

      vector = malloc(n*sizeof(double));
      result_vec = malloc(n*sizeof(double));
      matrix = malloc(n*n*sizeof(double));
      // int *matrix[n];
             
      Create_matrix(matrix, n, my_rank, comm);
      Create_vector(vector, n, my_rank, comm_sz, comm);
      mat_vect_mult(matrix, vector, result_vec, local_n, n, my_rank, comm_sz, comm);

      MPI_Finalize();
      return 0;
}

void Read_n(int* n, int* local_n, int my_rank, int comm_sz, MPI_Comm comm) 
{
      int local_ok = 1;

      if (my_rank == 0)
      {
            printf("please enter n for nxn array:");
            scanf("%d", n);             
      } 

      MPI_Bcast(n, 1, MPI_INT, 0, comm);

      if (*n < 0 || *n % comm_sz != 0)
      local_ok = 0;


      *local_n = *n/comm_sz;
}  /* Read_n */

void Create_matrix(double matrix[], int n, int my_rank, MPI_Comm comm)
{     
      int i, j;
      if (my_rank == 0)
      {                           
            printf("user n input: %d\n", n);   
      
            for (i = 0; i < n*n; i++)
                  matrix[i] = rand()%10;
      }  
}

void Create_vector(double vector[], int n, int my_rank, int comm_sz, MPI_Comm comm)
{     
      int i;
      if (my_rank == 0)
      {
            for (i = 0; i < n; i++)
            {
                  vector[i] = (rand()%9)+1;
            }

            printf("Vector: ");
            for (i = 0; i < n; i++)
            {
                  printf("%.2lf ", vector[i]);
            }
            printf("\n");
      }
      MPI_Bcast(vector, n, MPI_DOUBLE, 0, comm);
}

void mat_vect_mult(double matrix[], double vector[], double result_vec[], int local_n, int n, int my_rank, int comm_sz, MPI_Comm comm)
{
      int i, j, row;
      double *x;
      int local_i;
      int local_ok = 1;

      if (my_rank == 0)
      {     row = 0;
            printf("Matrix:\n");
            for (i = 0; i < n*n; i++)
            {
                  if (i % n == 0)
                  {
                        printf("Row %d: ", row);
                        row++;
                  }
                  printf("%.2lf ", matrix[i]);
                  if (i % n == (n-1))
                        printf("\n");

            }
      }     

      x = malloc(n*local_n*sizeof(double));
      double *total;
      total = malloc(local_n*sizeof(double));

      MPI_Scatter(matrix, n*local_n, MPI_DOUBLE, x, n*local_n, MPI_DOUBLE, 0, comm);

      for (i = 0; i < local_n; i++)
      {
            for (j = 0; j < n; j++)
            {
                  total[i] += x[(i*n)+j] * vector[j];
            }
            
      }
      // printf("Total[%d]: %.2lf\n", my_rank, total);

      // for (i = 0; i < local_n; i++)
      // {
      //       printf("Total[%d]: %.2lf\n", my_rank, total[i]);            
      // }
      

      double *temp;
      temp = malloc(n*sizeof(double));

      for (i = 0; i < n; i++)
      {
            temp[i] = 0;
      }
      
      for (i = 0; i < local_n; i++)
      {
            temp[local_n*my_rank + i] = total[i];
      }
      
      MPI_Reduce(temp, result_vec, n, MPI_DOUBLE, MPI_SUM, 0, comm);

      if (my_rank == 0)
      {
            for (i = 0; i < n; i++)
                  printf("Total[%d]: %.2lf\n", i, result_vec[i]);
      }
      free(x);
}
