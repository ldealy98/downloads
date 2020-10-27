#ifndef __PARALLELVECTOR_H__
#define __PARALLELVECTOR_H__

#include "serialVector.h"

// parallel vector with data distributed over many processes
typedef struct parallelVector_tag{
	int nLocal;
	float *dataLocal;
}parallelVector;

// initialization function to set up the space of dataLocal and set dimensions
int initializeParallelVector(parallelVector *vecP, int nTotal); // note: nTotal of serial vector, this will automatically calculate number of entries nLocal for the parallel subset

// functions scatter serial vector data on one process into a parallel distributed vector on all processes (that are already initialized)
int scatterVectorSender(serialVector *vecS, parallelVector *vecP); // called by the process that has the serial vector already
int scatterVectorReceiver(int senderRank, parallelVector *vecP); // called by all other processes

// function to calculate the L1 norm of a vector (sum of absolute values of entries)
float parallelNorm(parallelVector *vecP); 

// functions to free the space of dataLocal
int cleanupParallel(parallelVector *vec);

#endif
