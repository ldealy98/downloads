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

// functions scatter serial vector data on one process into a parallel distributed vector on all processes (assume serialVector is initialized on root process and parallelVector is initialized on all processes)
int scatterVector(serialVector *vecS, parallelVector *vecP, int root); 

// function to calculate the L1 norm of a distribtued vector (sum of absolute values of entries) on the root rank, and send to all other ranks
float parallelNorm(parallelVector *vecP, int root); 

// functions to free the space of dataLocal
int cleanupParallel(parallelVector *vec);

#endif
