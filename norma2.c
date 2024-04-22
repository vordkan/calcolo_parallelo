#include <stdio.h>
#include <omp.h>
#include <math.h>
#include <time.h>

#define N 10 // Dimensione del vettore
#define NUM_THREADS 2 // Numero di thread

int main() {
    int vec[N] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
    int sums[N] = {};
    int partial_sum = 0, total_sum = 0, norm = 0, i = 0;
    double t0, t1, t_tot;
    
    t0 = omp_get_wtime();
    #pragma omp parallel for private(i) shared(vec) reduction(+:partial_sum, total_sum)
    for(i = 0; i < N ; i++)
    {
       partial_sum += vec[i] * vec[i];
       sums[i] = partial_sum;
       total_sum += sums[i];
    }

    norm = sqrt(total_sum);
    t1 = omp_get_wtime();
    t_tot = t1 - t0;
    
    printf("La norma del vettore è: %d\nCon tempo %f\n", norm,t_tot);

    return 0;
}
