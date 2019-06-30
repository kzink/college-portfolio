# Kevin Zink
# kzink
# makefile for Bjack card program

CC=gcc
CFLAGS=-Wall -g 
LDFLAGS=-Wall -g

war: queue.o cards.o
	gcc -Wall queue.o cards.o -o bjack
queue.o: queue.c queue.h
	gcc -Wall -c queue.c
cards.o: cards.c queue.h
	gcc -Wall -c cards.c
clean:
	-rm -f *.o bjack
