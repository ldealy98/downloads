#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <mpi.h>
#include "serialVector.h"
#include "parallelVector.h"

int main(int argc, char** argv){
	MPI_Init(&argc, &argv);
	int rank, n_procs;
	MPI_Comm_rank(MPI_COMM_WORLD,&rank);
	MPI_Comm_size(MPI_COMM_WORLD,&n_procs);
	
	int nTotal = 5000;
	int flagSum = 0;
	int serialRank = n_procs-1; // this is the rank that'll have the serial vector initially created on it
	
	if(rank == serialRank){

		// serial vector filled with random values in [0,1] and calculate its L1-norm
		serialVector vecS; 
		initializeSerialVector(&vecS, nTotal); // allocate space for data
		int i;
		for(i=0; i<nTotal; ++i) vecS.dataTotal[i] = (float)rand() / (float)RAND_MAX; // fill with random values
		float sNorm = serialNorm(&vecS);

		// distribute the serial vector  and calculate same L1-norm in parallel
		parallelVector vecP;
		flagSum += initializeParallelVector(&vecP, nTotal); // allocate space for subset of data
		flagSum += scatterVectorSender(&vecS, &vecP); // fill in all vecP with data subsets
		float pNorm  = parallelNorm(&vecP); // calculate norm in parallel

		// check whether the parallel norm is correct
		if(fabs(sNorm - pNorm) < fabs(sNorm)*1e-6){
			printf("Serial norm and parallel norm match: %e \n",sNorm); // what you want to  see
		}
		else{
			printf("WARNING: serial norm was %e but parallel norm was %e \n", sNorm, pNorm); // what you'll see if you've made an error
		}

		// cleanup phase
		cleanupSerial(&vecS);
		cleanupParallel(&vecP);
	}
	else{ // all other ranks execute this to do their part in parallel norm calculation

		// setup this portion of distributed vector, geetting data from rank 0
		parallelVector vecP;
		flagSum += initializeParallelVector(&vecP, nTotal); // allocate space for local subset of data
		flagSum += scatterVectorReceiver(serialRank, &vecP); // get data from serial vector
			
		// help calculate the norm in parallel
		float pNorm = parallelNorm (&vecP);
		
		printf("Rank %d got parallel norm %e \n", rank, pNorm); // should match parallel L1-norm recorded on all other ranks

		// cleanup phase
		cleanupParallel(&vecP); 
	}	

	MPI_Finalize();
}
