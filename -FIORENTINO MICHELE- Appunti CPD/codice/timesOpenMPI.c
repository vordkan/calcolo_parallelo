#include <stdlib.h>
#include <omp.h>

int main(){
	double t, t1, t2;

	t1 = omp_get_wtime();
	#pragma omp parallel
	{
		//istruzioni...
	}
	t2 = omp_get_wtime();

	t=t2-t1;

	return 0;
}