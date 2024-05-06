#include <stdio.h>
#include <omp.h>

#define N 10   // dimensione della matrice
#define P 4    // numero di righe da processare per iterazione
#define SCALAR 2 // lo scalare da moltiplicare

int main() {
    int A[N][N], C[N][N]; // matrice di input e output
    int i, j;

    // inizializza la matrice A
    for (i = 0; i < N; i++) {
        for (j = 0; j < N; j++) {
            A[i][j] = i * N + j;
        }
    }

    // moltiplicazione scalare usando la direttiva parallel for
    #pragma omp parallel for num_threads(P) private(i, j)
    for (i = 0; i < N; i++) {
        for (j = 0; j < N; j++) {
            C[i][j] = A[i][j] * SCALAR;
        }
    }

    // stampa la matrice risultante
    printf("Matrice C:\n");
    for (i = 0; i < N; i++) {
        for (j = 0; j < N; j++) {
            printf("%d ", C[i][j]);
        }
        printf("\n");
    }

    return 0;
}