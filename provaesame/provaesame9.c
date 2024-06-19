/**
 * Implementare un programma parallelo per l’ambiente multicore con np unità processanti che impieghi la libreria OpenMP.
 * Il programma deve essere organizzato come segue:
 * 1) Il core master deve leggere un vettore a, di dimensione M, e uno scalare alpha.
 * 2) I core devono collaborare per costruire una matrice A, di dimensione M×M,
 *    la cui diagonale principale ha per elementi quelli del vettore a moltiplicati per lo scalare alpha.
 */

#include <stdio.h>
#include <omp.h>
#include <stdlib.h>
#include <time.h>

int* AllocaVettore(int size){
    int* vettore = (int*) malloc (size * sizeof(int));

    return vettore;
}

void PopolaVettore(int size, int* vettore){
    int i;
    for(i = 0; i < size; i++){
        vettore[i] = (rand () % 9) + 1;
    }
}

void LeggiVettore(int size, int* vettore){
    int i;
    for(i = 0; i < size; i++){
        printf("[%d]\t", vettore[i]);
    }
}

int** AllocaMatrice(int righe, int colonne){
    int** matrice = (int**) malloc (righe * sizeof(int*));
    int i;

    for(i = 0; i < righe; i++){
        matrice[i] = (int*) malloc(colonne * sizeof(int));
    }

    return matrice;
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
    int i, j, alpha, m, np;
    if(argc < 3){
        perror("Inserire m, alpha ed np\n");
        exit(-1);
    }

    m      = atoi(argv[1]);
    np     = atoi(argv[3]);

    int* a = AllocaVettore(m);

    // 1) Il core master deve leggere un vettore a, di dimensione M, e uno scalare alpha.
    #pragma omp master
    {
        PopolaVettore(m,a);
        alpha  = atoi(argv[2]);

        printf("\nVettore a:\n");
        LeggiVettore(m,a);
        printf("\nm = %d, alpha = %d, np =%d\n",m , alpha, np);
    }

    //2) I core devono collaborare per costruire una matrice A, di dimensione M×M,
    //   la cui diagonale principale ha per elementi quelli del vettore a moltiplicati per lo scalare alpha.
    int** A = AllocaMatrice(m,m);

    #pragma omp parallel for shared(A, a, alpha) private(i, j) num_threads(np)
    for (i=0; i<m; i++) {
        for (j=0; j<m; j++) {
            if (j == i)
                A[i][j] = a[i] * alpha;
            else
                A[i][j] = (rand() % 9) + 1;
        }
    }

    printf("\nMatrice A:\n");
    LeggiMatrice(A,m,m);
}