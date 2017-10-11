#include <stdio.h>

void Read_data(double* a, double* b);

int main () 
{
   double a, b;
   
   Read_data(&a, &b);
      
   printf("a Value: %lf\n", a);
   printf("b Value: %lf\n", b);
}

void Read_data(double* a, double* b)
{
	printf("Please enter value for a: ");
	scanf("%lf", a);
	
	printf("Please enter value for b: ");
	scanf("%lf", b);
}
