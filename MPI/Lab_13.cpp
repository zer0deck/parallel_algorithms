#include <cstdlib>
#include "mpi.h"

int const N = 500;

using namespace std;

int main(int argc, char **argv)
{
	int rank, size;
	MPI_Init(&argc, &argv);
	MPI_Comm_size(MPI_COMM_WORLD, &size);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);

	// Matrix initialization by each process
	int **a = new int*[N];
	int **b = new int*[N];
	int **c = new int*[N];
	for (int i = 0; i < N; i++) {
		a[i] = new int[N];
		b[i] = new int[N];
		c[i] = new int[N];
		for (int j = 0; j < N; j++) {
			a[i][j] = rand() % 10;
			b[i][j] = rand() % 10;
			c[i][j] = 0;
		}
	}

	MPI_Barrier(MPI_COMM_WORLD); //barrier process synchronization
	// start timing for each process
	double start_time = MPI_Wtime();

	// matrix multiplication
	for (int i = 0; i < N; i++) {
		for (int j = 0; j < N; j++) {
			c[i][j] = 0;
			for (int k = 0; k < N; k++) {
				c[i][j] += a[i][k] * b[k][j];
			}
		}
    }

	// end of timing
	double end_time = MPI_Wtime() - start_time;
	// output the execution time of matrix multiplication at each process
	cout << "Rank " << rank << ". Elapsed time is " << end_time << endl;

	MPI_Finalize();
}