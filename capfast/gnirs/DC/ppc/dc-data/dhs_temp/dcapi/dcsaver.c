
static struct {
        void *v;
        char *c;
} rcsid = {
        &rcsid,
        "$Id: dcsaver.c,v 1.2 2009/05/27 19:32:38 fkraemer Exp $"
};



/*
 * Copyright 1997 Association of Universities for Research in Astronomy, Inc.
 * See the file COPYRIGHT for more details.
 *
 * FILENAME
 * dcsaver.c
 *
 * DESCRIPTION
 * This file and the associated header file implement the "Detector Controler"
 * (DC) API for VxWorks using routines from imhutil.c, which communicate
 * with the sunSaver program on a Solaris machine. A number of support and test
 * routines are also included.
 * 
 * FUNCTION NAME(S):
 * dcInitFits - initalize the DC library (API)
 * dcExitFits - close the DC library (API)
 * 
 * DEPENDENCIES
 *
 *INDENT-OFF*
 * $Log: dcsaver.c,v $
 * Revision 1.2  2009/05/27 19:32:38  fkraemer
 * fkraemer - copied my complete working dir over trunk
 *
 * Revision 1.1.1.1  2001/10/30 21:15:25  mbec
 * checking in
 *
 * Revision 1.1.1.1  2001/10/30 20:25:59  mbec
 *
 *
 * Revision 1.1.1.1  2001/07/18 03:17:04  mbec
 * checkin
 *
 * Revision 1.1.1.1  2001/03/28 07:36:03  gemvx
 * checking in split gn-4
 *
 * Revision 1.1  2000/12/19 00:45:05  mbec
 * adding dhs_temp
 *
 * Revision 1.3  1998/11/30 15:49:54  pruckle
 * cleanup
 *
 * Revision 1.2  1998/11/20 17:13:24  pruckle
 * speed up saver, log messages
 *
 * Revision 1.1.1.1  1998/09/30 16:41:03  pruckle
 * Initial Release
 *
 *INDENT-ON* 
 */

/* NOTES
 
 * 
 * Where the DC specifies "host name or IP address", only the IP address is
 * used and implemented.
 *
 * The DHS DataTypes (DHS_DT_*) are implemented using IRAF type values
 * (from imhutils.h) because the basic structures and the sunSaver program
 * were originally designed around the IRAF format. The default here is to
 * save FITS images, however.
 *
 * The communication with the saver is very straightforward. A socket 
 * connection with the sunSaver server is made, a communications header
 * describing the upcoming data is sent, followed by the data and image
 * headers. The socket is then closed. If there are multiple images in the 
 * dataSet, each image is sent serially with an independent connection to the
 * saver.
 *
 * The default port for communications with the saver is 5557, as set in the 
 * static variable below.
 */

#include <vxWorks.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <taskLib.h>
#include <sysLib.h>
#include <fitsio.h>
#include <carRecord.h>
#include "dcvx.h"
#include "vxSockUtil.h"
#include "epCommon.h"
#include "gnDQSocket.h"	
#include "gnDCADefs.h"
#include "saver.h"

void ckDir(char *dir);
char *imMakeFileName(char *dir, char *base, int type, int filetype, char *filename);
STATUS save_Fits_image(image *im);
STATUS gnGetEpics (char* name,unsigned short type, void *val);
STATUS gnGetEpicsT (char *top,char* name,unsigned short type, void *val);
STATUS gnPutEpics (char* name,unsigned short type, void *val);
STATUS gnPutEpicsT (char *top,char* name,unsigned short type, void *val);

/* local variables*/
static int serverPort = 5557;        /* port to connect to saver server on */

/* static int sfdSaver = ERROR;   */       /* socket file descriptor for saver */
/* static char svrServer[MAXSTRING];   */  /* IP address of saver machine */
/* static char svrName[MAXSTRING];  */     /* name of saver machine */

/* global variables*/
char tmp[80];


/*external variables*/
extern char *dbTop;
extern saverParams svrP;


