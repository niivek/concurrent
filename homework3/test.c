#include <stdio.h>
#include <stdlib.h>

int main()
{
    double x;
    
    x = 2*(double)rand()/(double)RAND_MAX - 1;

    printf("%.2lf\n", x);
    return 0;
}
