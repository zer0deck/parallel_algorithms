#include <stdio.h>
#include <string.h>
#include "mpi.h"

int main(int argc, char **argv)
{
	MPI_Init(&argc, &argv);
    
	char port_name[MPI_MAX_PORT_NAME];
	MPI_Comm intercomm;

	// Get port_name from argv
	strcpy(port_name, argv[1]);

	// Connect to server using port_name
	MPI_Comm_connect(port_name, MPI_INFO_NULL, 0, MPI_COMM_SELF, &intercomm);

	// Send message to server
	char message[11] = "Hi server!";
	MPI_Send(message, 11, MPI_CHAR, 0, 0, intercomm);

	MPI_Finalize();
	return 0;
}