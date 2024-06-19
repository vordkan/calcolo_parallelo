#include <stdlib.h>
#include <stdio.h>
#include <omp.h>
#include <time.h>
#include "printers_cpdProg9_2085.h"

typedef long long int lli;

lli** createMatrix(int n, int m, int maxVal){
	int i,j;

	lli **A = malloc(n*sizeof(lli*));
	for(i=0; i<n; i++)
		A[i] = malloc(m*sizeof(lli));

	for(i=0; i<n; i++)
		for(j=0; j<m; j++)
			A[i][j] = rand()%maxVal+1; //avoids zeros

	return A;
}

//algorithm to calculate the punctual product of a Matrix
lli* partialPunctualProduct(lli **A, int n, int m){

	//allocation for this core result Vector
	lli *res = malloc(m*sizeof(lli));

	int i,j;
	for(i=0; i<m; i++){
		res[i] = A[0][i];
		for(j=1; j<n; j++)
			res[i] *= A[j][i];
	}

	return res;
}

lli** parallelPunctualProduct(lli **A, int n, int p){
	
	int i,j,idt,nloc,r,step;
	double t,t1,t2;

	//allocation for the result Vectors
	lli **resVectors = malloc(p*sizeof(lli*));

	//t1 = omp_get_wtime();
	#pragma omp parallel shared(A,n,p) private(i,j,idt,nloc,r,step) num_threads(p)
	{
		//calculate the number of rows for each core
		idt = omp_get_thread_num();
		nloc = n/p;
		r = n%p;

		if(idt < r){
			nloc++;
			step = 0;
		} else {
			step = r;
		}
		
		//get the result Vector from the function
		resVectors[idt] = partialPunctualProduct(&A[idt*nloc+step],nloc,n);
	}
	//t2 = omp_get_wtime();
	//t = t2-t1;
	//printf("Total Time with %d processors and n=%d: %f\n\n",p,n,t);

	return resVectors;
}

int main(int argc, char** argv){

	if(argc<5){
		printf("Not enough arguments. Insert: dim, maxVal, numP (default=0), view (0,1)\n");
		return -1;
	}

	srand(time(NULL));

	int n = atoi(argv[1]);
	int maxVal = atoi(argv[2]);
	int p = atoi(argv[3]);
	int view = atoi(argv[4]);

	if(p == 0)
		#pragma omp parallel master shared(p)
		p = omp_get_num_threads();

	if(n<p){
		printf("the number of cores cannot be less than N\n");
		return -2;
	}

	//CREATION OF A NxN MATRIX OF RANDOM NUMBERS
	lli **A = createMatrix(n,n,maxVal);

	//PUNCTUAL PRODUCT
	lli **resVectors = parallelPunctualProduct(A,n,p);


	//VISUALIZATION
	if(view){
		printMatrix("Generated Matrix",A,n,n);
		printSubdividedMatrix("Generated Matrix",A,n,n,p);
		printMatrix("Result Vectors",resVectors,p,n);
		printTransposedMatrix("Result Vectors",resVectors,p,n);
	}

	return 0;
}