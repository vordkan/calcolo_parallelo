/*
    Implementare un programma parallelo per l'ambiente multicore con np unità processanti che impieghi la libreria openmp. 
    Il programma deve essere organizzato come segue:

    1. Il core master deve generare una matrice B di dimensioni NxN e due vettori a,b di lunghezza N.

    2. I core devono collaborare per costruire in parallelo una nuova matrice A ottenuta sommando alla diagonale principale della matrice B il vettore b.
       
    3. Quindi i core devono collaborare per calcolare in parallelo il prodotto tra la matrice Axa distribuendo il lavoro per colonne.

    4. In fine il core master stampa il risultato finale ed il tempo di esecuzione.
*/
#include <stdio.h>
#include <stdlib.h>
#include <omp.h>
#include <time.h>

int** AllocaMat(int** matrice, int n){
    int i = 0;
    matrice = (int**) malloc (n * sizeof(int*));

    for(i = 0; i < n; i++){
        matrice[i] = (int*) malloc (n * sizeof(int));
    }
    return matrice;
}

int* AllocaVet(int* vettore, int n){
    vettore = (int*) malloc (n * sizeof (int));
    return vettore;
}

void PopolaMat(int** matrice, int n){
    int i,j;
    for(i = 0; i < n; i++){
        for(j = 0; j < n; j++){
            matrice[i][j] = (rand() % 10) + 1;
        }
    }
}

void PopolaVet(int* vettore, int n){
    int i = 0;
    for(i = 0; i < n; i++){
        vettore[i] = (rand() % 10) + 1;
    }
}

void VisualizzaMat(int** matrice, int n){
    int i,j;
    for(i = 0; i < n; i++){
        for(j = 0; j < n; j++){
            printf("[%d] \t", matrice[i][j]);
        }
        printf("\n");
    }
}

void VisualizzaVet(int* vettore, int n){
    int i = 0;
    for(i = 0; i < n; i++){
        printf("[%d]\t", vettore[i]);
    }
    printf("\n");
}

int main(int argc, char* argv[]){
    int n, np, i, j, somma = 0, prodotto = 1;
    srand(time(NULL));
    if(argc < 2){
        perror("Inserire da input la n e np\n");
        return 0;
    }

    n = atoi(argv[1]);
    np = atoi(argv[2]);
    int** A = AllocaMat(A,n);
    int** B = AllocaMat(B,n);
    int* a = AllocaVet(a,n);
    int* b = AllocaVet(b,n);
    int* prodotto_colonne = AllocaVet(prodotto_colonne, n);
    double ti, tf, tot;

    // 1. Il core master deve generare una matrice B di dimensioni NxN e due vettori a,b di lunghezza N.
    #pragma omp master
    {
        PopolaMat(B,n);
        PopolaVet(a,n);
        PopolaVet(b,n);
    }

    printf("\nMatrice B:\n");
    VisualizzaMat(B,n);

    printf("\nVettore a:\n");
    VisualizzaVet(a,n);

    printf("\nVettore b:\n");
    VisualizzaVet(b,n);

    //2. I core devono collaborare per costruire in parallelo una nuova matrice A ottenuta
    // sommando alla diagonale principale della matrice B il vettore b.
    ti = omp_get_wtime();
    #pragma omp parallel for shared(A,B,b) private(i,j) num_threads(np) reduction(+:somma)
    for(i = 0; i < n; i++)
    {
        for(j = 0; j < n; j++){
            if(i == j){
                somma = B[i][j] + b[i];
                A[i][j] = somma;
            }
            else
                A[i][j] = B[i][j];
        }
    }

    printf("\nMatrice A:\n");
    VisualizzaMat(A,n);

    //3. Quindi i core devono collaborare per calcolare in parallelo il prodotto tra la matrice Axa
    // distribuendo il lavoro per colonne.
    #pragma omp parallel for shared(A,a) private(i,j) num_threads(np) reduction(*:prodotto)
    for(i = 0; i < n; i++)
    {
        for(j = 0; j < n; j++){
            prodotto = A[j][i] * a[i];
            prodotto_colonne[i] += prodotto;
        }
    }
    tf = omp_get_wtime();
    tot = tf - ti;

    // 4. In fine il core master stampa il risultato finale ed il tempo di esecuzione.
    #pragma omp master
    {
        printf("\nVettore prodotto per colonne:\n");
        VisualizzaVet(prodotto_colonne,n);
        printf("\nTempo totale del programma: %f\n",tot);
    }
    return 0;
}