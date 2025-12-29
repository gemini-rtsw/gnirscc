
static struct {
        void *v;
        char *c;
} rcsid = {
        &rcsid,
        "$Id: gnSocketIntrfc.c,v 1.2 2009/05/27 19:32:39 fkraemer Exp $"
};

 
/* #define DEBUG  */
/*****************************************************************************
 * Copyright 1997 Association of Universities for Research in Astronomy, Inc. 
 * See the file COPYRIGHT for more details.
 *
 * Filename: 	
 * 	gnEPICSIntrfc.c
 *
 * Description:
 * 	This file contains the EPICS interface routines for the final gnaac
 *		data coadder program.
 *
 * Function name(s)
 *	setTop - Set the database prefix
 *	gnSetEPICSName - set the name of the epics computer
 * 	gnGetGNAACParams - uses Channel access to get the appropriate
 *		observation parameters from the EPICS IOC into the Data coadder
 *		processor 
 *	gnOpenSocket - Opens a socket to the machine running EPICS. 
 *      gnGetROIVal - get roi values from epics
 *	gnGetEpicsT - get an epics variable 
 *	gnPutEpicsT   - put a value into an epics variable
 *	getWCS - Get wcs variables from epics
 *	cicsLogMessage - send a message to the cics logger

 * Dependencies
 * 	
 *
 * Orginial Author:
 *	Nick C. Buchholz
 *
 * History:
 *	11-Jun-1997: Created original version - ncb
 *
 ***************************************************************************/
/* #define DHS_SAVER */
/* includes*/
#include <sys/types.h> 
#include <sys/times.h> 

#include "bc350Time.h"

#include <stdio.h>
#include <vxWorks.h>  
#include <ioLib.h>    
#include <pipeDrv.h>  
#include <cadRecord.h>
#include <cadef.h> 
#define NODBACCESS
#define DQ_IOC
#include <epCommon.h>
#include <gnerrno.h>
#include "gnDCADefs.h"
#define EPICS
#undef MAIN
#include "gnDCAVars.h"

#include "gnDQSocket.h"
#include "sockutil.h"
#include "saver.h"
#include "localWcs.h"

/* externs*/
extern MSG_Q_ID sendQ,receiveQ;

extern int dqDebug;
extern saverParams svrP;

/* globals*/
static SEM_ID socketSem;/* mutual exclusion semaphore*/
static int gnEPICSSocket = 0;/* socket descriptor*/
static int gnEPICSPort = 5301;/* port to connect to*/
static char gnEPICSName[80];/* machine to connect to*/

int gnOpenSocket(char *name,int port);

/* function prototypes*/
void sleep (int a, int b);
void closeSocket()
{

    dcaMsgStruct msg,ret;

    msg.op = DCACLOSESOCKET;
    msgQSend(sendQ,(char *)&msg,sizeof(msg),NO_WAIT,
	     MSG_PRI_NORMAL); 
    msgQReceive(receiveQ,(char *)&ret,sizeof(ret),
		WAIT_FOREVER);
    printf("return message = %ld\n",ret.op);
    
}
int epicsServer()
{
    int retVal;
#ifdef DEBUG
    int t1 = 1;
#endif
    dcaMsgStruct msg,ret;
    int connected = OK;
    sendQ = msgQCreate( 2,sizeof(dcaMsgStruct),MSG_Q_FIFO);
    receiveQ = msgQCreate( 2,sizeof(dcaMsgStruct),MSG_Q_FIFO);
    
    /*loop forever*/
    while(1)    
	{
		DPRINT( t1,"waiting for new connection\n");
		printf ("epicsServer waiting for new connection ...\n");
		/* 	connected = gnOpenSocket(gnEPICSName,gnEPICSPort); */
		connected = OK;
		gnEPICSSocket = 0;
		while (connected != ERROR)	
		{
			
			msgQReceive(sendQ,(char *)&msg,sizeof(msg),WAIT_FOREVER);
			
		/* 	printf("\n\nmsg size = %d\n",sizeof(msg));	 */
/* 			printf("\nop size = %d\n",sizeof(msg.op)); */
/* 			printf("\nid size = %d\n",sizeof(msg.id)); */
/* 			printf("\nstatus size = %d\n",sizeof(msg.status)); */
/* 			printf("\nname size = %d\n",sizeof(msg.name)); */
/* 			printf("\ntype size = %d\n",sizeof(msg.type)); */
/* 			printf("\nval size = %d\n",sizeof(msg.val)); */
			if(gnEPICSSocket == 0)
			{
				printf("reconnecting to socket %s %d\n",gnEPICSName,gnEPICSPort);
				connected = gnOpenSocket(gnEPICSName,gnEPICSPort);
				printf("socket connected\n");
			}
			
			retVal = sockWrite(gnEPICSSocket,(char *)&msg,
							   sizeof(msg));
			
			if(retVal == OK)
				retVal = sockRead(gnEPICSSocket,(char *)&ret,
								  sizeof(ret));
			
			if(retVal == OK)
			{
				msgQSend(receiveQ,(char *)&ret,sizeof(ret),NO_WAIT,
						 MSG_PRI_NORMAL);
				
			}
			else
			{
				if(msgQSend(sendQ,(char *)&msg,sizeof(msg),NO_WAIT,
							MSG_PRI_NORMAL) == ERROR)
				{
					printf("msgQ error\n");
					ret.status = ERROR;
					msgQSend(receiveQ,(char *)&ret,sizeof(ret),NO_WAIT,
							 MSG_PRI_NORMAL);
					
					
					
				} 
				printf("closing socket\n");
				connected = ERROR;
				closeEpicsSocket();
			}
			
		}
		
		taskDelay(60);
		printf ("connection dropped, trying again...\n");
		
	}
}
void sendSocket ()
{
    dcaMsgStruct msg,ret;

    msg.op = 12;
    msgQSend(sendQ,(char *)&msg,sizeof(msg),NO_WAIT,
	     MSG_PRI_NORMAL); 
    printf("sendSocket:waiting for return message\n");
    msgQReceive(receiveQ,(char *)&ret,sizeof(ret),
			WAIT_FOREVER);
    printf("sendSocket:return message = %ld\n",ret.op);
    
    
}
int openEpicsSocket()
{
    int status; 
    status = gnOpenSocket(gnEPICSName,gnEPICSPort);
  
  
    return status;
}
int closeEpicsSocket()
{
    int status = OK;
    dcaMsgStruct msg;
    msg.op = DCACLOSESOCKET;
/*     sockWrite(gnEPICSSocket,(char *)&msg,sizeof(dcaMsgStruct)); */
/*     sockRead(gnEPICSSocket,(char *)&ret,sizeof(dcaMsgStruct)); */
     sockClose(&gnEPICSSocket);
  
   return status;
}
/* set an epics car record*/
long setCar(char *name,long ival,long ierr,char *errString,char *error)
{
  int status = OK;
  char record[80];
  /*     status = openEpicsSocket(); */
  if(status == OK) {
	sprintf(record,"%s.IERR",name);

	status = gnPutEpicsT(dbTop, record, DCALONG,   &ierr);  
	sprintf(record,"%s.IMSS",name);


	if(status == OK)
      status = gnPutEpicsT(dbTop, record, DCASTRING, errString); 
	sprintf(record,"%s.IVAL",name); 

	if(status == OK)
      gnPutEpicsT(dbTop, record, DCALONG, &ival); 
    /* 	closeEpicsSocket(); */
  }
  if(status != OK)
    {
      return ERROR;
      printf("Error opening socket in savedhsdata\n");
    }
  return OK;
}

