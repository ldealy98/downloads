#include <stdio.h>
#include <stdlib.h>
#include "checkPt.h"
#include "material.h"
#include "simulation.h"


// initialize the space for times and stateSnapshots (note: assumes thisMaterial already initialized)
int initCheckPtTime(checkPtTime *thisCheckPt, material *thisMaterial, sim *thisSim, int nSnaps){
	thisCheckPt->nSnaps = nSnaps; // number of time snapshots to be recorded
	thisCheckPt->currentSnapIdx = 0; // start out on the 0th snapshot
	int flag = 0; // 0 if nothing's wrong, 1 if errors encountered
	if(nSnaps > 0){
    	thisCheckPt->times = malloc(nSnaps*sizeof(float));
    	thisCheckPt->thisMaterial = thisMaterial; // set a pointer to this material so you can always grab number of points in space
    	int nSpacePts = thisMaterial->Nx * thisMaterial->Ny; // number of points in space per snapshot
    	thisCheckPt->stateSnapshots = malloc(nSnaps*nSpacePts*sizeof(float)); 
    	thisCheckPt->thisSim = thisSim; // set a pointer to this simulation os you can always get access to the simulation's current state and time

    	if((thisCheckPt->times == NULL) || (thisCheckPt->stateSnapshots == NULL)){
    		printf("WARNING: in initCheckPtTime issue initializing arrays \n");
    		flag = 1;
    	}
	}
	
	return flag;
};

// Calculate the number of snapshots you'll make if you start at the
// 0th time step as the 0th snapshot, and take a new snapshot every 
// stepsPerCheckPt time steps in a simulation that will have nSteps
// including the 0th time step.
int calcNSnaps(int nSteps, int stepsPerCheckPt){
	if(nSteps % stepsPerCheckPt == 0){
		return nSteps/stepsPerCheckPt;
	}
	else{
		return 1+(nSteps/stepsPerCheckPt);
	}
};

// record the current snapshot
int recordSnap(checkPtTime *thisCheckPt){
	int flag = 0; // will be nonzero if there's an issue
	if((thisCheckPt->times == NULL) || (thisCheckPt->stateSnapshots == NULL)){
		printf("WARNING: recordSnap was called on a check point with uninitialized arrays \n");
		flag = 1;
	}

	// record the current time in seconds of this snapshot
	int currentId = thisCheckPt->currentSnapIdx;
	thisCheckPt->times[currentId] = (float)((thisCheckPt->thisSim)->currentTimeIdx) * (thisCheckPt->thisSim)->dt;

	// record the current snapshot of the temperature field
	float *currentSnapshot = (thisCheckPt->thisSim)->priorState; // pointer to the current state in thee simulation
	int nSpacePts = (thisCheckPt->thisMaterial)->Nx * (thisCheckPt->thisMaterial)->Ny; // number of points in space per snapshot
	int startID = nSpacePts * currentId; // current index within stateSnapshots to start
	float *start = thisCheckPt->stateSnapshots + startID; // beginning of the current snapshot in thisCheckPt
	// actually copy entries of the current temperature field form the simulation to the checkPtTime's array
	int i;
	for(i=0; i<nSpacePts; ++i) start[i] = currentSnapshot[i];

	// next one you'll record will be the next snapshot index, so move along
	thisCheckPt->currentSnapIdx = currentId + 1;
	
	return flag;
};

// Write all snapshots to a file at end of simulation (file named as filename). 
// Data will be in form:
// Nx
// Ny
// nSnaps
// 1stSnapTime, all, entries, of, first, snapshot, in, order, in, one, row
// 2ndSnapTime, all, entries, of, second, snapshot, in, order, in, one, row
// etc...
int writeTimeSnapsToFile(checkPtTime *thisCheckPt, const char *filename){
    if(thisCheckPt->nSnaps){
    
    	FILE *filePtr;
       	filePtr = fopen(filename, "w");
    	if(filePtr == NULL){
    		printf("ERROR in opening file in writeToFile \n");
    		return 1;
    	}
    	// write the first few lines saying the dimensions of the 3D array you're about to put  in the file
    	int Nx = (thisCheckPt->thisMaterial)->Nx;
    	fprintf(filePtr, "%d\n",Nx);
    	int Ny = (thisCheckPt->thisMaterial)->Ny;
    	fprintf(filePtr, "%d\n",Ny);
    	int nSnaps = thisCheckPt->nSnaps;
    	fprintf(filePtr, "%d\n",nSnaps);
    	// write each snapshot
    	int i,k;
    	for(i=0; i<nSnaps; ++i){
    		// write the time of the simulation
    		fprintf(filePtr,"%f ,",thisCheckPt->times[i]);
    		// write the state in a flattened array at that time
    		for(k=0; k<Nx*Ny; ++k){
    			fprintf(filePtr," %f ,",thisCheckPt->stateSnapshots[k+(i*Nx*Ny)]);
    		}
    		fprintf(filePtr,"\n");
    	}
    	fclose(filePtr);	
    }
	return 0; 
};

