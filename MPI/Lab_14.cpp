#include <stdio.h>
#include "mpi.h"
#define n 1000

using namespace std;

// User-defined operation - params: invec, outvec, len, type
void my_max(void *a, void *b, int *l, MPI_Datatype *type) {
	b = (b > a)? b : a;
}

int main(int argc, char **argv)
{
	int rank, size, i;
	int res_max = -1;
	MPI_Init(&argc, &argv);
	MPI_Op op;
	MPI_Comm_size(MPI_COMM_WORLD, &size);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	// Create custom operation
	MPI_Op_create(&my_max, 1, &op);
	// Perform reduce using custom func
	MPI_Reduce(&rank, &res_max, 1, MPI_INT, op , 0, MPI_COMM_WORLD);
	// Free custom operation
	MPI_Op_free(&op);
	// Print results
	if (rank == 0) {
		cout << "Max value by custom func is " << res_max << endl;
	}
	// Perform reduce using custom func
	MPI_Reduce(&rank, &res_max, 1, MPI_INT, MPI_MAX, 0, MPI_COMM_WORLD);
	// Print results
	if (rank == 0) {
		cout << "Max value by lib func is " << res_max << endl;
	}
	MPI_Finalize();
}