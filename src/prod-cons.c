#include "queue.h"
#include "functions.h"
#include "timer.h"
#include <sys/time.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

FILE* consumerTimeLog;

void* consumer (void *q)
{
	Queue* fifo = q;
	void* result = NULL;
	while (1)
	{
		pthread_mutex_lock (fifo->mut);
		while (fifo->nContents==0 && fifo->nProducers>0) 
		{
			//printf ("consumer: Queue EMPTY.\n");
			pthread_cond_wait (fifo->notEmpty, fifo->mut);
		}
		if (fifo->nContents==0 && fifo->nProducers==0)
		{
			pthread_mutex_unlock(fifo->mut);
			return NULL;
		}
		
		//calculate drift and consumer time
		struct timeval received, finished;
		gettimeofday(&received,NULL);

		//extract from queue
		Timer* out = queueDel(fifo);		
		pthread_mutex_unlock(fifo->mut);
		pthread_cond_signal (fifo->notFull);
		result = out->timerFcn(out->userData);
		
		gettimeofday(&finished,NULL);
		fprintf(consumerTimeLog,"%lld\n",(finished.tv_sec-received.tv_sec)*1000000ll +finished.tv_usec-received.tv_usec);
		fprintf(out->driftLog,"%lld\n",-1000*out->msPeriod +(received.tv_sec-out->prevExecution.tv_sec)*1000000ll +received.tv_usec-out->prevExecution.tv_usec);

		out->prevExecution = received;
		
		free(result);
	}
	return NULL;
}

int main(int argc, char* argv[])
{
	if (argc < 2) 
	{
		printf("Usage: ./prod-cons <msTimer1Period> <msTimer2Period> ...\nAbort\n");
		return -1;
	}
	
	srand(time(NULL));
	Queue* fifo = queueInit();
	producerTimeLog = fopen("producerTimeLog.txt","w");
	consumerTimeLog = fopen("consumerTimeLog.txt","w");
	fprintf(producerTimeLog,"Producer lag (us)\n");
	fprintf(consumerTimeLog,"Consumer time (us)\n");
	
	int nTimers = argc-1;
	fifo->nProducers = nTimers;
	pthread_t con;
	Timer* tim[nTimers];
	for (int i=0; i<nTimers; i++)
	{
		tim[i] = timerInit(atoi(argv[1+i]),fifo);
		timerStart(tim[i]);
	}
	pthread_create (&con, NULL, consumer, fifo);
	
	for (int i=0; i<nTimers; i++) pthread_join (tim[i]->pro, NULL);
	fifo->nProducers = 0;
	// unblock all consumers, so that they detect that all producers are terminated
	pthread_cond_broadcast(fifo->notEmpty); 
	pthread_join (con, NULL);
	
	queueDelete (fifo);
	for (int i=0; i<nTimers; i++) timerDestruct(tim[i]);
	return 0;
}
