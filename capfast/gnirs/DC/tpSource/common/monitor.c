static char rcid[]="$Id: monitor.c,v 1.2 2009/05/27 19:33:22 fkraemer Exp $";
/******************************************************************************
 * Program:	common
 * File:	monitor.c
 * Purpose:	provide a watchdog that will monitor a given channel, sending
 *		an error message to the controller if something doesn't
 *		happen in the given time.
 * Author:	David Koski
 * Copyright:   Aura Inc.  All rights reserved.
 * History:
 *		26Dec91	written						dak
 *
 ******************************************************************************/

#include "protocol.h"
#include "common.h"

void monitor_proc(Process *junk,
	Channel *watch, Channel *error, int ticks, int error_code)
{
	while (1)
        {
	    if (ProcTimerAlt(Time() + ticks, watch, NULL) == -1)
	    {
		    SEND(MESSAGE(ERROR) | TO_NODE(_node_number) | OF_LENGTH(1),
			    (char *) &error_code, error);
		    ChanInInt(watch);		/* wait for restart message */
	    }
	}
}

void monitor(Channel *watch, Channel *error, int usec, int error_code)
{
	Process *p;

	p = ProcAlloc(monitor_proc, 256, 4, (int) watch, (int) error, usec / 64,
			error_code);
	ProcRunLow(p);
}
