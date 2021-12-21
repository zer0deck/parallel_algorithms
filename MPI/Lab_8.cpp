#include <iostream>
#include <mpi.h>
using namespace std;

int const N = 100;

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
	double elapsed_time;
    
    //Use only two process
    if (rank == 0 || rank == 1) {
        // Set number of elements with step *10
        for (int i = 1; i <= 10000000; i = i * 10) {
            int *a = new int[i];
            // Init array of ints 
            for (int k = 0; k < i; k++) {
                a[k] = rand();
            }
            // Init elapsed_time in main process
            if (rank == 0) {
                elapsed_time = 0.0;
            }
            // Perform bandwidth 100 times and measure the time
            for (int j = 0; j < N; j++) {
                if (rank == 0) {
                    // First send and second recieve messages
                    double start_time = MPI_Wtime();
                    MPI_Send(a, i, MPI_INT, 1, 0, MPI_COMM_WORLD);
                    MPI_Recv(a, i, MPI_INT, 1, 0, MPI_COMM_WORLD, &status);
                    double end_time = MPI_Wtime();
                    elapsed_time += end_time - start_time;
                }
                if (rank == 1) {
                    // First recieve and second send messages
                    MPI_Recv(a, i, MPI_INT, 0, 0, MPI_COMM_WORLD, &status);
                    MPI_Send(a, i, MPI_INT, 0, 0, MPI_COMM_WORLD);
                }
            }

            delete[] a;
            if (rank == 0) {
                cout << "Overall messages length is " << ((2*N*i*sizeof(int))/(1024.0*1024.0)) << " Mb"
                    " and avg elapsed time is " << elapsed_time/(2*N) << endl 
                    << "Bandwidth R is " << ((2*N*i*sizeof(int))/(1024.0*1024.0*1024.0))/elapsed_time << " Gb/s" << endl;
            }
        }

        // Measure latency
        int *a = new int[0];
        if (rank == 0) {
            elapsed_time = 0.0;
        }
        for (int j = 0; j < N; j++) {
            if (rank == 0) {
                // First send and second recieve messages
                double start_time = MPI_Wtime();
                MPI_Send(a, 0, MPI_INT, 1, 0, MPI_COMM_WORLD);
                MPI_Recv(a, 0, MPI_INT, 1, 0, MPI_COMM_WORLD, &status);
                double end_time = MPI_Wtime();
                elapsed_time += end_time - start_time;
            }
            if (rank == 1) {
                // First recieve and second send messages
                MPI_Recv(a, 0, MPI_INT, 0, 0, MPI_COMM_WORLD, &status);
                MPI_Send(a, 0, MPI_INT, 0, 0, MPI_COMM_WORLD);
            }
        }
         delete[] a;
            if (rank == 0) {
                cout << "Latency is " << elapsed_time/(2*N) << endl;
            }
    }
	MPI_Finalize();
}