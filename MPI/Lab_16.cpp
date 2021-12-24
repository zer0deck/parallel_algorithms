#include <mpi.h>
#include <iostream>

using namespace std;

int main(int argc, char **argv)
{
	int rank, size, rank1;
	MPI_Init(&argc, &argv);
	MPI_Comm comm_revs;
	MPI_Comm_size(MPI_COMM_WORLD, &size);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	int root_rank = 0;

	if (root_rank == rank) {
		cout << "Color to rank%2" << endl;
	}

	MPI_Comm_split(MPI_COMM_WORLD, rank % 2, size - rank, &comm_revs);
	MPI_Comm_rank(comm_revs, &rank1);
    //Display rank and rank1
	cout << "rank: " << rank << " rank1: " << rank1 << endl;
	MPI_Barrier(comm_revs);
	MPI_Comm_free(&comm_revs);
	MPI_Barrier(MPI_COMM_WORLD);

	if (root_rank == rank) {
		cout << "Color to rank%3" << endl;
	}

	MPI_Comm_split(MPI_COMM_WORLD, rank % 3, size - rank, &comm_revs);
	MPI_Comm_rank(comm_revs, &rank1);
	//Display rank and rank1
	cout << "rank: " << rank << " rank1: " << rank1 << endl;
	MPI_Comm_free(&comm_revs);

	MPI_Finalize();
}