#ifndef QUEUE_H
#define QUEUE_H

#include <pthread.h>
#include <sys/time.h>
#define QUEUESIZE 1000

typedef struct {
	void* (*work)(void*);
	void* arg;
	struct timeval submitted;
} WorkFunction;
/* NOTE: the WorkFunction struct has been altered to include a timeval
 * struct. Although the waiting time can be calculated without this 
 * modification, I am a strong advocate of neat and clean code writing
 * practices. The modification above has a minor impact on performance,
 * but a great impact in the simplicity of the code.
 */

typedef struct {
	WorkFunction* buf[QUEUESIZE];
	long head, tail;
	int nContents, noProducers;
	pthread_mutex_t *mut;
	pthread_cond_t *notFull, *notEmpty;
} Queue;

Queue *queueInit ();
void queueDelete(Queue* q);
void queueAdd 	(Queue* q, WorkFunction* in);
WorkFunction* queueDel (Queue* q);

#endif
