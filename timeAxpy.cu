#include <stdio.h>
#include <cuda.h>
#include <cuda_runtime_api.h>
#include <time.h>

__global__ void axpy(float a, float *xVec, float *yVec){
	int idx = blockIdx.x * blockDim.x + threadIdx.x;
	yVec[idx] = a*xVec[idx] + yVec[idx];
}

int main(int argc, char** argv){

	int N = atoi(argv[1]);
	float a = 0.5;

	float *x_host = (float *)malloc(N*sizeof(float));
	float *y_host = (float *)malloc(N*sizeof(float));

	int i;
	for(i=0; i<N; i++){
		x_host[i] = (float)i;
		y_host[i] = 1.0;
	}

	float *z = (float *)malloc(N*sizeof(float));
	clock_t startTime = clock();
	int rep;
	int nReps = 100;
	for(rep=0; rep<nReps; rep++){
		for(i=0; i<N; i++) z[i] = a*x_host[i] + y_host[i];
	}
	clock_t endTime = clock();
	float cpuTime = float(endTime-startTime)/(float)(nReps*CLOCKS_PER_SEC);
	printf("CPU took %e seconds to run %d entry axpy \n", cpuTime, N);
	free(z);
	z = NULL;

	//starting GPU time
	startTime = clock();
	
	
	float* x_device;
	float* y_device;
	cudaMalloc((void**)&x_device, N*sizeof(float));
	cudaMalloc((void**)&y_device, N*sizeof(float));

	// copy from host to device
	cudaMemcpy(x_device, x_host, N*sizeof(float), cudaMemcpyHostToDevice);
	cudaMemcpy(y_device, y_host, N*sizeof(float), cudaMemcpyHostToDevice);  

	int n_threads_per_block = 16;
	int n_blocks = N/n_threads_per_block;

	axpy <<<n_blocks, n_threads_per_block>>>  (a, x_device, y_device);

	
	cudaMemcpy(y_host, y_device, N*sizeof(float), cudaMemcpyDeviceToHost);
	
	cudaFree(x_device);
	cudaFree(y_device);
	x_device = NULL;
	y_device = NULL;
	
	endTime = clock();
	float timeGPU = (float)(endTime-startTime)/(float)(nReps*CLOCKS_PER_SEC);
	printf("Time to run GPU code: %e \n", timeGPU);	

	free(x_host);
	free(y_host);
	x_host=NULL;
	y_host=NULL;

	return 0;
}
