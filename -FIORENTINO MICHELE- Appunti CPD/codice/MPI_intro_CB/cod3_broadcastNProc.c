#include <stdio.h>
#include "mpi.h"

int main(int argc, char *argv[]){
    int menum, nproc;
    int n, tag, num;

    MPI_Init(&argc, &argv);
    MPI_COMM_rank(MPI_COMM_WORLD, &menum);

    if(menum==0){
        scanf("%d", &n);
    }

    MPI_Bcast(&n,1,MPI_INT,0,MPI_COMM_WORLD);

    MPI_Finalize();
    return 0;
}
