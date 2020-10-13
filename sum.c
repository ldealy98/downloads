#include<stdlib.h>
#include<stdio.h>
#include<omp.h>

int main(){
	// create array of 1000 entries 0, 0.1, 0.2, 0.3, ....
	int N = 1000;
	float *data = malloc(N*sizeof(float));
	int i;
	for(i=0; i<N; ++i) data[i] = 0.1*(float)i;

	// check how many threads are contributing
	int NThreads = omp_get_max_threads();
	printf("OpenMP has access to %d threads \n",NThreads);

	// each thread will track its own sub-sum
	float s[NThreads];
	for(i=0; i<NThreads; ++i) s[i] = 0; 
	
	// actually calculate the sum of all entries per thread
#pragma omp parallel for
	for(i=0; i<N; ++i){
		int threadNum = omp_get_thread_num(); // get index of this thread
		s[threadNum] += data[i];
	}
// back in the master thread after for loop

	// calculate total sum of the sub-sums
	float totalSum = 0;
	for(i=0; i<NThreads; ++i) totalSum += s[i];

	// print the sum out
	printf("sum is %f \n",totalSum);

	// cleanup
	free(data);
	data = NULL;

	return 0;
}