/*
 *+
 * FUNCTION NAME:
 * dcHdrSet
 *
 * INVOCATION:
 * header *str;
 * char *hdrarray[][2];
 * header = dcHdrSet(hdrarray);
 *
 * PARAMETERS: (">" input, "!" modified, "<" output)
 * > hdrarray (char *[2]) - array of pairs of string header names and values
 *
 * FUNCTION VALUE:
 * header * - pointer to a new header list
 *
 * PURPOSE:
 * Create a header from name/value pairs of strings. Create linked list of header values
 * for fits headers
 *
 * DESCRIPTION:
 * This routine takes a string array of pairs of header names & string values
 * and puts them in a newly malloc'ed header list, returning a pointer to the 
 * list if the last parameter is null. If there is a pointer value in the last
 * parameter, the new list is appended to that one.
 *
 * EXTERNAL VARIABLES:
 * none
 *
 * PRIOR REQUIREMENTS:
 * None
 *
 * DEFICIENCIES:
 * Comments for headers are always set to "".
 *
 * HISTORY:
 * 14-Aug-1997 Original version - Tad Morgan
 * Mar - 2000 modified to allow addition to existing linked list Peter Ruckle
 *-
 */
header *dcHdrSet(char hdrarray[255][2][MAX_STRING_SIZE], header *in)
{
    header *first = NULL,*curr;
    int n;
    int i;
#ifdef DEBUG
    int t2 = 0;
#endif
    
    n = 0;
    i = 1;
    curr = in;
    first = in;
    do
    {
	
	if (first == NULL) /* 1st time initialization */
	{
	    DPRINT(t2,"\ndcHdrSet set first \n");
	    first = (header *) malloc(sizeof(header));
	    if (first == NULL) 
	    {
/* 		openEpicsSocket(); */
		cicsLogMessage(0,"ERROR - dcHdrSet: Error allocating header structure 0\n");
	/* 	closeEpicsSocket(); */
		goto Error;
	    }
	    curr = first;
	}
	else          /* usual case */
	{     	
	    /* find last entry */
	    while ((curr->next != NULL)&&(strcmp(((header*)curr->next)->name,END_HEAD) != 0))
		curr = (header *)curr->next;
	    
	/* allocate header and link the next */
	    if(curr->next == NULL)
		curr->next = malloc(sizeof(header));
	    if(curr->next ==  NULL || (int)curr->next == 0x1f979c4)
	      curr->next = malloc(sizeof(header));	
	    curr = curr->next;	    
	}
	
	i++;
/* 	printf("create pointer = %x\n",curr); */
	/* set all pointers in structure to NULL */  
	curr->next = NULL;
    
	/* set structure values */
	curr->format  = STRING_DT;
	strncpy(curr->name,hdrarray[n][0], MAX_STRING_SIZE);
	strncpy(curr->data,hdrarray[n][1], MAX_STRING_SIZE);
	strncpy(curr->comment,"", MAX_STRING_SIZE);
    }    while (strcmp(hdrarray[n++][0],END_HEAD) != 0) ;

    
    return first;
Error:
    imFreeHdrs(&first);
    printf("error in dcHdrSet");
    first = NULL;
    return first;
}





/*
 *+
 * FUNCTION NAME:
 * dcInitFits
 *
 * INVOCATION:
 * void *connection
 * char *server;
 * char *name;
 * connection = dcInitFitsSaver(server, port, name);
 *
 * PARAMETERS: (">" input, "!" modified, "<" output)
 * > server (char *) - sunSaver machine IP address
 * > name (char *) - sunSaver machine name
 *
 * FUNCTION VALUE:
 * void * to connection ID
 *
 * PURPOSE:
 * Initalize server address and name used by DC
 *
 * DESCRIPTION:
 * Sets the address & name for connections to the DC.
 *
 * EXTERNAL VARIABLES:
 * < sfdSaver (static int) - socket file descriptor for sunSaver connection
 *
 * PRIOR REQUIREMENTS:
 * dcInitSaver be run with port number if port is not default 5557 or last
 * number used.
 *
 * DEFICIENCIES:
 * Only uses IP address of  server, not name.
 *
 * HISTORY:
 * 14-Aug-1997 Original version - Tad Morgan
 *-
 */

long dcInitFits(fitsParams *pParam)
{
    
    /* get socket to Saver on Sun */
/*     printf("dcInitFits port = %d, ip = %s\n",pParam->port, pParam->ip); */

    pParam->sfd = sockConnect(pParam->port, pParam->ip);
    if (pParam->sfd == ERROR)
    {
        sprintf(tmp, "ERROR: dcInitFits - unable to open port %i on machine %s\n",
			pParam->port, pParam->ip);
/* 	cicsLogMessage(0,tmp); */
	printf(tmp);
	return ERROR;
    }
#define SETXFERBUFSIZE
#ifdef SETXFERBUFSIZE
    /*
     * Set the maximum memory that will be used for data
     * transfers.  If this is too small, data transfer can be
     * really slow.  [hty]
     */

    {
	int size;
	int ret;

/*	size = 16384;  If this is too big, it causes an error */
	size = 32768;
	
	ret = setsockopt(pParam->sfd, SOL_SOCKET, SO_SNDBUF,
			 (char *)&size, sizeof(size));
	if (ret == ERROR)
	  {
	   /*  cicsLogMessage(0,"setsockopt Returned error\n"); */ 
	      printf("setsockopt Returned error\n");
	    return ERROR;
	  }
/* 	else */
/* 	    cicsLogMessage(2,"setsockopt OK\n"); */
    }
#endif

/*     printf("dcInitFits OK\n"); */
    return OK;
}

