#include <stdio.h>
#include <mpi.h>

int main(int argc, char** argv){
	MPI_Init(&argc, &argv);

	int rank;
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);

	int tag = 0;
	int send_data = 7;
	int recv_data = -1;
	MPI_Status status;

	printf("Rank %d before send/recv: send_data = %d, recv_data %d \n", rank, send_data, recv_data);

	if(rank == 0) MPI_Send(&send_data, 1, MPI_INT, 1, tag, MPI_COMM_WORLD);
	if(rank == 1) MPI_Recv(&recv_data, 1, MPI_INT, 0, tag, MPI_COMM_WORLD, &status);

	printf("Rank %d after send/recv: send_data = %d, recv_data %d \n", rank, send_data, recv_data);

	MPI_Finalize();
}
