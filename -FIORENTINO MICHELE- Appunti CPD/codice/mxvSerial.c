#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <omp.h>
#include <time.h>

int** createMatrix_NxN(int n, int maxVal){
	int i,j;

	int **A = malloc(n*sizeof(int*));
	for(i=0; i<n; i++)
		A[i] = malloc(n*sizeof(int));

	for(i=0; i<n; i++)
		for(j=0; j<n; j++)
			A[i][j] = rand()%maxVal+1; //avoids zeros

	return A;
}

void printMatrix(int** A, int n, int m){

	if(n == 0)
		#pragma omp parallel master shared(n)
		n = omp_get_num_threads();

	printf("A =\n");
	for(int i=0; i<n; i++){
		for(int j=0; j<m; j++){
			printf("%d\t", A[i][j]);
		}
		printf("\n");
	}
	printf("\n");
}

void printVector(int *b, int n){
	printf("b =\n");
	for(int i=0; i<n; i++)
		printf("%d\t", b[i]);
	printf("\n\n");
}

int* serialPunctualProduct(int** A, int n){	

	int *b = malloc(n*sizeof(int));

	int i,j;
	for(i=0; i<n; i++){
		b[i] = A[0][i];
		for(j=1; j<n; j++)
			b[i] *= A[j][i];
	}

	return b;
}

int* partialPunctualProduct(int**A, int n, int m){ //int* , vettore risultato

	int *b = malloc(m*sizeof(int));

	int i,j;
	for(i=0; i<m; i++){
		b[i] = A[0][i];
		for(j=1; j<n; j++)
			b[i] *= A[j][i];
	}

	return b;
} 

int** parallelPunctualProduct(int** A, int n, int numt){	//int** , vettore di vettori ris

	int i,j,idt,nloc,r,step;

	if(numt == 0)
		#pragma omp parallel master shared(numt)
		numt = omp_get_num_threads();

	int** bVectors = malloc(numt*sizeof(int*));
	
	double t,t1,t2;
	t1 = omp_get_wtime();
	#pragma omp parallel shared(A,n,numt) private(i,j,idt,nloc,r,step) num_threads(numt)
	{
		idt = omp_get_thread_num();
		nloc = n/numt;
		r = n%numt;

		if(idt < r){
			nloc++;
			step = 0;
		} else {
			step = r;
		}
		
		bVectors[idt] = partialPunctualProduct(&A[idt*nloc+step],nloc,n);
	}
	t2 = omp_get_wtime();

	t = t2-t1;

	printf("Total Time with %d processors and n=%d: %f\n\n",numt,n,t);

	return bVectors;
}

int main(int argc, char** argv){

	if(argc<4){
		printf("Not enough arguments. Insert: dim, maxVal, numP (default=0)\n");
		return -1;
	}

	//INITIALIZE

	srand(time(NULL));

	int n = atoi(argv[1]);
	int maxVal = atoi(argv[2]);
	int p = atoi(argv[3]);

	if(n<p){
		printf("the number of cores cannot be less than N\n");
		return -2;
	}

	int **A = createMatrix_NxN(n,maxVal);
	printMatrix(A,n,n);

	//PUNCTUAL PRODUCT

	int *b = serialPunctualProduct(A,n);
	printVector(b,n);

	int** bVectors = parallelPunctualProduct(A,n,p);
	printMatrix(bVectors,p,n);


	return 0;
}