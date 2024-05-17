/*
* Assegnata una matrice di dimensione NxN. 
* Estrarre in parallelo gli elementi della diagonale principale e ricopiarli in n vettore. 
* Calcolarne il massimo, con una procedura parallela.
*/


/*
    if su NC se è 2,4,8,16 con reduction
    altrimenti critical()
*/
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <omp.h>
#define NC 8

int** creaMatrice(int n){
    int **mat = (int**)malloc(n * sizeof(int*));
    
    for(int i = 0; i < n; i++)
        mat[i] = (int*)malloc(n * sizeof(int));

    for(int i = 0; i < n; i++){
        for(int j = 0; j < n; j++){
            mat[i][j] = rand() % 5000;
        }
    }

    return mat;
}

void visualizzaMat(int **mat, int n){
    printf("Matrice:\n");
    for(int i = 0; i < n; i++){
        for(int j = 0; j < n; j++){
            printf("%d\t",mat[i][j]);
        }
        printf("\n");
    }
}

int* copiaDiagonale(int **mat, int n){
    int *diagonale = (int*)malloc(n * sizeof(int));
    int i = 0;
    #pragma omp parallel for shared(diagonale, mat, n) private(i) num_threads(NC)
    for(i = 0; i < n; i++)
    {
        diagonale[i] = mat[i][i];
    }

    return diagonale;
}

void visualizzaDiagonale(int *diagonale, int n){
    printf("\nVettore:\n");
    for(int i = 0; i < n; i++){
        printf("%d\t", diagonale[i]);
    }
    printf("\n");
}

int max_strategia1(int *diagonale, int n) {
    int massimo = 0, i;

    #pragma omp parallel for shared(diagonale, n) private(i) num_threads(NC)
    for(i = 0; i < n; i++) {
        #pragma omp critical
        {
            if(diagonale[i] > massimo)
                massimo = diagonale[i];
        }
    }
    return massimo;
}

int max_strategia2(int *diagonale, int n){
    int massimo = 0, i = 0;
    #pragma omp parallel for shared(diagonale, n) private (i) reduction(max:massimo) num_threads(NC)
    for(i = 0; i < n; i++)
    {
        if(diagonale[i] > massimo)
            massimo = diagonale[i];
    }

    return massimo;
}

int main(int argc, char *argv[]){
    double t0,t1,t;
    int massimo = 0;
    if(argc != 2){
        printf("Inserire un argomento all'avvio da linea di comando...\nEsempio ./a.out 100\n");
        return 1;
    }    
    int n = atoi(argv[1]);

    // Creazione e visualizzazione matrice
    int **mat = creaMatrice(n);
    visualizzaMat(mat,n);

    // Inizio calcolo tempo
    t0 = omp_get_wtime();

    // Copia della diagonale nel vettore
    int *diagonale = copiaDiagonale(mat,n);
    visualizzaDiagonale(diagonale,n);

    // Trovare il massimo del vettore
    if(NC == 2 || NC == 4 || NC == 8 || NC ==16)
        massimo = max_strategia2(diagonale,n);
    else
        massimo = max_strategia1(diagonale,n);

    // Fine calcolo tempo 
    t1 = omp_get_wtime();
    t = t1 - t0;

    printf("\nMassimo della diagonale: %d\nCon tempo %f\n", massimo,t);
    return 0;
}