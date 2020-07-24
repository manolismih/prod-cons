#ifndef TIMER_H
#define TIMER_H

#include "queue.h"
#include <pthread.h>
#include <sys/time.h>
#include <stdio.h>
FILE* producerTimeLog;
typedef struct Queue Queue;

struct Timer{
	int msPeriod, nTasksToExecute, msStartDelay;
	Queue* queue;
	pthread_t pro;
	FILE* driftLog;
	struct timeval prevExecution;
	void* userData;
	void* (*timerFcn)(void*);
};
typedef struct Timer Timer;

Timer* timerInit(int msPeriod, Queue* queue);
void timerDestruct(Timer* timer);
void timerStart(Timer* timer);
void timerStartAt(Timer* timer, int y, int m, int d, int h, int min, int sec);
void* producer(void* timer);

#endif
