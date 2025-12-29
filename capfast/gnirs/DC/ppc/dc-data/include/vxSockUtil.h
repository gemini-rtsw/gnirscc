/* File:        vxSockUtil.h 
 * Description: defines & headers for sockutil.c
 *
 * Routines: none
 * Author:   Tad Morgan (tmorgan)
 * Copyright (C) 1996 AURA, Inc. All rights reserved.
 * Date:     96/05/07
 *
 * parts adapted from VxWorks Programmer's Guide, Ch 6 
 * and naacdrvr.c by Diana Kennedy.
 *
 * 96/06/06 - changed to use EPICS instead of flag files
 */

#ifndef VX_SOCKUTIL_H

#include "sockutil.h"
#include <netinet/in.h>



/* #ifdef DEBUG */
/* #define DPRINT(a) (fputs (a,stderr)) */
/* #else */
/* #define DPRINT(a) {} */
/* #endif */

/* sockutil.c functions */

int sockConnect(int port, char *saver_inet_addr);

#define VX_SOCKUTIL_H
#endif


