#ifndef __SERIALVECTOR_H__
#define __SERIALVECTOR_H__

// serial vector with all data stored on one process
typedef struct serialVector_tag{
	int nTotal;
	float *dataTotal;
}serialVector;

// initialization functions to set up the space of data and set dimensions
int initializeSerialVector(serialVector *vecS, int nTotal);

// functions to calculate the L1 norm of a vector (sum of absolute values of  entries)
float serialNorm(serialVector *vecS); 

// functions to free the space of data
int cleanupSerial(serialVector *vec);

#endif
