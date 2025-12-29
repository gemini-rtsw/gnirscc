static char rcid[]="$Id: cmnd_funcs.c,v 1.2 2009/05/27 19:33:24 fkraemer Exp $";
/******************************************************************************
 * Program:	INSTRUMENT CONTROL software
 * File:        cmnd_funcs.c     
 * Purpose:     Command and control functions used by the INSTRUMENT CONTROL
 *		transputer. 
 *
 * Author:      Nick C Buchholz 
 * Copyright:   Aura Inc.  All rights reserved.
 * History:
 *	10-Dec-1991 - created file - ncb
 *
 *****************************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <conc.h>
#include <math.h>

#include "irstd.h"
#include <protdefs.h>
#include <protocol.h>
#include <common.h>

static int ME = INST_CNTRL;     /* self identifier for debugging messages */

#include "config_st.h"	       /* description of configuration information */
#include <instdefs.h>          /* definitions specific to B016 */
#define NOINSTVARS
#include <instvars.h>          /* global variables specific to B016 */ 
#define NOINSTPROCS              /* already defined in sqidmain */
#include <instprocs.h>	       /* process descriptors & channel assignments*/

/******************************************************************************
 * Program: inst_cntl
 * Routine: check_hk()
 * Purpose: check whether housekeeping variables with hardware-determined
 *          values are within tolerance.
 * Inputs:  none
 * Returns: nothing
 * NOTES:   1) Only certain voltage variables are checked at present.
 *          2) If periodic reading of a/d hardware is currently squelched,
 *             this request will cause it to resume. See also READ_HK.
 *****************************************************************************/
int check_hk()
{
    float * cur_hk_buf;   /* pointer to current hk_buf */
    float gotv;           /* observed value of variable */
    float gotv_err;       /* observed departure from standard value */
    int errkt = 0;        /* count of errors */
    int i, j;           /* work indices */
    char rbuf[800];       /* area for reply and debugging messages */
    char *base_name;      /* pointer to base of variable name (may be
			     extended by a color designator) */

    if (a2d_freeze)			/* is reading of hk a2d's squelched? */
    {					/* yes: */
	a2d_freeze = 0;			/* remove the gag */
    }
    
    SemV(idxlock);
    cur_hk_buf = hk_buf[hkbufix];
    rbuf[0] = 0x00;			/* initialize empty reply buffer */
    for (j = 0; j < NUM_ARRAYS; j++)
    {
	for (i = 0; i < NUM_RAILS; i++)
	{
	    gotv = cur_hk_buf[inst.array[j].rail_chan[i]];
	    gotv_err = fabsf(gotv - inst.array[j].rails[i]);
	    if (gotv_err > tolerance)
	    {
		errkt++;
		send_debug("ERROR %s now = %6.3f, out of tolerance by = %6.3f",
		  inst.array[j].rail_names[i], (double) gotv, (double) gotv_err);
	    }
	}             /* bottom of loop on rails */
    }              /* bottom of loop on arrays */
    
    for (j = 0; j < NUM_ARRAYS; j++)
    {
	for (i = 0; i < NUM_D2A; i++)
	{
	    gotv = cur_hk_buf[inst.array[j].voltage[i].d2a_chan];
	    gotv_err = fabsf(gotv - inst.array[j].voltage[i].norm);
	    if (gotv_err > tolerance)
	    {
		errkt++;
		send_debug("ERROR %s now = %6.3f, out of tolerance by = %6.3f",
		  inst.array[j].D2A_names[i], (double) gotv, (double) gotv_err);
	    }
	}             /* bottom of loop on D2A'S */
    }              /* bottom of loop on arrays */

    SemP(idxlock);
    
    if (errkt)
    {
	send_debug("ERROR %d VOLTAGES OUT OF SPEC ***\n",	errkt);
    }
    else
	send_debug("All hardware-determined voltages are within tolerance.\n");

    return (OK);
}              /* End of check_hk() */


/**************************************************************************
 * Program: inst_cntl
 * Routine: start_picture
 * Purpose: send messages to start picture taking.
 * Inputs:  int - header - header of message received
 *	    int * - buf - rest of the message rceived
 * Returns: err = true if an error occurred
 *************************************************************************/
int start_picture(header, buf)
int header;
int *buf;
{
    int i, j;
    
    a2d_freeze = TRUE;      /* squelch periodic reading of a/d hardware
				   while picture-taking is in progress */
    ProcWait(200);
    /* send start_msg to all sequencers send to chain bottom up*/

/*    for(j=(seq_chain_len[i] -1); j >= 0; j--)
	for (i = 0; i < nchains; i++)
	    send(MESSAGE(START_MSG) | OF_LENGTH(0) | 
		 TO_NODE(seq_node_num[i][j]), buf);
*/    
    ChanOutInt(Cntrl_to_Pic_Ctrl, 10);
/*    send_debug("Instrument forwarded START msg to do_pics");*/
    return(OK);
}


/**************************************************************************
 * Program: inst_cntl
 * Routine: abort_img
 * Purpose: halts readout of instrument data.  Actually just terminates
 *		current integration time and reads out the array and then
 *		tosses the result. 
 * Inputs:  int - header - header of message received
 *	    int * - buf - rest of the message rceived
 * Returns: err = true if an error occurred
 *************************************************************************/
int abort_img(header, buf)
int header;
int *buf;
{
    int i, j;

    /* send abort message to sequencers then restart housekeeping readout */
    /* send start_msg to all sequencers send to chain bottom up*/

/*    for(j=(seq_chain_len[i] -1); j >= 0; j--)
	for (i = 0; i < nchains; i++)
	    send(MESSAGE(ABORT_MSG) | OF_LENGTH(0) | 
		 TO_NODE(seq_node_num[i][j]),buf);
*/

    a2d_freeze = FALSE;			/* resume reading hk a2d's */
    return (OK);
}




