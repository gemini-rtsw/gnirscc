static char rcid[]="$Id: debug.c,v 1.2 2009/05/27 19:33:22 fkraemer Exp $";
/******************************************************************************
 * Program:	common
 * File:	debug.c
 * Purpose:	provide a way to send debugging messages up to the sun
 * Author:	David Koski
 * Copyright:   Aura Inc.  All rights reserved.
 * History:
 *		 6Jan92	created						dak
 *		 4Aug92	changed to allow printf style formatting	dak
 *
 ******************************************************************************/

#include <conc.h>
#include <stdio.h>
#include <stdarg.h>
#include <string.h>
#include "common.h"
#include "protocol.h"
#include "shared.h"

static Semaphore dbglock = SEMAPHOREINIT;

Channel DEBUG_CHAN = NOPROCESS;

static queue *header_q, *buf_q = NULL;
static Channel *Proc_to_Debug = NULL;
static Channel *Buf_to_Send = NULL;

/* buffer the debug messages */

void debug_buf()
{
    int header, buf, i;
    Channel *list[3];

    list[0] = Proc_to_Debug;
    list[1] = Buf_to_Send;
    list[2] = NULL;

    while (1) {
 	i = ProcAltList(list); 
	if (i == 0) {
	    header = ChanInInt(Proc_to_Debug);
	    buf = ChanInInt(Proc_to_Debug);
	    enq(header_q, header);
	    enq(buf_q, buf);
	} else {
	    if (!emptyq(header_q)) {
 		ChanInInt(Buf_to_Send); 
		header = deq(header_q);
		buf = deq(buf_q);
		ChanOutInt(Buf_to_Send, header);
		ChanOutInt(Buf_to_Send, buf);
	    } else {
		ProcWait(1600);
	    }
	}
    }
}

/* send out any pending debug messages.  the reader will throttle this
   back if it gets out of control. */

void debug_send()
{
    int header, buf;

    while (1) {
	ChanOutInt(Buf_to_Send, 0);
	header = ChanInInt(Buf_to_Send);
	buf = ChanInInt(Buf_to_Send);
	SEND(header, buf, &DEBUG_CHAN);
    }
}

void send_debug(char *format, ...)
{
        char *buf;
	int header;
	va_list list;
	static Process *Pdebug_buf = NULL;
	static Process *Pdebug_send = NULL;

	HSemP(dbglock);
	if (Pdebug_buf == NULL) {
	    header_q = q_alloc();
	    buf_q = q_alloc();
	    Proc_to_Debug = ChanAlloc();
	    Buf_to_Send = ChanAlloc();
	    Pdebug_buf = ProcAlloc(debug_buf, 2048, 0);
	    Pdebug_send = ProcAlloc(debug_send, 2048, 0);
 	    ProcRun(Pdebug_buf); 
 	    ProcRun(Pdebug_send);
	}
 	buf = mem_alloc(MEM_BLOCK_SIZE);
	va_start(list, format);
	vsprintf(buf, format, list);
	va_end(list);
	header = MESSAGE(DEBUG_MSG) | FROM_NODE(_node_number) | TO_NODE(0);
	header |= OF_LENGTH((strlen(buf)+4)/4);

 	ChanOutInt(Proc_to_Debug, header);
	ChanOutInt(Proc_to_Debug, (int) buf);
	HSemV(dbglock);
	return;
}
