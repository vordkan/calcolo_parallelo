#include <stdlib.h>
#include <stdio.h>
#include <time.h>

double** createMatrix(int n, int maxVal){
	double **A = malloc(n*sizeof(double*));
	for(int i=0; i<n; i++)
		A[i] = malloc(n*sizeof(double));

	for(int i=0; i<n; i++)
		for(int j=0; j<n; j++)
			A[i][j] = ((double)rand()*maxVal)/(double)RAND_MAX;

	return A;
}

void printMatrix(double** A, char* name, int n, int m){
	printf("%s = [\n",name);
	for(int i=0; i<n; i++){
		for(int j=0; j<m; j++){
			printf("%.2f\t",A[i][j]);
		}
		printf("\n");
	}
	printf("]\n");
}

int main(int argc, char** argv){

	if(argc<3){
		printf("Not enough arguments: insert dim and maxVal\n");
		return -1;
	}

	//INITIALIZE

	srand(time(NULL));
	int n = atoi(argv[1]);
	int maxVal = atoi(argv[2]);
	double **A = createMatrix(n,maxVal);

	
	printMatrix(A,"A",n,n);


	return 0;
}