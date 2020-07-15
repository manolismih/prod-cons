#include "timer.h"
#include <stdlib.h>

Timer* timerInit(int msPeriod, Queue* queue)
{
	Timer* ret = malloc(sizeof(Timer));
	ret->msPeriod = msPeriod;
	ret->queue = queue;
	ret->nTasksToExecute = 60*60*1000/msPeriod; //run for 1 hour
	ret->msStartDelay = 0;
	ret->timerFcn = mult_10x10_matrix;
	ret->userData = mult_10x10_matrixArgsGen();
	return ret;
}

void timerStart(Timer* timer)
{
	usleep(1000*timer->msStartDelay);
	pthread_create (timer->pro, NULL, producer, timer);
}

void timerStartAt(Timer* timer, int y, int m, int d, int h, int min, int sec)
{
}

void* producer(void* timer)
{
	while (timer->nTasksToExecute--)
	{
		WorkFunction *new = malloc(sizeof(WorkFunction));
		new->work = timer.timerFcn;
		new->arg  = timer.userData;
		
		pthread_mutex_lock (fifo->mut);
		while (fifo->nContents == QUEUESIZE) 
		{
			//~ printf ("producer: Queue FULL.\n");
			pthread_cond_wait (fifo->notFull, fifo->mut);
		}
		queueAdd (fifo, new);
		pthread_mutex_unlock (fifo->mut);
		pthread_cond_signal (fifo->notEmpty);
	}
	return NULL;
}