int socktest = 1;
int socketTest()
{
    int status;
    char buf[80];
    struct time_struct b,a;
    float time;

    socktest = 1;
    while (socktest)
    {
	
		getTime(&a); 
		status = gnGetEpicsT(dbTop, NUM_LNRS ".VAL",DCALONG, &numLNRs);
		getTime(&b); 
		time = (b.msec -a.msec)+1000000*((b.sec-a.sec) + 60*((b.min-a.min) + 60*(b.hour-a.hour)));
		time = time /1000000;
		sprintf(buf,"time = %f\n",time);
		sleep(2,0);
    }
    return OK;
}
/*****************************************************************************
 * Function name:
 * setTop
 *
 * Invocation:
 * setTop (name)
 *
 * PARAMETERS:
 *	char *name - name of database prefix
 *
 * FUNCTION VALUE:
 * 	void
 *
 * PURPOSE:
 *    This function sets the default database to be the one specified in the 
 *  parameter.
 *
 * DESCRIPTION:
 *     The dbTop and dbSadTop variable is set to the parameter top
 *
 * EXTERNAL VARIABLES:
 * 	Lots - see gnaaDCAVars.h
 *
 * PRIOR REQUIREMENTS:
 *
 *
 * DEFICIENCIES:
 * 	None known
 *
 * HISTORY:
 * 	11-June-1997  Original version  N Buchholz
 *
 *****************************************************************************/

void setTop(char *top,char *sadtop)
{
    if(dbTop != NULL)
	free (dbTop);
     if(dbSadTop != NULL)
	free (dbSadTop);
    dbSadTop = malloc(strlen(sadtop) + 1);
    strncpy(dbSadTop,sadtop,MAX_STRING_SIZE);

    dbTop = malloc(strlen(top) + 1);
    strcpy(dbTop,top);
}


/*****************************************************************************
 * Function name:
 * 	gnOpenSocket
 *
 * Invocation:
 * 	status = gnOpenSocket(char *name, int port  );
 *
 * PARAMETERS:
 *	name - machine name to connect to
 *      port  - port number to connect to (defaults to 5301)
 *
 * FUNCTION VALUE:
 * 	int - Status value returned by function 
 *
 * PURPOSE:
 * 	Opens a socket to the machine running EPICS. 
 *
 * DESCRIPTION:
 *      A socket is created using the name and socket given in the parameters.
 *   If no port is given, the port is asigned to 5301 initially.  This is the 
 *   client side of the socket.
 *
 * EXTERNAL VARIABLES:
 * 	gnEPICSSocket - the socket descriptor
 *
 * PRIOR REQUIREMENTS:
 *    
 *
 * DEFICIENCIES:
 * 	None known
 *
 * HISTORY:
 * 	15-May-1998  Original version  P Ruckle
 *
 *****************************************************************************/

int gnOpenSocket(char *name,int port)
{
  
 
    int sfd = ERROR;

    if(port == 0)
    {
		port = 5301;
    }
    while (sfd == ERROR)
    {
		sfd = sockConnect(port,name);
		taskDelay(60*4);
		if (sfd == ERROR) 
		{
			printf ("gnOpenSocket error %s\n",name);
		}
    }
    strncpy (gnEPICSName,name,80);
    gnEPICSSocket = sfd;
    gnEPICSPort = port;
    return OK;
}
/*****************************************************************************
* Function name:
 * gnSetEPICSName
 *
 * Invocation:
 * gnSetEPICSName (name)
 *
 * PARAMETERS:
 *	char *name - computer that is running epics 
 *
 * FUNCTION VALUE:
 * 	void
 *
 * PURPOSE:
 *    Set the name of the machine to connect to.  Where the epics
 * database is running.
 *
 * DESCRIPTION:
 *     Set the name of the machine to connect to.  Where the epics
 * database is running.
 *
 * EXTERNAL VARIABLES:
 * 	gnSetEPICSName - 
 *
 * PRIOR REQUIREMENTS:
 *
 *
 * DEFICIENCIES:
 * 	None known
 *
 * HISTORY:
 * 	11-June-1997  Original version  N Buchholz
 *
 *****************************************************************************/

void gnSetEPICSName(char *name)
{
    strncpy(gnEPICSName,name,80);
    socketSem = semBCreate(SEM_Q_PRIORITY,SEM_FULL);
  
}


