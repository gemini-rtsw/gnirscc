static struct {
        void *v;
        char *c;
} rcsid = {
        &rcsid,
        "$Id: sockutil.c,v 1.2 2009/05/27 19:32:48 fkraemer Exp $"
};
/*
 * Copyright 1997 Association of Universities for Research in Astronomy, Inc.
 * See the file COPYRIGHT for more details.
 *
 * FILENAME
 * vxsockutil.c
 *
 * DESCRIPTION
 * Routines for handling socket connections (opening, closing, reading, 
 * writing, etc.). VxWorks implementation.
 * 
 * FUNCTION NAME(S):
 *     sockNew - create a socket and provide a handle to it
 *     sockBind - attempt to bind socket to requested port
 *     sockListen - create queue for client connection request
 * sockAccept - accept a new connection
 * sockCreate - create, bind & set queues for a socket (server)
 *              (calls sockNew, sockBind, sockListen)
 * sockConnect - create and open a socket (client)
 * sockClose - close a socket
 * sockWrite - write a buffer to a socket
 * sockRead - read a buffer from a socket
 * 
 * DEPENDENCIES
 * imhutil.h for structure definitions & #defines
 * dcvx.h for strndup
 *
 *INDENT-OFF*
 * $Log: sockutil.c,v $
 * Revision 1.2  2009/05/27 19:32:48  fkraemer
 * fkraemer - copied my complete working dir over trunk
 *
 * Revision 1.1.1.1  2001/10/30 21:15:25  mbec
 * checking in
 *
 * Revision 1.1.1.1  2001/10/30 20:26:00  mbec
 *
 *
 * Revision 1.1.1.1  2001/07/18 03:17:04  mbec
 * checkin
 *
 * Revision 1.1  2001/03/29 00:13:43  gemvx
 * brought back sockutile and time in utilSource
 * chunked away the dca saver once for all
 *
 * Revision 1.1  2000/12/19 01:07:03  mbec
 * adding utilSource
 *
 * Revision 1.2  1998/11/20 17:16:30  pruckle
 * speed up saver, log messages
 *
 * Revision 1.1.1.1  1998/09/30 16:40:20  pruckle
 * Initial Release
 *
 *INDENT-ON* 
 */
/* File:      sockutil.c
 * Program:   dqControl.o
 * Purpose:   Routines to create and manage TCP socket connections.
 *
 * Routines:  sockGetRange, sockCreate, sockBind, sockListen, sockAccept
 *            flagRaise, flagLower, flagSet.
 * Author:    Tad Morgan (tmorgan)
 * Copyright  (C) 1996 AURA, Inc. All rights reserved.
 * Date:      96/05/07
 *
 *
 * Revisions: 96/06/06 - tmorgan - major revisions removing socket range, 
 *                                 moving port stuff to EPICS, changing 
 *                                 params, removed dprint and made it a macro
 *                                 in common.
 *
 * parts adapted from VxWorks Programmer's Guide, Ch 6 
 * and naacdrvr.c by Diana Kennedy.
 */

#ifdef vxWorks

#include <vxWorks.h>
#include <stdio.h>
#include <fioLib.h>
#include <errno.h>
#include <string.h>
#include <unistd.h>
#include <sockLib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include "vxSockUtil.h"
#include <hostLib.h>
#include <errnoLib.h>
#else

#include <strings.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include "sockutil.h"

#endif
int sockdebug = 0;





/*
 *+
 * FUNCTION NAME:
 * sockNew
 *
 * INVOCATION:
 * int status;
 * int sFd;
 * status = sockNew(&sFd);
 *
 * PARAMETERS: (">" input, "!" modified, "<" output)
 * ! sFd (int) - socket file descriptor to be returned
 *
 * FUNCTION VALUE:
 * STATUS - OK or ERROR
 *
 * PURPOSE:
 * create a new socket
 *
 * DESCRIPTION:
 * Create a new socket.
 *
 * EXTERNAL VARIABLES:
 * none
 *
 * PRIOR REQUIREMENTS:
 * None
 *
 * DEFICIENCIES:
 * none
 *
 * HISTORY:
 * 
 *-
 */
STATUS sockNew(int *sFd) 
{
    STATUS retval = OK;
    DPRINT(sockdebug,"Creating socket - ");
/*     if ((*sFd = socket(PF_INET, SOCK_STREAM, 0)) == ERROR)  */
    if ((*sFd = socket(PF_INET, SOCK_STREAM, 0)) == ERROR) 
    {
	perror ("socket");
	retval = ERROR;
    }
    else
      {
        DPRINT(sockdebug,"DONE\n");
	;
      }

    return retval;
}


