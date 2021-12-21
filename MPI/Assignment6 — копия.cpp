#include <iostream>
#include <mpi.h>
using namespace std;
int main(int argc, char **argv)
{
	int rank, size, ibuf;
	MPI_Status status;
	float rbuf;
	// Initialize the MPI environment
	MPI_Init(&argc, &argv);
	// Get the number of processes associated with the communicator
	MPI_Comm_size(MPI_COMM_WORLD, &size);
	// Get the rank of the calling process
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	ibuf = rank;
	rbuf = 1.0 * rank;
	// Process 1 send int with tag 5 to other processes
	if (rank == 1) MPI_Send(&ibuf, 1, MPI_INT, 0, 5, MPI_COMM_WORLD);
	// Process 2 send int with tag 5 to other processes
	if (rank == 2) MPI_Send(&rbuf, 1, MPI_FLOAT, 0, 5, MPI_COMM_WORLD);
	if (rank == 0) {
		// Checks if there is new incoming message with tag 5 (blocking wait)
		MPI_Probe(MPI_ANY_SOURCE, 5, MPI_COMM_WORLD, &status);
		// If there is first new message with source process num 1
		if (status.MPI_SOURCE == 1) {
			// First recive message from process 1
			MPI_Recv(&ibuf, 1, MPI_INT, 1, 5, MPI_COMM_WORLD, &status);
			// Second recive message from process 1
			MPI_Recv(&rbuf, 1, MPI_FLOAT, 2, 5, MPI_COMM_WORLD, &status);
			cout << "Process 0 recv " << ibuf << " from process 1, " << rbuf << " from process 2\n";
		}
		// If there is first new message with source process num 2
		else if (status.MPI_SOURCE == 2) {
			// First recive message from process 2
			MPI_Recv(&rbuf, 1, MPI_FLOAT, 2, 5, MPI_COMM_WORLD, &status);
			// First recive message from process 1
			MPI_Recv(&ibuf, 1, MPI_INT, 1, 5, MPI_COMM_WORLD, &status);
			cout << "Process 0 recv " << rbuf << " from process 2, " << ibuf << " from process 1\n";
		}
	}
	MPI_Finalize();
}