/*****************************************************************************
 * Function name:
 * 	gnGetGNAACParams
 *
 * Invocation:
 * 	status = gnGetGNAACPArams(  );
 *
 * PARAMETERS:
 *	None
 *
 * FUNCTION VALUE:
 * 	int - Status value returned by function 
 *
 * PURPOSE:
 * 	obtains a full set of parameters needed by the Data coadderprogram from
 *		the EPICS IOC.
 *
 * DESCRIPTION:
 * 	Uses gnGetEpicsT to obtain the values of the important data coadder
 *		variables stored in the EPICS IOC. This includes ucode
 *		description, operation modes, header data, ROI descriptions, 
 *		etc.  These values are stored in 'C' for
 *		use in controlling the Data coadder
 *
 * EXTERNAL VARIABLES:
 * 	Lots - see gnaaDCAVars.h
 *
 * PRIOR REQUIREMENTS:
 *
 *
 * DEFICIENCIES:
 * 	None known
 *
 * HISTORY:
 * 	11-June-1997  Original version  N Buchholz
 *
 *****************************************************************************/
int dq_debug;
int dlCount;
int gnGetGNAACParams(int *setupNeeded,int *acqSetupNeeded)
{
#ifdef DEBUG
    int t1 = 1;
#endif
  
    static int firstTime = TRUE;
    int retVal = OK;
    int status = (ECA_NORMAL);
    int error, idx;
    char type [MAX_STRING_SIZE];
    int aSz, nLs, nCA, pM, hT, hD, fPC, uCT, nDs, nPic;
    int dCount;
    char odhsDatalabel[MAX_STRING_SIZE+1];
	
    /* save old values for later checking to see if anything has changed */
    if(retVal != ERROR)    {
	if (firstTime)    {
	    aSz = nLs = nCA = pM = hT = hD = fPC = uCT = nDs = nPic = dCount = -1;        
	}
	else     {
	    aSz = arSize; 
	    nLs = numLNRs;       
	    strcpy (type,detType);    
	    dCount = dlCount;
/* 	    strcpy (ucName,); */
	    nCA = numCoAdds;       
	    pM = svrP.procMode;        
	    hT = hdrTiming;       
	    hD = hdrDetail;       
	    fPC =framesPerCycle;  
	    uCT = uCodeType;       
	    nDs = svrP.roi.numRois;
	    nPic = numPics;
	}
    
/* 	printf("gnGetGNAACParams: 3  format = %d\n",svrP.param.dhs.format); */
	/*     cicsLogMessage(3,"Getting gnaac parameters\n"); */
  
	strncpy(odhsDatalabel,svrP.param.dhs.dhsDatalabel, MAX_STRING_SIZE);
/* 	DPRINT(t1,"get first epics record\n"); */
	status |= gnGetEpicsT(dbTop, OBSSETUP_CAD ".VALA",DCALONG, &arSize);
	/* get current array size*/
	idx=0;
	while (arSize != arsize[idx] && idx++ < 4)
	    ;
	if (idx < 4) arSizeIdx = idx;
	status |= gnGetEpicsT(dbTop, DRROISET_CAD ".VALA",DCALONG, 
			      &(svrP.roi.numRois));
	status |= gnGetEpicsT(dbSadTop, DL_COUNT ".VAL",DCALONG, 
			      &dlCount);
	status |= gnGetEpicsT(dbTop, NUM_LNRS ".VAL",DCALONG, &numLNRs);

	status |= gnGetEpicsT(dbTop, DET_TYPE ".VAL",DCASTRING, detType);
	status |= gnGetEpicsT(dbTop, NUM_COADDS ".VAL",DCALONG, &numCoAdds);
	status |= gnGetEpicsT(dbTop, PROC_MODE ".VAL",DCALONG, &svrP.procMode);
	status |= gnGetEpicsT(dbTop, OBSSETUP_CAD ".VALN",DCALONG, &hdrTiming);
	status |= gnGetEpicsT(dbTop, HDR_DETAIL ".VAL",DCALONG, &hdrDetail);
	status |= gnGetEpicsT(dbTop, UC_FRMSPCYCLE ".VAL",DCALONG, 
			      &framesPerCycle);
	status |= gnGetEpicsT(dbTop, UC_CODETYPE ".VAL",DCALONG, &uCodeType);
  
	status |= gnGetEpicsT(dbTop, OBSERVE_CAD ".VALA",DCASTRING, svrP.param.dhs.dhsDatalabel);
	status |= gnGetEpicsT(dbTop, IM_PATH,DCASTRING, svrP.param.fits.pixeldir);
 

	status |= gnGetEpicsT(dbTop,OBSSETUP_CAD ".VALH",DCALONG, &svrP.param.dhs.headers);
	status |= gnGetEpicsT(dbTop,SETDHSINFO_CAD ".VALA",DCASTRING, svrP.param.dhs.qlStream);
	status |= gnGetEpicsT(dbTop,SETDHSINFO_CAD ".VALB",DCALONG, &svrP.param.dhs.lifetime);
	status |= gnGetEpicsT(dbTop,SETDHSINFO_CAD ".VALC",DCALONG, &svrP.param.dhs.format);

	status |= gnGetEpicsT(dbTop,  NUM_PICS ".VAL",DCALONG, &numPics);

#ifdef TRACE
    printf("numRois = %d\n",svrP.roi.numRois);
#endif
	
	switch (svrP.roi.numRois)    {
	  case 4: status |= gnGetROIVal("4", &(svrP.roi.roi[3]));
	  case 3: status |= gnGetROIVal("3", &(svrP.roi.roi[2]));
	  case 2: status |= gnGetROIVal("2", &(svrP.roi.roi[1]));
	  case 1: status |= gnGetROIVal("1", &(svrP.roi.roi[0]));
	
	    break;
	  case 0:
	    /* if no rois are defined, use the whole array*/
	    svrP.roi.numRois = 1;
	    svrP.roi.roi[0].lowX = 0;
	    svrP.roi.roi[0].lowY = 0;
		
	    if((strcmp(A2,detType) == 0) || (strcmp(A3_A,detType) == 0))
		{
			svrP.roi.roi[0].hiX = arsize[arSizeIdx]-1;
			svrP.roi.roi[0].hiY =  arsize[arSizeIdx]- 1;
			svrP.roi.roi[0].rows = arsize[arSizeIdx];
			svrP.roi.roi[0].cols = arsize[arSizeIdx];
		}
	    else if(strcmp(A3_B,detType) == 0)
		{
			svrP.roi.roi[0].hiX = a3csize[arSizeIdx]-1;
			svrP.roi.roi[0].hiY =  a3rsize[arSizeIdx]- 1;
			svrP.roi.roi[0].rows = a3rsize[arSizeIdx];
			svrP.roi.roi[0].cols = a3csize[arSizeIdx];
		}
	    else
		{
			status = ERROR;
			printf ("unsupported detector %s\n",detType);
		}
	    break;
	}
	if (status != ECA_NORMAL)    {
	    cicsLogMessage(0,"gnGetEpics error: Data coadder Parameters not set \n");
	    error = DQ_GETPARAM_ERR;
		/*     gnPutEpicsT( dbTop, OBSERVE_CAR ".IERR",DCALONG ,&error);  */
	    status = DQ_GETPARAM_ERR;
		/* 	printf("gnGetGNAACParams: 5  format = %d\n",svrP.param.dhs.format); */
	    return (status);
	}
	*acqSetupNeeded = (aSz != arSize) || (dCount != dlCount);
	*setupNeeded =
	    (int) ((aSz != arSize) || (pM != svrP.procMode) || (fPC != framesPerCycle) || (dCount != dlCount) ||
			   (uCT != uCodeType) || (strcmp(type, detType) != 0) || (*setupNeeded));
	
	firstTime = FALSE;
	/* 	closeEpicsSocket(); */
    }
    else 
	{
		printf("error opening socket in gngetgnaacparams\n");
    }
	/* 	printf("gnGetGNAACParams: 6  format = %d\n",svrP.param.dhs.format); */
    return (retVal);

}
/*****************************************************************************
 * Function name:
 * gnGetROIVal
 *
 * Invocation:
 * int gnGetROIVal(char *N, tRect *Rect);
 *
 * PARAMETERS:
 *	char *N - roi number in character format
 *      tRect *Rect - rectangle limits for this roi
 *
 * FUNCTION VALUE:
 * 	int - Status value returned by function 
 *
 * PURPOSE:
 * get the roi limits from the epics data.
 *
 * DESCRIPTION:
 * 
 *
 * EXTERNAL VARIABLES:
 * 	Lots - see gnaaDCAVars.h
 *
 * PRIOR REQUIREMENTS:
 *
 *
 * DEFICIENCIES:
 * 	None known
 *
 * HISTORY:
 * 	11-June-1997  Original version  N Buchholz
 *
 *****************************************************************************/
 