/*
 *+
 * FUNCTION NAME:
 * sockBind
 *
 * INVOCATION:
 * int status;
 * int sFd;
 * int port
 * status = sockBind(sFd, port);
 *
 * PARAMETERS: (">" input, "!" modified, "<" output)
 * > sFd (int) - socket file descriptor
 * > port (int) - port number to connect to
 *
 * FUNCTION VALUE:
 * STATUS - OK or ERROR
 *
 * PURPOSE:
 * Attempt to bind socket to requested port.
 *
 * DESCRIPTION:
 * Attempt to bind socket to requested port.
 *
 * EXTERNAL VARIABLES:
 * none
 *
 * PRIOR REQUIREMENTS:
 * None
 *
 * DEFICIENCIES:
 * none
 *
 * HISTORY:
 * 
 *-
 */
STATUS sockBind(int sFd, int port) 	
{
    static struct sockaddr_in dqAddr;
    int sockAddrSize = sizeof(struct sockaddr_in);
                                         /* size of socket address structure */
    STATUS retval = OK;
    char buf[80];

    /* set up local address */
    bzero((char *) &dqAddr, sockAddrSize);
    dqAddr.sin_family      = PF_INET;
    dqAddr.sin_port        = htons(port);
    dqAddr.sin_addr.s_addr = htonl(INADDR_ANY);
    
    /* attempt to bind socket to local address */
    sprintf(buf, "Attempting to bind to socket %i - ", port);
    DPRINT(sockdebug,buf);
    if (bind(sFd, (struct sockaddr *) &dqAddr, sockAddrSize) == ERROR)
    {
      perror("bind");
      retval = ERROR;
    }
    else 
      {
	DPRINT(sockdebug,"SUCCESSFUL\n");
	;
      }

    return retval;
}    

/*
 *+
 * FUNCTION NAME:
 * sockListen
 *
 * INVOCATION:
 * int status;
 * int sFd;
 * int maxConnections;
 * status = sockListen(sFd, maxConnections);
 *
 * PARAMETERS: (">" input, "!" modified, "<" output)
 * > sFd (int) - socket file descriptor to be returned
 * > maxConnections (int) - maximum connections to allow queued
 *
 * FUNCTION VALUE:
 * STATUS - OK or ERROR
 *
 * PURPOSE:
 * create queue for client connection requests
 *
 * DESCRIPTION:
 * Create a queue for client connection requests.
 *
 * EXTERNAL VARIABLES:
 * none
 *
 * PRIOR REQUIREMENTS:
 * None
 *
 * DEFICIENCIES:
 * none
 *
 * HISTORY:
 * 
 *-
 */
STATUS sockListen(int sFd, int maxConnections) 
{
    STATUS retval = OK;

    DPRINT(sockdebug,"Calling listen - ");
    if (listen(sFd, maxConnections) == ERROR) {
	perror("listen");
	retval = ERROR;
    }
    else
      {
        DPRINT(sockdebug,"DONE\n");
	;
      }

    return retval;
}


/*
 *+
 * FUNCTION NAME:
 * sockAccept
 *
 * INVOCATION:
 * int status;
 * int sFd;
 * struct sockaddr_in clientAddr;
 * int newFd;
 * status = sockAccept(sFd, &clientAddr, &newFd);
 *
 * PARAMETERS: (">" input, "!" modified, "<" output)
 * > sFd (int) - socket file descriptor
 * ! clientAddr (struct sockaddr_in *) - information on client socket connection
 * < newFd (int *) - socket file descriptor for new connection
 *
 * FUNCTION VALUE:
 * STATUS - OK or ERROR
 *
 * PURPOSE:
 * accept a new connection
 *
 * DESCRIPTION:
 * Accept a new connection.
 *
 * EXTERNAL VARIABLES:
 * none
 *
 * PRIOR REQUIREMENTS:
 * None
 *
 * DEFICIENCIES:
 * none
 *
 * HISTORY:
 * 
 *-
 */
/* Routine:     sockAccept
 * Description: Accept a new connection 
 *
 * Parameters:
 *   sFd - socket file descriptor
 *   clientAddr - information on client socket connection, passed back
 *   newFd - new connection file descriptor
 */ 
