#include <stdio.h>
#include <omp.h>

#define N 10   // dimensione della matrice
#define P 4    // numero di blocchi da processare per iterazione
#define BLOCK_SIZE (N / P) // dimensione del blocco
#define SCALAR 2 // lo scalare da moltiplicare

int main() {
    int A[N][N], C[N][N]; // matrice di input e output
    int i, j, bi, bj;

    // inizializza la matrice A
    for (i = 0; i < N; i++) {
        for (j = 0; j < N; j++) {
            A[i][j] = i * N + j;
        }
    }

    // moltiplicazione scalare usando la direttiva parallel for
    #pragma omp parallel for num_threads(P) private(bi, bj, i, j)
    for (bi = 0; bi < N; bi += BLOCK_SIZE) {
        for (bj = 0; bj < N; bj += BLOCK_SIZE) {
            for (i = bi; i < bi + BLOCK_SIZE; i++) {
                for (j = bj; j < bj + BLOCK_SIZE; j++) {
                    C[i][j] = A[i][j] * SCALAR;
                }
            }
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
