static char rcid[]="$Id: queue.c,v 1.2 2009/05/27 19:33:23 fkraemer Exp $";
/******************************************************************************
 * Program:	common
 * File:	queue.c
 * Purpose:	provide queue routines.
 * Author:	David Koski
 * Copyright:   Aura Inc.  All rights reserved.
 * History:
 *		26Dec91	written						dak
 *
 ******************************************************************************/

#include <stdlib.h>
#include "common.h"

/******************************************************************************
 * Routine: q_alloc
 * Purpose: allocate and intialize a queue
 * Inputs:  none
 * Returns: pointer to queue
 * 
 ******************************************************************************/

queue *q_alloc()
{
	queue *q;
	q = (queue *) malloc(sizeof(queue));
	q->h = q->t = 0;
	return q;
}

/******************************************************************************
 * Routine: enqueue
 * Purpose: add an item to the tail of a queue
 * Inputs:  queue *q -- queue
 *	    int e -- element to add
 * Returns: none
 * 
 ******************************************************************************/

void enq(queue *q, int e)
{
	q->q[q->t] = e;
	q->t = (q->t + 1) % QUEUE_SIZE;
}

/******************************************************************************
 * Routine: deq
 * Purpose: remove an element from the head of the queue
 * Inputs:  queue *q -- the queue
 * Returns: int -- the element from the head
 * 
 ******************************************************************************/

int deq(queue *q)
{
	int v;

	v = q->q[q->h];
	q->h = (q->h + 1) % QUEUE_SIZE;
	return v;
}

/******************************************************************************
 * Routine: emptyq
 * Purpose: check if the queue is empty
 * Inputs:  queue *q -- the queue
 * Returns: int -- true (empty) or false (not)
 * 
 ******************************************************************************/

int emptyq(queue *q)
{
	return (q->h == q->t);
}

int fullq(queue *q)
{
    return (((q->t + 1) % QUEUE_SIZE) == q->h);
}
