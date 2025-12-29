static struct {
        void *v;
        char *c;
} rcsid = {
        &rcsid,
        "$Id: naacdrvr.c,v 1.2 2009/05/27 19:33:36 fkraemer Exp $"
};
/*******************************************************************************
 * Program:     naacdrvr
 * File:	naacdrvr.c
 * Purpose:	server for handling io from transputer network run as part of
 *		   naac_cntrl program.  
 * Author:	Diana Kennedy
 * History:
 *	11-Jan-1996 create file - djk
 *
 ******************************************************************************/
extern void nsleep (int a,int b);
#define __PROTOTYPE_5_0
#include        <vxWorks.h>  
#include	<stdio.h>
#include	<signal.h>
#include	<time.h>
#include	<fcntl.h>
#include	<sockLib.h>
#include        <logLib.h>
#include        <inetLib.h>
#include        <unistd.h>
#include        <taskLib.h>
#include	<netinet/in.h>
#include	<string.h>
#include	<errno.h>
extern int errno;


#include	"irstd.h"
#include	"drvr_defs.h"
#include	"drvr_b014.h"
#undef		NAACCIOV
#include	"drvr_vars.h"
#include	"drvrlink.h"
#include        "bioIsr.h"
#include <cicsLib.h>
extern B014_MAP b014;

int trid;
int twid;

int naacSrvr (int comm_fd, int cmd_fd, struct sockaddr_in clntSockAddr);


/******************************************************************************
 * Routine:	naacdrvr()
 * Purpose:	Opens up communication client process then handles 
 *              conmunication between the transputer link adapter on an 
 *              INMOS b014 Transputer board and the client
 * Parameters:  argc - int - An integer count of the command line arguments.
 *		argv - char ** - A pointer to an array of pointers which point
 *			to individual command line arguments.
 * Returns:	None.
 *
 * Returns:	int - 0 for success, non-zero for error
 * 
 ******************************************************************************/
int
naacdrvr()

