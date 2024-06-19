#include <stdio.h>
#include <stdlib.h>
#include "mpi.h"

int main(int argc, char *argv[]){
    int menum, nproc;
    int n, nloc, tag, i;
    int *x, *xloc;

    MPI_Status status;  //solo per questo file

    MPI_Init(&argv, &argc);
    MPI_Comm_rank(MPI_COMM_WORLD, &menum);
    MPI_Comm_size(MPI_COMM_WORLD, &nproc);

    if(menum==0){

        //P0 legge n e tutto il vettore x
        printf("Quanti numeri inserire? ");
        scanf("%d",&n);
        x = (int*) malloc(sizeof(int)*n);
        printf("\nInserire i numeri:\n");
        for(i=0; i<n; i++){
            scanf("%d",&x[i]);
        }

        //spedizione di n da P0 agli altri processori UNO PER VOLTA
        for(i=1; i<nproc; i++){
            tag=10+i;
            MPI_Send(&n,1,MPI_INT,i,tag,MPI_COMM_WORLD);
        }

    }
    else{
        tag = 10+menum;
        MPI_Recv(&n,1,MPI_INT,0,tag,MPI_COMM_WORLD, &status);
    }

    //calcolo quanti n. dovrà calcolare ogni p.
    nloc = n/nproc;
    rest = n%nproc;
    if(menum<rest) nloc++;

    if(menum==0){
        xloc = x;
        int tmp = nloc;
        int start = 0;

        for(i=1; i<nproc; i++){
            start = start+tmp;
            tag = 50+i;
            if(i==rest) tmp=tmp-1; //solo 1 volta!
            MPI_Send(&x[start],tmp,MPI_INT,i,tag,MPI_COMM_WORLD);
        }
    }
    else{
        tag = 50+menum;
        MPI_Recv(xloc,nloc,MPI_INT,0,tag,MPI_COMM_WORLD,&status);
    }

}
