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
int** allocaMat(int n, int m) {
    int i;
    int **matrice = (int**)malloc(n*sizeof(int));
    for(i=0; i<n; i++) { {
            matrice[i] = (int*)malloc(m*sizeof(int));
        }
    }
    return matrice;
}

void stampaMat(int** matrice, int n, int m){
    int i,j;
    for(i = 0; i < n; i++){
        for(j = 0; j < m; j++){
            printf("[%d]\t", matrice[i][j]);
        }
        printf("\n");
    }
}

int* allocaVet(int n) {
    int* vettore = (int*)malloc(n*sizeof(int));
    return vettore;
}

void popolaVet(int* vettore, int n) {
    int i;
    for(i=0; i<n; i++) {
        vettore[i] = (rand()%9) + 1;
    }
}

void stampaVet(int* vettore, int n){
    int i;
    for(i=0; i<n; i++) {
        printf("[%d] \t", vettore[i]);
    }
    printf("\n");
}


int main(int argc, char* argv[]){
    if(argc < 4){
        perror("Inserire n, m ed np\n");
        exit(-1);
    }

    int n, m, np, i, j, N;
    double t, ti, tf;
    n = atoi(argv[1]);
    m = atoi(argv[2]);
    np = atoi(argv[3]);
    N = n/np;
    // 1. Ogni core deve generare una matrice A di dimensione N/np x M,
    //    mentre solo il core master deve leggere un vettore b di lunghezza M
    int** A = allocaMat(N, m);
    int* b = allocaVet(m);

    #pragma omp parallel for private(i,j) shared(A, m, N) num_threads(np)
    for(i = 0; i < N; i++){
        for(j = 0; j < m; j++){
            A[i][j] = (rand() % 9) + 1;
        }
    }

    printf("\nMatrice A:\n");
    stampaMat(A,N,m);

    popolaVet(b,m);

    #pragma omp master
    {
        printf("\nVettore b:\n");
        stampaVet(b,m);
    }
    ti = omp_get_wtime();
    //2. Tutti i core devono calcolare localmente il prodotto c = A x b
    int* c = (int*) malloc (N * sizeof(int));
    int somma = 0;
    #pragma omp parallel for shared (A, b, c, N, m) private(i, j) num_threads(np) reduction(+:somma)
    for(i=0; i<N; i++)
    {
        somma = 0;
        for(j=0; j<m; j++) {
            somma += A[i][j] * b[j];
        }
        c[i] = somma;
    }

    printf("\nVettore c:\n");
    stampaVet(c, N);

    // 3. Quindi, i core devono organizzarsi per determinare il massimo
    // *    valore tra tutti gli elementi dei vettori locali c e il massimo
    // *    globale tra tutti i vettori locali c

    int massimo = 0;
    #pragma omp parallel for shared(c,m) private(i) num_threads(np) reduction(max:massimo)
    for(i = 0; i < N; i++)
    {
        if(c[i] > massimo )
             massimo = c[i];
    }
    tf = omp_get_wtime();
    t = tf - ti;

    // 4. Infine, il core master deve stampare il valore massimo
    //    globale trovato ed il tempo d'esecuzione.
    #pragma omp master
    {
        printf("\nMassimo trovato tra i vettori: %d. Tempo di esecuzione totale %f\n", massimo, t);
    }
}
