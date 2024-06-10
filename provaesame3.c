/* Implementare un programma parallelo per l’ambiente multicore con np unità processanti  che impieghi la libreria OpenMP.
   Il programma deve essere organizzato come segue: 

   1) Il core master deve generare e leggere una matrice A di dimensione M×N

   2) I core devono organizzarsi per estrarre ognuno una sottomatrice, decomponendo la matrice A per blocchi riga, e costruendo la trasposta di tale sottomatrice, in una matrice Bloc

   3) Infine i core devono collaborare per sommare le sotto matrici estratte Bloc in un’unica matrice finale C

   4) Il core master stampa la matrice risultato e il tempo d’esecuzione. 
 */

#include <stdio.h>
#include <stdlib.h>
#include <omp.h>
#include <time.h>

int** AllocaMat(int** matrice, int righe, int colonne){
    int i;
    matrice = (int**) malloc(righe * sizeof(int*));

    for(i = 0; i < righe ; i++){
        matrice[i] = (int*) malloc(colonne * sizeof(int));
    }
    return matrice;
}

void PopolaMat(int** matrice, int n, int m){
    int i, j;
    for(i = 0; i < n; i++){
        for(j = 0; j < m; j++){
            matrice[i][j] = (rand() % 10) +1;
        }
    }
}

void VisualizzaMat(int** matrice, int n, int m){
    int i, j;
    for(i = 0; i < n; i++){
        for(j = 0; j < m; j++){
            printf("[%d]\t", matrice[i][j]);
        }
        printf("\n");
    }
}

int main(){
    srand(time(NULL));
    int n, m, np;
    double ti, tf, t_tot;

    printf("Inserire il numero processi ch si vuole utilizzare\n");
    scanf("%d", &np);

    printf("Inserire il numero di righe\n");
    scanf("%d",&n);

    printf("Inserire il numero di colonne\n");
    scanf("%d",&m);

    int **A = AllocaMat(A,n,m);
    int **Bloc = AllocaMat(Bloc, m,n);
    int **c = AllocaMat(c,m, n);

    // 1) Il core master deve generare e leggere una matrice A di dimensione M×N
    #pragma omp master
    {
        printf("Sono il core master ora genero la matrice A...\n");
        PopolaMat(A,n,m);

        printf("Matrice A:\n");
        VisualizzaMat(A,n,m);
    }

    ti = omp_get_wtime();
    //2) I core devono organizzarsi per estrarre ognuno una sottomatrice,
    // decomponendo la matrice A per blocchi riga, e costruendo la trasposta di tale sottomatrice,
    // in una matrice Bloc


    int i,j;
    #pragma omp parallel for num_threads(np) shared(A,Bloc) private(i,j)
    for(i = 0; i < n; i++)
    {
        for(j = 0; j < m; j++){
            Bloc[j][i] = A[i][j];
        }
    }

    printf("\nMatrice Bloc:\n");
    VisualizzaMat(Bloc,m,n);

    //3) Infine i core devono collaborare
    // per sommare le sotto matrici estratte Bloc in un’unica matrice finale C



    #pragma omp parallel for num_threads(np) shared(c, Bloc) private(i, j)
    for (i = 0; i < m; i++) {
        for (j = 0; j < n; j++) {
            #pragma omp critical
            {
                c[i][j] += Bloc[i][j];
            }
        }
    }

    tf = omp_get_wtime();

    t_tot = tf - ti;
    //4) Il core master stampa la matrice risultato e il tempo d’esecuzione.
    #pragma omp master
    {
        printf("\nMatrice c:\n");
        VisualizzaMat(c,m,n);
        printf("\nTempo di esecuzione del programma: %f\n",t_tot);
    }
    return 0;
}