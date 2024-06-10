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

int N, M, np, MAX;
double  t_i, t_f, t_tot;

int** Alloc_mat(int n, int m){
    int** a = (int**)malloc(n * sizeof(int*));
    int i;

    for(i = 0; i < n; i++){
        a[i] = (int*)malloc(m * sizeof(int));
    }

    return a;
}

int* Alloca_vet(int n){
    int* b = (int*)malloc(n * sizeof(int));
    int i;

    for(i = 0; i < n; i++){
        b[i] = (rand()%10)+1;
    }

    return b;
}

void Genera_mat(int start_row, int end_row, int m, int **a){
    int i, j;
    for(i = start_row; i < end_row; i++){
        for(j = 0; j < m; j++){
            a[i][j] = (rand()%10)+1;
        }
    }
}


void Read_mat(int n, int m, int **a){
    printf("\nMatrice generata\n");
    int i, j;

    for(i = 0; i < n; i++){
        for(j = 0; j < m; j++){
            printf("%d \t", a[i][j]);
        }
        printf("\n");
    }
}

void Leggi_vett(int n, int *b){
    printf("\nVettore generato\n");
    int i;
    for(i = 0; i < n; i++)
        printf("[%d] \t", b[i]);
    printf("\n");
}

int* prodotto_locale(int **a, int *b, int *c){
    int i,j;
    #pragma omp parallel for shared(a, b, N, M) private(i, j) reduction(+:c[:M])
    for (i = 0; i < N; i++) {
        for (j = 0; j < M; j++) {
            c[i] += a[i][j] * b[j];
        }
    }

    return c;
}

int max(int n, int *a){
    int massimo = a[0], i;

    #pragma omp parallel for shared(N,M,a) private(i) reduction(max:MAX)
    for(i = 0; i < n; i++)
        if(a[i] > massimo)
            massimo = a[i];

    return massimo;
}


int main(){
    srand(time(NULL));
    
    printf("Inserire numero di righe\n");
    scanf("%d", &N);
    printf("Inserire numero di colonne\n");
    scanf("%d", &M);
    printf("Inserisci numero di processi\n");
    scanf("%d",&np);


    int **a = Alloc_mat(N, M); 
    int *b = Alloca_vet(M);
    int *c = (int*)malloc(M*sizeof(int));
    int n = N / np;
    int i, j;


    #pragma omp parallel num_threads(np) shared(a, N, M, np, n)
    {
        // assegno alla variabile l'id
        int thread_id = omp_get_thread_num();

        // assegno ad ogni thread un inizio e una fine
        int start_row = thread_id * n;
        int end_row = start_row + n;

        // assegno al primo thread il resto 
        if (thread_id == 0) {
            end_row = N; 
        }
        Genera_mat(start_row, end_row, M, a);
    }

    Read_mat(N, M, a);

    #pragma omp master
    {
        Leggi_vett(M,b);
    }

    c = prodotto_locale(a,b,c);
    Leggi_vett(M,c);

    t_i = omp_get_wtime();
    MAX = max(M,c);
    t_f = omp_get_wtime();

    t_tot = t_f - t_i;

    #pragma omp master
    {
        printf("Sono il core supremo prostati ai miei piedi\n");
        printf("\nIl massimo del vettore c trovato è %d\nIl tempo di esecuzione è %f\n", MAX,t_tot);
    }

    return 0;
}

