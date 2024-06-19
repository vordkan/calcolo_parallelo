#include <stdio.h>
#include "mpi.h"

int main(int argc, char *argv[]){
	int menum, nproc;

	MPI_Init(&argc, &argv);
	MPI_Comm_rank(MPI_COMM_WORLD, &menum);
	MPI_Comm_size(MPI_COMM_WORLD, &nproc);


	double t, t1, t2;

	MPI_Barrier(MPI_COMM_WORLD); 	//tempo di inizio
	t1=MPI_Wtime();

	//algoritmo...

	MPI_Barrier(MPI_COMM_WORLD); 	//tempo di fine
	t2=MPI_Wtime();

	t=t2-t1;						//tempo totale

	MPI_Reduce(&t, &max,1, MPI_DOUBLE, MPI_MAX, O, MPI_COMM_WORLD); //oppure MPI_ALLreduce


	MPI_Finalize();
	return 0;
}