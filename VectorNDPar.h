#ifndef __VECTORND_H__
#define __VECTORND_H__

typedef struct VectorND_tag {
	float* data;
	int dimension;
} VectorND;

int allocate_VectorND(VectorND* v, int dimension);

int deallocate_VectorND(VectorND* v);

int randFill(VectorND* v);

int regularAxpy(float alpha, VectorND* vx, VectorND* vy, VectorND* vz);

int threadedAxpy(float alpha, VectorND* vx, VectorND* vy, VectorND* vz);

#endif
