#include <stdio.h>
#include <omp.h>
#include <time.h>

#define N 100000000

int main(int argc, char **argv)
{
   float t0, t1, t_tot;
   long int i, n = N;
   double x, dx, f, sum, pi;
   
   printf("Numero di intervalli: %ld\n", n);
   sum = 0.0;
   dx = 1.0 / (double)n;
   t0 = omp_get_wtime();
   
   #pragma omp parallel for private(x,f,i) shared(dx,n) reduction(+:sum)
   for(i = 1; i <= n; i++)
   {
      x = dx * ((double) (i - 0.5));
      f = 4.0 / (1.0+x*x);
      
      //#pragma omp critical //per dare l'accesso uno alla volta 
      sum=sum+f;
   }
   t1 = omp_get_wtime();
   t_tot = t1 - t0;
   pi = dx*sum;
   printf("PI %.24f\nCon tempo %f\n", pi, t_tot);
   
   return 0;
}

//export OMP_NUM_THREADS=numero da utilizzare.
