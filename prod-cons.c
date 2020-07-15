#include "queue.h"
#include "functions.h"
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

#define LOOP 2000
FILE* waitLog;
FILE* queueLog;

void* consumer (void *q)
{
	Queue* fifo = q;
	void* result = NULL;
	while (1)
	{
		pthread_mutex_lock (fifo->mut);
		while (fifo->nContents==0 && !fifo->noProducers) 
		{
			//~ printf ("consumer: Queue EMPTY.\n");
			pthread_cond_wait (fifo->notEmpty, fifo->mut);
		}
		if (fifo->nContents==0 && fifo->noProducers)
		{
			pthread_mutex_unlock(fifo->mut);
			return NULL;
		}

		//extract from queue
		WorkFunction* out = queueDel(fifo);
		
		//issue receive timestamp
		struct timeval received;
		gettimeofday(&received,NULL);
		
		pthread_mutex_unlock(fifo->mut);
		pthread_cond_signal (fifo->notFull);
		
		//calculate waiting time
		long long waitingTime = received.tv_usec -out->submitted.tv_usec;
		waitingTime += (received.tv_sec -out->submitted.tv_sec)*1000000;
		fprintf(waitLog,"%lld\n",waitingTime);
		
		//execute work function
		result = out->work(out->arg);
		
		//clean the WorkFunction
		free(out->arg);
		free(result);
		free(out);
	}
	return NULL;
}

int main(int argc, char* argv[])
{
	if (argc < 3) 
	{
		printf("Usage: ./prod-cons <nConsumers> <msTimer1Period> <msTimer2Period> ...\nAbort\n");
		return -1;
	}
	
	srand(time(NULL));
	Queue* fifo = queueInit();
	waitLog 	= fopen("waitLog","w");
	queueLog 	= fopen("queueLog","w");
	
	int nTimers = argc-2;
	int nConsumers = atoi(argv[1]);
	fifo->nProducers = nTimers;
	pthread_t con[nConsumers];
	Timer* tim[nTimers];
	for (int i=0; i<nTimers; i++)
	{
		tim[i] = timerInit(atoi(argv[2+i]),fifo);
		timerStart(tim[i]);
	}
	for (int i=0; i<nConsumers; i++) pthread_create (con+i, NULL, consumer, fifo);
	
	for (int i=0; i<nProducers; i++) pthread_join (pro[i], NULL);
	fifo->noProducers = 1;
	// unblock all consumers, so that they detect that all producers are terminated
	pthread_cond_broadcast(fifo->notEmpty); 
	for (int i=0; i<nConsumers; i++) pthread_join (con[i], NULL);
	
	queueDelete (fifo);

	return 0;
}
