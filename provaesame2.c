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

int np, N;

int** AllocaMat(int** matrice, int n){
    matrice = (int**)malloc(n * sizeof(int*));

    for(int i = 0; i < n; i++)
        matrice[i] = (int*)malloc(n * sizeof(int));

    return matrice;
}

int* AllocaVett(int* vettore, int n){
    vettore = (int*)malloc(n*sizeof(int));
    return vettore;
}

void PopolaMat(int** matrice, int n){
    for(int i = 0; i < n; i++){
        for(int j = 0; j < n; j++){
            matrice[i][j] = (rand()%10) + 1;
        }
    }
}

void PopolaVett(int* vettore, int n){
    for(int i = 0; i < n; i++){
        vettore[i] = (rand()%10)+1;
    }
}

void VisualizzaMat(int** matrice, int n){
    for(int i = 0; i < n; i++){
        for(int j = 0; j < n; j++){
            printf("[%d]\t",matrice[i][j]);
        }
        printf("\n");
    }
}

void VisualizzaVett(int* vettore, int n){
    for(int i = 0; i < n; i++){
        printf("[%d]\t",vettore[i]);
    }
    printf("\n");
}

int main(){
    int** B;
    int** A;
    int* a;
    int* b;
    int i, j;
    double ti,tf,t_tot;
    srand(time(NULL));

    printf("Inserire il numero di processi\n");+
    scanf("%d",&np);

    printf("Inserire la dimensione N\n");
    scanf("%d",&N);

    // 1. Il core master deve generare una matrice B di dimensioni NxN e due vettori a,b di lunghezza N.
    #pragma omp master
    {
        printf("Genero la matrice...\n");
        B = AllocaMat(B,N);
        PopolaMat(B,N);

        printf("Genero i vettori a e b...\n");
        a = AllocaVett(a,N);
        b = AllocaVett(b,N);

        PopolaVett(a,N);
        PopolaVett(b,N);
    }

    printf("\nIl core master ha generato la matrice B:\n");
    VisualizzaMat(B,N);

    printf("\nIl core master ha generato il vettore a\n");
    VisualizzaVett(a,N);

    printf("\nIl core master ha generato il vettore b\n");
    VisualizzaVett(b,N);

    ti = omp_get_wtime();
    // 2. I core devono collaborare per costruire in parallelo una nuova matrice A ottenuta sommando alla diagonale principale della matrice B il vettore b.
    A = AllocaMat(A,N);
    #pragma omp parallel for private(i,j) shared(B,b) num_threads(np)
    for(i = 0; i < N; i++)
    {
        for(j = 0; j < N; j++){
            if(i == j)
                A[i][j] = B[i][j] + b[i];
            else 
                A[i][j] = B[i][j];
        }
    }
    
    printf("\nMatrice A:\n");
    VisualizzaMat(A,N);
    // 3. Quindi i core devono collaborare per calcolare in parallelo il prodotto tra la matrice Axa distribuendo il lavoro per colonne.
    #pragma omp parallel for private(i,j) shared(a) num_threads(np)
    for(j = 0; j < N; j++)
    {
        for(i = 0; i < N; i++){
            A[i][j] *= a[j];
        }
    }
    tf = omp_get_wtime();

    t_tot = tf - ti;

    // 4. In fine il core master stampa il risultato finale ed il tempo di esecuzione.
    #pragma omp master
    {
        printf("\nRisultato matrice A moltiplicando per colonne Axa:\n");
        VisualizzaMat(A,N);
        printf("\nTempo di esecuzione del programma %f:\n",t_tot);
    }

    return 0;
}

