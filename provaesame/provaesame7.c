/**Implementare un programma parallelo per l’ambiente multicore con np unità processanti che impieghi la libreria OpenMP.
 * Il programma deve essere organizzato come segue:
 * 1) il core master deve generareuna matrice di dimensione N×N,
 * 2) ogni core deve estrarre N/np righe e calcolare il prodotto puntuale tra i vettori corrispondenti alle righe estratte.
**/

#include <stdio.h>
#include <omp.h>
#include <stdlib.h>
#include <time.h>

int** AllocaMatrice(int n){
    int** matrice = (int**) malloc (n * sizeof(int*));
    int i;
    for(i = 0; i < n; i++){
        matrice[i] = (int*) malloc (n * sizeof(int));
    }
    return matrice;
}

void GeneraMatrice(int** matrice, int n){
    int i,j;
    for(i = 0; i < n; i++){
        for(j = 0; j < n; j++){
            matrice[i][j] = (rand() % 9) + 1;
        }
    }
}

void LeggiMatrice(int** matrice, int n){
    int i,j;
    for(i = 0; i < n; i++){
        for(j = 0; j < n; j++){
            printf("[%d]\t", matrice[i][j]);
        }
        printf("\n");
    }
}

int main(int argc, char* argv[]){
    int n, np, i, j, prodotto;

    if(argc < 2){
        perror("Inserire n ed np all'avvio\n");
        exit(-1);
    }

    n  = atoi(argv[1]);
    np = atoi(argv[2]);

    printf("n  -> %d\nnp -> %d\n", n, np);

    int** A = AllocaMatrice(n);
    //1) il core master deve generareuna matrice di dimensione N×N
    #pragma omp master
    {
        GeneraMatrice(A,n);
    }

    printf("\nMatrice A:\n");
    LeggiMatrice(A,n);

    //2) ogni core deve estrarre N/np righe e
    //   calcolare il prodotto puntuale tra i vettori corrispondenti alle righe estratte.

    int* prodotti = (int*) malloc(n * sizeof(int));
    #pragma omp parallel for private(i,j) shared(A,n,prodotti) num_threads(np) reduction(*:prodotto)
    for(i = 0; i < n; i++)
    {
        prodotto = 1;
        for(j = 0; j < n; j++){
            prodotto *= A[i][j];
        }
        prodotti[i] = prodotto;
    }

    printf("\nRisultati puntuali:\n");
    for(i = 0; i < n; i++){
        printf("[%d]\t", prodotti[i]);
    }
    printf("\n");
    return 0;
}