#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "serialVector.h"


// initialization function to set up the space for data and set dimensions
int initializeSerialVector(serialVector *vecS, int nTotal){
	vecS->nTotal = nTotal;
	//  check that dimensions are reasonable
	int flag = 0;
	if(nTotal <= 0){
		fprintf(stderr,"ERROR: nTotal must be positive. \n");
		flag = 1;
	}
	else{ // if so, allocate space
		vecS->dataTotal = (float *)malloc(nTotal*sizeof(float));
		if(vecS->dataTotal == NULL){ // check for good allocation
			fprintf(stderr,"ERROR: bad data allocation of serial vector. \n");
			flag = 1;
		}	
	}
	return flag;
};

// function to calculate the L1 norm of a serial vector (sum of absolute values  entries)
float serialNorm(serialVector *vecS){
	double sum = 0;
	int i;
	for(i=0; i<vecS->nTotal; ++i) sum += fabs((double)(vecS->dataTotal[i]));
	float norm = (float)sum;
	return norm;
};


// functions to cleanup the space of dataTotal/dataLocal
int cleanupSerial(serialVector *vec){
	vec->nTotal = 0;
	free(vec->dataTotal);
	vec->dataTotal = NULL;
	return 0;
};
