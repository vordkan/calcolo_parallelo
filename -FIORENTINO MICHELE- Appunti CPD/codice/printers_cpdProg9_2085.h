#include <stdlib.h>
#include <stdio.h>
#include <omp.h>

typedef long long int lli;

void printMatrix(char* name, lli** A, int n, int m){

	if(n == 0)
		#pragma omp parallel master shared(n)
		n = omp_get_num_threads();

	printf("%s =\n",name);
	for(int i=0; i<n; i++){
		for(int j=0; j<m; j++){
			printf("%lld\t", A[i][j]);
		}
		printf("\n");
	}
	printf("\n");
}

void printSubdividedMatrix(char* name, lli** A, int n, int m, int p){

	if(p == 0)
		#pragma omp parallel master shared(p)
		p = omp_get_num_threads();

	int* nloc = calloc(p,sizeof(int));
	int r = n%p;
	for(int i=0; i<p; i++){
		nloc[i] = n/p;
		if(i < r)
			nloc[i]++;
	}

	short flag = 0;
	printf("Subdivided %s =\n",name);
	int idx = 0;
	for(int i=0; i<n; i++){
		if(i%nloc[idx] == 0 && flag==1){
			for(int k=0; k<m; k++)
				printf("--------");
			printf("\n");
			idx++;
		}
		flag=1;
		for(int j=0; j<m; j++){
			printf("%lld\t", A[i][j]);
		}
		printf("\n");
	}
	printf("\n");
}

void printTransposedMatrix(char* name, lli** A, int n, int m){

	if(n == 0)
		#pragma omp parallel master shared(n)
		n = omp_get_num_threads();

	printf("%s (Transposed) =\n",name);
	for(int i=0; i<m; i++){
		for(int j=0; j<n; j++){
			printf("%lld\t", A[j][i]);
		}
		printf("\n");
	}
	printf("\n");
}