# Kevin Zink
# kzink
# makefile for Bjack card program

CC=gcc
CFLAGS=-Wall -g 
LDFLAGS=-Wall -g

cjack: bjclient.c bjacks
	gcc -Wall bjclient.c -o bjackc
bjacks: queue.o cards.o
	gcc -Wall queue.o cards.o -o bjacks
queue.o: queue.c queue.h
	gcc -Wall -c queue.c
cards.o: cards.c queue.h
	gcc -Wall -c cards.c
clean:
	-rm -f *.o bjacks bjackc
