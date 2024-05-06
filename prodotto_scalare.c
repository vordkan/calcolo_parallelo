#include <stdio.h>
#include <stdlib.h>
#include <omp.h>
#include <time.h>
#include <unistd.h>

// prodotto scalare tra due vettori

void main() {
    int i, N, core;
    float *a, *b, /* c ,*/result, seq_result;
    double t0_seq, t1_seq, t0, t1, tempo_totale, tempo_sequenziale;

    printf("\nInserire N: \t");
    scanf("%d", &N);

    printf("\nInserire numero di unità processanti da utilizzare: \t");
    scanf("%d", &core);

    a = (float*)calloc(N, sizeof(float));
    b = (float*)calloc(N, sizeof(float));
    // c = (float*)calloc(N, sizeof(float));

    printf("Inserire gli elementi del vettore a\n");
    for (i = 0; i < N; i++)
    {
        scanf("%f", &a[i]);
    }
    
    printf("Inserire gli elementi del vettore b\n");
    for (i = 0; i < N; i++)
    {
        scanf("%f", &b[i]);
    }

    t0_seq = omp_get_wtime();

    for (i = 0; i < N; i++) {
        seq_result += a[i] * b[i];
    }

    t1_seq = omp_get_wtime();

    t0 = omp_get_wtime();

    #pragma omp parallel for num_threads(core) shared(a, b) private(i) reduction(+:result)
    for (i = 0; i < N; i++) {
        result += a[i] * b[i];
        // c[i] = a[i] * b[i];
        // result += c[i];
    }

    t1 = omp_get_wtime();
    tempo_totale = t1 - t0;
    printf("Tempo finale parallelo: %lf\n", tempo_totale);

    tempo_sequenziale = t1_seq - t0_seq;
    printf("Tempo finale sequenziale: %lf\n", tempo_sequenziale);

    printf("Il risultato è: %f\n", result);

    free(a);
    free(b);
    // free(c);
}