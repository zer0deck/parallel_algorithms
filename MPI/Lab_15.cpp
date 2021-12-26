#include <iostream>
#include "mpi.h"

using namespace std;

int main(int argc, char **argv)
{
    int rank, size, i, rbuf;
    MPI_Init(&argc, &argv);
    MPI_Group group, new_group;
    MPI_Comm new_comm;
    int ranks[128], new_rank;
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_group(MPI_COMM_WORLD, &group);

    for (i = 0; i < size / 2; i++) ranks[i] = i;

    if (rank < size / 2)
    {
        MPI_Group_incl(group, size / 2, ranks, &new_group);
    }
    else
    {
        MPI_Group_excl(group, size / 2, ranks, &new_group);
    }

    MPI_Comm_create(MPI_COMM_WORLD, new_group, &new_comm);
    MPI_Allreduce(&rank, &rbuf, 1, MPI_INT, MPI_SUM, new_comm);
    MPI_Group_rank(new_group, &new_rank);

    //Display values: "rank =, newrank =, rbuf ="
    cout << "PR: = " << rank << " new PR: = " << new_rank << "  BUF = " << rbuf << endl;

    MPI_Finalize();
}