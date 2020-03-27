#include "queue.h"
#include "functions.h"
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

#define LOOP 2000
FILE* waitLog;
FILE* queueLog;

void* producer (void* q)
{
	Queue* fifo = q;
	for (int i=0; i<LOOP; i++) 
	{
		WorkFunction *new = malloc(sizeof(WorkFunction));
		int num = rand()%3;
		new->work = workFunctions[num].pointer;
		new->arg  = workFunctions[num].argsGen();
		
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
	if (argc != 3) 
	{
		printf("Usage: ./prod-cons <nProducers> <nConsumers>\nAbort\n");
		return -1;
	}
	
	srand(time(NULL));
	Queue *fifo;
	fifo = queueInit ();
	if (fifo ==  NULL) {
		fprintf (stderr, "main: Queue Init failed.\n");
		exit (1);
	}
	waitLog 	= fopen("waitLog","w");
	queueLog 	= fopen("queueLog","w");
	
	fifo->noProducers = 0;
	int nProducers = atoi(argv[1]);
	int nConsumers = atoi(argv[2]);
	pthread_t pro[nProducers], con[nConsumers];
	for (int i=0; i<nProducers; i++) pthread_create (pro+i, NULL, producer, fifo);
	for (int i=0; i<nConsumers; i++) pthread_create (con+i, NULL, consumer, fifo);
	for (int i=0; i<nProducers; i++) pthread_join (pro[i], NULL);
	fifo->noProducers = 1;
	// unblock all consumers, so that they detect that all producers are terminated
	pthread_cond_broadcast(fifo->notEmpty); 
	for (int i=0; i<nConsumers; i++) pthread_join (con[i], NULL);
	
	queueDelete (fifo);

	return 0;
}
