#include <iostream>
#include "mpi.h"

using namespace std;

int main(int argc, char* argv[])
{
	MPI_Init(&argc, &argv);
	int rank, n, i, message;
    double time_start, time_finish;
	string report;

	MPI_Status status;
	MPI_Comm_size(MPI_COMM_WORLD, &n);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    int src = rank - 1;
    int dst = rank + 1;

		if (rank == 7)
		{
			cout << "Hello from process " << rank << "\n";
			for (i = 1; i < n; i++) {
				MPI_Recv(&message, 1, MPI_INT, MPI_ANY_SOURCE, MPI_ANY_TAG, MPI_COMM_WORLD, &status);
				cout << "Hello from process " << double(message) / 7 << endl;
				time_finish = MPI_Wtime();
				cout<< "Time = " << (time_finish - time_start) << endl;
			}

		}
		else MPI_Send(&rank, 1, MPI_INT, 7, 0, MPI_COMM_WORLD);


	MPI_Finalize();
	return 0;
}