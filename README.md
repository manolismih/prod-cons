# Timer implementation using C and pthreads, targeted on a Rasberry Pi Zero.
_Programming assignment for the Real Time Embedded Systems course, school of Electrical and Computer Engineering, AUTH._  
_July of 2020_

Earlier version 1.0 of this project is enhanced with a timer implementation. A timer is an object that periodically inserts jobs in a fifo pending jobs queue. The timer consists of a producer function that runs in a seperate thread.
```C
struct Timer{
	int msPeriod, nTasksToExecute, msStartDelay;
	Queue* queue;
	pthread_t pro;
	FILE* driftLog;
	struct timeval prevExecution;
	void* userData;
	void* (*timerFcn)(void*);
};

```
Multiple timers insert jobs in the queue, while a consumer thread extracts and executes them. The goal is to experiment with a single or multiple timers running simultaneously, with different periods and measure the time needed from the producer to insert each job in the queue, for the consumer to execute it and also the drifting between two consecutive executions of the functions, as they are supposed to be periodical, but delays vary according to the load circumstances the queue faces.
## How to execute

First clone the repository. To run locally in a computer, use make:
```sh
cd prod-cons
make
./prod-cons <msTimer1Period> <msTimer2Period> ...
```

To run on a Rasberry Pi Zero, use 
```
cd prod-cons
make rasberry
scp prod-cons root@10.0.0.1:
ssh 10.0.0.1 -lroot
./prod-cons <msTimer1Period> <msTimer2Period> ...
```
provided you have successfully connected the rasberry. You can use htop to monitor the processes and threads, scp the results back to the computer and analyze them with matlab, octave, or use the analyze.m script.

A full report is available in greek [here](report.pdf).
