#include <stdio.h>
#include <stdlib.h>
#include <omp.h>

int main(){
	int n, nloc, idt, numt;
	float sumtot, sum, *a;
	sumtot=0;

	printf("Inserire n: ");
	scanf("%d", &n);
	a=(float*)calloc(n,sizeof(float));
	printf("Inserire i numeri da sommare:\n");
	for(int i=0; i<n; i++) scanf("%f", &a[i]);

	#pragma omp parallel private(sum,nloc,idt), shared(sumtot, numt)
	{
		idt = omp_get_thread_num();
		numt = omp_get_num_threads();
		nloc = n/numt;

		sum=0;
		for(int i=0; i<nloc; i++) //somma parziale
			sum += a[i+nloc*idt];

		printf("somma parziale del thread %d di %d thread: %.2f\n", idt, numt, sum);

		sumtot+=sum;	//aggiunta somma parziale a somma finale
	}

	printf("somma totale: %f\n", sumtot);
	return 0;
}