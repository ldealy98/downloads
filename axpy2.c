#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <omp.h>
#include "VectorNDPar.h"

int main(int argc, char *argv[]){
	// check that a command line argument specifying the dimension is there
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


	// --------do an axpy (z = a*x + y) in regular order----------
	beginning = clock(); // this is the way we time serial code
	// run the axpy in regular order nRuns times
	int i;
	for(i=0; i<nRuns; ++i) regularAxpy(a, &x, &y, &z);
	ending = clock();  // this is the way we time serial code
	double secondsToRun = (double)(ending-beginning) / (double)CLOCKS_PER_SEC;
	double avgSecPerRunReg = secondsToRun/(double)nRuns;
	printf("Regular axpy took %e seconds per run \n", avgSecPerRunReg);


	// --- don't do this it's using the serial timer which is wrong-----
	// --------do an openmp axpy (z = a*x + y) in regular order----------
	beginning = clock(); // this is the way we time serial code
	// run the axpy in regular order nRuns times
	for(i=0; i<nRuns; ++i) threadedAxpy(a, &x, &y, &z);
	ending = clock();  // this is the way we time serial code
	double secondsToRunOMP = (double)(ending-beginning) / (double)CLOCKS_PER_SEC;
	double avgSecPerRunOMP = secondsToRunOMP/(double)nRuns;
	printf("Threaded axpy took %e seconds per run \n", avgSecPerRunOMP);


	// --------do an openmp axpy (z = a*x + y) in regular order----------
	double omp_start = omp_get_wtime(); // this is the way we time openmp code
	// run the axpy in regular order nRuns times
	for(i=0; i<nRuns; ++i) threadedAxpy(a, &x, &y, &z);
	double omp_end = omp_get_wtime();  // this is the way we time openmp code
	double secondsToRunOMPFixed = omp_end - omp_start;
	double avgSecPerRunOMPFixed = secondsToRunOMPFixed/(double)nRuns;
	printf("Threaded axpy took %e seconds per run with right timer \n", avgSecPerRunOMPFixed);




	

	// cleanup
	deallocate_VectorND(&x);
	deallocate_VectorND(&y);
	deallocate_VectorND(&z);

	return 0;

}
