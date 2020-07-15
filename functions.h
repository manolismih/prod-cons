#ifndef FUNCTIONS_H
#define FUNCTIONS_H

typedef struct {
	double angle[10];
} sin10Args;
void* sin10ArgsGen();
void* sin10(void* args);

//**********************************************************************

typedef struct {
	double A[10][10];
	double B[10][10];
} mult_10x10_matrixArgs;
void* mult_10x10_matrixArgsGen();
void* mult_10x10_matrix (void* args);

//**********************************************************************

void* print_nothingArgsGen();
void* print_nothing(void* args); //args will be ignored

//**********************************************************************

void* stopFcn();
void* errorFcn();

//**********************************************************************

typedef struct {
	void* (*pointer)(void*);
	void* (*argsGen)();
} Function;
extern Function workFunctions[3];

#endif
