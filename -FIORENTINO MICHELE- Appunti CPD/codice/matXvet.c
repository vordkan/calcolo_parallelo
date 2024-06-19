#include <stdlib.h>
#include <stdio.h>
#include <time.h>

typedef int* Array;
typedef int** Matrix;

void createMatrix(Matrix *A, int n, int m, int maxVal){
	int i,j;

	*A = malloc(n*sizeof(int*));
	for(i=0; i<n; i++)
		(*A)[i] = malloc(m*sizeof(int));

	for(i=0; i<n; i++)
		for(j=0; j<m; j++)
			(*A)[i][j] = rand()%maxVal;
}

void createVector(Array *b, int m, int maxVal){
	*b = malloc(m*sizeof(int));
	for(int i=0; i<m; i++)
		(*b)[i] = rand()%maxVal;
}

void printMatrix(Matrix A, int n, int m){
	printf("A =\n");
	for(int i=0; i<n; i++){
		for(int j=0; j<m; j++){
			printf("%d\t", A[i][j]);
		}
		printf("\n");
	}
	printf("\n");
}

void printVector(Array b, int m){
	printf("b =\n");
	for(int i=0; i<m; i++)
		printf("%d\t", b[i]);
	printf("\n\n");
}

int main(int argc, char** argv){

	if(argc<4){ 
		printf("Not enough arguments. Insert: rows, cols, maxVal\n"); 
		return -1; 
	}

	//INITIALIZE

	int **A;
	int *b;
	int n = atoi(argv[1]);
	int m = atoi(argv[2]);
	int maxVal = atoi(argv[3]);

	srand(time(NULL));
	createMatrix(&A,n,m,maxVal);
	createVector(&b,m,maxVal);

	printMatrix(A,n,m);
	printVector(b,m);

	//PRODUCT 

	int *c = calloc(n,sizeof(int));

	int i,j;
	for(i=0; i<n; i++)
		for(j=0; j<m; j++)
			c[i] = c[i]+A[i][j]*b[j];


	printVector(c,n);
	return 0;
}