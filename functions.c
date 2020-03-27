#include "functions.h"
#include <math.h>
#include <stdlib.h>
#include <stdio.h>

/*	Array with Function structs that give the generator and the pointer
	for each different available work function	*/
Function workFunctions[3] = {	
	{sin10, sin10ArgsGen},
	{mult_10x10_matrix, mult_10x10_matrixArgsGen},
	{print_nothing, print_nothingArgsGen} 
};
	
//**********************************************************************

void* sin10ArgsGen()
{
	sin10Args* args = malloc(sizeof(sin10Args));
	return args;
}

void* sin10(void* args)
{
	double* angle = ( (sin10Args*)args )->angle;
	double* result = malloc(10*sizeof(double));
	for (int i=0; i<10; i++) result[i] = sin(angle[i]);
	return result;
}

//**********************************************************************

void* mult_10x10_matrixArgsGen()
{
	mult_10x10_matrixArgs* args = malloc(sizeof(mult_10x10_matrixArgs));
	return args;
}

void* mult_10x10_matrix (void* args)
{
	double (*A)[10] = ( (mult_10x10_matrixArgs*)args )->A;
	double (*B)[10] = ( (mult_10x10_matrixArgs*)args )->B;
	double (*C)[10] = calloc(10*10,sizeof(double));
	for (int row=0; row<10; row++)
		for (int col=0; col<10; col++)
			for (int i=0; i<10; i++)
				C[row][col] += A[row][i]*B[i][col];
	return C;
}

//**********************************************************************

void* print_nothingArgsGen()
{
	return NULL;
}

void* print_nothing(void* args) //args will be ignored
{
	printf("");
	return args;
}
