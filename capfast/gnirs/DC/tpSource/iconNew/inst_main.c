static char rcid[]="$Id: inst_main.c,v 1.2 2009/05/27 19:33:26 fkraemer Exp $";
#define INST_MAIN 1
/******************************************************************************
 * Program:  INST CONTROL main Process 
 * Purpose:  Starts up all INST CONTROL processes, then waits for
 *	     inst_Cmnd_Cntrl to complete, which it should never do.  
 * File:     inst_main.c
 * Author:   Dick H. Fredericksen
 * Copyright: Aura Inc.  All rights reserved.
 * History:  
 *      22-Apr-1991 - created -dhf
 *	20-Nov-1991 - generalized file - ncb
 *
 *****************************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <conc.h>

#include <irstd.h>
#include <protdefs.h>
#include <protocol.h>
#include <common.h>

#include "config_st.h"
#include <instdefs.h>
#include "devdefs.h"
#undef NOINSTVARS
#include <instvars.h>
#undef NOINSTPROCS
#include <instprocs.h>		/* process descriptors & channel assignments*/
#include "prototypes.h"

static int ME = INST_MAIN;        /* self identifier for debugging messages */
/******************************************************************************
 * Program:  INST CONTROL software for WILDFIRE
 * Process:  main()
 * Purpose:  Entry point to INST CONTROL software,  Starts all other
 *           local processes.
 *
 *****************************************************************************/
int main(argc,argv)
int argc;
char *argv[];
{
    int ii;                         /* all-purpose counter */
    char dbug_buf[81];

    mem_init();

    debug = (1 << INST_MAIN);
    
    Reader_to_Writer = ChanAlloc();
    Writer_to_Control = ChanAlloc();
    Control_to_Reader = ChanAlloc();
    Cntrl_to_HK_A2D = ChanAlloc();
    HK_A2D_to_Reader = ChanAlloc();

    if (Reader_to_Writer == NULL || Writer_to_Control == NULL ||
	Control_to_Reader == NULL || Cntrl_to_HK_A2D == NULL || 
	HK_A2D_to_Reader == NULL )
    {
	status_report = -30;
	goto after_setup;
    }

    chan_list[0] = Control_to_Reader;
    chan_list[1] = HK_A2D_to_Reader;
    chan_list[7] = Reader_to_Writer;

    /* Start INST CONTROL processes: */
    
    if ((Reader = ProcAlloc(reader, 4096, 0)) == NULL)
    {
	status_report = -31;
	goto after_setup;
    }
    
    if ((Writer = ProcAlloc(writer, 4096, 0)) == NULL)
    {
	status_report = -32;
	goto after_setup;
    }

    if ((HK_A2D  = ProcAlloc(Hka2d, 8192, 0)) == NULL)
    {
	status_report = -33;
	goto after_setup;
    }

    ProcRunLow(Reader);
    ProcRunLow(Writer);
    ProcRunLow(HK_A2D);

    status_report = 1;
    
after_setup:
/*    send_debug("**started processes"); */

    if (status_report != 1)
	send_debug("ERROR Allocation error in inst_main, status_report = %d",
		status_report);

    INSTcmndctrl();

}