{
    struct sockaddr_in servAddr;               /* server's socket address */
    struct sockaddr_in clientAddr;             /* client's socket address */
    int addrsize = sizeof(struct sockaddr_in);

    prtdebug(0xFFFFFFFF, "Entering main\n");
   
    
    prtdebug(0xFFFFFFFF, "Setting up signals\n");
   
    /* Set up signals for server */
/*
    signal(SIGTTIN, SIG_IGN);
    signal(SIGTTOU, SIG_IGN);
    signal(SIGTSTP, SIG_IGN);
*/
    prtdebug(0xFFFFFFFF, "Getting address\n");

    /* set up local address */
    
    bzero((char *)&servAddr, addrsize);
    servAddr.sin_family = PF_INET;
    servAddr.sin_port = htons(SERVERPORT);
    servAddr.sin_addr.s_addr = htonl(INADDR_ANY);

    prtdebug(0xFFFFFFFF, "Creating socket\n");
    /* Create socket with socket() */
    if ((wait_socket = socket(PF_INET, SOCK_STREAM, 0)) == ERROR)
    {
	perror("Socket create failed");
	logMsg("socket create failed (77,naacdrvr.c)\n",0,0,0,0,0,0);
	return(ERROR);
    }

    /* Create socket with socket() */
    if ((csocket = socket(PF_INET, SOCK_STREAM, 0)) == ERROR)
    {
	perror("Socket create failed");
	logMsg("socket create failed (85,naacdrvr.c)\n",0,0,0,0,0,0);
	return(ERROR);
    }

    /* create socket address */
    
    prtdebug(0xFFFFFFFF, "Binding Comm Address\n");
    /* bind address to socket with bind() */
if (bind(wait_socket, (struct sockaddr *)&servAddr, sizeof(servAddr)) == ERROR)
    {
        perror("Bind of socket address failed");
	logMsg("bind of socket failed (97,naacdrvr.c)\n",0,0,0,0,0,0);
	return(ERROR);

    }
    servAddr.sin_port = SERVERPORT + 1;
    
    prtdebug(0xFFFFFFFF, "Binding Cmnd Address\n");
    /* bind address to socket with bind() */
if (bind(csocket, (struct sockaddr *)&servAddr, sizeof(servAddr)) == ERROR)
    {
        perror("Bind of socket address failed");
	logMsg("bind of socket failed (107,naacdrvr.c)\n",0,0,0,0,0,0);
	return(ERROR);
    }

    prtdebug(0xFFFFFFFF, "Listening for connections\n");
    /* listen for requests with listen() queue two requests only */
    if (listen(wait_socket, 2) == ERROR)

    {
	perror("Listen Failed");
	logMsg("listen failed (116,naacdrvr.c)\n",0,0,0,0,0,0);
	return(ERROR);
    }

    /* listen for requests with listen() queue two requests only */
    if (listen(csocket, 2) == ERROR)
    {
	perror("Listen Failed");
	logMsg("listen failed (124,naacdrvr.c)\n",0,0,0,0,0,0);
	return(ERROR);
    }

/* set the task ids to zero */
    trid = 0;
    twid = 0;
    b014.initialized = FALSE;

/* initilize input and output buffer control */

    IBin = IBout = IBcount = 0;
    OBin = OBout = OBcount = 0;
    OBflag = FALSE;
    OBloop = FALSE;
    closeFLAG = FALSE;

    /* wait for connections and process data with accept() read() and write() */
    FOREVER
    {
	/* set up "clientAddr" address */
	clientAddr.sin_family = AF_INET;


	prtdebug(0xFFFFFFFF, "Accepting connection\n");
	/* accept a connection from client */
	if ((comm_socket = accept(wait_socket, 
				  (struct sockaddr *) &clientAddr, 
				  &addrsize)) == ERROR)
	{
	    perror("Accept Failed");
            close(wait_socket);
	    logMsg("accept failed (141, naacdrvr.c)\n",0,0,0,0,0,0);
	    return(ERROR);
	}

	if ((cmnd_socket = accept(csocket, 
				  (struct sockaddr *) &clientAddr, 
				  &addrsize)) == ERROR)
	{
	    perror("Accept Failed");
            close(csocket);
	    logMsg("accept failed (151, naacdrvr.c)\n",0,0,0,0,0,0);
	    return(ERROR);
	}

	prtdebug(0xFFFFFFFF, "Entering server\n");
     
	/* Handle server requests from Transputer. */
       if (taskSpawn("tsrvr",SERVER_WORK_PRIORITY,0,SERVER_STACK_SIZE,
          (FUNCPTR)naacSrvr,comm_socket, cmnd_socket,clientAddr,0) == ERROR){
            perror("init and spawning reader and writer");
            close(cmnd_socket);
            close(comm_socket);
            logMsg("unable to spawn reader and writer task (161,naacdrvr,c)\n",
                     0,0,0,0,0,0);
            return (ERROR);
	  };
    }    

    /* close socket and leave */ 
    return(OK);    /* not reached */
}

/******************************************************************************
 * Routine:	fatal()
 * Purpose:	Display error message to user and play dead.
 * Parameters:  dump - bool - TRUE if the message buffer from the Transputer
 *			      should be displayed.
 *		str1 - string -	The first part of the error message to display.
 *		str2 - string - The second part of the error message.
 * Returns:	void
 *****************************************************************************/
void
fatal(dump, str1, str2)
bool	dump;
string	str1;
string	str2;
{
    char buf[180];
    sprintf(buf,"FATAL (naacdrvr.c): %s%s",str1,str2);
    cicsLogMessage(0,buf);
    logMsg("%s\n",buf,0,0,0,0,0);
    exit(ERROR);
}

/*****************************************************************************
 * Routine:	warning()
 * Purpose:	Display warning message to user.
 * Parameters:	str1 - string - The first part of the message to display.
 *		str2 - string - The second part of the message.
 * Returns:	void
 ****************************************************************************/
void
warning(str1, str2)
string	str1;
string	str2;
{
  char tmp[80];
    sprintf(tmp, "WARNING: %s%s\n", str1, str2);
    cicsLogMessage(0,tmp);

}

/******************************************************************************
 * Routine:	handle_args()
 * Purpose:	Handles all command line arguments
 * Parameters:	argc - int - count of the number of arguments
 *		agrv - string * - an array containing the arguiment strings
 * Returns:	void
 *****************************************************************************/
void
handle_args(argc, argv)
int argc;
string argv[];
{

}