int gnGetROIVal(char *N, tRect *Rect)
{
    int status = (ECA_NORMAL);
    char buf[80];
    char name[MAX_NAME_SIZE+1];

    /* create string and get values for each parameter in ROI N */
    sprintf(name, "%s%s%s.VAL",dbTop, LOWCOL, N); 
    status |= gnGetEpics(name,DCALONG, &(Rect->lowX));
/*     printf("%s = %d, status = %d\n",name,Rect->lowX,status); */
    sprintf(name, "%s%s%s.VAL",dbTop, LOWROW, N); 
    status |= gnGetEpics(name,DCALONG, &(Rect->lowY));
/*     printf("%s = %d, status = %d\n",name,Rect->lowY,status); */
    sprintf(name, "%s%s%s.VAL",dbTop, HICOL, N); 
    status |= gnGetEpics(name,DCALONG, &(Rect->hiX));
/*     printf("%s = %d, status = %d\n",name,Rect->hiX,status); */
    sprintf(name, "%s%s%s.VAL",dbTop, HIROW, N); 
    status |= gnGetEpics(name,DCALONG, &(Rect->hiY));
/*     printf("%s = %d, status = %d\n",name,Rect->hiY,status); */

    Rect->cols = Rect->hiY - Rect->lowY +1;
    Rect->rows = Rect->hiX - Rect->lowX +1;
    sprintf(buf,"Got roi values for roi[%s]\n",N);
    sprintf(buf,"lowx = %ld, lowy = %ld, hiX = %ld, hiY = %ld\n",Rect->lowX,Rect->lowY,Rect->hiX,Rect->hiY);
    return (status);
} 


/*****************************************************************************
 * Function name:
 *	gnGetEpicsT
 * 
 *
 * Invocation:
 * STATUS  = gnGetEpicsT(char *top,char* name,unsigned short type, void *val)
 *
 * PARAMETERS:
 *	char *top - epics database prefix
 *      char* name - name of epics record
 *      unsigned short type - type stored in record (check gnGetEpics for 
 *                            allowable types
 *      void *val - value of the record
 *
 * FUNCTION VALUE:
 * 	int - Status value returned by function 
 *
 * PURPOSE:
 * Get a value from the epics database
 *
 * DESCRIPTION:
 * This just appends the top and name and calls gnGetEpics which does the work
 *
 * EXTERNAL VARIABLES:
 * 	
 *
 * PRIOR REQUIREMENTS:
 *
 *
 * DEFICIENCIES:
 * 	None known
 *
 * HISTORY:
 * 	june 98 pbr
 *
 *****************************************************************************/


STATUS gnGetEpicsT(char *top,char* name,unsigned short type, void *val)
{
    char n[80];


    strncpy(n,top,MAX_STRING_SIZE);
    strcat(n,name);
    return gnGetEpics(n,type,val);

}/*****************************************************************************
 * Function name:
 * gnPutEpicsT
 *
 * Invocation:
 * STATUS gnPutEpicsT(char *top,char* name,unsigned short type, void *val)
 *
 * PARAMETERS:
 *	char *top - epics database prefix
 *      char* name - name of epics record
 *      unsigned short type - type stored in record (check gnGetEpics for 
 *                            allowable types
 *      void *val - value of the record
 *
 * FUNCTION VALUE:
 * 	int - Status value returned by function 
 *
 * PURPOSE:
 * Put a value in the epics database
 *
 * DESCRIPTION:
 *This just appends the top and name and calls gnPutEpics which does the work
 *
 * EXTERNAL VARIABLES:
 * 	Lots - see gnaaDCAVars.h
 *
 * PRIOR REQUIREMENTS:
 *
 *
 * DEFICIENCIES:
 * 	None known
 *
 * HISTORY:
 * 	11-June-1997  Original version  N Buchholz
 *
 *****************************************************************************/