// cleanup space  allocated for times and stateSnapshots in checkPtTime struct
int cleanupCheckPtTime(checkPtTime *thisCheckPt){
    if(thisCheckPt->nSnaps){
	    free(thisCheckPt->times);
    	thisCheckPt->times = NULL;
	    free(thisCheckPt->stateSnapshots);
	    thisCheckPt->stateSnapshots = NULL;
    }
	return 0;
};


// initialize the space for times and stateHistory (note: assumes thisMaterial already initialized)
int initCheckPtSpace(checkPtSpace *thisCheckPt, material *thisMaterial, sim *thisSim, int nSteps, int xIdx, int yIdx){
	int flag = 0;
	// attach thisMaterial
	thisCheckPt->thisMaterial = thisMaterial;
	// attach thisSim
	thisCheckPt->thisSim = thisSim;
	// setup nSteps
	thisCheckPt->nSteps = nSteps;
	// setup location
	thisCheckPt->xIdx = xIdx;
	float dx = thisMaterial->dx;
	thisCheckPt->xLocation = xIdx * dx;
	thisCheckPt->yIdx = yIdx;
	float dy = thisMaterial->dy;
	thisCheckPt->yLocation = yIdx * dy;

	// allocate times 
	thisCheckPt->times = (float *)malloc(nSteps*sizeof(float));
	if(thisCheckPt->times == NULL) flag++;
	// allocate stateHistory to store temperatures
	thisCheckPt->stateHistory = (float *)malloc(nSteps*sizeof(float));
	if(thisCheckPt->stateHistory == NULL) flag++;
	
	// initial time index
	thisCheckPt->currentSnapIdx = 0;	

	return flag; 
};


// record the current entry in the stateHistory at this point
int recordStateHist(checkPtSpace *thisCheckPt){
	int flag = 0;

	// error checking of times and stateHistory
	if((thisCheckPt->stateHistory == NULL) || (thisCheckPt->times == NULL)){
		printf("WARNING: recordStateHist was called on a check point with uninitialized arrays \n");
		flag = 1;
	}	
	// record the current time (in seconds)
	int currentID = thisCheckPt->currentSnapIdx;
	thisCheckPt->times[currentID] = (float)((thisCheckPt->thisSim)->currentTimeIdx) * (thisCheckPt->thisSim)->dt;

	// calculate the spatial index of this location in the array
	int Nx = (thisCheckPt->thisMaterial)->Nx;
	int index = (thisCheckPt->yIdx)*Nx + (thisCheckPt->xIdx);
	// record the current temperature at this location
	thisCheckPt->stateHistory[currentID] = (thisCheckPt->thisSim)->priorState[index];

	// move on to the next snapshot time index
	thisCheckPt->currentSnapIdx = currentID + 1;

	return flag;
};


// Write the time series of temperature at a single location to a file. 
// Should be done at the end of simulation (file named as filename).
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
int writeStateHistToFile(checkPtSpace *thisCheckPt, const char *filename){
	int flag = 0; // will change to nonzero if issues encountered
	
	// open file to write
	FILE *filePtr;
	filePtr = fopen(filename, "w");
	if(filePtr == NULL){
		printf("ERROR in opening file in writeStateHistToFile \n");
		return 1;
	}
	// write the first few lines with xIdx, xLocation, yIdx, yLocation, nSteps
	fprintf(filePtr,"%d\n",thisCheckPt->xIdx);
	fprintf(filePtr,"%f\n",thisCheckPt->xLocation);
	fprintf(filePtr,"%d\n",thisCheckPt->yIdx);
	fprintf(filePtr,"%f\n",thisCheckPt->yLocation);
	fprintf(filePtr,"%d\n",thisCheckPt->nSteps);
	// write each entry of the time series at the location of interest
	int i;
	for(i=0; i < thisCheckPt->nSteps; ++i){ // for each time
		// write the time
		fprintf(filePtr,"%f , ",thisCheckPt->times[i]);
		// write the temperature at this time
		fprintf(filePtr,"%f\n",thisCheckPt->stateHistory[i]);
	}
	// close file
	fclose(filePtr);
	
	return flag;
};


// cleanup space  allocated for times and stateHistory in checkPtSpace struct
int cleanupCheckPtSpace(checkPtSpace *thisCheckPt){
	// free space for times
	free(thisCheckPt->times);
	thisCheckPt->times = NULL;
	// set nSteps and currentSnapIdx to 0 to indicate none
	thisCheckPt->nSteps = 0;
	thisCheckPt->currentSnapIdx = 0;
	// free space for stateHistory
	free(thisCheckPt->stateHistory);
	thisCheckPt->stateHistory = NULL;
	return 0;
};

