/******************************************************************************
 * Program:  Inst CONTROL Processes
 * Purpose:  Handle tasks reserved for SQIID CONTROL
 * File:     instprocs.h
 * Author:   Dick Fredericksen
 * History:  
 *	22-Apr-1991 - created file - dhf
 *      10-May-1991 - modified process names -dhf
 *
 *****************************************************************************/

#ifndef NOINSTPROCS
#define NOINSTPROCS

Process *Reader, *Writer;	/* processes for communications & routing */
Process *HK_A2D;       	        /* process to get a/d housekeeping data */
Process *HK_XM;                 /* process to transmit a/d housekeeping data */
Process *ARRAY_Cntrl;		/* process to control array related stuff */
Process *DEV_Cntrl;		/* process to control Device related stuff */

void Hka2d(void);	        /* housekeeping a/d process */

#else


#endif


