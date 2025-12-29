/******************************************************************************
 * File:	common.h
 * Purpose:	provide declarations for all routines in the 'common'
 *		directory.
 * Author:	David Koski
 * Copyright:   Aura Inc.  All rights reserved.
 * History:
 *		23Dec91	written						dak
 *
 ******************************************************************************/

#ifdef _COMMON_H_
#else
#define _COMMON_H_

#include <conc.h>

#define MEM_BLOCK_SIZE	1024	/* minimum block to allocate */
#define MEM_BLOCK_BITS	10	/* bits in above value */

#define MEM_FREE	0	/* memory is free flag */
#define MEM_IN_USE	2	/* memory in use flag */
#define MEM_END		3	/* memory in use end flag */

				/* declare size of memory */
#define _MEMORY(x)	int mem_size = x;\
                        int mem_blocks = ((x) / MEM_BLOCK_SIZE);\
                        char mem_use[( (x) / MEM_BLOCK_SIZE)]

#define MEMORY(x)       char _memory[x];\
			char *memory = _memory;\
			_MEMORY(x);

				/* macro to send a message */
#define SEND(header, buf, channel) { \
			ChanOutInt((channel), (header)); \
			if (LENGTH(header) != 0) \
			    ChanOutInt((channel), (int) (buf)); }


#if 1
#define QUEUE_SIZE	128	/* length of a queue */
#else
#define QUEUE_SIZE	64	/* length of a queue */
#endif

#define PROG_LEN	12000		/* words of downloadable code */
#define DIE		2		/* signal for proc to die */
#define P_STACK		2048		/* bytes of stack for proc */

#define gBASE                    0x80000200
#define gINT(x)                  (*((int *) (gBASE + ((x)*4)) ))

#define cntrl_reg	gINT(2)		/* control register */
#define PROC_to_Control	(*((Channel *) &gINT(1)))
					/* command channel */
typedef void (*fPtr)(char *, ...);
#define SEND_DEBUG	(*((fPtr *) (gBASE)))
					/* pointer to send_debug */

typedef void (*command_form)(int header, int *buff);	/* function pointer */

typedef struct queue {		/* queue type */
	int h, t;
	int q[QUEUE_SIZE];
} queue;

#define halt() while (1) ProcWait(10000)
	

void *mem_alloc(int size);	/* allocate memory */
void mem_dealloc(void *ptr);	/* deallocate memory */
void control();			/* controller process */
extern void read_var(), set_var();
				/* routines to set and read variables */
queue *q_alloc(void);		/* alloc a queue (uses normal malloc) */
void enq(queue *q, int e);	/* add to tail of a queue */
int deq(queue *q);		/* remove from head */
int empty(queue *q);		/* test empty */
void send_debug(char *format, ...);
			/* send a debugging msg with printf format */
void mem_free(void *ptr);
int emptyq(queue *q);
void monitor(Channel *watch, Channel *error, int usec, int error_code);
int fullq(queue *q);
extern void reader(), writer();
int bad_msg(int header, int *buf), do_error(int header, int *buf), 
	send_akk(int *buf, int err);
extern int var(), kill_proc(), execute_proc();
				/* other processes and routines */
extern int prog[];		/* downloadable process array */

extern char *memory;		/* all of the memory in the "heap" */

extern int mem_size;		/* number of bytes in the "heap" */
extern int mem_blocks;		/* number of blocks in the "heap" */
extern char mem_use[];		/* array of free blocks */

extern command_form command[];	/* structure that will tell what to do
				   with each message */
extern int num_command;		/* maximum value of command that this node
				   supports */

extern Channel *chan_list[];	/* list of channels for reader to watch */

extern int *var_ptr[];		/* variable pointers */
extern command_form readv_cmnds[], setv_cmnds[];
				/* function pointers for read and write */
extern Channel DEBUG_CHAN;	/* debugging message channel */
extern Channel *out_chan[4];	/* 4 hardware (or software links) that the 
				   router uses */
extern Channel *hw_chan[4];	/* 4 hardware channels */
#endif
