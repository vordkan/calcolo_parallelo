#include <stdio.h>
#include <omp.h>

#define SCALAR 2 // lo scalare da moltiplicare

int main() {
    int N, P;

    printf("Inserisci la dimensione della matrice: ");
    scanf("%d", &N);
    printf("Inserisci il numero di blocchi da processare per iterazione: ");
    scanf("%d", &P);

    int A[N][N], C[N][N]; // matrice di input e output
    int i, j;

    // inizializza la matrice A
    for (i = 0; i < N; i++) {
        for (j = 0; j < N; j++) {
            A[i][j] = i * N + j;
        }
    }

    double start_time = omp_get_wtime(); // tempo di inizio dell'esecuzione

    // moltiplicazione scalare usando la direttiva parallel for
    #pragma omp parallel for num_threads(P) private(i, j)
    for (i = 0; i < N; i++) {
        for (j = 0; j < N; j++) {
            C[i][j] = A[i][j] * SCALAR;
        }
    }

    double end_time = omp_get_wtime(); // tempo di fine dell'esecuzione
    double execution_time = end_time - start_time; // calcolo del tempo di esecuzione

    // stampa la matrice risultante
    printf("Matrice C:\n");
    for (i = 0; i < N; i++) {
        for (j = 0; j < N; j++) {
            printf("%d ", C[i][j]);
        }
        printf("\n");
    }

    printf("Tempo di esecuzione: %f secondi\n", execution_time);

    return 0;
}