/*
 *+
 * FUNCTION NAME:
 * dcExitFits
 *
 * INVOCATION:
 * void *connect;
 * dcExitFits(connect);
 *
 * PARAMETERS: (">" input, "!" modified, "<" output)
 * ! connect (void *) - pointer to connection (socket file descriptor)
 *
 * FUNCTION VALUE:
 * none
 *
 * PURPOSE:
 * Close the DC library (API).
 *
 * DESCRIPTION:
 * This routine closes the connection to the DC (and thus the socket to the 
 * sunSaver).
 *
 * EXTERNAL VARIABLES:
 * none
 *
 * PRIOR REQUIREMENTS:
 * None
 *
 * DEFICIENCIES:
 * None
 *
 * HISTORY:
 * 14-Aug-1997 Original version - Tad Morgan
 *-
 */
void dcExitFits(int *connect)
{

    sockClose(connect);

}



/*
 *+
 * FUNCTION NAME:
 * getFitsName
 *
 * INVOCATION:
 * STATUS status;
 * void *connect;
 * char *label;
 * int maxsize;
 * status = getFitsName(, label, maxsize);
 *
 * PARAMETERS: (">" input, "!" modified, "<" output)
 *
 * ! label (char *) - string to copy name into
 * > maxsize (int) - maximum string that will fit in label
 *
 * FUNCTION VALUE:
 * STATUS - OK or ERROR [OK]
 *
 * PURPOSE:
 * Get a unique name from server. (API)
 *
 * DESCRIPTION:
 * This routine generates a unique name to save the image as.
 *
 * EXTERNAL VARIABLES:
 * none
 *
 * PRIOR REQUIREMENTS:
 * None
 *
 * DEFICIENCIES:
 *
 *
 * HISTORY:
 * 14-Aug-1997 Original version - Tad Morgan
 * 17-dec-99  added error checking changed name to getFitsName
 *-
 */
int getFitsName(char *label)
{
    int status = OK;
    char buf[20];
    int num;
    /*  strncpy(label, tmpnam(NULL), maxsize); */
  /*   status = openEpicsSocket(); */
    if(status == OK) {
	if(gnGetEpicsT(dbTop, IM_NAME ".VAL", DCASTRING,label) != OK)    {
	    /* print error*/
	    printf("getFitsName Error: couldn't get name from epics\n");
	    status = ERROR;
	}

	/* add number and set new number in imNum*/    
	if(gnGetEpicsT(dbTop, IM_NUM ".VAL", DCALONG,&num) != OK)    {
	    /* print error*/
	    printf("getFitsName Error: couldn't get frame # from epics\n");
	    status = ERROR;
	}
	sprintf(buf,"%04d",num);
	num++;
 
	if(gnPutEpicsT(dbTop, IM_NUM ".VAL", DCALONG,&num) != OK)    {
	    /* print error*/
	    printf("getFitsName Error: couldn't put frame # to epics\n");
	    status = ERROR;
	}
	strcat(label,buf);
/* 	closeEpicsSocket(); */
    }
    else
	printf("Error opening epics socket in getfitsname\n");
    return status;
}



/*
 *+
 * FUNCTION NAME:
 * dcSetPort
 *
 * INVOCATION:
 * int port;
 * dcSetPort(port);
 *
 * PARAMETERS: (">" input, "!" modified, "<" output)
 * > port (int)  port number of saver on Sun
 *
 * FUNCTION VALUE:
 * none
 *
 * PURPOSE:
 * Set the port number of saver server program. 
 *
 * DESCRIPTION:
 * This routine sets the port number used by the other routines to connect
 * the sunSaver server program.
 *
 * EXTERNAL VARIABLES:
 * < serverPort (static int)
 *
 * PRIOR REQUIREMENTS:
 * None
 *
 * DEFICIENCIES:
 * None
 *
 * HISTORY:
 * 14-Aug-1997 Original version - Tad Morgan
 *-
 */
void dcSetPort(int port)
{

    serverPort = port;

}