STATUS gnPutEpicsT(char *top,char* name,unsigned short type, void *val)
{
    char n[80];


    strncpy(n,top,MAX_STRING_SIZE);
    strcat(n,name);
    return gnPutEpics(n,type,val);

}/*****************************************************************************
 * Function name:
 * gnGetEpics
 *
 * Invocation:
 * STATUS gnGetEpics (char* name,unsigned short type, void *val)
 *
 * PARAMETERS:
 *	char* name - name of epics record
 *      unsigned short type - type stored in record (check gnGetEpics for 
 *                            allowable types
 *      void *val - value of the record
 *
 * FUNCTION VALUE:
 * 	int - Status value returned by function 
 *
 * PURPOSE:
 * Get a value from the epics database
 *
 * DESCRIPTION:
 *   This routine opens a socket to a server running on the EPICS machine, 
 * sends a request for a value, and waits for the response
 *
 * EXTERNAL VARIABLES:
 *    socketSem - mutual exclusion sem
 *
 * PRIOR REQUIREMENTS:
 *
 *
 * DEFICIENCIES:
 * 	None known
 *
 * HISTORY:
 * 	june 97 pbr
 *      june 98 modified to use sockets instead of channel access
 *
 *****************************************************************************/

STATUS gnGetEpics (char* name,unsigned short type, void *val)
{
#ifdef DEBUG
    int t1 = 1;
#endif
   
    int retVal;
    char tempbuf[80];
    dcaMsgStruct msg,ret;

    dqDebug = 0;
    sprintf(tempbuf,"\n\ngnGetEpics name = %s \n",name);
	DPRINT(t1,"waiting for socketSem gngetepics\n");
    semTake(socketSem,WAIT_FOREVER); 
    DPRINT(t1,"got socket\n");
  /*   gnOpenSocket(gnEPICSName,gnEPICSPort); */
    
    /* set structure values to send through socket*/
    msg.type = type;
    msg.op = DCAREAD;
    strncpy(msg.name,name,MAX_STRING_SIZE);
    /* send request*/
    retVal = msgQSend(sendQ,(char *)&msg,sizeof(msg),NO_WAIT,MSG_PRI_NORMAL);
   
    /*    retVal = sockWrite(gnEPICSSocket,(char *)&msg,sizeof(dcaMsgStruct)); */
    /* wait for reply*/
    if(retVal != ERROR)
    {
/* 	printf("waiting for reply\n"); */
	retVal = msgQReceive(receiveQ,(char *)&ret,sizeof(ret),WAIT_FOREVER);
	/* 	retVal = sockRead(gnEPICSSocket,(char *)&ret,sizeof(dcaMsgStruct)); */
	if(retVal != ERROR) {
	    if(ret.status == OK)
		switch (msg.type)
		{
		  case DCALONG:
		    *(long *)val = ret.val.l;
  /*   printf("gnGetEpics: %s = %d, status = %d\n",name,ret.val.l,ret.status); */
	
		    break;
		  case DCADOUBLE:
		    *(double *)val = ret.val.d;
  /*   printf("gnGetEpics: %s = %f, status = %d\n",name,ret.val.d,ret.status); */
	  
		    break;
		  case DCASTRING:
		    strncpy((char *)val,ret.val.s,MAX_STRING_SIZE);
  /*   printf("gnGetEpics: %s = %s, status = %d\n",name,ret.val.s,ret.status); */
	   
		    break;
		  default:
		    DPRINT(1,"bad type in gnGetEpics");
	  
		}
	}
    }
/*     sockClose(&gnEPICSSocket); */
/*     printf("status = %d\n\n\n",ret.status); */
    semGive(socketSem);
    if(retVal == ERROR)
    {
		printf("gnGetEpics error %s\n",name);
		ret.status = retVal;
    }
    return ret.status;
    
}
/*****************************************************************************
 * Function name:
 * gnPutEpics
 *
 * Invocation:
 * STATUS gnPutEpics (char* name, unsigned short type, void *val)
 *
 * PARAMETERS:
 *	char* name - name of epics record
 *      unsigned short type - type stored in record (check gnGetEpics for 
 *                            allowable types
 *      void *val - value of the record
 *
 * FUNCTION VALUE:
 * 	int - Status value returned by function 
 *
 * PURPOSE:
 * Put a value in the epics database
 *
 * DESCRIPTION:
 * This routine opens a socket to a server running on the EPICS machine, 
 * sends a record to update, and waits for the response
 *
 * EXTERNAL VARIABLES:
 *      socketSem mutual exclusion sem
 *
 * PRIOR REQUIREMENTS:
 *
 *
 * DEFICIENCIES:
 * 	None known
 *
 * HISTORY:
 * 	11-June-1997  Original version  N Buchholz
 *
 *****************************************************************************/

