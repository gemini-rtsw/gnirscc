static char rcid[]="$Id: vars.c,v 1.2 2009/05/27 19:33:28 fkraemer Exp $";
/******************************************************************************
 * Program:	seq.tld
 * File:	var.c
 * Purpose:	provide methods for setting/reading int/fint time + startup
 *		for downloaded processes.
 * Author:	David Koski
 * Copyright:   Aura Inc.  All rights reserved.
 *
 *****************************************************************************/

#include <conc.h>
#include "protocol.h"
#include "common.h"
#include "seqhdw.h"
#include "seq_vars.h"

void startMsg(int header, int *buf)
{
    int val = 0;

    if (buf)
	val = *buf;
    ChanOutInt((Channel *) &Control_to_Seq, val);
    send_debug("**ucode started");
    

}

void myExecuteProc(int header, int *buf)
{
    node = _node_number;

    execute_proc(header, buf);
    send_debug("**process started");
}

void set_int(int header, int *buf)
{
    int varnum = VAR_NUMVAL(buf);
    float time;
    int *sec, *msec;

    if (var_ptr[varnum] == &Int_Time_Seconds) {
	sec = &Int_Time_Seconds;
	msec = &Int_Time_MilliSecs;
    } else {
	sec = &FInt_Time_Seconds;
	msec = &FInt_Time_MilliSecs;
    }

    time = *((float *)&buf[VAR_VAL]);

    *sec = (int) time;
    *msec = ((int)(time * 1000.0)) % 1000;
    if (var_ptr[varnum] == &Int_Time_Seconds) 
	send_debug ("integration time set to %d.%d",*sec,*msec);
    else
	send_debug ("F integration time set to %d.%d",*sec,*msec);
}

void read_int(int header, int *buf)
{
    int varnum = VAR_NUMVAL(buf);
    float time;
    int *sec, *msec;
    int *newbuf;
    char buf1[80];

    if (var_ptr[varnum] == &Int_Time_Seconds) {
	sec = &Int_Time_Seconds;
	msec = &Int_Time_MilliSecs;
    } else {
	sec = &FInt_Time_Seconds;
	msec = &FInt_Time_MilliSecs;
    }

    time = *sec + (*msec / 1000.0);

    header = TO_NODE(buf[REPLY_TO]) | MESSAGE(VAR_READ) | OF_LENGTH(3);
    newbuf = mem_alloc(sizeof(int) * 3);

    *((float *) &newbuf[RETURN_VAL]) = time;
    newbuf[FROM] = _node_number;
    newbuf[VAR_NUM] = varnum;
    sprintf (buf1,"** val = %f to node %d",(double)time,TO_NODE(buf[2]));
    send_debug( buf1);
    SEND(header, (char *) newbuf, Control_to_Reader);
}

