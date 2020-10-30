#include <stdio.h>
#include <stdlib.h>
#include <omp.h>
#include "../code/material.h"
#include "../code/checkPt.h"
#include "../code/simulation.h"
#include "../code/suite.h"

int main(int argc, char **argv){
	unsigned int nTemps = (unsigned int)atoi(argv[1]); // number of temperatures
	unsigned int nBdryVals = (unsigned int)atoi(argv[2]); // number of boundary values
	int parFlag = atoi(argv[3]); // 0 if static, 1 if dynamic, 2 if guided

	// setup the material
	// diffusivity (homogeneous throughout material
	float alpha = 2.0;
	// discretization parameter
	unsigned int Nx = 200;
	unsigned int Ny = 300;
	float dx = 1.5;
	float dy = 1.0;
	// actually create the material
	material thisMaterial;
	int flag = initMaterial(&thisMaterial, Nx, Ny, dx, dy, alpha); 
	if(flag) printf("WARNING: error in initMaterial \n");
	
	// create the range of parameters that will be used for the simulation suite
	float minBdryVal = 0.1;
	float maxBdryVal = 10; // boundary temps rand between 0.1 and 10 for the simulations
	unsigned int minSrcIdxX = 10;
	unsigned int maxSrcIdxX = 12; 
	unsigned int srcIdxY = 20;
	float minSrcTemp = 10;
	float maxSrcTemp = 100;
	float dt = 0.1;
	unsigned int timeSteps = 50;
	// point where you'll record a single point's history of temperature values
	unsigned int xHistID = 15;
	unsigned int yHistID = 22;


	// create the suite of simulations
	suite thisSuite;
	flag = initializeSuite(&thisSuite, &thisMaterial, timeSteps, minBdryVal, maxBdryVal, nBdryVals, minSrcIdxX, maxSrcIdxX, minSrcTemp, maxSrcTemp, nTemps, dt, srcIdxY, xHistID, yHistID); 
	if(flag) printf("WARNING: error in initializeSuite\n");


	// run the suite of static parallelized simulations
	if(parFlag == 0){
		// =============ADD YOUR TIMING CALL HERE================
		flag = runSuiteStatic(&thisSuite, 0); // note: can change between 0 (non-verbose) and 1 (verbose) for debugging
		// =============ADD YOUR TIMING CALL HERE================
		// =============ADD YOUR TIMING PRINT STATEMENT HERE=======
	}
	else{
		if(parFlag == 1){
			// =============ADD YOUR TIMING CALL HERE================
			flag = runSuiteDynamic(&thisSuite, 0); // note: can change between 0 (non-verbose) and 1 (verbose) for debugging
			// =============ADD YOUR TIMING CALL HERE================
			// =============ADD YOUR TIMING PRINT STATEMENT HERE=======
		}
		else{
			// =============ADD YOUR TIMING CALL HERE================
			flag = runSuiteGuided(&thisSuite, 0); // note: can change between 0 (non-verbose) and 1 (verbose) for debugging
			// =============ADD YOUR TIMING CALL HERE================
			// =============ADD YOUR TIMING PRINT STATEMENT HERE=======
		}
	}	


    // write the means and maxes to a file
    const size_t nChars = 50; // max number of characters in the file name
	char filename[nChars];
	snprintf(filename, nChars, "results/means_maxs_%d.txt", parFlag);
	FILE *filePtr;
	filePtr = fopen(filename, "w");
	if(filePtr == NULL){
		printf("ERROR in opening file in writeStateHistToFile \n");
		return 1;
	}
	// write the first line to say what is below
	fprintf(filePtr,"Each line stores mean[i] \t max[i] for the ith simulation\n");
	unsigned int nSims = thisSuite.nSims;
	unsigned int simNumber;
	for(simNumber=0; simNumber < nSims; ++simNumber){
	    fprintf(filePtr,"%f \t %f\n", thisSuite.meanTemp[simNumber], thisSuite.maxTemp[simNumber]);	
	}
	fclose(filePtr);

	
	// cleanup the simulation and material struct
	flag = cleanupSuite(&thisSuite);
	if(flag) printf("WARNING: issue cleaning up the suite \n");

	return 0;
}