STATUS gnPutEpics (char* name, unsigned short type, void *val)
{
    int retVal;
#ifdef DEBUG
    int t1 = 1;
#endif
  
    char buf[80];
    dcaMsgStruct msg,ret;

    sprintf(buf,"\n\ngnPutEpics name = %s \n",name);

    semTake(socketSem,WAIT_FOREVER); 
  
   /*  gnOpenSocket(gnEPICSName,gnEPICSPort); */
  
    msg.type = type;
    strncpy(msg.name,name,MAX_STRING_SIZE);
    msg.op = DCAWRITE;

    switch (msg.type)
    {
	  case DCAUSHORT:
/* 		printf("val = %d\n",msg.val.us); */	
		msg.val.us =*(unsigned short *)val;
      case DCALONG:
		msg.val.l=*(long *)val;
		sprintf(buf,"gnPutEpics: name = %s, val = %ld\n",name,msg.val.l);
		DPRINT(t1,buf);
		break;
      case DCADOUBLE:
		msg.val.d = *(double *)val;
		sprintf(buf,"gnPutEpics: name = %s,val = %f\n",name,msg.val.d);
		DPRINT(t1,buf);
		break;
      case DCASTRING:
		strncpy(msg.val.s,(char *)val,MAX_STRING_SIZE);
		sprintf(buf,"gnPutEpics:  name = %s,val = %s\n",name,msg.val.s);
		DPRINT(t1,buf);
		break;
      default:
		DPRINT(1,"bad type in gnPutEpics");
	  
    }
    /* send request*/
    retVal = msgQSend(sendQ,(char *)&msg,sizeof(msg),NO_WAIT,MSG_PRI_NORMAL);
	/*   retVal = sockWrite(gnEPICSSocket,(char *)&msg,sizeof(dcaMsgStruct)); */
    /* wait for status*/
    if(retVal != ERROR)
		retVal = msgQReceive(receiveQ,(char *)&ret,sizeof(ret),WAIT_FOREVER);
	/*  retVal = sockRead(gnEPICSSocket,(char *)&ret,sizeof(dcaMsgStruct)); */
    else
		printf("Error writing message to epics %s\n",msg.name);
	/*     sockClose(&gnEPICSSocket); */
    semGive(socketSem);
 
    if(retVal != ERROR)
    {
		return ret.status;
    }
    printf("gnputEpics error %s\n",name);
    return retVal;

}  
/*****************************************************************************
 * Function name:
 * getEpics
 *
 * Invocation:
 * STATUS getEpics (char* name, void *val,int count,void **ch)
 *
 * PARAMETERS:
 *	char* name - name of epics record
 *     
 *      void *val - value of the record
 *      int count - number of elements if an array, otherwise 1
 *       static void *ch -  channel access id

 *
 * FUNCTION VALUE:
 * 	int - Status value returned by function 
 *
 * PURPOSE:
 * Get a channel access value from the epics database
 *
 * DESCRIPTION:
 *   This routine opens a socket to a server running on the EPICS machine, 
 * sends a request for a value, and waits for the response
 *
 * EXTERNAL VARIABLES:
 *    socketSem - mutual exclusion sem
 *
 * PRIOR REQUIREMENTS:
 *
 *
 * DEFICIENCIES:
 * 	None known
 *
 * HISTORY:
 * 	june 97 pbr
 *      june 98 modified to use sockets instead of channel access
 *
 *****************************************************************************/

STATUS getEpics (char* name, void *val, int count ,int id, unsigned short type)
{
#ifdef DEBUG
    int t1 = 1;
#endif
   
    int retVal;
    char tempbuf[80];
    dcaMsgStruct msg,ret;

    dqDebug = 0;
    sprintf(tempbuf,"\n\ngnGetEpics name = %s \n",name);
	DPRINT(0,"waiting for socketSem gngetepics\n");
    semTake(socketSem,WAIT_FOREVER); 
    DPRINT(0,"got socket\n");
  /*   gnOpenSocket(gnEPICSName,gnEPICSPort); */
    
    /* set structure values to send through socket*/
	strcpy (msg.name,name);
	msg.id = id;
    msg.type = type;
    msg.op = CAREAD;
/* 	printf("getEpics: op = %d,id = %d, name = %s, type = %d, LONG = %d, double = %d, string = %d\n",msg.op,msg.id,msg.name,msg.type,DCALONG,DCADOUBLE,DCASTRING); */
/*     strncpy(msg.name,name,MAX_STRING_SIZE); */
    /* send request*/
    retVal = msgQSend(sendQ,(char *)&msg,sizeof(msg),NO_WAIT,MSG_PRI_NORMAL);
   
    /*    retVal = sockWrite(gnEPICSSocket,(char *)&msg,sizeof(dcaMsgStruct)); */
    /* wait for reply*/
    if(retVal != ERROR)
    {
/* 		printf("getEpics Waiting for reply\n"); */
		retVal = msgQReceive(receiveQ,(char *)&ret,sizeof(ret),WAIT_FOREVER);
		/* 	retVal = sockRead(gnEPICSSocket,(char *)&ret,sizeof(dcaMsgStruct)); */
/* 		printf("getEpics: retVal = %d, status = %d\n",retVal,ret.status); */
		if(retVal != ERROR) 
		{
			if(ret.status == OK)
				switch (msg.type)
				{
				  case DCALONG:
					*(long *)val = ret.val.l;
/* 					printf("getEpics: %s = %d, status = %d\n",name,ret.val.l,ret.status);  */
					
					break;
				  case DCADOUBLE:
					*(double *)val = ret.val.d;
/* 					printf("getEpics: %s = %f, status = %d\n",name,ret.val.d,ret.status); */
					
					break;
				  case DCASTRING:
					strncpy((char *)val,ret.val.s,MAX_STRING_SIZE);
/* 					printf("getEpics: %s = %s, status = %d\n",name,ret.val.s,ret.status); */
					
					break;
				  default:
					DPRINT(1,"bad type in gnGetEpics");
					
				}
		}
    }
	/*     sockClose(&gnEPICSSocket); */
	/*     printf("status = %d\n\n\n",ret.status); */
    semGive(socketSem);
    if(retVal == ERROR)
    {
		printf("getEpics error %s\n",name);
		ret.status = retVal;
    }
    return ret.status;
    
}
/*****************************************************************************
 * Function name:
 * putEpics
 *
 * Invocation:
 * STATUS putEpics (char* name, void *val,int count,void **tmp, unsigned short type)
 *
 * PARAMETERS:
 *	char* name - name of epics record
 *      unsigned short type - type stored in record (check gnGetEpics for 
 *                            allowable types
 *      void *val - value of the record
 *      int count - number of elements if an array, otherwise 1
 *      static void *ch -  channel access id
 *
 * FUNCTION VALUE:
 * 	int - Status value returned by function 
 *
 * PURPOSE:
 * Put a channel access value in the epics database
 *
 * DESCRIPTION:
 * This routine opens a socket to a server running on the EPICS machine, 
 * sends a record to update, and waits for the response
 *
 * EXTERNAL VARIABLES:
 *      socketSem mutual exclusion sem
 *
 * PRIOR REQUIREMENTS:
 *
 *
 * DEFICIENCIES:
 * 	None known
 *
 * HISTORY:
 * 	11-June-1997  Original version  N Buchholz
 *
 *****************************************************************************/

