#include <stdlib.h> // for random number generation
#include <time.h> // for clock timing
#include <stdio.h> // for input/output
#include "VectorND.h"

void randIdxShuffle(int *orig, int dimension){
	// This function randomly permutes (in place) the entries of the int array pointed to by orig
	srand(time(NULL)); // see random number generator
	int i;	
	for(i=0; i<dimension; ++i){
		// randomly choose an entry of origIndices 
		int swapIdx = rand() % dimension;
		// swap the entries at i and swapIdx
		int temp = orig[i];
		orig[i] = orig[swapIdx];
		orig[swapIdx] = temp;
	}
}


int main(int argc, char *argv[]){

	// check that a command line argument specifying the dimension is present
	if(argc < 2){
		printf("WARNING: Missing one input argument. \n");
		printf("Run code as > executable_name dimension_of_test (filename_for_timing_data) \n");
		return 1;
	} 
	// argv[1] holds the dimension of the test vectors as a string
	// so use atoi function (part of stdlib.h) to change to an interger
	int dim = atoi(argv[1]);
	printf("---------- Testing %d dimensional axpy ----------- \n",dim);

	
	// create a, x, y, z (where x and y are initialized with random values)
	float a = 1.5;
	VectorND x;
	VectorND y;
	VectorND z;
	// allocate the vectors and fill x and y with random numbers
	allocate_VectorND(&x, dim);
	allocate_VectorND(&y, dim);
	allocate_VectorND(&z, dim);
	randFill(&x);
	randFill(&y);

	// number of runs, and declare timers
	clock_t beginning, ending;
	int nRuns = 100; 
	printf("Starting timing tests with %d repeat runs \n",nRuns);



	// ---------do an axpy (z = a*x + y) in random order-----------
	// First, choose the random order (so not part of timing)
	int *idxOrder = (int*)calloc(dim, sizeof(int));
	int i;
	for(i=0; i<dim; ++i) idxOrder[i] = i; // start with indices in order
	randIdxShuffle(idxOrder,dim); // randomly permute the indices
	beginning = clock();
	// run the axpy in random order nRuns times
	for(i=0; i<nRuns; ++i) randomAxpy(a, &x, &y, &z, idxOrder);
	ending = clock();
	double secondsToRun = (double)(ending-beginning) / (double)CLOCKS_PER_SEC;
	double avgSecPerRunRand = secondsToRun/(double)nRuns;
	printf("Randomly ordered axpy took  %e seconds per run \n", avgSecPerRunRand);



	// --------do an axpy (z = a*x + y) in regular order----------
	beginning = clock();
	// run the axpy in regular order nRuns times
	for(i=0; i<nRuns; ++i) regularAxpy(a, &x, &y, &z);
	ending = clock();
	secondsToRun = (double)(ending-beginning) / (double)CLOCKS_PER_SEC;
	double avgSecPerRunReg = secondsToRun/(double)nRuns;
	printf("Regularly ordered axpy took %e seconds per run \n", avgSecPerRunReg);


	// cleanup
	free(idxOrder);
	deallocate_VectorND(&x);
	deallocate_VectorND(&y);
	deallocate_VectorND(&z);

	return 0;
}