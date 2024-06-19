// Implementare un programma parallelo per l’ambiente multicore con np unità processanti che impieghi la libreria OpenMP.
// Il programma deve essere organizzato come segue:
//
// 1) il core master deve generare una matrice A di dimensione N×M.
// 2) Ogni core deve estrarre N/p colonne ed effettuare localmente
//    la somma degli elementi delle sottomatrici estratte,
//    conservando il risultato in un vettore b di dimensione M.

#include <stdlib.h>
#include <stdio.h>
#include <omp.h>
#include <time.h>

int** AllocaMatrice(int righe, int colonne){
    int** matrice = (int**) malloc (righe * sizeof(int*));
    int i;

    for(i = 0; i < righe; i++){
        matrice[i] = (int*) malloc(colonne * sizeof(int));
    }

    return matrice;
}

void PopolaMatrice(int **matrice, int righe, int colonne){
    int i, j;
    for(i = 0; i < righe; i++){
        for(j = 0; j < colonne; j++){
            matrice[i][j] = (rand() % 9) + 1;
        }
    }
}

void LeggiMatrice(int** matrice, int righe, int colonne){
    int i, j;

    for(i = 0; i < righe; i++){
        for(j = 0; j < colonne; j++){
            printf("[%d]\t", matrice[i][j]);
        }
        printf("\n");
    }
}

int main(int argc, char* argv[]){
    int n, m, np, i, j;
    if(argc < 3){
        perror("Inserire n, m, np\n");
        exit(-1);
    }

    n  = atoi(argv[1]);
    m  = atoi(argv[2]);
    np = atoi(argv[3]);

    printf("n  -> %d\nm  -> %d\nnp -> %d\n", n,m,np);

    int** A = AllocaMatrice(n,m);
    // 1) il core master deve generare una matrice A di dimensione N×M.
    #pragma omp master
    {
        PopolaMatrice(A,n,m);
    }

    printf("\nMatrice A\n");
    LeggiMatrice(A,n,m);

    // 2) Ogni core deve estrarre N/p colonne ed effettuare localmente
    //    la somma degli elementi delle sottomatrici estratte,
    //    conservando il risultato in un vettore b di dimensione M.

    int* b = (int*) malloc (m * sizeof(int));
    int somma;

    #pragma omp parallel for private(i,j) shared(n,m,A) num_threads(np) reduction(+:somma)
    for(j = 0; j < m; j++)
    {
        somma = 0;
        for(i = 0; i < n; i++){
            somma += A[i][j];
        }
        b[j] = somma;
    }

    printf("Vettore b:\n");
    for(i=0; i<m; i++) {
        printf("[%d] \t", b[i]);
    }
    printf("\n");
}