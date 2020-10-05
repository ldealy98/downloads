#include <stdlib.h> // for random number generation
#include <time.h> // for clock timing
#include <stdio.h> // for input/output
#include "VectorND.h"


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
	printf("------- Testing %d dim. axpy vs repeated scalar multiply add -------- \n",dim);

	
	// create a, b, c, d, x, y, z (where x and y are initialized with random values)
	float a = 1.5;
	float b = -4.0;
	float c = 3.1415926;
	float d;
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



	// ---------do repeated scalar multiply add (d = a*b + c) -----------
	beginning = clock();
	// run the multiply add dim times, and repeat nRuns times
	int i,j;
	for(i=0; i<nRuns; ++i){
		for(j=0; j<dim; ++j) d = a*b + c;
	}
	ending = clock();
	double secondsToRun = (double)(ending-beginning) / (double)CLOCKS_PER_SEC;
	double avgSecPerRunScalar = secondsToRun/(double)nRuns;
	printf("Repeated scalar multiply add took  %e seconds per run \n", avgSecPerRunScalar);



	// --------do an axpy (z = a*x + y) in regular order----------
	beginning = clock();
	// run the axpy in regular order nRuns times
	for(i=0; i<nRuns; ++i) regularAxpy(a, &x, &y, &z);
	ending = clock();
	secondsToRun = (double)(ending-beginning) / (double)CLOCKS_PER_SEC;
	double avgSecPerRunReg = secondsToRun/(double)nRuns;
	printf("Regularly ordered axpy took %e seconds per run \n", avgSecPerRunReg);


	// cleanup
	deallocate_VectorND(&x);
	deallocate_VectorND(&y);
	deallocate_VectorND(&z);

	return 0;
}
