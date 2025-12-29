static struct {
        void *v;
        char *c;
} rcsid = {
        &rcsid,
        "$Id: sockutil.c,v 1.2 2009/05/27 19:32:08 fkraemer Exp $"
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
 * Identical to same routine in the GMOS software package except for
 * renaming of sdsuLogMessage to gnirsLogMessage, and inclusion of
 * gnirs.h instead of sdsu.h .
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
 * Revision 1.2  2009/05/27 19:32:08  fkraemer
 * fkraemer - copied my complete working dir over trunk
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
#include <stdlib.h>
#include <sockLib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <hostLib.h>
#include <errnoLib.h>
#include "gnirsCC.h"
#include "sockutil.h"
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
    gnirsLogMessage(CICS_DB_FULL, "Creating socket - ");
    if ((*sFd = socket(PF_INET, SOCK_STREAM, 0)) == ERROR) 
    {
        gnirsLogMessage(CICS_DB_ERROR, "sockutil: socket");
        perror ("socket");
        retval = ERROR;
    }
    else
      {
        gnirsLogMessage(CICS_DB_FULL, "DONE.");
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

    /* set up local address */
    bzero((char *) &dqAddr, sockAddrSize);
    dqAddr.sin_family      = PF_INET;
    dqAddr.sin_port        = htons(port);
    dqAddr.sin_addr.s_addr = htonl(INADDR_ANY);
    
    /* attempt to bind socket to local address */
    gnirsLogMessage(CICS_DB_FULL, "Attempting to bind to socket %i - ", port);
    if (bind(sFd, (struct sockaddr *) &dqAddr, sockAddrSize) == ERROR)
    {
      gnirsLogMessage(CICS_DB_ERROR, "sockutil: bind");
      perror("bind");
      retval = ERROR;
    }
    else 
      {
        gnirsLogMessage(CICS_DB_FULL, "SUCCESSFUL");
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

    gnirsLogMessage(CICS_DB_FULL, "Calling listen - ");
    if (listen(sFd, maxConnections) == ERROR) {
        gnirsLogMessage(CICS_DB_ERROR, "sockutil: listen");
        perror("listen");
        retval = ERROR;
    }
    else
      {
        gnirsLogMessage(CICS_DB_FULL, "DONE.");
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

    gnirsLogMessage(CICS_DB_FULL, "sockAccept: Calling accept - ");
    if ((*newFd = accept(sFd, (struct sockaddr *) clientAddr,
                   &sockAddrSize)) == ERROR) 
    {
        gnirsLogMessage(CICS_DB_ERROR, "sockutil: accept");
        perror("accept");
        retval = ERROR;
    }
    else 
      {
        gnirsLogMessage(CICS_DB_FULL, "CONNECTION ACCEPTED.");
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
    gnirsLogMessage(CICS_DB_FULL, "create_socket:  before sockCreate.");
    if ( sockNew(&sFd) == ERROR )
        goto Error;

    /* Find a free socket in range and bind to it */    
    gnirsLogMessage(CICS_DB_FULL, "create_socket:  before sockbind.");
    if (sockBind(sFd, *port) == ERROR) 
        goto Error;

    /* create queue for client connection request */
    gnirsLogMessage(CICS_DB_FULL, "create_socket:  before socklisten.");
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
 * sockConnectTO
 *
 * INVOCATION:
 * int sFd;
 * int port;
 * char *saver_inet_addr;
 * int to;
 * sFd = sockConnectTO(port, saver_inet_addr,to);
 * 
 * PARAMETERS: (">" input, "!" modified, "<" output)
 * > port (int) - port number to connect to
 * > saver_inet_addr (char *) - IP ("dot") address of saver server, as a string
 * > to time to wait for a connection
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
int sockConnectTO(int port, char *saver_inet_addr,int t)
{
  int sFd,ipAdrs;
  struct sockaddr_in saverSockAddr;
  struct timeval to;
  
       
  sFd = 0;
  /* saver_inet_addr can contain the IP address or the name of the computer */
  if ((ipAdrs = inet_addr (saver_inet_addr)) == ERROR)
     
#ifdef vxWorks
    if ((ipAdrs = hostGetByName (saver_inet_addr)) == ERROR)
      {
        errnoSet (S_hostLib_UNKNOWN_HOST); 
        gnirsLogMessage(CICS_DB_ERROR,
                "tnetDevCreate: ERROR--invalid host name %s\n",
                (int) saver_inet_addr, NULL, NULL, NULL, NULL, NULL);
        sFd =   ERROR;
      } 
#else  /* if there is time, add the code to get the ipAdrs from the host name like above*/
  sFd = ERROR;
#endif
  bzero((char *) &saverSockAddr, sizeof(struct sockaddr_in));
  saverSockAddr.sin_family = PF_INET;
  saverSockAddr.sin_port   = htons(port);
  saverSockAddr.sin_addr.s_addr = ipAdrs;

  if ( (sFd != ERROR) && (sFd = socket(PF_INET, SOCK_STREAM, 0)) == ERROR)
    {
      gnirsLogMessage(CICS_DB_ERROR, "sockutil: socket");
      perror("socket");
      sFd = ERROR;
    }
  else
    {
      if (t <= 0) {
        if (connect(sFd, (struct sockaddr *) &saverSockAddr, sizeof(struct sockaddr_in)) == ERROR)
          {
            gnirsLogMessage(CICS_DB_ERROR, "sockutil: connect");
            /*   perror("connect"); */
            close(sFd);
            sFd = ERROR;
          }
      } else {
          to.tv_sec = t;
          to.tv_usec = 0;
#ifdef vxWorks
          if (connectWithTimeout(sFd, (struct sockaddr *) &saverSockAddr, sizeof(struct sockaddr_in), &to) == ERROR)
            {
              gnirsLogMessage(CICS_DB_ERROR, "sockutil: connect");
              /*   perror("connect"); */
              close(sFd);
              sFd = ERROR;
            }
#else
            sFd = ERROR;
#endif
          }
    }

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
  return (sockConnectTO(port, saver_inet_addr,0));
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
  
            
    for (i=0;i<=sz;i++)
        buf1[i]=0;
    strcpy(buf1,buf);
    if ( write (sfd, buf1, sz ) == ERROR)
        retval = ERROR;
    free (buf1);
    return retval;
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
STATUS sockRead(int sfd, char *buffer,int sz)
{
    int len;
    STATUS retval = OK;

    if ((buffer == NULL) || ((len = read(sfd, buffer,sz))) != sz)
    {
/*      printf("sockread size = %d, read %d\n",sz,len); */
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
/*      printf("sockread size = %d, read %d\n",sz,len); */
        retval = -1;
    }
    return len;
}
