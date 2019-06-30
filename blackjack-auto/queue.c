// Kevin Zink
// Kzink
// Queue struct compilation file

#include<stdlib.h>
#include"queue.h"

#define DECK_SIZE 52

// This function increases the array size as necessary.
static void grow_arr ( QUEUE * q )
{
    q->size += 10;
    q->arr = ( int * ) realloc( q->arr, q->size * sizeof( int ) );
}

// "constructor" for the queue struct.
void init ( QUEUE * q )
{
    q->size = 10;
    q->arr = ( int * ) malloc( q->size * sizeof( int ) );
    q->back = 0;
}

// inserts an element to the back of the queue.
void enqueue( QUEUE * q , const int foo )
{
    if ( q->back >= q->size )
        grow_arr( q );
    q->arr[ (q->back)++ ] = foo;
}

// removes the first element from the queue.
int dequeue( QUEUE * q )
{
    int i; //loop counter
    int rval = 0;

    if ( empty( q ) )
    {
        printf( "queue is empty!" );
    }
    else
    {
        rval = q->arr[0];
        for (i = 1; i < q->back; i++)  //loop that moves every element=
            q->arr[i-1] = q->arr[i];   //to the next "slot" in the array
        q->back--;
    }
    return rval;
}

// returns the length of the queue.
int length ( QUEUE * q)
{
    return (q->back);
}

// returns whether the queue is full or not.
int full ( QUEUE * q )
{
    return ( q->back >= DECK_SIZE );
}

// returns whether the queue is empty or not.
int empty ( QUEUE * q )
{
    return ( q->back <= 0 );
}
