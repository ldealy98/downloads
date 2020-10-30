#ifndef __CHECKPT_H__
#define __CHECKPT_H__

// forward declarations of structs a checkPtTime will have pointers to
typedef struct sim_struct sim;
typedef struct material_struct material;

typedef struct checkPtTime_struct{
	material *thisMaterial; // pointer to an already initialized material grid
	sim *thisSim; // a pointer to an already initialized simulation
	int nSnaps; // number of snapshots to record
	int currentSnapIdx; // index of the current snapshot (within times and stateSnapshots)
	float *times; // record times (in seconds) of each snapshot (nSnaps entries)	
	float *stateSnapshots; // pointer to the snapshots (nSnaps x thisMaterial.Nx x thisMaterial.Ny
} checkPtTime;

// Calculate the number of snapshots you'll make if you start at the
// 0th time step as the 0th snapshot, and take a new snapshot every 
// stepsPerCheckPt time steps in a simulation that will have nSteps
// including the 0th time step.
int calcNSnaps(int nSteps, int stepsPerCheckPt);

// initialize the space for times and stateSnapshots (note: assumes thisMaterial already initialized)
int initCheckPtTime(checkPtTime *thisCheckPt, material *thisMaterial, sim *thisSim, int nSnaps);

// record the current snapshot
int recordSnap(checkPtTime *thisCheckPt);

// write all snapshots to a file at end of simulation (file named as filename). 
// Data will be in form:
// Nx
// Ny
// nSnaps
// 1stSnapTime, all, entries, of, first, snapshot, in, order, in, one, row
// 2ndSnapTime, all, entries, of, second, snapshot, in, order, in, one, row
// etc...
int writeTimeSnapsToFile(checkPtTime *thisCheckPt, const char *filename);

// cleanup space  allocated for times and stateSnapshots in checkPtTime struct
int cleanupCheckPtTime(checkPtTime *thisCheckPt);


// structure to record time series of temperatures at a single point
typedef struct checkPtSpace_struct{
	material *thisMaterial; // pointer to an already initialized material grid
	sim *thisSim; // a pointer to an already initialized simulation
	int nSteps; // number of time steps to be recorded (every time)
	int currentSnapIdx; // index of the current snapshot
	float *times; // record times (in seconds) of each time when temperature is recorded
	float *stateHistory; // array storing one temperature at each time for the location of interest
	float xLocation; // position of record (x direction)
	int xIdx; // index of the column (x direction index)
	float yLocation; // position of record (y direction)
	int yIdx; // index of the row (y direction index)
} checkPtSpace;

// initialize the space for times and stateHistory (note: assumes thisMaterial already initialized)
int initCheckPtSpace(checkPtSpace *thisCheckPt, material *thisMaterial, sim *thisSim, int nSteps, int xIdx, int yIdx);

// record the current entry in the stateHistory at this point
int recordStateHist(checkPtSpace *thisCheckPt);

// write the time series to a file at the end of simulation (file named as filename).
// Data will be in form:
// xIdx
// xLocation
// yIdx
// yLocation
// nSteps
// 1stSnapTime, 1stTemperature
// 2ndSnapTime, 2ndTemperature
// 3rdSnapTime, 3rdTemperature
// etc...
int writeStateHistToFile(checkPtSpace *thisCheckPt, const char *filename);

// cleanup space  allocated for times and stateHistory in checkPtSpace struct
int cleanupCheckPtSpace(checkPtSpace *thisCheckPt);

#endif
