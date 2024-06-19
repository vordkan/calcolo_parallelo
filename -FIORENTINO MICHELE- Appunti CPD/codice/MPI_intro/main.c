#include <stdio.h>
#include "mpi.h"

int main(int argc, char *argv[]){
    int menum;
    int n, tag, num;

    MPI_Status info;

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &menum);

    if(menum==0){
        scanf("%d", &n);
        tag=10; //arbitrary value
        MPI_Send(&n,1,MPI_INT,1,tag,MPI_COMM_WORLD);
    }
    else{
        tag=10;
        MPI_Recv(&n,1,MPI_INT,0,tag,MPI_COMM_WORLD);
    }

    MPI_Get_count(&info, MPI_INT_&num);
    MPI_Finalize();
    return 0;
}
