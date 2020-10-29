#include <stdio.h>
#include <mpi.h>

int main(int argc, char** argv){

	MPI_Init(&argc, &argv);
	int rank, n_procs;
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	MPI_Comm_size(MPI_COMM_WORLD, &n_procs);

	// MPI communication setup
	int tag = 0;
	MPI_Status status;

	int data = -1; // original data on each process
	if(rank == 0) data = 7; // rank 0 has different initial data
	printf("Rank %d: before broadcast data is %d \n", rank, data);

	// sends and receives so that all ranks hav edata from rank 0
	if(rank == 0){
		// do the sends
		int i;
		for(i=1; i<n_procs; i++){ // for each other process, send data
			MPI_Send(&data, 1, MPI_INT, i, tag, MPI_COMM_WORLD);
		}
	}
	else{
		// do the receives
		MPI_Recv(&data, 1, MPI_INT, 0, tag, MPI_COMM_WORLD, &status);
	}

	printf("Rank %d: after broadcast data is %d \n", rank, data);

	MPI_Finalize();
	return 0;
}
