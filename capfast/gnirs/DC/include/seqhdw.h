/******************************************************************************
 * File:	seqhdw.h
 * Purpose:	provides constants and macros needed for the sqiid hardware
 * Author:	David Koski
 * Copyright:   Aura Inc.  All rights reserved.
 * History:
 *		16Jul91	created						dak
 *		25Jul91	working						dak
 *		16Jul92 64 bit acb support added			dak
 *
 ******************************************************************************/

#include "common.h"

#define FIFO_RESET	(*((int *) 0x90000000))
#define FIFO_WRITE	(*((int *) 0xa0000000))
#define FIFO_LOAD	(*((int *) 0xb0000000))
#define FIFO_EXEC	(*((int *) 0xc0000000))
#define FIFO_READ	(*((int *) 0xe0000000))

static int *fifo_read = (int *) 0xe0000000;
static int *fifo_write = (int *) 0xa0000000;

#define WAIT_LENGTH	10			/* uSec pause to unload fifo */
#define LOW		250			/* low water mark */
#define HIGH		274			/* high water mark (delta) */

#define FIFO_EMPTY		(!(*fifo_read & 16))
#define ALMOST_EMPTY		(!(*fifo_read & 8))
#define ALMOST_FULL		(!(*fifo_read & 2))

#define reset_fifo(initial_state)	{				\
		FIFO_EXEC = 0;			/* execute off */	\
		FIFO_RESET = 0;			/* reset fifo */	\
						/* set initial state */	\
		FIFO_LOAD = LOW;		/* low water, 250 */	\
		FIFO_LOAD = HIGH;		/* high water, 750 */	\
		FIFO_LOAD = 0;			/* scratch register */  \
		}


#define start_fifo()			{				\
		FIFO_EXEC = 1;			 /* execute on */ }

#define fast_write(value)\
		*fifo_write = (value)

#define fast_write_wave(wave_name, wave_len) \
		*fifo_write = (wave_len << 16) | ((wave_name) >> 3)

#if 1
#define write_wave(wave_name, wave_len)	{				\
		while (ALMOST_FULL)					\
		    ProcWait(WAIT_LENGTH); 				\
		*fifo_write = (wave_len << 16) | ((wave_name) >> 3); }
#else
#define write_wave(wave_name, wave_len)	{				\
		if (ALMOST_FULL)					\
			while (!ALMOST_EMPTY)				\
				;					\
		*fifo_write = (wave_len << 16) | ((wave_name) >> 3); }
#endif

#define TITLE(x)

/* bit defines for control register for sequence programs */
#define IDLE_FLAG	1		/* put sequencer into idle mode */
#define DIE		2		/* kill sequencer */
#define Read_Flag	4		/* read array flag */
#define SIM_FLAG	8		/* new simulation flag */
#define SDT_MODE	256		/* continuous sdt mode */
#define ABORT_INT	512		/* abort integration */

					/* global vars in static locations */
/*	 cntrl_reg		INT(1)   -- defined in common.h */
#define node			gINT(8)
#define Int_Time_Seconds	gINT(9)
#define Int_Time_MilliSecs	gINT(10)
#define FInt_Time_Seconds	gINT(11)
#define FInt_Time_MilliSecs	gINT(12)
#define Spad_Filter		gINT(13)
#define Seq_to_Reader		gINT(14)
#define Control_to_Seq		gINT(15)

#define LastHdwVar		16 /* same as last global variable number */

#define suicide()	{ ChanOutInt(&PROC_to_Control, 0); ProcStop(); }





