// Implementare un programma parallelo per l’ambiente multicore con np unità processanti che impieghi la libreria OpenMP.
// Il programma deve essere organizzato come segue:

// 1) il core master deve leggere quattro valori scalari a0, a1, a2, a3
// 2) I core devono collaborare per costruire un vettore di dimensione 4np
//    i cui elementi sono dati dalla somma di tali valori con il proprio identificativo.

#include <stdio.h>
#include <omp.h>
#include <stdlib.h>
#include <time.h>

int main(int argc, char* argv[]){
    int np, n, i, somma;
    int* a = (int*) malloc (4 * sizeof(int));
    if(argc < 5){
        perror("Inserire i 4 elementi e np\n");
        exit(-1);
    }

    // 1) il core master deve leggere quattro valori scalari a0, a1, a2, a3
    #pragma omp master
    {
        for(i = 1; i < 5; i++){
            a[i] = atoi(argv[i]);
        }

        np = atoi(argv[5]);

        for(i = 1; i < 5; i++){
            printf("a[%d] -> %d\n", i-1, a[i]);
        }

        printf("np   -> %d\n", np);
    }

    // 2) I core devono collaborare per costruire un vettore di dimensione 4np
    //    i cui elementi sono dati dalla somma di tali valori con il proprio identificativo.
    n = 4 * np;
    int* b = (int*) malloc (n * sizeof(int));
    int indice;

    #pragma omp parallel for shared(a,n) private(i) reduction(+:somma)
    for(i = 0; i < n; i++)
    {
        somma = 0;

        // faccio in modo da rendere il vettore circolare
        indice = i % 4;
        somma += a[indice] + indice;

        b[i] = somma;
    }


    printf("\nVettore b:\n");
    for (i = 0; i < n; i++) {
        printf("b[%d] = %d\n", i, b[i]);
    }

}