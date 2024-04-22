#include <stdio.h>
#include <omp.h>
#include <time.h>

#define N 16

void printArray(int *vett){
    for (int i = 0; i < N; i++){
        printf("[%d]",vett[i]);
    }
    printf("\n");
}

int main() {
    int A[17] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17};
    int B[17] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17};
    int C[N] = {};
    int i = 0;
    float t0, t1, t_tot;

    omp_set_num_threads(4);
    t0 = omp_get_wtime();
    #pragma omp parallel for private(i) shared(A,B) reduction(+:C)
    for (i = 0; i < N; i++) 
    {
       C[i] = A[i] + B[i];
       printf("Thread %d: C[%d] = A[%d] + B[%d] = %d\n", omp_get_thread_num(), i, i, i, C[i]);
    }
    t1 = omp_get_wtime();
    t_tot = t1 - t0;
    
    printf("La somma degli indici dei due array è:\n");
    printArray(C);
    printf("Con tempo %f:\n", t_tot);

    return 0;
}