STATUS sockAccept(int sFd, struct sockaddr_in *clientAddr, int *newFd) 
{
    int sockAddrSize;
    STATUS retval = OK;

    sockAddrSize = sizeof(struct sockaddr_in);

    DPRINT(sockdebug,"sockAccept: Calling accept - ");
    if ((*newFd = accept(sFd, (struct sockaddr *) clientAddr,
		   &sockAddrSize)) == ERROR) 
    {
	perror("accept");
	retval = ERROR;
    }
    else 
      {
        DPRINT(sockdebug,"CONNECTION ACCEPTED\n");
	;
      }

    return retval;
}


/*
 *+
 * FUNCTION NAME:
 * sockCreate
 *
 * INVOCATION:
 * int status;
 * int portl
 * status = sockCreate(&port)
 *
 * PARAMETERS: (">" input, "!" modified, "<" output)
 * ! sFd (int) - socket file descriptor to be returned
 *
 * FUNCTION VALUE:
 * STATUS - OK or ERROR
 *
 * PURPOSE:
 *   create a new socket
 *
 * DESCRIPTION:
 * Create a new socket.
 *
 * EXTERNAL VARIABLES:
 * none
 *
 * PRIOR REQUIREMENTS:
 * None
 *
 * DEFICIENCIES:
 * none
 *
 * HISTORY:
 * 
 *-
 */
int sockCreate(int *port)
{
    int sFd;

    sFd = ERROR;

    /* create a socket */
    DPRINT(sockdebug,"create_socket:  before sockCreate\n");
    if ( sockNew(&sFd) == ERROR )
        goto Error;

    /* Find a free socket in range and bind to it */    
    DPRINT(sockdebug,"create_socket:  before sockbind \n");
    if (sockBind(sFd, *port) == ERROR) 
        goto Error;

    /* create queue for client connection request */
    DPRINT(sockdebug,"create_socket:  before socklisten \n");
    if (sockListen(sFd, MAX_QUEUED_CONNECTIONS) == ERROR) 
        goto Error;

    goto Return;

 Error:
    if (sFd != ERROR)
        close(sFd);

 Return:
    return sFd;
}

/*
 *+
 * FUNCTION NAME:
 * sockConnect
 *
 * INVOCATION:
 * int sFd;
 * int port;
 * char *saver_inet_addr;
 * sFd = sockConnect(port, saver_inet_addr);
 *
 * PARAMETERS: (">" input, "!" modified, "<" output)
 * > port (int) - port number to connect to
 * > saver_inet_addr (char *) - IP ("dot") address of saver server, as a string
 *
 * FUNCTION VALUE:
 * int - socket file descriptor for new socket
 *
 * PURPOSE:
 * create and connect to a new socket
 *
 * DESCRIPTION:
 * Create and connect to a new socket.
 *
 * EXTERNAL VARIABLES:
 * none
 *
 * PRIOR REQUIREMENTS:
 * None
 *
 * DEFICIENCIES:
 * none
 *
 * HISTORY:
 * 
 *-
 */
int sockConnect(int port, char *saver_inet_addr)
{
    int sFd=0,ipAdrs;
    struct sockaddr_in saverSockAddr;
    /* saver_inet_addr can contain the IP address or the name of the computer*/
    if ((ipAdrs = inet_addr (saver_inet_addr)) == ERROR)
    {
#ifdef vxWorks
   
	printf("%s   %d\n",saver_inet_addr,ipAdrs);
      if ((ipAdrs = hostGetByName (saver_inet_addr)) == ERROR)
	{
	  errnoSet (S_hostLib_UNKNOWN_HOST); 
/* 	  logMsg ("tnetDevCreate: ERROR--invalid host name %s\n", */
/* 		  (int) saver_inet_addr, NULL, NULL, NULL, NULL, NULL); */
	  printf ("tnetDevCreate: ERROR--invalid host name %s\n",saver_inet_addr);
	    sFd =   ERROR;
	}	
#else  /* if there is time, add the code to get the ipAdrs from the host name like above*/
      sFd = ERROR;
#endif
    }
    bzero((char *) &saverSockAddr, sizeof(struct sockaddr_in));
    saverSockAddr.sin_family = PF_INET;
    saverSockAddr.sin_port   = htons(port);
    saverSockAddr.sin_addr.s_addr = ipAdrs;

	printf("socket\n");
    if ( (sFd != ERROR) && (sFd = socket(PF_INET, SOCK_STREAM, 0)) == ERROR)
    {
        perror("socket");
	sFd = ERROR;
    }
    else
    {
		printf("connect\n");
        if (connect(sFd, (struct sockaddr *) &saverSockAddr, sizeof(struct sockaddr_in)) == ERROR)
        {
	      perror("connect");
	      close(sFd);
	      sFd = ERROR;
	}
    }

    return sFd;
}

