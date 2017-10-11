#include <stdio.h>
#include <mpi.h>

void Read_data(double* a, double* b);

int main () 
{
   MPI_Init(NULL,NULL);
   MPI_Comm comm;
   comm = MPI_COMM_WORLD;
   
   int my_size;
   MPI_Comm_size(MPI_COMM_WORLD, &my_size);
   
   int my_rank;
   MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);
   
   
   printf("Hello world rank %d out of %d processors\n", my_rank, my_size);
   
   int ping_pong_count = 0;
   int partner_rank = (my_rank + 1) % 2;
   
   while (ping_pong_count < 20)
   {
      if (my_rank == ping_pong_count % 2)
      {
         ping_pong_count++;
         MPI_Send(&ping_pong_count, 1, MPI_INT, partner_rank, 0, comm);
         printf("%d sent and incremented ping_pong_count %d to %d\n", my_rank, ping_pong_count, partner_rank);
      }
      else
      {
         MPI_Recv(&ping_pong_count, 1, MPI_INT, partner_rank, 0, comm, MPI_STATUS_IGNORE);
         printf("%d received ping_pong_count %d from %d\n", my_rank, ping_pong_count, partner_rank);
      }
   }
   
   if (ping_pong_count == 20)
   {
      if (my_rank == 0)
      {
         double a, b;
         Read_data(&a, &b);
      
         printf("a Value: %lf\n", a);
         printf("b Value: %lf\n", b);
      }
   }
   
   
   
   MPI_Finalize();
   
   
}

void Read_data(double* a, double* b)
{
	printf("Please enter value for a: ");
	scanf("%lf", a);
	
	printf("Please enter value for b: ");
	scanf("%lf", b);
}
