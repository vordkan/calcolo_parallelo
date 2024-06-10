/* Implementare un programma parallelo per l’ambiente multicore con np unità processanti  che impieghi la libreria OpenMP.
   Il programma deve essere organizzato come segue:

   1) Il core master deve generare e leggere una matrice A di dimensione M×N

   2) I core devono organizzarsi per estrarre ognuno una sottomatrice, decomponendo la matrice A per blocchi riga, e costruendo la trasposta di tale sottomatrice, in una matrice Bloc

   3) Infine i core devono collaborare per sommare le sotto matrici estratte Bloc in un’unica matrice finale C

   4) Il core master stampa la matrice risultato e il tempo d’esecuzione.
 */

#include <stdio.h>
#include <stdlib.h>
#include <omp.h>
#include <time.h>


int** allocaMat(int** matrice, int n) {
    int i;
    matrice = (int**)malloc(n*sizeof(int*));
    for (i=0; i<n; i++) {
        matrice[i] = (int*)malloc(n*sizeof(int));
    }
    return matrice;
}

void popolaMat(int** matrice, int n, int m) {
    int i, j;
    for(i=0; i<n; i++) {
        for(j=0; j<m; j++) {
            matrice[i][j] = (rand()%10) + 1;
        }
    }
}

void stampaMat(int** matrice, int n, int m) {
    int i, j;
    for(i=0; i<n; i++) {
        for(j=0; j<m; j++) {
            printf("[%d] \t", matrice[i][j]);
        }
        printf("\n");
    }
}

int main() {
    srand(time(NULL));
    int N, M, np, i, j;
    double tempo_iniziale, tempo_finale, tempo_totale;

    printf("Dammi mommo il numero di processi:\n");
    scanf("%d", &np);

    printf("Dammi mommo il numero di righe:\n");
    scanf("%d", &N);

    printf("Dammi mommo il numero di colonne:\n");
    scanf("%d", &M);


    int **A = allocaMat(A, N);
    int **Bloc = allocaMat(Bloc, M);
    int **C = allocaMat(C, M);

    //Punto 1
    #pragma omp master
    {
        printf("Sno il master di tutti voi miseri plebei, genero per voi la matrice A:\n");
        popolaMat(A,N,M);
        stampaMat(A,N,M);
    }

    //Punto 2
    tempo_iniziale = omp_get_wtime();
    #pragma omp parallel for shared(A, Bloc) num_threads (np) private(i,j)
    for(i=0; i<N; i++)
    {
        for(j=0; j<M; j++)
        {
            Bloc[j][i] = A[i][j];
        }
    }
    printf("\nMatrice Bloc:\n");
    stampaMat(Bloc, M, N);

    //Punto 3
    #pragma omp parallel for shared(C, Bloc) private(i, j) num_threads(np)
    for (i = 0; i < N; i++) {
        for (j = 0; j < M; j++) {
            #pragma omp critical
            {
                C[i][j] += Bloc[i][j];
            }
        }
    }

    tempo_finale = omp_get_wtime();
    tempo_totale = tempo_finale - tempo_iniziale;
    //Punto 4
    #pragma omp master
    {
        printf("\nMatrice C:\n");
        stampaMat(C, M, N);

        printf("\nIL tempo d'esecuzione e': [%f]", tempo_totale);
    }


    return 0;
}

