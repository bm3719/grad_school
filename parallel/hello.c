#include <stdio.h>
#include "mpi.h"

int main (int argc, char **argv) {
    int myrank;
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &myrank);
    printf("Hello world from node %d!\n", myrank);
    MPI_Finalize();
    return(0);
}