STATUS putEpics (char* name, void *val, int id, unsigned short type)
{
    int retVal;
#ifdef DEBUG
    int t1 = 1;
#endif
  
    char buf[80];
    dcaMsgStruct msg,ret;

    sprintf(buf,"\n\ngnPutEpics name = %s \n",name);

    semTake(socketSem,WAIT_FOREVER); 
  
   /*  gnOpenSocket(gnEPICSName,gnEPICSPort); */
	msg.id = id;
    msg.type = type;
    strncpy(msg.name,name,MAX_STRING_SIZE);
    msg.op = CAWRITE;

    switch (msg.type)
    {
	  case DCAUSHORT:
/* 		printf("val = %d\n",msg.val.us); */	
		msg.val.us =*(unsigned short *)val;
      case DCALONG:
		msg.val.l=*(long *)val;
		sprintf(buf,"gnPutEpics: name = %s, val = %ld\n",name,msg.val.l);
		DPRINT(t1,buf);
		break;
      case DCADOUBLE:
		msg.val.d = *(double *)val;
		sprintf(buf,"gnPutEpics: name = %s,val = %f\n",name,msg.val.d);
		DPRINT(t1,buf);
		break;
      case DCASTRING:
		strncpy(msg.val.s,(char *)val,MAX_STRING_SIZE);
		sprintf(buf,"gnPutEpics:  name = %s,val = %s\n",name,msg.val.s);
		DPRINT(t1,buf);
		break;
      default:
		DPRINT(1,"bad type in gnPutEpics");
	  
    }
    /* send request*/
    retVal = msgQSend(sendQ,(char *)&msg,sizeof(msg),NO_WAIT,MSG_PRI_NORMAL);
	/*   retVal = sockWrite(gnEPICSSocket,(char *)&msg,sizeof(dcaMsgStruct)); */
    /* wait for status*/
    if(retVal != ERROR)
		retVal = msgQReceive(receiveQ,(char *)&ret,sizeof(ret),WAIT_FOREVER);
	/*  retVal = sockRead(gnEPICSSocket,(char *)&ret,sizeof(dcaMsgStruct)); */
    else
		printf("Error writing message to epics %s\n",msg.name);
	/*     sockClose(&gnEPICSSocket); */
    semGive(socketSem);
 
    if(retVal != ERROR)
    {
		return ret.status;
    }
    printf("gnputEpics error %s\n",name);
    return retVal;

}
STATUS getWCS(wcsHeader *wcs,int roiNum)
{
    char buf[200];
    dcaMsgStruct msg,ret;
    int status= OK;
    char name[80];
#ifdef DEBUG
    int t1 = 1;
#endif
    int retVal;
/*     status = openEpicsSocket(); */
    if(status == OK)      {
	sprintf(buf,"\n\ngetWCS name = %s \n",name);
	cicsLogMessage(4,buf);
	cicsLogMessage(4,"gnPutEpics taking sem\n"); 
	status = semTake(socketSem,WAIT_FOREVER); 
      
	/*  gnOpenSocket(gnEPICSName,gnEPICSPort); */
  
	DPRINT(t1,"Get wcs information\n");
	/*printf("Get wcs information\n"); */
	msg.op = DCAWCS;
	msg.val.l = roiNum;
	/* send request*/
 	/*printf("message.op = %d\n",msg.op); */
	DPRINT(t1,"calculating wcs values\n");
	/*printf("calculating wcs values\n");*/
	retVal = msgQSend(sendQ,(char *)&msg,sizeof(msg),NO_WAIT,MSG_PRI_NORMAL);
	/* 	retVal = sockWrite(gnEPICSSocket,(char *)&msg,sizeof(dcaMsgStruct)); */
	/* wait for status*/
	if(retVal != ERROR)	  {
	    DPRINT(t1,"waiting for return message from socket for wcs\n"); 
	    /*printf("waiting for return message from socket for wcs\n");*/ 
	    retVal = msgQReceive(receiveQ,(char *)&ret,sizeof(ret),WAIT_FOREVER);
	    /* 	    retVal = sockRead(gnEPICSSocket,(char *)&ret,sizeof(dcaMsgStruct)); */
	}
	else	  {
	    printf("error writing wcs request to socket\n");
	}
	
	  
	/* 	  sockClose(&gnEPICSSocket); */
	/*printf("retVal = %i\n",retVal);
	printf("status = %i\n",status);*/

	  status = semGive(socketSem);
	  
	if((retVal != ERROR) && (status == OK))	  { 
	    DPRINT(t1,"Getting individual wcs values\n");
	    /*printf("****Getting individual wcs values\n");*/
	    gnGetEpicsT(dbTop,WCS_CTYPE1,DCASTRING,&wcs->ctype1);
	    gnGetEpicsT(dbTop,WCS_CRPIX1,DCADOUBLE,&wcs->crpix1);
	    gnGetEpicsT(dbTop,WCS_CRVAL1,DCADOUBLE,&wcs->crval1);
	    gnGetEpicsT(dbTop,WCS_CTYPE2,DCASTRING,&wcs->ctype2);
	    gnGetEpicsT(dbTop,WCS_CRPIX2,DCADOUBLE,&wcs->crpix2);
	    gnGetEpicsT(dbTop,WCS_CRVAL2,DCADOUBLE,&wcs->crval2);
	    gnGetEpicsT(dbTop,WCS_CD1_1,DCADOUBLE,&wcs->cd1_1);
	    gnGetEpicsT(dbTop,WCS_CD1_2,DCADOUBLE,&wcs->cd1_2);
	    gnGetEpicsT(dbTop,WCS_CD2_1,DCADOUBLE,&wcs->cd2_1);
	    gnGetEpicsT(dbTop,WCS_CD2_2,DCADOUBLE,&wcs->cd2_2);
	    gnGetEpicsT(dbTop,WCS_RADECSYS,DCASTRING,&wcs->radecsys);
	    gnGetEpicsT(dbTop,WCS_EQUINOX,DCADOUBLE,&wcs->equinox);
	    gnGetEpicsT(dbTop,WCS_MJDOBS,DCADOUBLE,&wcs->mjdobs);
	    sprintf(buf,"crpix1 = %f, crval1 = %f, ctype2 = %s, crpix2 = %f, crval2 = %f\n",wcs->crpix1,wcs->crval1,wcs->ctype2,wcs->crpix2,wcs->crval2);
	    DPRINT(t1,buf);
	    /*printf("crpix1 = %f, crval1 = %f, ctype2 = %s, crpix2 = %f, crval2 = %f\n",wcs->crpix1,wcs->crval1,wcs->ctype2,wcs->crpix2,wcs->crval2);*/
	   
	}
	else {
#ifdef TRACE
	    printf("WCS not set - use defaults pixel ref\n");
#endif
	    printf("WCS not set - use defaults pixel ref\n");
		sprintf(wcs->ctype1,"PIXEL");
		wcs->crpix1=0.0;
		wcs->crval1=0.0;
		sprintf(wcs->ctype2,"PIXEL");
		wcs->crpix2=0.0;
		wcs->crval2=0.0;
		wcs->cd1_1=1.0;
		wcs->cd1_2=0.0;
		wcs->cd2_1=0.0;
		wcs->cd2_2=1.0;
		sprintf(wcs->radecsys,"INDEF");
		wcs->equinox=0.0;
		wcs->mjdobs=0.0; 

		}
/* 	closeEpicsSocket(); */
    }
    else     {
	printf("Error opening socket in getWCS");
	status = ERROR;
    }
    return status;
}

