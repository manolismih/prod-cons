CFLAGS = -Wall -g -O3 -pthread
INCLUDE = ../inc

########################################################################

pc:
	cd src; gcc $(CFLAGS) -I $(INCLUDE) prod-cons.c queue.c functions.c timer.c -o ../prod-cons -lm

rasberry:
	cd src; arm-linux-gnueabihf-gcc $(CFLAGS) -I $(INCLUDE) prod-cons.c queue.c functions.c timer.c -o ../prod-cons -lm
