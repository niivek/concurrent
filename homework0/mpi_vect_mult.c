#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <mpi.h> 

void Read_n(int* n_p, int* local_n_p, int my_rank, int comm_sz, 
      MPI_Comm comm);
void Check_for_error(int local_ok, char fname[], char message[], 
      MPI_Comm comm);
void Read_data(double local_vec1[], double local_vec2[], double* scalar_p,
      int local_n, int my_rank, int comm_sz, MPI_Comm comm);
void Print_vector(double local_vec[], int local_n, int n, char title[], 
      int my_rank, MPI_Comm comm);
double Par_dot_product(double local_vec1[], double local_vec2[], 
      int local_n, MPI_Comm comm);
void Par_vector_scalar_mult(double local_vec[], double scalar, 
      double local_result[], int local_n);

int main(void) {
   int n, local_n;
   double *local_vec1, *local_vec2;
   double scalar;
   double *local_scalar_mult1, *local_scalar_mult2;
   double dot_product;
   int comm_sz, my_rank;
   MPI_Comm comm;

   MPI_Init(NULL, NULL);
   comm = MPI_COMM_WORLD;
   MPI_Comm_size(comm, &comm_sz);
   MPI_Comm_rank(comm, &my_rank);
   Read_n(&n, &local_n, my_rank, comm_sz, comm);
   
   local_vec1 = malloc(local_n*sizeof(double));
   local_vec2 = malloc(local_n*sizeof(double));
   
   local_scalar_mult1 = malloc(local_n*sizeof(double));
   local_scalar_mult2 = malloc(local_n*sizeof(double));
   
   Read_data(local_vec1, local_vec2, &scalar, local_n, my_rank, comm_sz, comm);
   
   if(my_rank == 0) 
   {
      printf("------input data ------\n");
   }
      
   Print_vector(local_vec1, local_n, n, "1st vector: ", my_rank, comm);
   Print_vector(local_vec2, local_n, n, "2nd vector: ", my_rank, comm);
   
   if(my_rank == 0) 
   {
      printf("Scalar value: %f\n", scalar);
   }
   
   /* Print results */
   if(my_rank == 0 )
   {
      printf("------print results------\n");
   }
   
   /* Compute and print dot product */
   dot_product = Par_dot_product(local_vec1, local_vec2, local_n, comm);
   if(my_rank == 0)
   {
      printf("Dot product: %f\n", dot_product);
   }
   
   /* Compute scalar multiplication and print out result */
   Par_vector_scalar_mult(local_vec1, scalar, local_scalar_mult1, local_n);
   Par_vector_scalar_mult(local_vec2, scalar, local_scalar_mult2, local_n);
   
   Print_vector(local_scalar_mult1, local_n, n, "Scalar vector 1: ", my_rank, comm);
   Print_vector(local_scalar_mult2, local_n, n, "Scalar vector 2: ", my_rank, comm);
   
   /*MPI_Scatter(a, local_n, MPI_DOUBLE, local_vec1, local_n, MPI_DOUBLE, 0, comm)*/
   
   
   free(local_scalar_mult2);
   free(local_scalar_mult1);
   free(local_vec2);
   free(local_vec1);
   
   MPI_Finalize();
   return 0;
}


/*-------------------------------------------------------------------*/
void Check_for_error(
                int       local_ok   /* in */, 
                char      fname[]    /* in */,
                char      message[]  /* in */, 
                MPI_Comm  comm       /* in */) 
{
   int ok;
   
   MPI_Allreduce(&local_ok, &ok, 1, MPI_INT, MPI_MIN, comm);
   if (ok == 0) 
   {
      int my_rank;
      MPI_Comm_rank(comm, &my_rank);
      if (my_rank == 0) 
      {
         fprintf(stderr, "Proc %d > In %s, %s\n", my_rank, fname, 
               message);
         fflush(stderr);
      }
      MPI_Finalize();
      exit(-1);
   }
}  /* Check_for_error */


/*-------------------------------------------------------------------*/
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
      
   Check_for_error(local_ok, "Read_n", "n should be > 0 and evenly divisible by comm_sz", comm);
   
   *local_n_p = *n_p/comm_sz;
}  /* Read_n */


/*-------------------------------------------------------------------*/
void Read_data(double local_vec1[], double local_vec2[], double* scalar_p,
      int local_n, int my_rank, int comm_sz, MPI_Comm comm) 
{
   double* a = NULL;
   double* b = NULL;
   int i;
   if (my_rank == 0)
   {
      printf("Scalar Value: \n");
      scanf("%lf", scalar_p);
   }
   
   MPI_Bcast(scalar_p, 1, MPI_DOUBLE, 0, comm);
   
   if (my_rank == 0)
   {
      a = malloc(local_n * comm_sz * sizeof(double));
      b = malloc(local_n * comm_sz * sizeof(double));
      
      printf("Enter the 1st vector:\n");
      for (i = 0; i < local_n * comm_sz; i++) 
         scanf("%lf", &a[i]);
      
      MPI_Scatter(a, local_n, MPI_DOUBLE, local_vec1, local_n, MPI_DOUBLE, 0, comm);
      
      printf("Enter the 2nd vector:\n");
      for(i = 0; i < local_n * comm_sz; i++)
         scanf("%lf", &b[i]);
         
      MPI_Scatter(b, local_n, MPI_DOUBLE, local_vec2, local_n, MPI_DOUBLE, 0, comm);
      
      free(a);
      free(b);
      
   } else 
   {
      MPI_Scatter(a, local_n, MPI_DOUBLE, local_vec1, local_n, MPI_DOUBLE, 0, comm);
      MPI_Scatter(b, local_n, MPI_DOUBLE, local_vec2, local_n, MPI_DOUBLE, 0, comm);
   }
}  /* Read_data */

/*-------------------------------------------------------------------*/
void Print_vector(double local_vec[], int local_n, int n, char title[], 
      int my_rank, MPI_Comm comm) 
{
   double* a = NULL;
   int i;
   
   if (my_rank == 0) 
   {
      a = malloc(n * sizeof(double));
      MPI_Gather(local_vec, local_n, MPI_DOUBLE, a, local_n, MPI_DOUBLE, 0, comm);
      printf("%s", title);
      for (i = 0; i < n; i++)
      {
         printf("%.2f ", a[i]);         
      }
      printf("\n");
      free(a);
   } else 
   {
      MPI_Gather(local_vec, local_n, MPI_DOUBLE, a, local_n, MPI_DOUBLE, 0, comm);
   }
}  /* Print_vector */


/*-------------------------------------------------------------------*/
double Par_dot_product(double local_vec1[], double local_vec2[], 
      int local_n, MPI_Comm comm) 
{
   int i;
   double dot_product, local_dot_product = 0;
   
   for (i = 0; i < local_n; i++)
   {
     local_dot_product += local_vec1[i]*local_vec2[i];   
   }
   
   MPI_Reduce(&local_dot_product, &dot_product, 1, MPI_DOUBLE, MPI_SUM, 0, comm);
   return dot_product;

}  /* Par_dot_product */


/*-------------------------------------------------------------------*/
void Par_vector_scalar_mult(double local_vec[], double scalar, 
      double local_result[], int local_n) 
{
   int i;
   for (i = 0; i < local_n; i++)
   {
      local_result[i] = local_vec[i]*scalar;
   }

}  /* Par_vector_scalar_mult */
