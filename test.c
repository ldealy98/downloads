#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <mpi.h>
#include "serialVector.h"
#include "parallelVector.h"

int main(int argc, char** argv){
	MPI_Init(&argc, &argv);
	int rank;
	MPI_Comm_rank(MPI_COMM_WORLD,&rank);
	
	int nTotal = 5001;
	int flagSum = 0;
	int serialRank = 0; // this is the rank that'll have the serial vector initially created on it

	// all processes have a serial and parallel vector (but only serialRank has space allocated for the serial vector)
	serialVector vecS;
	parallelVector vecP;

	// serial test (baseline)	
	float sNorm;
	if(rank == serialRank){
		// serial vector filled with random values in [0,1] and calculate its norm
		initializeSerialVector(&vecS, nTotal); // allocate space for data
		int i;
		for(i=0; i<nTotal; ++i) vecS.dataTotal[i] = (float)rand() / (float)RAND_MAX; // fill with random values
		// zero pad if nTotal not divisible by n_procs
		if(nTotal < vecS.nTotal){
			for(i=nTotal; i<vecS.nTotal; ++i) vecS.dataTotal[i] = 0;
		}
		// calculate the serial norm
		sNorm = serialNorm(&vecS);
	}

	// distribute the serial vector  and calculate same norm in parallel
	flagSum += initializeParallelVector(&vecP, nTotal); // allocate space for subset of data
	flagSum += scatterVector(&vecS, &vecP, serialRank); // fill in all vecP with data subsets
	float pNorm  = parallelNorm(&vecP, serialRank); // calculate norm in parallel

	// check serial vs parallel results on the serialRank
	if(rank == serialRank){
		// check whether the parallel norm is correct
		if(fabs(sNorm - pNorm) < fabs(sNorm)*1e-6){
			printf("Serial norm and parallel norm match: %e \n",sNorm); // what you want to  see
		}
		else{
			printf("WARNING: serial norm was %e but parallel norm was %e \n", sNorm, pNorm); // what you'll see if you've made an error
		}
	}
	else{
		printf("Rank %d got parallel norm %e \n", rank, pNorm); // should match parallel norm recorded on all other ranks
	}
	
	// only serialRank needs to cleanup serial vector
	if(rank == serialRank) cleanupSerial(&vecS);
	// all ranks need to cleanup parallel  vector
	cleanupParallel(&vecP); 

	MPI_Finalize();
}
