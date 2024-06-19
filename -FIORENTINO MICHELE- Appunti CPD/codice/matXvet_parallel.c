#include <stdlib.h>
#include <stdio.h>
#include <time.h>

typedef double* Array;
typedef double** Matrix;

//Utilities

void createMatrix(Matrix *A, int n, int m, double maxVal){
	int i,j;

	*A = malloc(n*sizeof(double*));
	for(i=0; i<n; i++)
		(*A)[i] = malloc(m*sizeof(double));

	for(i=0; i<n; i++)
		for(j=0; j<m; j++)
			(*A)[i][j] = ((double)rand()*maxVal)/(double)RAND_MAX;
}

void createVector(Array *x, int m, double maxVal){
	*x = malloc(m*sizeof(double));
	for(int i=0; i<m; i++)
		(*x)[i] = ((double)rand()*maxVal)/(double)RAND_MAX;
}

void printMatrix(Matrix A, int n, int m){
	printf("A =\n");
	for(int i=0; i<n; i++){
		for(int j=0; j<m; j++){
			printf("%.2f\t", A[i][j]);
		}
		printf("\n");
	}
	printf("\n");
}

void printVector(Array b, int m){
	printf("b =\n");
	for(int i=0; i<m; i++)
		printf("%.2f\t", b[i]);
	printf("\n\n");
}


//Product in parallel

double* matxvet(double **A, double *x, int n, int m){
	int i,j;
	double *b = calloc(n,sizeof(double)); //allocation

	#pragma omp parallel for default(none) shared(A,x,b,n,m) private(i,j)
	for(i=0; i<n; i++)
		for(j=0; j<m; j++)
			b[i] += A[i][j]*x[j];

	return b; //returned from master
}


int main(int argc, char** argv){

	if(argc<4){
		printf("not enough arguments. Insert: rows, cols, maxVal");
		return -1;
	}

	double **A;
	double *x;
	int n = atoi(argv[1]);
	int m = atoi(argv[2]);
	double maxVal = atof(argv[3]);

	srand(time(NULL));
	createMatrix(&A,n,m,maxVal);
	createVector(&x,m,maxVal);

	double *b = matxvet(A,x,n,m);

	printMatrix(A,n,m);
	printVector(x,m);
	printVector(b,n);
	return 0;
}