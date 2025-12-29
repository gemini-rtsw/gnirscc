static char rcid[]="$Id: proc.c,v 1.2 2009/05/27 19:33:22 fkraemer Exp $";
/******************************************************************************
 * Program:	seq
 * File:	proc.c
 * Purpose:     to start and stop processes on the sequencer
 * Author:	David Koski
 * Copyright:   Aura Inc.  All rights reserved.
 * History:
 *		5-17-91	created						dak
 *		5-20-91	controller stuff added				dak
 *		6-27-91	dynamic process loading complete		dak
 *
 ******************************************************************************/

#include <conc.h>

#include "common.h"

/******************************************************************************
 * Routine: kill_proc
 * Purpose: stops a process on the sequencer
 * Inputs:  none
 * Returns: none
 * 
 ******************************************************************************/

int prog[PROG_LEN] = { 0xf0220000 };	/* start off with a return */

int running = 0;			/* is the prog running? */
Process *PROC;

kill_proc(int header, int *buf)
{
	if (running)
	{
	    cntrl_reg = DIE;
            send_debug("** Killing procedure");
	    ChanInInt(& PROC_to_Control);
            send_debug("** Procedure Killed");
	    ProcWait(1500);
	    ProcFree(PROC);
	    running = 0;
	}
	/* tell the top level we have stopped. This is the proceed message 
	 * for the top level  
	 */
            send_debug("** Procedure Killed");
	send_debug("Stopped.");

}
/******************************************************************************
 * Routine: execute_proc
 * Purpose: starts a process on the sequencer
 * Inputs:  none
 * Returns: none
 * 
 ******************************************************************************/
execute_proc(int header, int *buf)
{

	PROC_to_Control = NOPROCESS;
        cntrl_reg = 0;
	SEND_DEBUG = send_debug;
	PROC = ProcAlloc((void (*)()) prog,P_STACK,0);
	ProcRunHigh(PROC);
	/* tell the top level we have started. This is the continue message 
	 * for the top level code 
	 */
	send_debug("running");
	running = 1;
}
