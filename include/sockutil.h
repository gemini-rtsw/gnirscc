/* File:        sockutil.h 
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

#ifndef SOCKUTIL_H
/* #include <debug.h> */
/* #include "common.h" */
#include <netinet/in.h>

#define MAX_QUEUED_CONNECTIONS (0)
#define SOCK_FIRST 5000



/* sockutil.c functions */
STATUS sockNew(int *sFd);
STATUS sockBind(int sFd, int port);
STATUS sockListen(int sFd, int maxConnections);
STATUS sockAccept(int sFd, struct sockaddr_in *clientAddr, int *newFd);
int    sockCreate(int *port);
STATUS sockRead(int sfd, char *buffer,int sz);
STATUS socknRead(int sfd, char *buffer,int sz);
STATUS sockWrite(int sfd, char *buf,int sz);
int sockConnect(int port, char *saver_inet_addr);
void sockClose(int *sFd);
#define SOCKUTIL_H
#endif


