static char rcid[]="$Id: talk.c,v 1.2 2009/05/27 19:33:23 fkraemer Exp $";
/******************************************************************************
 * Program:	common
 * File:	talk.c
 * Purpose:	two processes that read and write channels
 * Author:	David Koski
 * Copyright:   Aura Inc.  All rights reserved.
 * History:
 *		26Dec91	written (router must be completed)		dak
 *
 *****************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <conc.h>
#include "protocol.h"
#include "common.h"
#include "shared.h"

#define THROTTLE	4		/* # time through to allow */
#define WAIT		20

extern Channel *Reader_to_Writer, *Writer_to_Control;

Channel *out_chan[4] = {
	LINK0OUT, LINK1OUT, LINK2OUT, LINK3OUT
};

Channel *hw_chan[4] = {
	LINK0IN, LINK1IN, LINK2IN, LINK3IN
};

/******************************************************************************
 * Routine: router
 * Purpose: route a message to a given destination node
 * Inputs:  dest_node -- where to go
 * Returns: Channel * --  channel to send out of
 * really simple for the NAAC controller see /source/wfire/src/tp/common for
 * the routing for a complex tp network.
 ******************************************************************************/

Channel *router(int dest_node)
{
    if (_node_number == 2) {		/* icon */

	if (dest_node == 0)
	    return out_chan[0];
	else
	    return out_chan[1];		/* must be node 10, seq */

    } else if (_node_number <= 99) {	/* seq */

	if (dest_node > _node_number)
	    return out_chan[1];
	else
	    return out_chan[0];

    }
}


/******************************************************************************
 * Routine: reader
 * Purpose: read all channels and pass them on to the writer
 * Inputs:  none
 * Returns: none (process)
 * 
 *****************************************************************************/

void reader()
{
    char *buf;
    unsigned int header, chan;
    int cur_chan = 0;
    int *quota, max;
    Channel *cp;
    queue *header_q, *buf_q, *chan_q;
    int through = 0;		/* number of times through (throttle) */

    header_q = q_alloc();
    buf_q = q_alloc();
    chan_q = q_alloc();

    /* count up the number of channels */
    cur_chan = 0;
    while (chan_list[cur_chan] != NULL)
    	cur_chan += 1;
    max = QUEUE_SIZE / (cur_chan - 1);
    quota = malloc(sizeof(int) * (cur_chan - 1) + 16);
    for (chan = 0; chan < cur_chan; chan++)
    	quota[chan] = 0;
    
    cur_chan = 0;
    while (1)
    {
	while (1) {
	    if (quota[cur_chan] > max || 
	    	(chan = ProcSkipAlt(chan_list[cur_chan], NULL)) == -1) {
		cur_chan ++;
		if (chan_list[cur_chan] == NULL) {
		    cur_chan = 0;
		    through ++;
		    if (through == THROTTLE) {
			ProcWait(WAIT);
			through = 0;
		    }
		}
	    } else {
		chan = cur_chan;
		cur_chan ++;
		if (chan_list[cur_chan] == NULL) {
		    cur_chan = 0;
		    through ++;
		}
		break;
	    }
	}
	cp = chan_list[chan];

        if (cp == Reader_to_Writer || fullq(header_q)) {
	    cp = Reader_to_Writer;
	    if (!emptyq(header_q)) {
		ChanInInt(cp);
		header = deq(header_q);
		buf = (char *) deq(buf_q);
		chan = deq(chan_q);
		quota[chan] --;
		ChanOutInt(cp, (int)header);
		if (LENGTH(header) != 0)
		    ChanOutInt(cp, (int) buf);
	    }
	} else {
	    through = 0;			/* reset throttle */
	    header = ChanInInt(cp);
	    enq(header_q, (int)header);

	    if (cp == hw_chan[0] || cp == hw_chan[1] || cp == hw_chan[2] || 
		    cp == hw_chan[3]) {
						/* it is a non local message */
		if (LENGTH(header) != 0) {
		    buf = mem_alloc((int)(LENGTH(header) * 4));

		    while (buf == NULL) {
			ProcWait(1600);
			buf = mem_alloc((int)(LENGTH(header) * 4));
		    }
		    ChanIn(cp, buf,(int) (LENGTH(header) * 4));
		} else
		    buf = 0;

	    } else {					/* it is local */

		if (LENGTH(header) != 0) {
		    buf = (char *) ChanInInt(cp);
		}
	    }

	    enq(buf_q, (int) buf);
	    quota[chan]++;
	    enq(chan_q, (int)chan);
	}	/* if chan == 0 */

    }
}


/******************************************************************************
 * Routine: writer
 * Purpose: send the message to whoever should get it.
 * Inputs:  none, watches the Reader_to_Writer channel
 * Returns: none (process)
 * 
 ******************************************************************************/

void writer()
{
    char *buf;
    int header;
    Channel *out;

    while (1)
    {
	ChanOutInt(Reader_to_Writer, 0);
	header = ChanInInt(Reader_to_Writer);
	if (LENGTH(header) != 0)
	    buf = (char *) ChanInInt(Reader_to_Writer);	/* get pointer */
	else
	    buf = 0;
	
	if (NODE(header) == _node_number)	/* it is for this transputer */
	{

	    ChanOutInt(Writer_to_Control, header);
	    if (LENGTH(header) != 0)
		ChanOutInt(Writer_to_Control, (int) buf);

	} else {				/* pass it along */

	    if (FROM_WHERE(header) == 0)
		header |= FROM_NODE(_node_number);
	    out = router((int)(NODE(header)));	    /* figure out how to send */
	    ChanOutInt(out, header);
	    if (LENGTH(header) != 0) {
		ChanOut(out, buf, LENGTH(header) * 4);
		mem_free(buf);
	    }

	}
    }
}


