#include <stdio.h>
#include <stdlib.h>
#include <omp.h>

int main(){
	int n, nloc, idt, numt, r, step;
	float sumtot, sum, *a;
	sumtot=0;

	printf("Inserire n: ");
	scanf("%d", &n);
	a=(float*)calloc(n,sizeof(float));
	printf("Inserire i numeri da sommare:\n");
	for(int i=0; i<n; i++) scanf("%f", &a[i]);

	#pragma omp parallel private(sum,nloc,idt,step), shared(sumtot, numt, r)
	{
		idt = omp_get_thread_num();
		numt = omp_get_num_threads();
		nloc = n/numt;
		r = n%numt;

		if(idt < r){
			nloc++;
			step = 0;
		} else {
			step = r;
		}

		sum=0;
		for(int i=0; i<nloc; i++) //somma parziale
			sum += a[i+nloc*idt+step];

		printf("sp del thread %d di %d thread: %.2f,\t nloc: %d, step: %d\n", idt, numt, sum, nloc, step);

		sumtot+=sum;	//aggiunta somma parziale a somma finale
	}

	printf("somma totale: %f\n", sumtot);
	return 0;
}