#ifndef TIMER_H
#define TIMER_H

typedef struct {
	int msPeriod, nTasksToExecute, msStartDelay;
	Queue* queue;
	pthread_t pro;
	void* userData;
	void* (*timerFcn)(void*);
} Timer;

Timer* timerInit(int msPeriod);
void timerStart(Timer* timer);
void timerStartAt(Timer* timer, int y, int m, int d, int h, int min, int sec);
void* producer(void* timer);

#endif
