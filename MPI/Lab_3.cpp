#include <iostream>
#include "mpi.h"
using namespace std;

int main(int argc, char* argv[]) {
	// Initialize the MPI environment
	MPI_Init(&argc, &argv);
	int rank, n, i, message;
	MPI_Status status;
	// Get the number of processes associated with the communicator
	MPI_Comm_size(MPI_COMM_WORLD, &n);
	// Get the rank of the calling process
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	if (rank == 0)
	{
		cout << "Hello from process " << rank << "\n";
		for (i  = 1; i < n; i++) {
			// Recieve (with blocking) int from any other threads
			MPI_Recv(&message, 1, MPI_INT, MPI_ANY_SOURCE, MPI_ANY_TAG, MPI_COMM_WORLD, &status);
			cout << "Hello from process " << message << endl;
		}
	}
	// Send int rank to other threads
	else MPI_Send(&rank, 1, MPI_INT, 0, 0, MPI_COMM_WORLD);
	// Finalize and free all resources
	MPI_Finalize();
	return 0;
}