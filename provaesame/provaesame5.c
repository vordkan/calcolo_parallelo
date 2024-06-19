// Implementare un programma parallelo per l’ambiente multicore con np unità processanti che impieghi la libreria OpenMP.
// Il programma deve essere organizzato come segue:

// 1) Il core master deve leggere un vettore a, di dimensione N ed uno scalare beta.
// 2) I core devo collaborare per verificare se nel vettore  esista almeno un elemento uguale al valore beta.

#include <stdio.h>
#include <stdlib.h>
#include <omp.h>
#include <time.h>

int* AllocaVett(int n){
    int* vettore = (int*) malloc (n * sizeof(int));
    return vettore;
}

void RiempiVett(int* vettore, int n){
    int i;
    for(i = 0; i < n; i++)
        vettore[i] = (rand() % 20) + 1;
}

void LeggiVett(int* vettore, int n){
    int i;
    for( i = 0; i < n; i++)
        printf("[%d]\t", vettore[i]);
    printf("\n");
}
int main(int argc, char* argv[]){
    int n, beta, i;

    if(argc < 2){
        perror("Inserire n e lo scalare beta all'avvio del programma\n");
        exit(-1);
    }

    n    = atoi(argv[1]);
    beta = atoi(argv[2]);

    printf("Valore di n -> %d\n", n);
    int* a = AllocaVett(n);
    RiempiVett(a,n);

    // 1) Il core master deve leggere un vettore a, di dimensione N ed uno scalare beta.
    #pragma omp master
    {
        printf("\nVettore a:\n");
        LeggiVett(a,n);
        printf("\nValore dello scalare beta -> %d\n", beta);
    }

    int flag = 0;
    // 2) I core devo collaborare per verificare se nel vettore  esista almeno un elemento uguale al valore beta.
    #pragma omp parallel for private(i) shared(a,beta)
    for(i = 0; i < n; i++)
    {
        if(a[i] == beta){
            printf("È stato trovato beta in posizione %d\n", i+1);
            flag = 1;
        }
    }
    if(flag == 0){
        perror("Non esistono elementi di a uguali allo scalare beta\n");
        exit(-1);
    }
    return 0;
}