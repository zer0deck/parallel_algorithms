#include <stdio.h>
#include "mpi.h"

int main(int argc, char **argv)
{
	int rank;
	MPI_Init(&argc, &argv);
	MPI_File fh;
	char buf[30];

	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	MPI_File_open(MPI_COMM_WORLD, "randtext.txt", MPI_MODE_RDONLY, MPI_INFO_NULL, &fh);
	MPI_File_set_view(fh, rank * 30, MPI_CHAR, MPI_CHAR, "native", MPI_INFO_NULL);
	MPI_File_read_all(fh, buf, 30, MPI_CHAR, MPI_STATUS_IGNORE);

	printf("RP: %d, buffer has = %s\n", rank, buf);

	MPI_File_close(&fh);

	MPI_Finalize();
}