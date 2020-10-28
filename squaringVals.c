#include <stdlib.h>
#include <stdio.h>

int main(int argc, char *argv[]){
	int N = atoi(argv[1]);

	// create array on the heap with N entries
	int *arr = (int *)malloc(N*sizeof(int));
	// fill with consecutive integers
	int i;
	for(i=0; i<N; ++i) arr[i] = i;

	// create array on the heap with N entries to store squares of values in arr
	int *sqr = (int *)malloc(N*sizeof(int));
	// fill with squares of arr
	for(i=0; i<N; ++i) sqr[i] = (arr[i])*(arr[i]);

	// write both arrays to a file
	FILE *filePtr;
	filePtr = fopen("output.txt", "w");
	if(filePtr == NULL){
		printf("ERROR in opening file \n");
	}
	else{
		fprintf(filePtr,"%d entries, with each line being arr[i] \t sqr[i] \n",N);
		for(i=0; i<N; ++i) fprintf(filePtr,"%d \t %d \n",arr[i],sqr[i]);
	}
	fclose(filePtr);

	// cleanup 
	free(arr);
	free(sqr);

	return 0;
}
