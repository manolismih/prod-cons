CFLAGS = -Wall -g -O3 -pthread

########################################################################

target:
	gcc $(CFLAGS) prod-cons.c queue.c functions.c -o prod-cons -lm
