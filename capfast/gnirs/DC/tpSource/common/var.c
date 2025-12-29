static char rcid[]="$Id: var.c,v 1.2 2009/05/27 19:33:23 fkraemer Exp $";
/******************************************************************************
 * Program:	common
 * File:	var.c
 * Purpose:	handle variable setting and reading
 *	          calls a different routine depending the variable being set
 * Author:	David Koski
 * Copyright:   Aura Inc.  All rights reserved.
 * History:
 *		26Dec91	written						dak
 *
 ******************************************************************************/

#include "protocol.h"
#include "common.h"
extern int trace_flag;

int var(int header, int *buf)
{
	int var = VAR_NUMVAL(buf);

 	if (trace_flag > 50) 
  	send_debug("** var = %d, message = %d", var,MESSAGE(header));  


	if (MESSAGE(header) == READ_VAR && readv_cmnds[var] != NULL) {
		(readv_cmnds[var]) (header, buf);
	} else if (MESSAGE(header) == SET_VAR && setv_cmnds[var] != NULL) {
	  
		(setv_cmnds[var]) (header, buf);
	}

	mem_free(buf);
}
