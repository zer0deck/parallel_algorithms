#include <mpi.h>
using namespace std;

int const N_TEST = 100000000;

int main(int argc, char *argv[])
{
	int rank, size;	
    // Initialize the MPI environment
	MPI_Init(&argc, &argv);
    // Get the number of processes associated with the communicator
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    // Get the rank of the calling process
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    
    int dotProduct;
    int n;
    int *a,*b;

    // In main process init n and arrays
    if(rank == 0){
        if (argc == 1) {
            n = N_TEST;
        } else {
            n = atoi(argv[1]);
        }
    	a = new int[n];
    	b = new int[n];
        fill(a, a + n, 1);
        fill(b, b + n, 1);
    }

    // Sednd size of arrays (n) to other processes
    MPI_Bcast(&n, 1, MPI_INT, 0, MPI_COMM_WORLD);
    
    int *va = new int[n/size]; 
    int *vb = new int[n/size];
    int sum = 0;

    // Send parts of arrays to each processes
	MPI_Scatter(a, n/size, MPI_INT, va, n/size, MPI_INT, 0, MPI_COMM_WORLD);
	MPI_Scatter(b, n/size, MPI_INT, vb, n/size, MPI_INT, 0, MPI_COMM_WORLD);

    // Compute dotProduct of recieved parts
	for(int i=0; i < n/size; i++){
		sum += va[i] * vb[i];
	}

    // Sumup all parts of sums
	MPI_Reduce(&sum, &dotProduct, 1, MPI_INT, MPI_SUM, 0,MPI_COMM_WORLD);

    // Clean memmory and print output
    delete[] va;
    delete[] vb;
    if (rank == 0) {
        delete[] a;
        delete[] b;
        cout << "Dot Product: " << dotProduct << endl;
    }

    MPI_Finalize();
	return 0;
}