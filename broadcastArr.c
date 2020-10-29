#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>

int main(int argc, char** argv){

	MPI_Init(&argc, &argv);
	int rank, n_procs;
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	MPI_Comm_size(MPI_COMM_WORLD, &n_procs);

	// MPI communication setup
	int tag = 0;
	MPI_Status status;

	// create data array
	int N = 2;
	int *data = (int *)malloc(N*sizeof(int)); // original data on each process
	if(rank == 0){ //rank 0 has different data from other ranks
		data[0] = 7;
		data[1] = 8;
	}
	else{
		data[0] = -1;
		data[1] = 0;
	}
	printf("Rank %d: before broadcast data is %d %d \n", rank, data[0],data[1]);

	// sends and receives so that all ranks hav edata from rank 0
	if(rank == 0){
		// do the sends
		int i;
		for(i=1; i<n_procs; i++){ // for each other process, send data
			MPI_Send(data, 2, MPI_INT, i, tag, MPI_COMM_WORLD);
		}
	}
	else{
		// do the receives
		MPI_Recv(data, 2, MPI_INT, 0, tag, MPI_COMM_WORLD, &status);
	}

	printf("Rank %d: after broadcast data is %d %d \n", rank, data[0], data[1]);

	// cleanup
	free(data);
	data = NULL;

	MPI_Finalize();
	return 0;
}
