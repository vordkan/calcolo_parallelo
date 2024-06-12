/**
 * PROVA SCRITTA LABORATORIO TRACCIA 1: 29-06-2023
 *
 * Implementare un programma parallelo per l'ambiente multicore
 * con np unità processanti che impieghi la libreria OpenMP.
 *
 * Il programma deve essere organizzato come segue:
 *
 * 1. Ogni core deve generare una matrice A di dimensione N/np x M,
 *    mentre solo il core master deve leggere un vettore b di lunghezza M
 *
 * 2. Tutti i core devono calcolare localmente il prodotto c = A x b
 *
 * 3. Quindi, i core devono organizzarsi per determinare il massimo
 *    valore tra tutti gli elementi dei vettori locali c e il massimo
 *    globale tra tutti i vettori locali c
 *
 * 4. Infine, il core master deve stampare il valore massimo
 *    globale trovato ed il tempo d'esecuzione.
*/


#include <stdio.h>
#include <stdlib.h>
#include <omp.h>
#include <time.h>


int** allocaMat(int** matrice, int n, int m) {
    int i;
    matrice = (int**)malloc(n*sizeof(int));
    for(i=0; i<n; i++) { {
            matrice[i] = (int*)malloc(m*sizeof(int));
        }
    }
    return matrice;
}

void popolaMat(int** matrice, int riga_inizio, int riga_fine, int m) {
    int i, j;
    for(i=riga_inizio; i<riga_fine; i++) {
        for(j=0; j<m; j++) {
            matrice[i][j] = (rand()%20) +1;
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

int* allocaVet(int* vettore, int n) {
    vettore = (int*)malloc(n*sizeof(int));
    return vettore;
}

void popolaVet(int* vettore, int n) {
    int i;
    for(i=0; i<n; i++) {
        vettore[i] = (rand()%20) + 1;
    }
}

void stampaVet(int* vettore, int n){
    int i;
    for(i=0; i<n; i++) {
        printf("[%d] \t", vettore[i]);
    }
    printf("\n");
}


int main(int argc, char* argv[]) {
    srand(time(NULL));
    int N, M, np, i, j, massimo = 0;
    double tempo_iniziale, tempo_finale, tempo_totale;

    if(argc < 2) {
        perror("Inserire il numero di righe, colonne ed processi");
        return -1;
    }

    N = atoi(argv[1]);
    M = atoi(argv[2]);
    np = atoi(argv[3]);


    int** A = allocaMat(A, N, M);
    int* b = allocaVet(b, M);
    int* c= allocaVet(c, M);

    //Punto 1
    #pragma omp parallel shared (A, N, M) num_threads(np)
    {
        int id = omp_get_thread_num();

        int inizio = id * (N/np);
        int fine = inizio + (N/np);

        if (id == 0) {
            fine = N;
        }

        popolaMat(A, inizio, fine, M);
    }

    printf("Matrice A:\n");
    stampaMat(A, N, M);


    popolaVet(b, M);
    #pragma omp master
    {
        printf("\nVettore b:\n");
        stampaVet(b, M);
    }

    tempo_iniziale = omp_get_wtime();
    //Punto 2
    #pragma omp parallel for shared (A, b, c, N, M) private(i, j) num_threads(np)
    for(i=0; i<N; i++)
    {
        for(j=0; j<M; j++) {
            c[j] += A[i][j] * b[j];
        }
    }

    printf("\nVettore c:\n");
    stampaVet(c, M);

    //Punto 3
    #pragma omp parallel for shared(c) private(i) reduction(max:massimo)
    for (i=0; i<M; i++)
    {
        if(c[i] > massimo) {
            massimo = c[i];
        }
    }

    tempo_finale = omp_get_wtime();
    tempo_totale = tempo_finale - tempo_iniziale;
    //Punto 4
    #pragma omp master
    {
        printf("\nIl massimo globale è: %d\nIl tempo d'esecuzione è: %f\n", massimo, tempo_totale);
    }
    return 0;
}