STATUS gnGetTime (char *val)
{
    int retVal;
    dcaMsgStruct msg,ret;

    semTake(socketSem,WAIT_FOREVER); 
    /* set structure values to send through socket*/
    msg.op = DCATIME;
    /* send request*/
    retVal = msgQSend(sendQ,(char *)&msg,sizeof(msg),NO_WAIT,MSG_PRI_NORMAL);
   
    /* wait for reply*/
    if(retVal != ERROR) {
		retVal = msgQReceive(receiveQ,(char *)&ret,sizeof(ret),WAIT_FOREVER);
		if(retVal != ERROR)
			strncpy((char *)val,ret.val.s,MAX_STRING_SIZE);
		}
	semGive(socketSem);
    return retVal;   
}

STATUS gnGetDate (char *val)
{
    int retVal;
    dcaMsgStruct msg,ret;

    semTake(socketSem,WAIT_FOREVER); 
    /* set structure values to send through socket*/
    msg.op = DCADATE;
    /* send request*/
    retVal = msgQSend(sendQ,(char *)&msg,sizeof(msg),NO_WAIT,MSG_PRI_NORMAL);
   
    /* wait for reply*/
    if(retVal != ERROR) {
		retVal = msgQReceive(receiveQ,(char *)&ret,sizeof(ret),WAIT_FOREVER);
		if(retVal != ERROR)
			strncpy((char *)val,ret.val.s,MAX_STRING_SIZE);
		}
	semGive(socketSem);
    return retVal;
}








/*****************************************************************************
 * Function name:
 * cicsLogMessage
 *
 * Invocation:
 * STATUS cicsLogMessage (int i,char* string)
 *
 * PARAMETERS:
 *     int i - debug level of message
 *     char* string - message
 *
 * FUNCTION VALUE:
 * void
 *
 * PURPOSE:
 *     Log a message to the cics logger
 *
 * DESCRIPTION:
 * This routine opens a socket to a server running on the EPICS machine, 
 * sends a message, and waits for the response
 *
 * EXTERNAL VARIABLES:
 *      socketSem mutual exclusion sem
 *      dqDebug - local debug level
 *
 * PRIOR REQUIREMENTS:
 *
 *
 * DEFICIENCIES:
 * 	None known
 *
 * HISTORY:
 * 	1-October-1998  Original version  P Ruckle
 *
 *****************************************************************************/

void cicsLogMessage(const long i,const char *string)
{
    int retVal;
  dcaMsgStruct msg,ret;


    semTake(socketSem,WAIT_FOREVER);
 /*    gnOpenSocket(gnEPICSName,gnEPICSPort); */

    msg.type = i;
    strncpy(msg.name,string,MAX_STRING_SIZE);
    msg.op = DCALOG;
    if(dqDebug >=i)
      printf(string);

    retVal = msgQSend(sendQ,(char *)&msg,sizeof(msg),NO_WAIT,MSG_PRI_NORMAL);
/*     retVal = sockWrite(gnEPICSSocket,(char *)&msg,sizeof(dcaMsgStruct)); */
    /* wait for status*/

    if(retVal != ERROR)
	retVal = msgQReceive(sendQ,(char *)&ret,sizeof(ret),WAIT_FOREVER);
/* 	sockRead(gnEPICSSocket,(char *)&ret,sizeof(dcaMsgStruct)); */

/*     sockClose(&gnEPICSSocket); */
    semGive(socketSem);
  return;	
}





/*****************************************************************************
 * Function name:
 *  setObsFlags
 *
 * Invocation:
 * int setObsFlags(int prep, int acq, int rdout)
 *
 * PARAMETERS:
 *	int prep
 *      int acq
 *      int rdout
 *
 * FUNCTION VALUE:
 * 	int - Status value returned by function 
 *
 * PURPOSE:
 * 
 *
 * DESCRIPTION:
 * 
 *
 * EXTERNAL VARIABLES:
 * 	dbSadTop - prefix of database
 *
 * PRIOR REQUIREMENTS:
 *
 *
 * DEFICIENCIES:
 * 	None known
 *
 * HISTORY:
 * 	11-June-1997  Original version  N Buchholz
 *
 *****************************************************************************/

int setObsFlags(int prep, int acq, int rdout)
{
    char buf[80];
    sprintf(buf,"setobsflag %d %d %d",prep,acq,rdout);
/*     if(openEpicsSocket() == OK) { */
	cicsLogMessage(2,buf);
	gnPutEpicsT(dbSadTop, PREP ".VAL",DCALONG, &prep);
	gnPutEpicsT(dbSadTop, ACQ ".VAL",DCALONG, &acq);
	gnPutEpicsT(dbSadTop, RDOUT ".VAL",DCALONG, &rdout);
/* 	closeEpicsSocket(); */
   /*  } */
/*     else */
/* 	printf("Error opening socket in setObsFlags\n"); */


    return (OK);

}

