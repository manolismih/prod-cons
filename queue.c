#include "queue.h"
#include <stdlib.h>
#include <pthread.h>
#include <stdio.h>

extern FILE* queueLog;

Queue* queueInit (void)
{
	Queue* q = malloc (sizeof (Queue));
	if (q ==  NULL) {
		fprintf (stderr, "Queue Init failed.\n");
		exit (1);
	}

	q->nProducers = 0;
	q->nContents = 0;
	q->head = 0;
	q->tail = 0;
	q->mut 			= malloc (sizeof (pthread_mutex_t));
	q->notFull 		= malloc (sizeof (pthread_cond_t));
	q->notEmpty 	= malloc (sizeof (pthread_cond_t));
	pthread_mutex_init 	(q->mut, NULL);
	pthread_cond_init 	(q->notFull, NULL);
	pthread_cond_init 	(q->notEmpty, NULL);
	
	return q;
}

void queueDelete (Queue* q) //make sure Queue is empty first
{
	pthread_mutex_destroy (q->mut);
	pthread_cond_destroy (q->notFull);
	pthread_cond_destroy (q->notEmpty);
	free (q->mut);	
	free (q->notFull);
	free (q->notEmpty);
	free (q);
}

void queueAdd (Queue* q, WorkFunction* in)
{
	//~ printf("Trying to add to queue which has %d elements\n",q->nContents);
	//issue submit timestamp
	gettimeofday(&in->submitted,NULL);
	
	q->buf[q->tail] = in;
	q->tail++;
	if (q->tail == QUEUESIZE) q->tail = 0;
	q->nContents++;
	fprintf(queueLog,"%d\n",q->nContents);
}

WorkFunction* queueDel (Queue* q)
{
	//~ printf("Trying to remove from queue which has %d elements\n",q->nContents);
	WorkFunction *out = q->buf[q->head];
	q->head++;
	if (q->head == QUEUESIZE) q->head = 0;
	q->nContents--;
	fprintf(queueLog,"%d\n",q->nContents);
	return out;
}