/*
 *+
 * FUNCTION NAME:
 * sockClose
 *
 * INVOCATION:
 * int sFd;
 * sockClose(&sFd);
 *
 * PARAMETERS: (">" input, "!" modified, "<" output)
 * ! sFd (int *) - sock file descriptor to close
 *
 * FUNCTION VALUE:
 * None
 *
 * PURPOSE:
 * close socket
 *
 * DESCRIPTION:
 * Close the specified socket.
 *
 * EXTERNAL VARIABLES:
 * none
 *
 * PRIOR REQUIREMENTS:
 * None
 *
 * DEFICIENCIES:
 * none
 *
 * HISTORY:
 * 
 *-
 */
void sockClose(int *sFd)
{

    close(*sFd);
    *sFd = ERROR;
}


/*
 *+
 * FUNCTION NAME:
 * sockWrite
 *
 * INVOCATION:
 * STATUS status;
 * int sFd;
 * char *buf;
 * int sz;
 * status = sockWrite(sFd, buf, sz);
 *
 * PARAMETERS: (">" input, "!" modified, "<" output)
 * > sfd (int) - socket file descriptor to write to
 * > buf (char *) - buffer to write to socket
 * > sz - (int) - size of buffer to write
 *
 * FUNCTION VALUE:
 * STATUS - OK or ERROR
 *
 * PURPOSE:
 * write buffer to socket
 *
 * DESCRIPTION:
 * Write buffer of specified length to a socket.
 *
 * EXTERNAL VARIABLES:
 * none
 *
 * PRIOR REQUIREMENTS:
 * None
 *
 * DEFICIENCIES:
 * none
 *
 * HISTORY:
 *  written by Peter Ruckle
 *-
 */
STATUS sockWrite(int sfd, char *buf,int sz)
{
    STATUS retval = OK;

    if ( write (sfd, buf, sz ) == ERROR)
	retval = ERROR;

    return retval;
}


STATUS sockWriteString(int sfd, char *buf,int sz)
{
    STATUS retval = OK;
    int i;
    char *buf1;
   
    buf1 = malloc((sz+1)*sizeof(char));
  
    if(buf1)
    {
	for (i=0;i<=sz;i++)
	    buf1[i]=0;
	strcpy(buf1,buf);
	if ( write (sfd, buf1, sz ) == ERROR)
	    retval = ERROR;
	free (buf1);
	return retval;
    }
    printf("sockWriteString: Error mallocing data\n");
    return ERROR;
}

/*
 *+
 * FUNCTION NAME:
 * sockRead
 *
 * INVOCATION:
 * STATUS status;
 * int sFd;
 * char *buf;
 * int sz;
 * status = sockRead(sFd, buf, sz);
 *
 * PARAMETERS: (">" input, "!" modified, "<" output)
 * > sfd (int) - socket file descriptor to read from
 * < buf (char *) - buffer to read into from socket
 * > sz - (int) - maximum characters to read
 *
 * FUNCTION VALUE:
 * STATUS - OK or ERROR
 *
 * PURPOSE:
 * write buffer to socket
 *
 * DESCRIPTION:
 * Write buffer of specified length to a socket.
 *
 * EXTERNAL VARIABLES:
 * none
 *
 * PRIOR REQUIREMENTS:
 * None
 *
 * DEFICIENCIES:
 * none
 *
 * HISTORY:
 *  written by Peter Ruckle
 *-
 */
#if 0
STATUS sockRead(int sfd, char *buffer,int sz)
{
    STATUS retval = OK;

    if ((buffer == NULL) || (fioRead(sfd, buffer,sz)) != sz)
    {
	printf("Error sockRead\n");
	retval = ERROR;
    }
    
    return retval;
}
#endif

STATUS sockRead(int sfd, char *buffer,int sz)
{
    int len;
    STATUS retval = OK;

    if ((buffer == NULL) || ((len = read(sfd, buffer,sz))) != sz)
    {
	printf("sockread size = %d, read %d\n",sz,len);
	perror("");
	retval = ERROR;
    }
    return retval;
}


int socknRead(int sfd, char *buffer,int sz)
{
    int len;
    STATUS retval = OK;

    if ((buffer == NULL) || ((len = read(sfd, buffer,sz))) != sz)
    {
	printf("sockread size = %d, read %d buffer = %x\n",sz,len,(unsigned int)buffer);
	retval = -1;
    }
    return len;
}
