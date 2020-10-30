#ifndef __SUITE_H__
#define __SUITE_H__

typedef struct material_struct material;

typedef struct suite_struct{
	unsigned int nSims; // number of simulations to be carried out by the suite (nBdryVals * nSrcX * nSrcTemps)
	// info about common parameters of the simulations
	material *thisMaterial; // a pointer to the material this suite of simulations will happen on
	unsigned int nSteps; // number of time steps per simulation
	float dt; // time step size in all simulations (same as dt in simulations in suite)
	// info about initial condition distribution and boundary value distribution	
	float minBdryVal; // minimum boundary value used for some simulations
	float maxBdryVal; // maximum boundary value used by some simulations
	unsigned int nBdryVals; // number of different boundary values to try
	unsigned int minSrcIdxX; // minimum source column location used for some simualtions
	unsigned int maxSrcIdxX; // maximum source column location used for some simulations
	unsigned int nSrcX; // should be maxSrcIdxX - minSrcIdxX + 1 (number of source x indicees to try)
	unsigned int srcIdxY; // index of source row location used for all simulations (constant)
	unsigned int xIDHist; // column index of location where time series of temperature will be recorded
	unsigned int yIDHist; // row index of location where time series of temperature will be recorded
	float minSrcTemp; // minimum source temperature used for some simulations
	float maxSrcTemp; // maximum source temperature useed for some simulations
	unsigned int nSrcTemps; // number of different source temperatures to try
    // arrays to track setup of simulation (each array has 1 value per simulation)
    float *bdryVal; // boundary value for each simulation
    unsigned int *srcIdxX; // source x index for each simulation
    float *srcTemp; // initial source temperature for each simulation
	// stuff about tracking the min and max temperatures
	float *meanTemp; // for each simulation record the mean temperature over all space and time
	float *maxTemp; // for each simulation record the maximum temperature over all space and time 
} suite;

// setup to get space allocated for all arrays and track simulation metrics
// (be sure to check for valid source x indices given material parameters)
int initializeSuite(suite *thisSuite, material *thisMaterial, unsigned int nSteps, float minBdryVal, float maxBdryVal, unsigned int nBdryVals, unsigned int minSrcIdxX, unsigned int maxSrcIdxX, float minSrcTemp, float maxSrcTemp, unsigned int nSrcTemps, float dt, unsigned int srcIdxY, unsigned int xIDHist, unsigned int yIDHist); 

// from end-to-end set up a simulation, run a simulation, record its mean and max temps and tear it down
// To name output files: parCode = 0 if static, 1 if dymamic, 2 if guided
int wholeSimulation(suite *thisSuite, unsigned int testNumber, float *mean, float *max, int parCode);

// run the suite of simulations in parallel with static scheduling (1 for verbose during debugging, 0 to suppress output)
int runSuiteStatic(suite *thisSuite, int verbose);

// run the suite of simulations in parallel with dynamic scheduling (1 for verbose during debugging, 0 to suppress output)
int runSuiteDynamic(suite *thisSuite, int verbose);

// run the suite of simulations in parallel with guided scheduling (1 for verbose during debugging, 0 to suppress output)
int runSuiteGuided(suite *thisSuite, int verbose);

// clean up all memory allocated for this suite
int cleanupSuite(suite *thisSuite);

#endif
