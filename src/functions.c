#include "functions.h"
#include <stdlib.h>
#include <stdio.h>

void* mult_100x100_matrixArgsGen()
{
	mult_100x100_matrixArgs* args = malloc(sizeof(mult_100x100_matrixArgs));
	return args;
}

void* mult_100x100_matrix (void* args)
{
	double (*A)[100] = ( (mult_100x100_matrixArgs*)args )->A;
	double (*B)[100] = ( (mult_100x100_matrixArgs*)args )->B;
	double (*C)[100] = calloc(100*100,sizeof(double));
	for (int row=0; row<100; row++)
		for (int col=0; col<100; col++)
			for (int i=0; i<100; i++)
				C[row][col] += A[row][i]*B[i][col];
	return C;
}

void* stopFcn()
{
	printf("Timer finished\n");
	return NULL;
}

void* errorFcn()
{
	printf("Queue is full!!!\n");
	return NULL;
}
