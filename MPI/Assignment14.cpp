#include <stdio.h>
#include "mpi.h"
#define n 1000

// User-defined operation - params: invec, outvec, len, type
void smod5(void *a, void *b, int *l, MPI_Datatype *type) {
	int i;
	for (i = 0; i < *l; i++)
		((int*)b)[i] = (((int*)a)[i] + ((int*)b)[i]) % 5;
}

int main(int argc, char **argv)
{
	int rank, size, i;
	int a[n];
	int b[n];
	MPI_Init(&argc, &argv);
	MPI_Op op;
	MPI_Comm_size(MPI_COMM_WORLD, &size);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	for (i = 0; i < n; i++) {
		a[i] = i + rank + 1;
	}
	printf("process %d a[0] = %d\n", rank, a[0]);
	// Create custom operation
	MPI_Op_create(&smod5, 1, &op);
	// Perform reduce
	MPI_Reduce(a, b, n, MPI_INT, op , 0, MPI_COMM_WORLD);
	// Free custom operation
	MPI_Op_free(&op);
	// Print results
	if (rank == 0) printf("b[0] = %d\n", b[0]);
	MPI_Finalize();
}