#include <iostream>
#include "mpi.h"
using namespace std;
int main(int argc, char **argv)
{
	int rank, size, prev, next;
	int buf[2];
	MPI_Init(&argc, &argv);
	MPI_Request reqs[4];
	MPI_Status stats[4];
	MPI_Comm_size(MPI_COMM_WORLD, &size);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	// Set prev and next process rank
	prev = rank - 1;
	next = rank + 1;
	if (rank == 0) prev = size - 1;
	if (rank == size - 1) next = 0;
	//Non-blocking tasks to receive message from prev and next processes 
	MPI_Irecv(&buf[0], 1, MPI_INT, prev, 5, MPI_COMM_WORLD, &reqs[0]);
	MPI_Irecv(&buf[1], 1, MPI_INT, next, 6, MPI_COMM_WORLD, &reqs[1]);
	//Non-blocking tasks to send message to prev and next processes
	MPI_Isend(&rank, 1, MPI_INT, prev, 6, MPI_COMM_WORLD, &reqs[2]);
	MPI_Isend(&rank, 1, MPI_INT, next, 5, MPI_COMM_WORLD, &reqs[3]);
	// Wait all processes tasks
	MPI_Waitall(4, reqs, stats);
	
	//Your code here.
	//Here you need to display the number of the current process, and what it receives from the previous and next processes.
	cout << "Rank " << rank << ". Message from prev process is " << buf[0] 
	<< ". Message from next process is " << buf[1] << endl;

	MPI_Finalize();
}