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

   /*print_vector();*/
   
   
	/*int vec_size;
	 
	printf("Please input desired vector size...");
	scanf("%i", &vec_size);	

	double vec1[vec_size], vec2[vec_size];

	int array_size = sizeof(vec1)/sizeof(vec1[0]);

	printf("Vector size will be %i", array_size);

	printf("\n1st vector...\n");
	int i;
	double tmp;
	for(i=0;i<vec_size;i++)
	{	
		printf("Please input value %i:", i);
		scanf("%lf", &tmp);
		vec1[i] = tmp;
	}
	   
	printf("2nd vector...\n");
	for(i=0;i<vec_size;i++)
	{	
		printf("Please input value %i:", i);
		scanf("%lf", &tmp);
		vec2[i] = tmp;
	}
	   
	printf("Please input value for scalar...");
	scanf("%lf", &scalar);

	printf("Vector 1 values:\n"); 

	for(i=0;i<vec_size;i++)
	{
		printf("%lf ", vec1[i]);
	}

	printf("\nVector 2 values:\n"); 

	for(i=0;i<vec_size;i++)
	{
		printf("%lf ", vec2[i]);
	}

	printf("\nScalar Value: %lf\n", scalar);*/
   
   /* Print results */
   
   /* Compute and print dot product */
   
   /* Compute scalar multiplication and print out result */
   
   /*MPI_Scatter(a, local_n, MPI_DOUBLE, local_vec1, local_n, MPI_DOUBLE, 0, comm)
   
   
   free(local_scalar_mult2);
   free(local_scalar_mult1);
   free(local_vec2);
   free(local_vec1);
   
   MPI_Finalize();
   return 0;
}

MPI_Scatter(){

}
/*-------------------------------------------------------------------*/
void Check_for_error(
                int       local_ok   /* in */, 
                char      fname[]    /* in */,
                char      message[]  /* in */, 
                MPI_Comm  comm       /* in */) {
   int ok;
   
   MPI_Allreduce(&local_ok, &ok, 1, MPI_INT, MPI_MIN, comm);
   if (ok == 0) {
      int my_rank;
      MPI_Comm_rank(comm, &my_rank);
      if (my_rank == 0) {
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
      MPI_Comm comm) {


   /*if(my_rank == 0) {
	printf("What size should the array be? \n");
	scanf("%d", n_p);	
	MPI_Bcast(n_p, 1, MPI_INT, 0, comm);
   }
   else {
	MPI_Bcast(n_p, 1, MPI_INT, 0, comm);
   }*/
}  /* Read_n */


/*-------------------------------------------------------------------*/
void Read_data(double local_vec1[], double local_vec2[], double* scalar_p,
      int local_n, int my_rank, int comm_sz, MPI_Comm comm) {
   double* a = NULL;
   int i;
   if (my_rank == 0){
      printf("What is the scalar?\n");
      scanf("%lf", scalar_p);
   }
   
   MPI_Bcast(scalar_p, 1, MPI_DOUBLE, 0, comm);
   
   if (my_rank == 0){
      a = malloc(local_n * comm_sz * sizeof(double));
      printf("Enter the first vector\n");
      for (i = 0; i < local_n * comm_sz; i++) 
         scanf("%lf", &a[i]);
   } else {
   }
}  /* Read_data */

/*-------------------------------------------------------------------*/
void Print_vector(double local_vec[], int local_n, int n, char title[], 
      int my_rank, MPI_Comm comm) {
   double* a = NULL;
   int i;
   
   if (my_rank == 0) {
      a = malloc(n * sizeof(double));
      free(a);
   } else {
   }

}  /* Print_vector */


/*-------------------------------------------------------------------*/
double Par_dot_product(double local_vec1[], double local_vec2[], 
      int local_n, MPI_Comm comm) {
}  /* Par_dot_product */


/*-------------------------------------------------------------------*/
void Par_vector_scalar_mult(double local_vec[], double scalar, 
      double local_result[], int local_n) {
}  /* Par_vector_scalar_mult */
