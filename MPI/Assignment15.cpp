#include "mpi.h"

using namespace std;

int main(int argc, char **argv)
{
	int rank, size, i, rbuf;
	MPI_Init(&argc, &argv);
	MPI_Group group, new_group;
	MPI_Comm new_comm;
	int ranks[128], new_rank;
	// Get the number of processes associated with the communicator
	MPI_Comm_size(MPI_COMM_WORLD, &size);
	// Get the rank of the calling process
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	// Get the group associated with a communicator
	MPI_Comm_group(MPI_COMM_WORLD, &group);

	// Fill ranks values of first half of processes
	for (i = 0; i < size / 2; i++) {
		ranks[i] = i;
	}

	// Include first part of processes to new group and 
	if (rank < size / 2) {
		// Add process to new group
		MPI_Group_incl(group, size / 2, ranks, &new_group); 
	}
	else  {
		// Remove process to new group
		MPI_Group_excl(group, size / 2, ranks, &new_group);
	}
	
	// Create new coomunicator based on new group
	MPI_Comm_create(MPI_COMM_WORLD, new_group, &new_comm);
	// Sum all ranks of processes in new group and set it in rbuf
	MPI_Allreduce(&rank, &rbuf, 1, MPI_INT, MPI_SUM, new_comm);
	// Return process rank in new group 
	MPI_Group_rank(new_group, &new_rank);
	
	//Display values: "rank =, newrank =, rbuf ="
	cout << "Rank = " << rank << ", newrank = " << new_rank << ", rbuf = " << rbuf << endl; 
	
	MPI_Finalize();
}