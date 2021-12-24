#include <iostream>
#include <mpi.h>

#define VEC_SIZE    1000000

using namespace std;

int main(int argc, char **argv)
{
    // Init MPI
    int proc_rank, proc_num, part_size;
    int root_rank = 0;
    // Init arrays
    int *vec_a, *vec_b;
    int sum = 0;
    int global_sum = 0;

    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &proc_num);
    MPI_Comm_rank(MPI_COMM_WORLD, &proc_rank);

    if (proc_rank == root_rank)
    {
        vec_a = new int[VEC_SIZE];
        vec_b = new int[VEC_SIZE];

        // Split arrays between processes as equally as possbile
        part_size = VEC_SIZE / proc_num;

        for (int i = 0; i < VEC_SIZE; i++)
        {
            vec_b[i] = vec_a[i] = (i / part_size) + 1; // Set 1 to first part, 2 to second, etc
        }

        cout << "Two vectors of " << VEC_SIZE << " elements were generated and divided to " <<
        proc_num << " parts of " << part_size << " elements" << endl;
    }

    // Send and receive size of transfering part of vector
    MPI_Bcast(&part_size, 1, MPI_INT, root_rank, MPI_COMM_WORLD);
    // Allocate space for parts of vectors
    int *vec_a_part = new int[part_size];
    int *vec_b_part = new int[part_size];

    // Send by root proc parts of first vector to rest procs
    MPI_Scatter(vec_a, part_size, MPI_INT, vec_a_part, part_size, MPI_INT, root_rank, MPI_COMM_WORLD);
    // Send by root proc parts of second vector to rest procs
    MPI_Scatter(vec_b, part_size, MPI_INT, vec_b_part, part_size, MPI_INT, root_rank, MPI_COMM_WORLD);

    // Main routine
    for (int i = 0; i < part_size; i++)
    {
        sum += vec_a_part[i] * vec_b_part[i];
    }

    // Gather sum in root process and output result
    MPI_Reduce(&sum, &global_sum, 1, MPI_INT, MPI_SUM, root_rank, MPI_COMM_WORLD);
    if (proc_rank == root_rank)
    {
        cout << "Root process: " << proc_rank << ": local sum = " << sum  << endl;
        cout << "Root process: " << proc_rank << " resulting SUM = " << global_sum << endl;
        // Release allocated memory
        delete vec_a;
        delete vec_b;
    }
    else
    {
        cout << "Process: " << proc_rank << ": local sum = " << sum  << endl;
    }

    // Delete allocated memory
    delete[] vec_a_part;
    delete[] vec_b_part;

    // Finalize MPI
    MPI_Finalize();
    return 0;
}