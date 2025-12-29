static char rcid[]="$Id: control.c,v 1.2 2009/05/27 19:33:22 fkraemer Exp $";
/******************************************************************************
 * Program:	common
 * File:	control.c
 * Purpose:	provide a standard message handling/controller facility
 * Author:	David Koski
 * Copyright:   Aura Inc.  All rights reserved.
 * History:
 *		26Dec91	written						dak
 *
 ******************************************************************************/
extern int trace_flag;
#include <conc.h>
#include <stdio.h>
#include "protocol.h"
#include "common.h"
#include "shared.h"

extern Channel *Writer_to_Control;

/******************************************************************************
 * Routine: control
 * Purpose: generic controller
 * Inputs:  none, watches Writer_to_Control for messages.
 * Returns: nothing, never exits
 * 
 ******************************************************************************/

void control()
{
	int *buf, header;

	while (1)
	{
	    if (trace_flag > 50)
		send_debug("** control: var = %d, message = %d", var,MESSAGE(header)); 
	    header = ChanInInt(Writer_to_Control);
	    if (LENGTH(header) != 0)
		buf = (int *) ChanInInt(Writer_to_Control);
	    else
		buf = NULL;
	    
	    if (MESSAGE(header) > num_command ||
		command[MESSAGE(header)] == NULL)
		{
		    /* junk the message */
		} else {
		    
		    (command[MESSAGE(header)]) (header, buf); 
		}
	    
	    mem_free(buf);
	}
}
