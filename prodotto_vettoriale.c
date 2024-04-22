//gcc-13 -fopenmp esempio.c -o es

#include <stdio.h>
#include <stdlib.h>
#include <omp.h>

int main() {
	int i, N, p;
	float *a, *b, result;
	double t_seq_start, t_seq_end, t_seq_tot, t_par_end, t_par_start, t_par_tot;

	printf("Inserire N: ");
	scanf("%d", &N);

	// allocazione
	a=(float *)calloc(N,sizeof(float));
	b=(float *)calloc(N,sizeof(float));

	// lettura
	printf("Inserire gli elementi nel vettore a\n");
	for (i=0;i<N;i++) {
		scanf("%f", &a[i]);
	}
	
	printf("Inserire gli elementi del vettore b\n");
	for (i=0;i<N;i++) {
		scanf("%f",&b[i]);
	}

	// Tempo 1
	t_seq_start = omp_get_wtime();
	//printf("t1: %f\n", t1);

	// Esecuzione in sequenziale
	for (i=0;i<N;i++) {
		result += a[i]*b[i];
	}

	// Tempo 2
	t_seq_end = omp_get_wtime();
	t_seq_tot = t_seq_end - t_seq_start;

	printf("Il prodotto scalare tra i due vettori in sequenziale e': %f\n", result);

	// Esecuzione in parallelo
	printf("Esecuzione del prodotto scalare in parallelo\n Inserisci il numero di core: ");
	scanf("%d", &p);
	result = 0.0;
	t_par_start = omp_get_wtime();
	#pragma omp parallel for num_threads(p) shared(a,b) private(i) reduction(+:result)
    	for (i = 0; i < N; i++)
    	{
        	result += a[i] * b[i];
    	}
    	t_par_end = omp_get_wtime();
    	t_par_tot = t_par_end - t_par_start;

    	printf("Il prodotto scalare tra i due vettori in parallelo con %d core e': %f\n", p, result);

       //Stampa dei tempi
       printf("\n\nIl tempo in sequenziale e': %f\n", t_seq_tot);
       printf("\nIl tempo in parallelo e': %f\n", t_par_tot);

      // Libera la memoria allocata
      free(a);
      free(b);
      return 0;
}
