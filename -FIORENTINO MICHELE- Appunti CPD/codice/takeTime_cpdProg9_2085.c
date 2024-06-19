#include <stdlib.h>
#include <stdio.h>
#include <omp.h>
#include <time.h>
#include <unistd.h>
#include "printers_cpdProg9_2085.h"

typedef long long int lli;

void swap(double* xp, double* yp)
{
    double temp = *xp;
    *xp = *yp;
    *yp = temp;
}
 
// Function to perform Selection Sort
void selectionSort(double arr[], int n)
{
    int i, j, min_idx;
 
    // One by one move boundary of unsorted subarray
    for (i = 0; i < n - 1; i++) {
 
        // Find the minimum element in unsorted array
        min_idx = i;
        for (j = i + 1; j < n; j++)
            if (arr[j] < arr[min_idx])
                min_idx = j;
 
        // Swap the found minimum element
        // with the first element
        swap(&arr[min_idx], &arr[i]);
    }
}

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

lli* partialPunctualProduct(lli **A, int n, int m){

	lli *res = malloc(m*sizeof(lli));

	int i,j;
	for(i=0; i<m; i++){
		res[i] = A[0][i];
		for(j=1; j<n; j++)
			res[i] *= A[j][i];
	}

	return res;
}

double parallelPunctualProduct(lli **A, int n, int p){
	
	int i,j,idt,nloc,r,step;
	double t,t1,t2;

	if(p == 0)
		#pragma omp parallel master shared(p)
		p = omp_get_num_threads();

	lli **resVectors = malloc(p*sizeof(lli*));

	t1 = omp_get_wtime();
	#pragma omp parallel shared(A,n,p) private(i,j,idt,nloc,r,step) num_threads(p)
	{
		idt = omp_get_thread_num();
		nloc = n/p;
		r = n%p;

		if(idt < r){
			nloc++;
			step = 0;
		} else {
			step = r;
		}
		
		resVectors[idt] = partialPunctualProduct(&A[idt*nloc+step],nloc,n);
	}
	t2 = omp_get_wtime();

	t = t2-t1;
	//printf("Total Time with %d processors and n=%d: %f\n\n",p,n,t);

	//just for taking times, we don't care about resVectors
	for(i=0; i<p; i++)
		free(resVectors[i]);
	free(resVectors);

	return t;
}

int main(int argc, char** argv){

	if(argc<5){
		printf("Not enough arguments. Insert: dim, maxVal, numP (default=0), iterations\n");
		return -1;
	}

	srand(time(NULL));

	int n = atoi(argv[1]);
	int maxVal = atoi(argv[2]);
	int p = atoi(argv[3]);
	int its = atoi(argv[4]);

	if(n<p){
		printf("the number of cores cannot be less than N\n");
		return -2;
	}

	//CREATION OF A NxN MATRIX OF RANDOM NUMBERS
	lli **A = createMatrix(n,n,maxVal);

	//PUNCTUAL PRODUCT TIMES
	double *times = malloc(its*sizeof(double));
	printf("0%%\n");
	for(int i=0; i<its; i++){
		times[i] = parallelPunctualProduct(A,n,p);
		if(i%5 == 0)
			printf("%d%%\n",i+5);
	}

	/*for(int i=0; i<its; i++)		//before
		printf("%lf\n", times[i]);*/
	
	selectionSort(times,its);
	
	/*printf("\n");
	for(int i=0; i<its; i++)		//after
		printf("%lf\n", times[i]);*/

	double median = times[its/2];

	printf("\nthe median of the algorithm for n = %d executed %d times is: %lf\n\n", n,its,median);

	return 0;
}