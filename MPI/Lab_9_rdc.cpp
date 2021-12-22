#include <iostream>
#include <mpi.h>

#define VEC_SIZE    1000000

using namespace std;

int main(int argc, char **argv)
{
    // Init MPI
    int proc_rank, proc_num, part_size;
    MPI_Status status;
    double time_start, time_stop;
    int root_rank = 0;
    int *base_arr;
    int *arr_part;
    int loc_sum = 0, global_sum = 0;

    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &proc_num);
    MPI_Comm_rank(MPI_COMM_WORLD, &proc_rank);

    if (proc_rank == root_rank)
    {
        // Init array
        base_arr = new int[VEC_SIZE];

        for (int i = 0; i < VEC_SIZE; i++)
        {
            base_arr[i] = 1;
        }
        // Split arrays between processes as equally as possbile
        part_size = VEC_SIZE / proc_num;
    }

    // Send and receive size of transfering array
    MPI_Bcast(&part_size, 1, MPI_INT, root_rank, MPI_COMM_WORLD);
    // Allocate space for part of array
    arr_part = new int[part_size]();
    // Send and receive part of array
    MPI_Scatter(base_arr, part_size, MPI_INT, arr_part, part_size, MPI_INT, root_rank, MPI_COMM_WORLD);

    // Find sum of our part
    for (int i = 0; i < part_size; i++)
    {
        loc_sum += arr_part[i];
    }

    // Merging answers
    if (proc_rank == root_rank)
    {
        time_start = MPI_Wtime();
    }
    MPI_Reduce(&loc_sum, &global_sum, 1, MPI_INT, MPI_SUM, root_rank, MPI_COMM_WORLD);

    // Output answer
    if (proc_rank == root_rank)
    {
        time_stop = MPI_Wtime();
        printf("Root process #%d: final answer is : %d\n", proc_rank, global_sum);
        printf("Point to point asnwer merging took %.0f us\n", (time_stop - time_start) * 1000000);
    }

    // Finalize MPI
    MPI_Finalize();
    return 0;
}