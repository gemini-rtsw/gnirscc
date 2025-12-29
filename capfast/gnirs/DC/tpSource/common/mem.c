static char rcid[]="$Id: mem.c,v 1.2 2009/05/27 19:33:22 fkraemer Exp $";
/******************************************************************************
 * Program:	common
 * File:	mem.c
 * Purpose:	high speed memory allocater/deallocater.  it uses a 'map'
 *		to determine whether a block is free or not.
 *		(this probably is not as fast as I originally thought,
 *		 maybe it should be changed.  oh well, it does work)
 * Author:	David Koski
 * Copyright:   Aura Inc.  All rights reserved.
 * History:
 *		26Dec91	written						dak
 *
 ******************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "common.h"
#include "shared.h"

#define USE_MALLOC0

Semaphore mem_sem = SEMAPHOREINIT;

#ifdef USE_MALLOC
extern void *_heapend, *_heapstart;
#endif

#if 1
/******************************************************************************
 * Routine: mem_print
 * Purpose: print out the state of the heap.  F = free, U = in use, E = end
 * Inputs:  none
 * Returns: none
 * 
 ******************************************************************************/

void mem_print()
{
    char buf[80];
    int i;

    sprintf(buf, "** node %d: ", _node_number);
    for (i = 0; i < (mem_size / MEM_BLOCK_SIZE); i++) {
	switch(mem_use[i]) {
	    case MEM_FREE:
	        strcat(buf, "F");
		break;
	    case MEM_IN_USE:
	        strcat(buf, "U");
		break;
	    case MEM_END:
	        strcat(buf, "E");
		break;
	    default:
	        strcat(buf, "?");
		break;
	}
	if (i % 40 == 39) {
	    send_debug(buf);
	    sprintf(buf, "** node %d: ", _node_number);
	}
    }
    send_debug(buf);
}

#endif


/******************************************************************************
 * Routine: mem_init
 * Purpose: initialize memory to all free state
 * Inputs:  none
 * Returns: none
 * 
 ******************************************************************************/

void mem_init()
{
#ifdef USE_MALLOC
#else
	int i;

	for (i = 0; i < mem_blocks - 1; i++)
		mem_use[i] = MEM_FREE;
	mem_use[i] = MEM_IN_USE;		/* trailing IN_USe to prevent
						   overrun */
#endif
}


/******************************************************************************
 * Routine: mem_alloc
 * Purpose: allocate a block of the given size
 * Inputs:  int size -- size in bytes
 * Returns: void * -- pointer to the block
 * 
 ******************************************************************************/

void *mem_alloc(int size)
{
#ifdef USE_MALLOC
	void *p;

	p = malloc(size);
	return p;
#else
	int i, j, ptr;
	int tsize;
	int csize;

	if (size == 0)
		return NULL;

	/* convert size to a round multiple of MEM_BLOCK_SIZE */
	size += MEM_BLOCK_SIZE - 1;
	size >>= MEM_BLOCK_BITS;
	/* size is now the number of blocks to allocate */

	/* find a free block of that size using a fist fit algorithm */
	/* this is not semaphore protected, hopefully no problems will occur */
	ptr = -1;
	tsize = mem_blocks - size + 1;
	HSemP(mem_sem);
	for (i = 0; i < tsize; )
	{
		if (mem_use[i] == MEM_FREE)
		/* the current block is free */
		{
			if (mem_use[i + size - 1] == MEM_FREE)
			/* the end block is free, check in middle */
			{
				csize = i + size - 1;
				ptr = i;
			 	for (j = i + 1; j < csize; j++)
					if (mem_use[j] != MEM_FREE) {
					    ptr = -1;
					    break;
					}
				if (ptr != -1) 
					break;
				else 
					i = j + 1;
			} else
				/* skip over this block */
				i += size + 1;
		} else {
			if (mem_use[i + size] != MEM_FREE)
				/* there is no block in between big enough */
				i += size + 1;
			else
				i ++;
		}
	}

	if (ptr == -1) {
		HSemV(mem_sem);
		return (void *) 0;	/* no more memory */
	}

	/* now allocate the memory */
	/* csize points to the end of the memory that we need */
	size --;	/* we really want one block less (for an end mark) */
	for (i = ptr; i < ptr + size; )
		mem_use[i++] = MEM_IN_USE;
	mem_use[i] = MEM_END;
	HSemV(mem_sem);

	/* now return the address */
	return (void *) ((int)memory + MEM_BLOCK_SIZE * ptr);
#endif
}


/******************************************************************************
 * Routine: mem_free
 * Purpose: free a block of memory
 * Inputs:  void *ptr -- pointer to block
 * Returns: none
 * 
 ******************************************************************************/

void mem_free(void *ptr)
{
#ifdef USE_MALLOC
#if 0
	if (ptr < (void *) _heapstart || ptr > (void *) _heapend)
		return;				/* it was not allocated */
#endif
	if (ptr == NULL)
		return;

	free(ptr);
#else
	int i;

	if (ptr < (void *) memory || ptr > (void *) (memory + mem_size))
		return;				/* it was not allocated */
	
	/* convert ptr to index into mem_use */
	i = ((int) ((char *) ptr - memory)) >> MEM_BLOCK_BITS;

	HSemP(mem_sem);
	while (mem_use[i] == MEM_IN_USE)
		mem_use[i++] = MEM_FREE;

	/* now clear end marker */
	mem_use[i] = MEM_FREE;
	HSemV(mem_sem);
#endif
}
