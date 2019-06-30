// Kevin Zink
// Kzink
// Queue struct header

#include<stdio.h>

typedef struct queue_st
{
    int * arr;
    int back;
    int size;
} QUEUE ;

void init ( QUEUE * s );
void enqueue( QUEUE * , const int );
int dequeue ( QUEUE * );
int full( QUEUE * );
int empty( QUEUE * );
int length( QUEUE * );
