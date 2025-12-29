/* File:     svcontrol.h
 * Purpose:  Header infomation for routines in svcontrol.c
 *
 * Routines: none
 * Author:   Tad Morgan (tmorgan)
 * Copyright (C) 1996 AURA, Inc. All rights reserved.
 * Date:     96/05/07
 *
 * parts adapted from VxWorks Programmer's Guide, Ch 6 
 * and naacdrvr.c by Diana Kennedy. 
 */

#ifndef SVCONTROL_H

#include <time.h>
#include <sys/filio.h>
#include <sys/types.h>
#include <unistd.h>
#include <fcntl.h>
#include "saverCommon.h"



/* Message response strings */
#define RPS_READY "READY"
#define RPS_DONE  "DONE"
#define RPS_ERROR "ERROR"
#define RPS_OK    "OK"


/* Routines */
int handleCnct(int sFds);

#define SVCONTROL_H
#endif
