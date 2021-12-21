#include <iostream>
#include "mpi.h"
using namespace std;
int main(int argc, char **argv)
{
	int rank, size, prev, next;
	float rbuf[2];
	float sbuf[2];
	MPI_Init(&argc, &argv);
	MPI_Request reqs[4];
	MPI_Status stats[4];
	MPI_Comm_size(MPI_COMM_WORLD, &size);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	prev = rank - 1;
	next = rank + 1;
	if (rank == 0) prev = size - 1;
	if (rank == size - 1) next = 0;
	// Create send and receive tasks
	MPI_Recv_init(&rbuf[0], 1, MPI_FLOAT, prev, 5, MPI_COMM_WORLD, &reqs[0]);
	MPI_Recv_init(&rbuf[1], 1, MPI_FLOAT, next, 6, MPI_COMM_WORLD, &reqs[1]);
	MPI_Send_init(&sbuf[0], 1, MPI_FLOAT, prev, 6, MPI_COMM_WORLD, &reqs[2]);
	MPI_Send_init(&sbuf[1], 1, MPI_FLOAT, next, 5, MPI_COMM_WORLD, &reqs[3]);
	// In each step of for circle perform send message tasks in current process and
	// receive messages in prev process and next process
	for (int i = 0; i < size; i++) {
		if (i == rank) {
			sbuf[0] = rank;
			sbuf[1] = rank;
			MPI_Start(&reqs[2]);
			MPI_Start(&reqs[3]);

			MPI_Wait(&reqs[2], &stats[2]);
			MPI_Wait(&reqs[3], &stats[3]);
		}
		if (i == prev) {
			MPI_Start(&reqs[0]);

			MPI_Wait(&reqs[0], &stats[0]);
		}
		if (i == next) {
			MPI_Start(&reqs[1]);

			MPI_Wait(&reqs[1], &stats[1]);
		} 
	}
	// Wait completion of all tasks
	MPI_Waitall(4, reqs, stats);
	
	// Prit received messages
	cout << "Rank " << rank << ". Message from prev " << rbuf[0] << " and message from next " << rbuf[1] << endl;
	
	// Free requests
	MPI_Request_free(&reqs[0]);
	MPI_Request_free(&reqs[1]);
	MPI_Request_free(&reqs[2]);
	MPI_Request_free(&reqs[3]);

	MPI_Finalize();
}