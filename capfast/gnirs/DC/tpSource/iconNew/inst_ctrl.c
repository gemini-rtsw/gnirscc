static char rcid[]="$Id: inst_ctrl.c,v 1.2 2009/05/27 19:33:25 fkraemer Exp $";
#define INST_CTRL 1
/* Command and control process for INSTRUMENT CONTROL. */
/******************************************************************************
 * Program:	INSTRUMENT CONTROL software
 * File:        inst_ctrl.c     
 * Purpose:     Command and control center for all processes running on the
 *              INSTRUMENT CONTROL transputer. Manages links upstream towards
 *              B016 and SUN, and downstream towards sequencer(s). Interfaces
 *              directly with some a/d and d/a devices, and delegates others
 *              to HK_DATA_CAPT process. Readings are passed upstream on
 *              demand. Signals handles motor and device control.  
 *
 * Author:      Nick C Buchholz - revised from Dick Fredericksen's sqidctrl.c
 * Copyright:   Aura Inc.  All rights reserved.
 * History:
 *	22-Apr-1991 - created file - dhf
 *      06-Nov-1991 - revised orginal file - ncb
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
#include "prototypes.h"

static int ME = INST_CTRL;     /* self identifier for debugging messages */

#include "config_st.h"	       /* description of configuration information */
#include <instdefs.h>          /* definitions specific to INST */
#define NOINSTVARS
#include <instvars.h>          /* global variables specific to INST */ 
#define NOINSTPROCS              /* already defined in sqidmain */
#include <instprocs.h>	       /* process descriptors & channel assignments*/


/******************************************************************************
 * Routine:	init_inst()
 * Purpose: 	Get configuration information from SUN the initializes 
 *		and configures the instrument controller
 * Parameters: 	none
 * Returns:	ERROR or OK
 *****************************************************************************/
int init_inst()
{
    int header,				/* var for storing header info */
        i,j,				/* all-purpose counters */
        which_in,			/* index of ready input channel */
        msg_pend = FALSE,		/* flag to indicate extra msg */
        dbg_kt;				/* debugging counter */
    char dbug_buf[161];

    status_report = 2;
    debug |= (1 << INST_CTRL);

    /* Initialize handling of a/d and d/a: */
    /* fill a2d array initially with recognizable content (for debugging) */
    for (i = 0; i < HK_CHANS; i++)
	A2D_array[i] = 10 * i;

#if defined(SQIID)
    if (*poweroff != 0xdeadbeef) /*  */
    {
	*poweroff = 0xdeadbeef;
	mod_scb_reg(0, 0xffffffff);
    }
    else
    {
	SCBreg_val = *scb_reg;
    }
#else
	mod_scb_reg(0, (int)0xffffffff);
#endif
 

    /* similarly fill the floating point buffers with recognizable content */
    for (j = 0; j < HK_BUFS; j++)
	for (i = 0; i < HK_CHANS; i++)
	    hk_buf[j][i] = ((float) j) + ((float) .002) * ((float) i);
    
    dbg_kt = 0;

    ChanOutInt(Cntrl_to_HK_A2D, 1);      /* launch periodic a/d reading */
    status_report = 3;

}

/******************************************************************************
 * Program: INST_controller
 * Routine: instcmndcntrl()
 * Purpose: initialize instrument configuration from Sun then, take commands
 *	    from user and/or related programs on SUN or B016, and carry them 
 *	    out or arrange for sequencers to do so.
 * Inputs:  no explicit arguments (but channels and processes have been
 *          initialized before "main" sends this process its go-ahead
 *          signal, and the variables referenced as TOP_SEQ and BOTM_SEQ,
 *          if they are to differ from their defaults, must be the
 *          object of SET_VAR signals from the SUN before the latter 
 *          sends signals of any other sort).
 *****************************************************************************/
int INSTcmndctrl()
{
    int request,                  /* dummy to receive go-ahead signal */
        header,			  /* var for storing header info */
        i,j,                    /* all-purpose counters */
        which_in,                 /* index of ready input channel */
        dbg_kt;                   /* debugging counter */
    char dbug_buf[161];

    init_inst();

    control();		/* start command handler loop */

}



















