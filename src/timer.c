#include "timer.h"
#include "functions.h"
#include <unistd.h>
#include <sys/time.h>
#include <stdlib.h>
#include <stdio.h>

Timer* timerInit(int msPeriod, Queue* queue)
{
	Timer* ret = malloc(sizeof(Timer));
	ret->msPeriod = msPeriod;
	ret->queue = queue;
	ret->nTasksToExecute = 60*60*1000/msPeriod; //run for 1 hour
	ret->msStartDelay = 0;
	ret->timerFcn = mult_100x100_matrix;
	ret->userData = mult_100x100_matrixArgsGen();
	char fileName[50];
	sprintf(fileName,"drift%dmsLog.txt",msPeriod);
	ret->driftLog = fopen(fileName,"w");
	fprintf(ret->driftLog,"Drift time (us)\n");
	gettimeofday(&ret->prevExecution,NULL);
	return ret;
}

void timerDestruct(Timer* timer)
{
	fclose(timer->driftLog);
	free(timer->userData);
	free(timer);
}

void timerStart(Timer* timer)
{
	usleep(1000*timer->msStartDelay);
	pthread_create (&timer->pro, NULL, producer, timer);
}

void timerStartAt(Timer* timer, int y, int m, int d, int h, int min, int sec)
{
}

void* producer(void* tim)
{
	Timer* timer = tim;
	Queue* fifo = timer->queue;
	struct timeval initiated, submitted;
	while (timer->nTasksToExecute--)
	{
		gettimeofday(&initiated,NULL);
		
		pthread_mutex_lock (fifo->mut);
		while (fifo->nContents == QUEUESIZE) 
		{
			errorFcn();
			pthread_cond_wait (fifo->notFull, fifo->mut);
		}
		queueAdd (fifo, timer);
		pthread_mutex_unlock (fifo->mut);
		pthread_cond_signal (fifo->notEmpty);
		
		gettimeofday(&submitted,NULL);
		long long usLag = (submitted.tv_sec-initiated.tv_sec)*1000000ll + submitted.tv_usec-initiated.tv_usec;
		fprintf(producerTimeLog,"%lld\n",usLag);
		if (usLag < timer->msPeriod*1000) usleep(timer->msPeriod*1000-usLag);
		//~ usleep(timer->msPeriod*1000);
	}
	stopFcn();
	return NULL;
}
