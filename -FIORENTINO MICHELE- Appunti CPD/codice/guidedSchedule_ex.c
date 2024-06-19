#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <omp.h>

#define N 1000

int main(){
	int i;

	#pragma omp parallel for private(i) schedule(guided) num_threads(4)
		for(i=0; i<N; i++){
			//aspetta un numero "i" di secondi
			sleep(i);
			printf("Il thread %d ha completato l\'operazione %d.\n", omp_get_thread_num(),i);
		}

	printf("Tutti i thread hanno terminato!\n");
	return 0;
}