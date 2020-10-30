#include <stdlib.h>
#include <stdio.h>
#include <omp.h>
#include "material.h"
#include "simulation.h"
#include "checkPt.h"
#include "suite.h"

// setup to get space allocated for all arrays and track simulation metrics
// (be sure to check for valid source x indices given material parameters)
int initializeSuite(suite *thisSuite, material *thisMaterial, unsigned int nSteps, float minBdryVal, float maxBdryVal, unsigned int nBdryVals, unsigned int minSrcIdxX, unsigned int maxSrcIdxX, float minSrcTemp, float maxSrcTemp, unsigned int nSrcTemps, float dt, unsigned int srcIdxY, unsigned int xIDHist, unsigned int yIDHist){
	int flag = 0; // change to nonzer if issues encountered
	// attach suite to a material
	thisSuite->thisMaterial = thisMaterial;
	// record time step desired and number of time steps
	thisSuite->dt = dt;
	thisSuite->nSteps = nSteps;
	// record y index of source (constant for all simulations in suite
	thisSuite->srcIdxY = srcIdxY;
	// record info about boundary values
	thisSuite->minBdryVal = minBdryVal;
	thisSuite->maxBdryVal = maxBdryVal;
	thisSuite->nBdryVals = nBdryVals;
	// record info about source x index
	thisSuite->minSrcIdxX = minSrcIdxX;
	thisSuite->maxSrcIdxX = maxSrcIdxX;
	thisSuite->nSrcX = maxSrcIdxX - minSrcIdxX + 1;
	// check for valid source x indices compared to bounds of array
	if(maxSrcIdxX < minSrcIdxX){
		printf("WARNING: in initializeSuite, maxSrcIdxX must be >= minSrcIdxX \n");
		flag++;
	}
	if(maxSrcIdxX >= thisMaterial->Nx){
		printf("WARNING: in initializeSuite, maxSrcIdxX and minSrcIdxX must be < thisMaterials Nx value \n");
		flag++;
	}
	// record info about location (xIDHist col and yIDHist row) where time series of temperatures will be recorded
    thisSuite->xIDHist = xIDHist;
    thisSuite->yIDHist = yIDHist;
    if((xIDHist >= thisMaterial->Nx) || (yIDHist >= thisMaterial->Ny)){
    	printf("WARNING: in initializeSuite, bad location to record temperature history xIDHist, yIDHist \n");
    	flag++;
    }
	// record info about source temperatures
	thisSuite->minSrcTemp = minSrcTemp;
	thisSuite->maxSrcTemp = maxSrcTemp;
	thisSuite->nSrcTemps = nSrcTemps;
	if(maxSrcTemp < minSrcTemp){
		printf("WARNING: in initializeSuite, maxSrcTemp must be > minSrcTemp \n");
		flag++;
	} 
	if(minSrcTemp < 0){
		printf("WARNING: in initializeSuite, temperatures must be >=0 \n");
		flag++;
	}

	// calculate total number of simulations in suite
	thisSuite->nSims = nBdryVals * (thisSuite->nSrcX) * nSrcTemps; // total number of simulations in suite
	// allocate arrays to store boundary values, source x index, and source temperature for each simulation
	thisSuite->bdryVal = (float *)malloc((thisSuite->nSims) * sizeof(float));
	thisSuite->srcIdxX = (unsigned int *)malloc((thisSuite->nSims) * sizeof(unsigned int));
	thisSuite->srcTemp = (float *)malloc((thisSuite->nSims) * sizeof(float));
	if((thisSuite->bdryVal == NULL) || (thisSuite->srcIdxX == NULL) || (thisSuite->srcTemp == NULL)){
		printf("WARNING: bad allocation for suite bdryVal, srcIdxX or srcTemp arrays \n");
		flag++;
	}
	
	// fill arrays with info about conditions of each simulation	
	// ===========================YOUR CODE HERE========================================
				// calculate source temperature initially
				// fill bdryVal, srcIdxX and srcTemp arrays
				// move onto next simulation index
	// ============================END OF YOUR CODE====================================

	// allocate arrays to track mean and max temperratures (one value in each array per simulation)
	thisSuite->meanTemp = (float *)malloc((thisSuite->nSims) * sizeof(float));
	thisSuite->maxTemp = (float *)malloc((thisSuite->nSims) * sizeof(float));
	if((thisSuite->maxTemp == NULL) || (thisSuite->meanTemp == NULL)){
		printf("WARNING: bad allocation for suite meanTemp or maxTemp arrays\n");
		flag++;
	}
	
	return flag;
};
 

