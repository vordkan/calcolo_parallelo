#include <omp.h>
#include <stdio.h>

int main(){
	int idt, numt;
	#pragma omp parallel private(idt), shared(numt)
	{
		idt = omp_get_thread_num();
		numt = omp_get_num_threads();
		printf("Hello from thread %d, of nthreads %d\n", idt, numt);
	}

	return 0;
}

