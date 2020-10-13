#include <stdlib.h>
#include <stddef.h>
#include <string.h>
#include <time.h>
#include <math.h>
#include <omp.h>
#include "VectorNDPar.h"

int allocate_VectorND(VectorND* v, int dimension) {

    v->dimension = dimension;
    v->data = malloc(sizeof(float)*dimension);

    // malloc failed
    if (v->data == NULL) return 1;

    memset(v->data, 0, sizeof(float)*dimension); 

    return 0;
}

int deallocate_VectorND(VectorND* v) {

    v->dimension = 0;

    free(v->data);
    v->data = NULL;

    return 0;
}

int randFill(VectorND* v){
    // seed with the current time
    srand(time(NULL));
    // check dimension of vector
    int dim = v->dimension;
    // loop through and fill vector with random floats between 0 and 1
    int i;
    for(i=0; i<dim; i++){
        // rand() generates ints between 0 and RAND_MAX, so scale by RAND_MAX
        v->data[i] = (float)rand() / RAND_MAX; 
    }

    return 0;
}


int regularAxpy(float alpha, VectorND *vx, VectorND *vy, VectorND *vz) {

    // Sanity check on array dimensions
    if ((vx->dimension != vy->dimension) || (vx->dimension != vz->dimension)){
        return 1;
    }

    int i;
    for(i=0; i < vx->dimension; i++){
        vz->data[i] = alpha*vx->data[i] + vy->data[i];
    }

    return 0;
}

int threadedAxpy(float alpha, VectorND *vx, VectorND *vy, VectorND *vz) {

    // Sanity check on array dimensions
    if ((vx->dimension != vy->dimension) || (vx->dimension != vz->dimension)){
        return 1;
    }

    int i;
#pragma omp parallel for
    for(i=0; i < vx->dimension; i++){
        vz->data[i] = alpha*vx->data[i] + vy->data[i];
    }

    return 0;
}

