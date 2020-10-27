#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <mpi.h>
#include "serialVector.h"
#include "parallelVector.h"

// initialization function to set up the space for data and set dimensions of parallel distributed vector
int initializeParallelVector(parallelVector  *vecP, int nTotal){
	// calculate how many entries expected for this parallel part of vector
	int n_procs, rank;
	MPI_Comm_size(MPI_COMM_WORLD, &n_procs);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	int nLocal = nTotal/n_procs;
        int n_ranks_with_extra = nTotal % n_procs;
	if(rank < n_ranks_with_extra) nLocal++;
	vecP->nLocal = nLocal;

	//  check that dimensions are reasonable
	int flag = 0;
	if(nLocal < 0){ // note: 0 is allowed for cases where there are more processes than total serial vector entries
		fprintf(stderr,"ERROR: nLocal must be non-negative. \n");
		flag = 1;
	}
	else{ // do the allocation of space
		vecP->dataLocal = (float *)malloc(nLocal*sizeof(float));
		if(vecP->dataLocal == NULL){ // check for good allocation
			int  rank;
			MPI_Comm_rank(MPI_COMM_WORLD, &rank);
			fprintf(stderr,"ERROR: bad data allocation of parallel vector for rank %d. \n", rank);
			flag = 1;
		}
	}
	return flag;
};

// function used by sender to scatter serial vector data on one process into a parallel distributed vector on all processes (that are already initialized)
int scatterVectorSender(serialVector *vecS, parallelVector *vecP){
	// bookkeeping of how many ranks 
	int tag = 0;
	int rank, n_procs;
	MPI_Comm_size(MPI_COMM_WORLD, &n_procs);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	int flagSum = 0;

	// sending the data subsets out one rank at a time
	int i;
	int startID = 0;
	for(i=0; i<n_procs; ++i){ // send data to all other processes, and copy into self
		//figure out how many entries should be in this portion of distributed vector
		int nEntries = vecS->nTotal / n_procs;
		if(i < (vecS->nTotal % n_procs)) nEntries++;

		if(i != rank){ // send data to other rank
			flagSum += MPI_Send(&(vecS->dataTotal[startID]), nEntries, MPI_FLOAT, i, tag, MPI_COMM_WORLD);
		}
		else{ // copy in  portion of vecS into own vecP
			int j;
			for(j=0; j<nEntries; ++j) vecP->dataLocal[j] = vecS->dataTotal[j+startID];
		}	
		// update starting location for next rank
		startID += nEntries;
	}
	return flagSum;
};


// function used by receiving processes to get parts of a serial vector's data from senderRank and 
// put it into a parallel distributed vector on this process (that is already initialized)
int scatterVectorReceiver(int senderRank, parallelVector *vecP){
	// ===============BEGINNING OF YOUR CODE============================

	// ================END OF YOUR CODE==================================

	return flag; // flag should represent the output of an MPI_Recv
};

// function to calculate the L1 norm  of a full parallel vector and send that norm back to all ranks
float parallelNorm(parallelVector *vecP){
	// bookkeeping about who and how many ranks, and communication status
	int rank, n_procs;
	// =============YOUR CODE HERE TO CHECK RANK AND NUMBER OF PROCESSES========
	
	// ================END OF YOUR CODE=========================================
	int tag = 0;
	MPI_Status status;
	int rankToCollectNorm = 0; // will do the full total norm calculation on rank 0

	// each rank calculates its own local sum of absolute values
	int i;
	double  localSum = 0;
	// ====================YOUR CODE HERE TO CALC LOCAL SUM OF ABS VALS==========

	// ======================+END OF YOUR CODE===================================
	
	// send the local sums all into the rankToCollectNorm and add up
	float totalNorm;
	// ===================YOUR CODE HERE TO CENTRALIZE AND ADD SUMS OF ABS VALS ====== 
	// on the rankToCollectNorm, add up the terms then take square root
	
	// from all other processes, send the local sum to rankToCollectNorm

	// ====================END OF YOUR CODE===========================================


	// send the totalNorm from rankToCollectTerms to all other ranks
	if(rank == rankToCollectNorm){
		for(i=0; i<n_procs; ++i){ // for each other rank, send it the totalNorm
			if(i != rankToCollectNorm){
				int flag = MPI_Send(&totalNorm, 1, MPI_FLOAT, i, tag, MPI_COMM_WORLD);
			}
		}
	}	
	else{ // receive the totalNorm from rankToCollectNorm
		int flag = MPI_Recv(&totalNorm, 1, MPI_FLOAT, rankToCollectNorm, tag, MPI_COMM_WORLD, &status);
	} 	

	// return norm of overall vector
	return totalNorm;
};


// functions to cleanup the space of dataTotal/dataLocal
int cleanupParallel(parallelVector *vec){
	vec->nLocal = 0;
	free(vec->dataLocal);
	vec->dataLocal = NULL;
	return 0;
};
