/* tnetDev.h */

/*
modification history
-------------------
01a,29apr96,bdg  created
*/

#ifndef INCtnetDevh
#define INCtnetDevh
#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>

#define COADD "dccoadd"
#define EPICS "dcepics"
#define BOTH "dcboth"
#define CC "cc"
#define WFS "wfs"
#define RESET_STRING "#1DO"

#define PORT1 7008
#define PORT2 7004
#define TNET_PTY_SIZE	4096		/* xfer buffer size */
#define TNET_TASK_PRI	3		/* connect task priority */
#define TNET_CONN_RETRY	15		/* retry to connect every 15 seconds */
#define ERROR -1
#define TRUE 1


#endif	/* !INCtnetDevh */