// from end-to-end set up a simulation, run a simulation, record its mean and max temps and time series at location of interest (row yID, col xID), and tear it down
// To name files: parCode = 0 if static, 1 if Dynamic, 2 if Guided
int wholeSimulation(suite *thisSuite, unsigned int testNumber, float *mean, float *max, int parCode){
	int flag = 0;

	// create intial state array based on bdryVal, srcIdxX and srcTemp at testNumber index
	unsigned int Nx = (thisSuite->thisMaterial)->Nx;
	unsigned int Ny = (thisSuite->thisMaterial)->Ny;
	float *initState = (float *)malloc(Nx*Ny*sizeof(float));
	if(initState == NULL) flag++; // check for bad allocation of initState
	float bdryVal = thisSuite->bdryVal[testNumber];
	unsigned int srcIdxX = thisSuite->srcIdxX[testNumber];
	float srcTemp = thisSuite->srcTemp[testNumber];
	unsigned int row,col;

	// fill entries of initState with the initial distribution 
	for(row=0; row<Ny; ++row){
		for(col=0; col<Nx; ++col){
			// all other points are set to bdryVal to start
			initState[row*Nx + col] = bdryVal;
			// inject single source point
			if((row == thisSuite->srcIdxY) && (col == srcIdxX)) initState[row*Nx + col] = srcTemp;
		}
	}

	// ===============YOUR CODE BEGINS HERE============================================
	// initialize the simulation of interest
	// initialize both the check pointing structs (although the checkPtTime can have 0 steps)

	// run the simulation of interest and record the mean and max values, and a time series at location of interest
	// =============YOUR CODE ENDS HERE===================================================	

	// generate filename based on testNumber
	const size_t nChars = 50; // max number of characters in the file name
	char filename[nChars];
	snprintf(filename, nChars, "results/timeseries_%d_%d.txt", testNumber, parCode);
	
	// =================================YOUR CODE BEGINS HERE=============================
	// write the time series to file

	// cleanup initState
	
	// cleanup thisSim

	// cleanup the checkPtSpace and checkPtTime
	
	// =================YOUR CODE ENDS HERE=============================================

	return flag;
};

// run the suite of simulations in parallel with static scheduling (1 for verbose during debugging, 0 to suppress output)
int runSuiteStatic(suite *thisSuite, int verbose){
	int flag = 0;

	// actually run the suite of all tests in parallel
	int testNumber, thisFlag; 
// ==================YOUR CODE BEGINS HERE================================
	// for loop over simulations	
		// print the simulation number if verbose
		// run the simulation for this test number
// ====================YOUR CODE ENDS HERE=================================	
	return flag;
};


// run the suite of simulations in parallel with dynamic scheduling (1 for verbose during debugging, 0 to suppress output)
int runSuiteDynamic(suite *thisSuite, int verbose){
	int flag = 0;

	// actually run the suite of all tests in parallel
	int testNumber, thisFlag;
// ==================YOUR CODE BEGINS HERE================================
	// for loop over simulations
		// print the simulation number if verbose
		// run the simulation for this test number
// ====================YOUR CODE ENDS HERE=================================	
	return flag;

};


// run the suite of simulations in parallel with guided scheduling (1 for verbose during debugging, 0 to suppress output)
int runSuiteGuided(suite *thisSuite, int verbose){
	int flag = 0;

	// actually run the suite of all tests in parallel
	int testNumber, thisFlag;
// ==================YOUR CODE BEGINS HERE================================
	// for loop over simulaitons
		// print the simulation number if verbose
		// run the simulation for this test number
// ====================YOUR CODE ENDS HERE=================================	
	return flag;
};


// clean up all memory allocated for this suite
int cleanupSuite(suite *thisSuite){
	free(thisSuite->bdryVal);
	thisSuite->bdryVal = NULL;
	free(thisSuite->srcIdxX);
	thisSuite->srcIdxX = NULL;
	free(thisSuite->srcTemp);
	thisSuite->srcTemp = NULL;
	free(thisSuite->meanTemp);
	thisSuite->meanTemp = NULL;
	free(thisSuite->maxTemp);
	thisSuite->maxTemp = NULL;
    return 0;
};
