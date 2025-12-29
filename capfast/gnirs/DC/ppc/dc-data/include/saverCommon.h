/* File:     common.h
 * Purpose:  Header information common to pieces of vxWorks server application
 *
 * Routines: none
 * Author:   Tad Morgan (tmorgan)
 * Copyright (C) 1996 AURA, Inc. All rights reserved.
 * Date:     96/05/07
 *
 * parts adapted from VxWorks Programmer's Guide, Ch 6 
 * and naacdrvr.c by Diana Kennedy. 
 */



#include <stdlib.h>
#include <stdio.h>
#include <signal.h>
#include <time.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>

#ifndef SAVER_COMMON_H

#ifdef vxWorks
/* sets up correct prototype for taskSpawn, etc. under vxWorks, must come 
 * before related header files */
#define __PROTOTYPE_5_0     

/* Use VxWorks header files */
#include <vxWorks.h>
#include <sockLib.h>
#include <logLib.h>
#include <inetLib.h>
#include <taskLib.h>
#include <netinet/in.h>
#include <fioLib.h>
#include <hostLib.h>

#else  /* UNIX includes, defines that vxWorks.h would define */
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
/* #include "stdhdrs.h" */

#define STATUS int
#define OK 0
#define ERROR (-1)
#define FOREVER for (;;)
#define fioRead read

#endif


/* Global constants */
#define BOOL int
#define TRUE 1
#define FALSE 0
#define MSG_SIZE 80

/* Macro to print out debug messages */
#include "debug.h"

#define SAVER_COMMON_H
#endif
