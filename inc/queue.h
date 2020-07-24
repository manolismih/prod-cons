#ifndef QUEUE_H
#define QUEUE_H

#include "timer.h"
#include <pthread.h>
#include <sys/time.h>
#define QUEUESIZE 1000

typedef struct Timer Timer;
struct Queue{
	Timer* buf[QUEUESIZE];
	long head, tail;
	int nContents, nProducers;
	pthread_mutex_t *mut;
	pthread_cond_t *notFull, *notEmpty;
};
typedef struct Queue Queue;

Queue *queueInit ();
void queueDelete(Queue* q);
void queueAdd 	(Queue* q, Timer* in);
Timer* queueDel (Queue* q);

#endif
