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
	if(nTotal  % n_procs != 0) nLocal++; //assumes 0 padding if nTotal not divisible by n_procs
	vecP->nLocal = nLocal;

	//  check that dimensions are reasonable
	int flag = 0;
	if(nLocal < 0){ // note: 0 is allowed for cases where there are more processes than total serial vector entries
		fprintf(stderr,"ERROR: nLocal must be non-negative. \n");
		flag = 1;
	}
	else{ // do the allocation of space
		vecP->dataLocal = (float *)malloc(vecP->nLocal*sizeof(float));
		if(vecP->dataLocal == NULL){ // check for good allocation
			fprintf(stderr,"ERROR: bad data allocation of parallel vector for rank %d. \n", rank);
			flag = 1;
		}
	}
	return flag;
};

// functions scatter serial vector data on one process into a parallel distributed vector on all processes (assume serialVector is initialized on root process and parallelVector is initialized on all processes)
int scatterVector(serialVector *vecS, parallelVector *vecP, int root){
	// YOUR CODE HERE
	
	return outMsg;
};


// function to calculate the L1 norm  of a full parallel vector and send that norm back to all ranks
float parallelNorm(parallelVector *vecP, int root){
	// bookkeeping about who and how many ranks, and communication status
	int rank, n_procs;
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	MPI_Comm_size(MPI_COMM_WORLD, &n_procs);
	int tag = 0;
	MPI_Status status;
	// Note: will do the full total norm calculation on root process

	// each rank calculates its own local sum of absolute values
	int i;
	double  localSum = 0;
	for(i=0; i<vecP->nLocal; ++i) localSum += fabs((double)(vecP->dataLocal[i]));
	
	// send the local sums all into the root rank and add up, then turn into a float so you have the total norm on the root process
	// YOUR CODE HERE


	// send the totalNorm from root to all other ranks
	// YOUR CODE HERE

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
