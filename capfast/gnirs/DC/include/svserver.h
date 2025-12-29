/* File:     svserver.h
 * Purpose:  Header infomation for routines in svserver.c
 *
 * Routines: none
 * Author:   Tad Morgan (tmorgan)
 * Copyright (C) 1996 AURA, Inc. All rights reserved.
 * Date:     96/05/07
 *
 * parts adapted from VxWorks Programmer's Guide, Ch 6 
 * and naacdrvr.c by Diana Kennedy. 
 */

#ifndef SVSERVER_H

#include "saverCommon.h"
#include "svcontrol.h"         /* for Response values */

#define ENDSVR ((OK) + 1)      /* value indicating to End Server task */
#define HOSTNAMELEN 80
#define STRINGLEN 256

#define SAVER_ENV   "SAVER_HOST"         /* environmental var naming machine 
                                          * saver is on */
#define SAVER_EPICS "PrcCtrl:SVR_Port"   /* EPICS variable containing name of
                                          * port # of Saver (0 if not up) */
#define DQC_EPICS   "PrcCtrl:DQC_Port"   /* EPICS variable containing name of
                                          * port # of DqControl (0 if not up)*/
#define PIPE_PROG   "watcher"            /* name of program that watches an
                                          * EPICS variable & receives values
					  * via a pipe. */

/* svserver.c functions */
STATUS svControl(int port);
STATUS readVarList(char *filename, char ***chanNames, int *numChans);
void freeVarList(char ***chanNames, int numChans);

#define SVSERVER_H
#endif


