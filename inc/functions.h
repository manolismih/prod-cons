#ifndef FUNCTIONS_H
#define FUNCTIONS_H

typedef struct {
	double A[100][100];
	double B[100][100];
} mult_100x100_matrixArgs;
void* mult_100x100_matrixArgsGen();
void* mult_100x100_matrix (void* args);

void* stopFcn();
void* errorFcn();

#endif
