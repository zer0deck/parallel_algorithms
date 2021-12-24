#include "mpi.h"

int main(int argc, char **argv)
{
    int rank;
    int size;
    MPI_Comm intercomm;
    MPI_Init(&argc, &argv);
    MPI_Comm_get_parent(&intercomm);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    if (rank < 2)
    {
        // First two procs just send their ranks
        MPI_Send(&rank, 1, MPI_INT, 0, rank, intercomm);
    }
    else
    {
        MPI_Comm_size(MPI_COMM_WORLD, &size);
        MPI_Send(&size, 1, MPI_INT, 0, rank, intercomm);
    }
    MPI_Finalize();

    return 0;
}