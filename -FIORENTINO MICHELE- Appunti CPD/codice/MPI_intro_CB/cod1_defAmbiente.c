#include <stdio.h>
#include "mpi.h"

int main(int argc, char *argv[]){
    int menum, nproc;

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &menum);
    MPI_Comm_size(MPI_COMM_WORLD, &nproc);

    printf("Sono %d di %d\n", menum, nproc);

    MPI_Finalize();
    return 0;
}


