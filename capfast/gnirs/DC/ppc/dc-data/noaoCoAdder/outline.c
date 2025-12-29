#include        "limits.h"

static struct {
    void *v;
    char *c;
} rcsid = {
    &rcsid,
    "$Id: outline.c,v 1.3 2011/08/18 20:59:33 gemvx Exp $"
};
long WRITE_FILE = 1;  
#define DEBUG
/*****************************************************************************
 * Copyright 1997 Association of Universities for Research in Astronomy, Inc. 
 * See the file COPYRIGHT for more details.
 *
 * Filename: 	
 * 	saveData.c
 *
 * Description:
 * 	This file contains the functions that save the data to  fits or to the
 *	    dhs.
 *
 * Function name(s)
 *	saveDataset 	- task that saves data.
 *	saveFitsData 	- function that saves data in fits format to noao 
 *				designed saver running on a sun.
 *	saveDhsData 	- function that saves data to a dhs server.
 *	dhsSave 	- 	
 * 	testCoadd 	- Test function to simulate coadder board when one 
 *			    isn't available compile epicsCntrlSrc and 
 *			    noaoCoadder area with -DCOADD_TEST for this 
 *			    functionality
 *	transFrame 	- test function:  waits for change in value of the 
 *			     coadd register, loads data and starts interrupt 
 *			     handler
 *	setDsHeader	 - loads header information into dhs dataset

 * Dependencies
 * 	
 *
 * Orginial Author:
 *	Peter Ruckle
 *
 * History:
 *	1-Jan-2000: Created original version - Peter Ruckle
 *
 ***************************************************************************/

/*includes*/
#include "gnDCADefs.h"
#include "gnDCAVars.h"
#include "saver.h"
/* #include "dcvx.h" */
#include <semLib.h>
#include <carRecord.h>

#undef DUM_GBLSOURCE
#include "imageHdr.h"

#include <vme.h>
#include <sysLib.h>

#include "timexLib.h"

extern STATUS getDsHeader( int timing ,int captBuf);
extern int servP_DhsInit();

void printHdr(char hdrarray[255][2][MAX_STRING_SIZE] );
/*global  variables*/
int saverOK = 1;
static char dummy[80];
int sdb = 1;



/********************************************************************************
 *
 *******************************************************************************/

#include "tickLib.h"
int logLevel = 2;
int memdebug = 0;
int useql    = 1;
int firstTick = 0;
int tps = 60.0;

typedef struct {
    int start;
    int sent;
    int ack;
} dhs_timer;

/*externs*/

extern coAdSems sem;
extern saverParams svrP;
extern SEM_ID dcaRegLock;
extern SEM_ID coaddSem;

/* function prototypes*/
STATUS getFrameHeader( int timing,int bufnum);
int DhsConnect(dhsParams *dhs);
int getFitsName(char *label);
int printName(int i);

int setDsHeader(DHS_BD_DATASET ds, int bufNum);
int setFrameHeader(DHS_BD_FRAME frame, int bufNum);

int dhsSave(int arg1 ,int arg2 ,int arg3 ,int arg4 ,int arg5 ,int arg6 ,
	    int arg7 ,int arg8 ,int arg9 ,int arg10 );
void simOn ()
{
    dcaLoadRegister(TESTENBLNSEL,1);
}
void simOff()
{

    dcaLoadRegister(TESTENBLNSEL,0);
}

/********************************************************************************
 * gemLogMsg
 *******************************************************************************/
void gemLogMsg(int level, const char *pFormat, ...) 
{
    va_list		pvar;
	
    if (level > logLevel) return;
	
    va_start (pvar, pFormat);
    vprintf(pFormat, pvar);
    va_end(pvar);
}
	
#ifdef TRACE
/********************************************************************************
 * gemTimeLogMsg
 *******************************************************************************/
void gemTimeLogMsg(dhs_timer *ptimer, int level, const char *pFormat, ...) {
    va_list		pvar;
    int tick;
	
    /* if (level > loglevel) return;*/
	
    va_start (pvar, pFormat);
    tick=tickGet();
    printf("tick: %d sec:%.2f ",tick-ptimer->start,(tick-ptimer->start)/(double)tps);
    vprintf(pFormat, pvar);
    va_end(pvar);
}
#endif

/********************************************************************************
 * dhsPutCallback
 *******************************************************************************/
void dhsPutCallback (DHS_CONNECT connect, DHS_TAG tag,
		     DHS_CMD_STATUS sendStatus, char *msg, char *dsname,
		     void *userData)
{ 
    gemLogMsg ( 5 ,"dhsPutCallback tag %d sendStatus %d msg %s <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<\n",tag,sendStatus,msg);
	
    if (memdebug) memShow(0);
}


/*
 *+
 * FUNCTION NAME:
 * 	saveDataset
 *
 * INVOCATION:
 * 	spawned at startup
 *
 * PARAMETERS: (">" input, "!" modified, "<" output)
 *	none
 *
 * FUNCTION VALUE:
 * 	 Only returns on error.
 *
 * PURPOSE:
 * 	Wait for datacoadder board to signal that a frame is ready to be saved and then
 *		save it.
 *
 * DESCRIPTION:
 * 
 *
 * EXTERNAL VARIABLES:
 * 	svrP - saver parameters
 *	sem - semaphores
 *	obsDone - done signal
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
int saveDataset()
{

    int buf;
    long status;
    roiParams *proi;
    char errString[80];
    DHS_STATUS rc = DHS_S_SUCCESS;
    dhsParams *dhs;
	
/*     sleep(5,0); */
    /* connect to server if we are using dhs*/
#if 1
    if(svrP.server == DHS_SAVE)    
    {
	dhs = &(svrP.param.dhs);
	rc = 0;

	if(gnPutEpicsT(dbSadTop, DHSCONNECTED ".VAL", DCALONG, &rc)!= OK)
	  printf ("saveDataset: failed to set DHSCONNECTED\n");
    }  

    if ( servP_DhsInit() != OK) {
      printf ("couldn't init DHS, CPU need to be rebooted\n");
      return -1;
    }

#endif
    while (1)    
    {
	/* wait for semaphore, this is released by the unscramble interrupt
	   routine  (a frame is ready to be saved)*/
	 	printf("Waiting for frame to save\n");
	gemLogMsg(10,"outline.c\n");
	if(semTake (sem.unScrambleFrame,WAIT_FOREVER) == ERROR)
	{
	    /* print fatal error*/
	    gemLogMsg(1,"saveDataset: Error taking semaphore\n");
	    goto Error;
	}
	printf("got unscramble int\n");
	gemLogMsg(10,"outline.c after semtake\n");
	
	/* 	setObsFlags(FALSE, FALSE, TRUE); */ /*set prep FALSE, acq FALSE, rdout TRUE */
	
	if (dhs->headers==HDR_NONE) 
	{
	    gemLogMsg(2,"not collecting headers\n");
	}
	else
	{
	    gemLogMsg(2,"collecting headers\n");
	}
	
	
	
	gemLogMsg( 5 ,"saver task got semaphore\n");
	
	/* get buffer to put data in */
	buf = getFullBuf();
	
	/* check to see if observation was aborted*/
	if(DCA_Save  == FALSE)
	{
	    gemLogMsg(1,"observation aborted\n");
	    if(buf != ERROR)
		putEmptyBuf(buf);
	    /* setObsFlags(FALSE, FALSE, FALSE); */
	    continue;
	}
	/* this should never happen unless the coadder hardware is broke, or
	   if sdt mode did not kill the interrupts */
	if(buf == ERROR)
	{
	    gemLogMsg (1,"error getting full buffer\n");
	    /*  setObsFlags(FALSE, FALSE, FALSE); */
	    continue;
	}
	
	if (dhs->headers != HDR_NONE)
	    status = getDsHeader(AFTER,buf);
	
	/*  roi info*/
	proi = &svrP.roi;
	
	/* dhs or fits*/
	if(svrP.server == DHS_SAVE)
	{
	    gemLogMsg( 5 ,"saveDhsData for %d\n",proi->numRois);
	    status = saveDhsData(buf,&(svrP.param.dhs), proi) ;
	}
	else  /* wrong image type*/ 
	{
	    gemLogMsg (1,"Only allow dhs images %d\n",svrP.server);
	}
	
	if(status != OK)
	{
	    /* print error*/
	    sprintf(errString,"saveDataset: Data not saved \n");
	    gemLogMsg (1,errString);
	    /* set car record*/ 
	    sleep(1,0);
	    setCar(OBSERVE_CAR,menuCarstatesERROR,ERROR,errString,dummy);
	    
	    
	} 
	
	/* 	setObsFlags(FALSE, FALSE, FALSE); */ /*set prep FALSE, acq FALSE, rdout FALSE */
	
	
    }
  Error:
    saverOK = 0;
    if(gnPutEpicsT(dbSadTop, DHSCONNECTED ".VAL", DCALONG, &saverOK)!= OK)
	gemLogMsg (1,"failed to set DHSCONNECTED\n");
    /* 	setObsFlags(FALSE, FALSE, FALSE); */	
    /*print error*/
    gemLogMsg (1,"saveDataset returned error:  exiting\n");
    gemLogMsg (1,"Cpu needs to be rebooted\n");
    return ERROR;
}


int *pbufReal;
int **pbufSend;

short  *pbufInt16;


/*
 *+
 * FUNCTION NAME:
 * 	saveDhsData
 *
 * INVOCATION:
 * STATUS status;
 * 	int bufNum;
 *	dhsParams dhs;
 *	roiParams roiStruct;
 *	status = saveDhsData(bufNum, &dhs, &roiStruct)
 *
 * PARAMETERS: (">" input, "!" modified, "<" output)
 *	> int bufNum -  buffer number of data on coadder
 *	! dhsParams dhs - dhs data structure
 *	> roiParams roiStruct - roi data structure
 *
 * FUNCTION VALUE:
 * STATUS - OK or ERROR
 *
 * PURPOSE:
 * 	send data to dhs server
 * 
 *
 * DESCRIPTION:
 * 
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
static   callBackStruct cb;
int saveDhsData(int bufNum, dhsParams *dhs, roiParams *roiStruct)
{

    DHS_STATUS status;
    char 			*contrib[1];
    char errString[80];
    static int run = 0;
    contrib[0] = "2DIRS";
    run++;
   
    /* add connect to headers and initialization ????? */
    status = DHS_S_SUCCESS;


printf ("**** IN saveDhsData\n");
    /* connect to dhs server if not done already*/
    printf("connect to dhs? %d\n",dhs->connect);
    if (dhs->connect == NULL)
    {
      printf("connecting to dhs\n");
      sleep(1,0);
	gemLogMsg( 5,"connecting to dhs name = %s from %s\n",dhs->name,dhs->impName);
	 
	if(DhsConnect(dhs) != OK)
	{
	    gemLogMsg (1,"couldn't connect to dhs\n");
	    goto DHSError1;
	} 
	if (*(int *)dhs->connect == NULL)
	{
	    sprintf(errString,"saveDhsData: ERROR: saveFrame - unable to initialize Saver connection\n");
	   
	    gemLogMsg (1,errString);
	    goto DHSError1;
	}
	gemLogMsg( 5 ,"connected to dhs\n");
    }

    status = DHS_S_SUCCESS;

    /* copy structure to local area*/
    memcpy(&cb.roi,roiStruct,sizeof(roiParams));
    memcpy(&cb.dhs,dhs,sizeof(dhsParams));
    cb.bufNum = bufNum;
    cb.run = run;


    /*
     *     sprintf(name,"tdhsSave%d",run);
     *     status =   taskSpawn(name,60,VX_FP_TASK,100000,dhsSave,&cb,
     *         	 0,0,0,0,0,0,0,0,0);
     *     if(status == ERROR) {
     *       printf("couldn't spawn task \n");
     *           goto DHSError1;
     *           }
     */
    /* 	printf("saveDhsData: format = %d\n",svrP.param.dhs.format); */
    dhsSave((int) &cb,0,0,0,0,0,0,0,0,0);
    return OK;

  DHSError1:
    setCar(OBSERVE_CAR,menuCarstatesERROR,ERROR,errString,dummy);
   
    gemLogMsg (1,"Error in saveDhsData\n");
    status = DHS_S_SUCCESS;
    /*     dhsDisconnect(dhs->connect,&status); */
    status = DHS_S_SUCCESS;
    dhsExit(&status);
    if(status != DHS_S_SUCCESS)
	gemLogMsg(1,"dhsExit failed\n");
    dhs->initDone = 0;
    dhs->connect = NULL;

    return ERROR;
}
/*
  *+
  * FUNCTION NAME:
  * 	dhsSave
  *
  * INVOCATION:
  * 	spawned by taskSpawn
  *
  * PARAMETERS: (">" input, "!" modified, "<" output)
  *	> int bufNum -  buffer number of data on coadder
  *	! dhsParams dhs - dhs data structure
  *	> roiParams roiStruct - roi data structure
  *
  * FUNCTION VALUE:
  * STATUS - OK or ERROR
  *
  * PURPOSE:
  * 	spawned task to send data to the dhs
  * 
 *
 * DESCRIPTION:
 * 
 *
 * EXTERNAL VARIABLES:
 * none
 *
 * PRIOR REQUIREMENTS:
 * dhs server must be running
 *
 * DEFICIENCIES:
 * 	
 *
 * HISTORY:
 *  written by Peter Ruckle
 *-
 */

/* global variable to signal error in another save process */
static int dhs_ok=1;

int dhsSave(int arg1 ,int arg2 ,int arg3 ,int arg4 ,int arg5 ,int arg6 ,
	    int arg7 ,int arg8 ,int arg9 ,int arg10 )
{ 
    int size;      /* size of current data to be sent*/
    DHS_TAG putTag;
    DHS_BD_DATASET ds = NULL;

    DHS_STATUS status = DHS_S_SUCCESS;
    DHS_BD_FRAME frame;
    int i = 0;
    int detector;
    unsigned long ndims[1];
    int dx, dy, dx1,dx2, dy1, dy2, index;
    unsigned long dims[2];
    unsigned long origin[2];
    char *qlStreams[1];
    int missing=0;
    int dySkip = 0;
    char *contrib[1];
    char *vme;           /* vme address of local buffer*/
    char errString[80];
    char label[MAXSTRING]; 
    char *dsName;   		/* dataset name*/
    char dsDHSdataLabel[MAXSTRING];   /* holder for OCS provided dataset name */
    int run;   /* current frame number, for debugging only*/
    int bufNum;
    callBackStruct *cb = NULL;/* structure passed from caller*/
    dhsParams *dhs;   /* dhs structure*/
    roiParams *roiStruct; /* roi structure*/
    tDCARegs reg;/* coadder register structure*/
    tRect *proi;

#ifdef TRACE
    dhs_timer *ptimer;
#endif

    int dhs_i;
    int dhs_j;
    long  *dhs_data32;
    /* initialize variables*/
	
#ifdef TRACE
    ptimer = (dhs_timer*) malloc (sizeof(dhs_timer));
    ptimer->start = tickGet();
#endif
	
    origin[0] = 1;
    origin[1] = 1;

    contrib[0] = "2DIRS";
    cb = (callBackStruct *)arg1;
    run  = cb->run;
    dhs = &cb->dhs;
    /* 	printf("dhsSave: format = %d, int32 = %d\n",dhs->format,DHS_DT_INT32); */
    roiStruct = &cb->roi;
    bufNum = cb->bufNum;
  
#ifdef TRACE
    gemTimeLogMsg(ptimer, 5 ,"%d, dhsSave task trace\n",run);
#else
    gemLogMsg(5 ,"%d, dhsSave task\n",run);
#endif


    /* DHSdataLabel */
    /* if one is supplied - do not set the lifetime (assumed to be done by OCS) */
    /* if not, query one from DHS and set lifetime */
    if(dhs_ok) 
    {
	/* one supplied */
	printf(" ********** *** label length = %d\n",strlen (dhs->dhsDatalabel)); 
	if (strlen(dhs->dhsDatalabel) >= 1 && strcmp(dhs->dhsDatalabel,"NONE")) 
	{
	    strncpy(dsDHSdataLabel,dhs->dhsDatalabel,MAX_STRING_SIZE);
	    dsName = &dsDHSdataLabel[0];
            
	}
	else 
	{
	     		  printf ("***** **** getting dhs label\n"); 
	    /* get it from the DHS server */
	    dsName = dhsBdName(dhs->connect,&status);
	    if(status != DHS_S_SUCCESS) 
	    {
		sprintf(errString,"saveDhsData: ERROR: saveFrame - unable to get image name\n");
		gemLogMsg (1,errString);
		goto DHSError;
	    }
	    /* need set lifetime */
	    if (dhs->lifetime == DHS_TEMP) 
	    {
				/* movie mode ... QlTool only - do not set any lifetime
				 * dhsBdCtl(dhs->connect, DHS_BD_CTL_LIFETIME,dsName,DHS_BD_LT_TRANSIENT, &status);
				 */
	    }
	    else
		dhsBdCtl(dhs->connect, DHS_BD_CTL_LIFETIME,dsName,DHS_BD_LT_PERMANENT, &status);
	    if(status != DHS_S_SUCCESS) 
	    {
		sprintf(errString,"saveDhsData: ERROR: saveFrame - unable to set lifetime\n");
		gemLogMsg (1,errString);
		goto DHSError;
	    }
	}
    }

     printf("set stream\n"); 
    
    if(strlen(dhs->qlStream) >0) 
    {
	qlStreams[0]=dhs->qlStream;
	dhsBdCtl(dhs->connect, DHS_BD_CTL_QLSTREAM,dsName, 1,qlStreams , &status);
	if(status != DHS_S_SUCCESS)
	{
	    sprintf(errString,"saveDhsData: ERROR: saveFrame - unable to set Ql stream\n");
	    gemLogMsg (1,errString);
	    goto DHSError;
	}
    }
    
    
    gemLogMsg( 4 ,"%d handling %d rois\n",run,roiStruct->numRois);
    
    while(i < roiStruct->numRois)   
    {
    	/* create dataset*/
    	if(dhs_ok)
	    ds = dhsBdDsNew(&status);
    	if (status != DHS_S_SUCCESS)
	{
	    sprintf(errString,"saveDhsData: ERROR: saveFrame - unable to create data set\n");
	    
	    gemLogMsg  (1,errString);
	    goto DHSError;
	}
    	gemLogMsg( 4 ,"%d created dataset %x \n",run,ds);
	
	/* 	if (dhs->headers != HDR_NONE)  */
	/* 	  { */
	/* 	    getDsHeader(NOW,bufNum); */
	/* write headers to dataset */ 
	/* 	    getFrameHeader(NOW,bufNum); */
	/* 	  } */
	
	
	gemLogMsg( 4 ,"%d handling roi #%d\n",run,i);
	
	proi = &roiStruct->roi[i];
	
	
	/*
	 * bufNum is not used but getWCSInfo !!!
	 */
	if (dhs->headers != HDR_NONE) 
	{
	    if( getWCSInfo(i,bufNum) == ERROR) 
	    {
		gemLogMsg (1,"error getting wcs info\n");
	    }
		  
	    setHeaderROI(i,proi,bufNum);
	}
	
	/* create frame - equ. of save fits should start here*/
	/* first transfer*/
	dx = dx1 = dx2 = proi->hiX - proi->lowX +1;
	dy = proi->hiY -  proi->lowY +1;
	 	printf("hiX %d , hiY %d , loX %d , loY %d, size = %d\n",proi->hiX ,proi->hiY,proi->lowX,proi->lowY,transsize[arSizeIdx]); 


	/*set parameters for 2 dma transfers*/
	if(proi->hiY > transsize[arSizeIdx])
	{
	    
	    if(proi->lowY < transsize[arSizeIdx])
	    {/* high and low bracket middle of array*/
		gemLogMsg(10,"case 1\n");
		gemLogMsg(10,"%s, %s, %s\n",A3_A,A3_B, detType);
		if(strcmp(A3_A,detType) == 0) 
		{
		    gemLogMsg(10,"a3a\n");

		    dySkip = 2;
		    dy2 = proi->hiY - transsize[arSizeIdx] - dySkip; 
		    dy1= transsize[arSizeIdx] - proi->lowY +1;
		    detector = 3;
		    dy -=dySkip;
		}
		else if (strcmp(A3_B,detType) == 0)
		{
		    gemLogMsg(10,"a3B\n");
		    dySkip = 0;
		    dy2 = proi->hiY - transsize[arSizeIdx] - dySkip +3; 
		    dy1= transsize[arSizeIdx] - proi->lowY -2 ;
		  
		    dy -=dySkip;
		    detector = 3;
		}
		else if (strcmp(A2,detType) == 0)
		{
		    dySkip = 0;
		    gemLogMsg(10,"a2\n");
		    dy2 = proi->hiY - transsize[arSizeIdx] - dySkip +1 ; 
		    dy1= transsize[arSizeIdx] - proi->lowY ;
		    dy -=dySkip;
		    detector = 2;
		    
		}
		else
		{
		    gemLogMsg(1,"unknown detector type \n");
		    status = ERROR;
		}
	    }
	    else  
	    { /* high and low in first two quadrants*/
		gemLogMsg(10,"case 2\n");
		dy2 = 0;
		dy1 =  proi->hiY - proi->lowY +1;
	    }
	}	
	else 
	{/* high and low in third and fourth quadrants*/ 
	    gemLogMsg(10,"case3\n");
	    dy2= proi->hiY - proi->lowY +1;
	    dy1 = 0;
	}
      

	

	
	ndims[0] = 2;
	dims[0] = dx;
	dims[1] = dy;/*change after test*/
	
	index = i;
	
	/* create label from ObservID*/
	sprintf(label,"%s:%d",dsName,index);
	gemLogMsg(5,"label = %s\n",label);
	 	printf("dhs_ok = %d\n",dhs_ok); 
	if(dhs_ok)
	{
            printf("IN DHS_OK: dhs->format = %i\n",dhs->format);
	    if (dhs->format == DHS_UINT32)
	    {
                printf("IN dhs->format == DHS_UINT32\n");
		frame = dhsBdFrameNew( ds, label, index, DHS_DT_INT32, 
				       ndims[0], dims, 
				       (const void **)&dhs->data,&status); 
	 	printf("frame = %x, success = %d, status = %d\n",frame, 
		 		       DHS_S_SUCCESS,status); 
	    }
	    else if (dhs->format == DHS_UINT16) 
	    {
		frame = dhsBdFrameNew( ds, label, index, DHS_DT_INT16, 
				       ndims[0], dims, 
				       (const void **)&dhs->data16,&status);
		dhs->data = (long*) malloc (sizeof(long)*dx*dy);
		/* 	printf("frame = %x, success = %d, status = %d\n",frame, */
		/* 		       DHS_S_SUCCESS,status); */
	    } 
	    else if (dhs->format == DHS_UINT8) 
	    {
		frame = dhsBdFrameNew( ds, label, index, DHS_DT_INT8, 
				       ndims[0], dims, 
				       (const void **)&dhs->data8,&status);
		dhs->data = (long*) malloc (sizeof(long)*dx*dy);
		/* 	printf("frame = %x, success = %d, status = %d\n",frame, */
		/* 		       DHS_S_SUCCESS,status); */
	    } 
            else if (dhs->format == DHS_FLT32) 
            {
                printf("IN dhs->format == DHS_FLT32\n");
                frame = dhsBdFrameNew( ds, label, index, DHS_DT_FLOAT, 
                                       ndims[0], dims, 
                                       (const void **)&dhs->dataf32,&status);
                printf( "dx = %i\n", dx);
                printf( "dy = %i\n", dy);
                dhs->data = (long*) malloc (sizeof(long)*dx*dy);
		 	printf("frame = %x, success = %d, status = %d\n",frame, 
		 		       DHS_S_SUCCESS,status); 
            }
	    else
		gemLogMsg(1,"dhs->format not set\n");
	}
	else
	    gemLogMsg(1,"didn't create frame\n");
	if (status != DHS_S_SUCCESS)
	{
	    sprintf(errString,"saveDhsData: ERROR: unable to allocate new frame buffer\n");
	    gemLogMsg(1,errString);
	    goto DHSError;
	}
	
	/* this is a  compulsary keyword */		
	dhsBdAttribAdd (ds, "instrument", DHS_DT_STRING, 0, NULL, "GNIRS" ,&status );
	
	if (status != DHS_S_SUCCESS)
	{
	    sprintf(errString,"saveDhsData: ERROR: unable to set instrument keyword .. expect the worst\n");
	    gemLogMsg(1,errString);
	    goto DHSError;
	}
	
	
	/* always set the debug keyword */
	if(proi->lowY < transsize[arSizeIdx])
	    setHeaderDEBUG(bufNum,(unsigned int)reg.pXferStart,
			   (unsigned int)(captBufAddr[bufNum]+ dy1/2*dx1),
			   (unsigned int)(dhs->data));
	else
	    setHeaderDEBUG(bufNum,(unsigned int)reg.pXferStart,0,
			   (unsigned int)(dhs->data));
	
	if (dhs->headers != HDR_NONE)
	{
	    /* build the DHS dataset header now */
	    missing+=setDsHeader( ds,bufNum); 
	}
	
	if(status != DHS_S_SUCCESS)
	{
	    /* print error*/
	    gemLogMsg(1,"saveDhsData: unable to allocate new frame !!!\n");
	    goto DHSError;
	}
	
	status = DHS_S_SUCCESS;
	/* create headers*/		
	dhsBdAttribAdd (frame, "origin", DHS_DT_INT32, 1, ndims, origin, 
			&status );			
	if(status != DHS_S_SUCCESS)
	{
	    /* print error*/
	    gemLogMsg(1,"saveDhsData: unable to set origin attribute !!!\n");
	    goto DHSError;
	}
	
	status = DHS_S_SUCCESS;
	dhsBdAttribAdd (frame, "axisSize", DHS_DT_INT32, 1, ndims, dims, 
			&status );		
	if(status != DHS_S_SUCCESS)
	{
	    /* print error*/
	    gemLogMsg(1,"saveDhsData: unable set axisSize attribute !!!\n");
	    goto DHSError;
	}
	status = DHS_S_SUCCESS;


	/* write	 headers to dataset*/
	if(dhs_ok)
	    if (dhs->headers != HDR_NONE) 
	    {
		missing+=setFrameHeader( frame,bufNum);
	    }
	/* 	printf("after setFrameHeader\n"); */


	if(status != DHS_S_SUCCESS)
	{
	    /* print error*/
	    gemLogMsg(1,"saveDhsData: frame new returned error\n");
	    goto DHSError;
	}
	
	/*set up coadder parameters*/
	gemLogMsg( 5 ,"dx = %d, dy = %d result = %x  run = %d\n",
		   dx,dy,(unsigned)dhs->data,run);
#ifdef COADD_TEST
	vme = (char *)dhs->data;
#else
	sysLocalToBusAdrs(VME_AM_EXT_USR_DATA,(char *)(dhs->data),(char **)&vme);
#endif
	

	
	/* 	   dy1 = 0; */
	/* DMA mods #1 */
	if(dy1 > 0)
	{
	    gemLogMsg(10,"case1, 2\n");
	    reg.numRows = dy1 ;
	    reg.numCols = dx ;
	
	    reg.pDMAStart = (int)vme;
	    /* calculate start address    */
	    reg.pXferStart =  MAKE_TRANS_ADDR(bufNum , proi->lowX , proi->lowY) ;
	    reg.szDMABlock = dx * dy1 ;
	    size = reg.numRows * reg.numCols;
	    
	    gemLogMsg( 5 ,"x0 = %d, y0 = %d, dx = %d, dy = %d, size = %d\n",proi->lowX, proi->lowY,reg.numCols,
		       reg.numRows,reg.szDMABlock);
	   
	    semTake(coaddSem,WAIT_FOREVER);
	    setRegisters(&reg,DMA);
	    semGive(coaddSem);
	
	
	    /* wait for completion of transfer*/
	
	    if(semTake(sem.transFrame,WAIT_FOREVER) == ERROR) 
	    {
		gemLogMsg(1,"Error taking sem.transFrame semaphore\n");
		goto  DHSError;
	    }
	}
	if(dy2 > 0) 
	{
	    gemLogMsg(10,"case1,3\n");
	    /* these will change for final version*/
	    reg.numRows = dy2 ;
	    reg.pDMAStart = (int)vme +(dy1*dx*4);
	    reg.numCols = dx ;
	    reg.szDMABlock = (reg.numRows) * (reg.numCols)  ;
	    gemLogMsg( 5 ,"dy = %d, dy1 = %d, dy2 = %d, dx = %d, size = %d\n",dy,dy1,dy2,dx,reg.szDMABlock);
	    /* calculate start address    */
	    /* 	    dy1 = transsize[arSizeIdx] +1; */

	    gemLogMsg(10,"ptrans = %d\n",(captBufTAddr[bufNum] + MAXCAPTCOLS*(proi->lowY + dy1) + proi->lowX)) ;
	    reg.pXferStart =  MAKE_TRANS_ADDR(bufNum , proi->lowX , proi->lowY +dy1+dySkip) ;
	    /*    reg.pXferStart = captBufTAddr[bufNum]+ dy1*dx; */
	    
	    gemLogMsg( 5 ,"dx = %d, dy = %d, size = %d vme addr = %d, CAAddr = %d\n",reg.numCols,
		       reg.numRows,reg.szDMABlock,reg.pDMAStart,reg.pXferStart);
	    semTake(coaddSem,WAIT_FOREVER);
	    setRegisters(&reg,DMA);
	    semGive(coaddSem);	
	    
	    if(semTake(sem.transFrame,WAIT_FOREVER) == ERROR) 
	    {
		gemLogMsg(1,"saveDhsData:  sem take transFrame returned error\n");
		goto DHSError;
	    }
	    
	}
	
	/* 	    semGive(coaddSem);	 */
	if (dhs->headers != HDR_NONE) 
	{
	    
	}
	else 
	{
	    missing=0;
	}
	
	if(gnPutEpicsT(dbSadTop, DHSMISSING ".VAL", DCALONG, &missing)!= OK)
	    gemLogMsg (1,"failed to set DHSMISSING to %d\n",missing);
	
	
	/* thses is just a time flag, not error */
	/*
	 * 	if (dhs->headers != HDR_NONE)
	 * 		rdBanCom635Time(-1,bufNum);
	 */
	
	/* send data*/
	/*dx = proi->cols;
	dy = proi->rows; */
	putEmptyBuf(bufNum);
	
	
	gemLogMsg( 5 ,"pbuf = %x, dx = %d, dy = %d\n",(unsigned int)(dhs->data),dx,dy);
	printf("pbuf = %x, dx = %d, dy = %d\n",(unsigned int)(dhs->data),dx,dy);
	
#if 1
	if(WRITE_FILE )
	{
	/*   for (i=0;i<1024*1022;i++) */
/* 	    { */
/* 	      dhs->data[i] = i%1022; */
/* 	    } */
	    gemLogMsg(6,"write file\n");
	    if ((dhs->format == DHS_UINT32) && (numLNRs > 1))  
	        { 
                  printf("IN WRITE FILE DHS_UINT32\n");
	    /* 		dhs_data32=dhs->data; */
	    /* 		for (dhs_i=0;dhs_i<dx;dhs_i++) */
	    /* 		  for(dhs_j=0;dhs_j<dy;dhs_j++)  */
	    /* 		    { */
	    /* 		      *dhs->data = (*dhs->data / numLNRs); */
	    /* 		      dhs->data++; */
	    /* 		    } */
	    /* 		dhs->data=dhs_data32; */
	    } 
	    else 
	    if (dhs->format == DHS_UINT16) 
	    {
		gemLogMsg( 3 ,"converting frame to 16 bits\n");
		dhs_data32=dhs->data;
		for (dhs_i=0;dhs_i<dx;dhs_i++)
		    for(dhs_j=0;dhs_j<dy;dhs_j++) 
		    {
			*dhs->data16++ = (short)(*dhs->data++);
		    }
		gemLogMsg( 3 ,"free 32 bits buffer\n");
		free(dhs_data32);
		gemLogMsg( 3 ,"conversion done\n");
	    }
	    else if (dhs->format == DHS_UINT8) 
	    {
		gemLogMsg( 3 ,"converting frame to 8 bits\n");
		dhs_data32=dhs->data;
		for (dhs_i=0;dhs_i<dx;dhs_i++)
		    for(dhs_j=0;dhs_j<dy;dhs_j++) 
		    {
			*dhs->data8++ = (char) (*dhs->data++);
		    }
		gemLogMsg( 3 ,"free 32 bits buffer\n");
		free(dhs_data32);
		gemLogMsg( 3 ,"conversion done\n");
	    }
            else if (dhs->format == DHS_FLT32) 
            {
                printf( "!!!!!converting frame to 32 bits float\n");
                printf( "dx = %i\n", dx);
                printf( "dy = %i\n", dy);
                dhs_data32=dhs->data;
                for (dhs_i=0;dhs_i<dx;dhs_i++)
                    for(dhs_j=0;dhs_j<dy;dhs_j++) {
                        *dhs->dataf32++ = (float)((double) (*dhs->data++) / (double) (numLNRs)); 
                    }
                gemLogMsg( 3 ,"free 32 bits buffer\n");
                free(dhs_data32);
                printf("conversion done\n");
                gemLogMsg( 3 ,"conversion done\n");
            }
	    
	    
	    
#ifdef TRACE
	    ptimer->start = tickGet();
#endif
	    
	    gemLogMsg( 1 ,"dhsBdPut ... run = %d ",run);
	    
	    if(dhs_ok) 
	    {
		if (dhs->lifetime == DHS_TEMP)
		    putTag = dhsBdPut(dhs->connect,dsName,DHS_BD_PT_DS_QL,DHS_TRUE,ds,cb,&status);
		else
		    putTag = dhsBdPut(dhs->connect,dsName,DHS_BD_PT_DS,DHS_TRUE,ds,cb,&status);
                
	    }
	    
	    gemLogMsg( 1 ," done...  %s\n",dsName);
	    
	    if(status != DHS_S_SUCCESS)
	    {
		gemLogMsg(0,"dhsBd put returned error\n");
		goto DHSError;
	    }
	    /* wait?   release tag*/
	    dhs->wait = 1; 
	    
	    /* wait for dhs to finish sending frame*/
	    if(dhs_ok)
		dhsWait (1, &putTag, &status);
	    
	    gemLogMsg( 1 ,"dhsWait done... run = %d\n",run);
	    
	    
	    /* now post the dataset has been done */
            /*printf("en gnPutEpicsT \n");*/
	    if(gnPutEpicsT(dbSadTop, DATALABEL ".VAL", DCASTRING, dsName)!= OK)
		gemLogMsg(0,"failed to set DATALABEL\n"); 
	    
	    
	    if(status != DHS_S_SUCCESS)
	    {
			gemLogMsg(0,"dhsWait returned error\n");
			goto DHSError;
	    }
	    
	    /* free dhs structures*/
            /*printf("to dhsTagFree\n"); */
	    if(dhs_ok)
			dhsTagFree (putTag, &status); 
	    
            /*printf("from  dhsTagFree\n"); */
	    gemLogMsg( 5 ,"dhsTagFree done... run = %d\n",run);
	    
	    if(status != DHS_S_SUCCESS)
	    {
                printf("dhs tagfree returned error\n");
		gemLogMsg(0,"dhs tagfree returned error\n");
		goto DHSError;
	    }
	}
	/*  semGive(coaddSem);	 */
#endif
	if(dhs_ok)
	    if (ds != NULL)       
            {
                /*printf("TO dhsBdDsFree \n"); */
		dhsBdDsFree(ds,&status); 
                /*printf("FROM dhsBdDsFree \n"); */
            }

        /*printf("ds = %x\n",ds);
        printf("status = %d\n",status); */
	if(status != DHS_S_SUCCESS)
	{
	    printf("dhsBdDsFree failed to free dataset !\n");
	    gemLogMsg(0,"dhsBdDsFree failed to free dataset !\n");
	    goto DHSError;
	}
	gemLogMsg( 5 ,"dhsBdDsFree done... run = %d\n",run);
	
	
	i++;
    }
   /*printf(" TO RETURN OK\n");*/ 
#ifdef TRACE
    free(ptimer);		
#endif
    
    return OK;
    
  DHSError:
    gemLogMsg(0,"%d dhs error\n",bufNum);
    dhs_ok = 0;
    if(gnPutEpicsT(dbSadTop, DHSCONNECTED ".VAL", DCALONG, &dhs_ok)!= OK)
	gemLogMsg (0,"failed to set DHSCONNECTED\n");
    
    setCar(OBSERVE_CAR,menuCarstatesERROR,ERROR,errString,dummy);
    
    putEmptyBuf(bufNum);
    gemLogMsg( 5 ,"free dataset\n");
    if (ds != NULL)       
	dhsBdDsFree(ds,&status); 
    
    /*     dhsDisconnect(dhs->connect,&status); */
    status = DHS_S_SUCCESS;
    dhsExit(&status);
    if(status != DHS_S_SUCCESS)
	gemLogMsg(0,"dhsExit failed\n");
    dhs->connect = NULL;
    dhs->initDone = 0;
#ifdef TRACE
    free(ptimer);		
#endif
    
    return ERROR;
}

/*
 *+
 * FUNCTION NAME:
 * 
 *
 * INVOCATION:
 * STATUS status;
 * 
 *
 * PARAMETERS: (">" input, "!" modified, "<" output)
 *
 *
 * FUNCTION VALUE:
 * STATUS - OK or ERROR
 *
 * PURPOSE:
 * 
 *
 * DESCRIPTION:
 * 
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
void testCoadd(int numCoadds,int fpc)
{
    int i;
    tDCARegs *p;
    

    printf( "	Coadder interrupt simulator running\n");
    p = oSystem.pDCARegs;
    for (i =0;i< numCoadds*fpc -1 ;i++)
    {
	   
	printf("	coad Test coadd int\n");
	/*call  coadd interrupt routine*/
	coAddIntHndlr(i);

    }
    printf("	coadTest unscramble int\n");
    /* 	call  unscramble interrupt*/
    unscrambleIntHndlr(i);	
}
/*
 *+
 * FUNCTION NAME:
 * 
 *
 * INVOCATION:
 * STATUS status;
 * 
 *
 * PARAMETERS: (">" input, "!" modified, "<" output)
 *
 *
 * FUNCTION VALUE:
 * STATUS - OK or ERROR
 *
 * PURPOSE:
 * 
 *
 * DESCRIPTION:
 * 
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
int transFrame()
{
    int i;
    int  *ip;
    tDCARegs *p;
    /*create memory area and  load into standard area oSystem.pDCARegs*/
    printf( "starting transFrame size = %d\n",sizeof(tDCARegs));
    oSystem.pDCARegs = (tDCARegs *) malloc (sizeof(tDCARegs));
    dcaLoadRegister(IVCOADDDONE,COADD_INT_NUM);
    dcaLoadRegister(IVXFERDONE,TRANS_INT_NUM);
    dcaLoadRegister(IVDESDONE,UNSCRAMBLE_INT_NUM);
    /* set test flag*/
    if(oSystem.pDCARegs != NULL)
    {
	/* 	printf("created registers %x\n",&oSystem.pDCARegs); */
	p = oSystem.pDCARegs;
	p->cmndStartDMA = 0;
	while ( 1)
	{
	    /* 	      printf("start = %d\n",p->cmndStartDMA); */
	    if (p->cmndStartDMA != 0)
	    {
		printf("sending frame\n");
	   
		/*	transfer data to p->pDMAStart size = p->szDMABlock */
		if(p->pDMAStart != NULL)
		{
		    ip = (int *)p->pDMAStart;

		    for (i = 0; i<p->szDMABlock;i++)
		    {
			*ip = i;
			ip++;
		    }
		    /* call transfer interupt routine*/
		    transIntHandlr(i);
		    /*    if (p->szDMABlock> MAX_DMA) */
		    /* 		      transIntHandlr(i); */
		    p->cmndStartDMA = 0;
		}
		else
		    printf("transfer error\n");
	    }
	    sleep(1,0);
	}
	
    }
    else
	printf("malloc failure in transFrame\n");
    return OK;
}

/******************************************************************************
 * setDsHeader
 ******************************************************************************/
int setDsHeader(DHS_AV_LIST ds, int bufNum)
{
    int n;
    DHS_STATUS status;
    int all_status;
	
    all_status=0;
    status = DHS_S_SUCCESS;
    n = 0;
    while (dhsDsHeader[n].keyword)
    {
	if (dsHdr[bufNum][n].status==OK) 
	{
	    if (dhsDsHeader[n].type==DCALONG)
		dhsBdAttribAdd( ds, dhsDsHeader[n].keyword, DHS_DT_INT32, 0, 
				NULL, dsHdr[bufNum][n].value.lval,&status);
	    else if(dhsDsHeader[n].type==DCADOUBLE)
		dhsBdAttribAdd( ds, dhsDsHeader[n].keyword, DHS_DT_DOUBLE, 0, 
				NULL, dsHdr[bufNum][n].value.dval,&status);
	    else if(dhsDsHeader[n].type==DCASTRING)
		dhsBdAttribAdd( ds, dhsDsHeader[n].keyword, DHS_DT_STRING, 0, 
				NULL, dsHdr[bufNum][n].value.sval,&status);
	    else
		printf ("header type not supported %d for %s\n",
			dhsDsHeader[n].type,dhsDsHeader[n].keyword);
	}
	else 
	{
	    gemLogMsg(6,"header %s [%d] has bad status [%d]\n",
		      dhsDsHeader[n].keyword,n,dsHdr[bufNum][n].status);
	    all_status++;
	}
		
	if(status != DHS_S_SUCCESS) 
	{
	    printf("ERROR: setDsHeader - unable to set header keyword %s\n",
		   dhsDsHeader[n].keyword);
	    all_status++;
	}
	n++;
    }  



    return all_status;
}
/******************************************************************************
 * setFrameHeader
 ******************************************************************************/
int setFrameHeader(DHS_AV_LIST frame, int bufNum)
{
    int n;
    DHS_STATUS status;
    int all_status;
	
    all_status=0;
	
    status = DHS_S_SUCCESS;
    n = 0;
    while (dhsFrameHeader[n].keyword)
    {
	if (frameHdr[bufNum][n].status==OK) 
	{
	    if (dhsFrameHeader[n].type==DCALONG)
		dhsBdAttribAdd( frame, dhsFrameHeader[n].keyword, DHS_DT_INT32, 0, NULL, frameHdr[bufNum][n].value.lval,&status);
	    else if(dhsFrameHeader[n].type==DCADOUBLE)
		dhsBdAttribAdd( frame, dhsFrameHeader[n].keyword, DHS_DT_DOUBLE, 0, NULL, frameHdr[bufNum][n].value.dval,&status);
	    else if(dhsFrameHeader[n].type==DCASTRING)
		dhsBdAttribAdd( frame, dhsFrameHeader[n].keyword, DHS_DT_STRING, 0, NULL, frameHdr[bufNum][n].value.sval,&status);
	    else
		printf ("header type not supported %d for %s\n",dhsFrameHeader[n].type,dhsFrameHeader[n].keyword);
	}
	else 
	{
	    gemLogMsg(6,"header %s [%d] has bad status\n",dhsFrameHeader[n].keyword,n);
	    all_status++;
	}
		
	if(status != DHS_S_SUCCESS) 
	{
	    printf("ERROR: setFrameHeader - unable to set header keyword %s\n",dhsFrameHeader[n].keyword);
	    all_status++;
	}
	n++;
    }
	
    n = 0;
    while (dhsFrameRoiHeader[n].keyword) 
    {
	if (frameRoiHdr[bufNum][n].status==OK) 
	{
	    if (dhsFrameRoiHeader[n].type==DCALONG)
		dhsBdAttribAdd( frame, dhsFrameRoiHeader[n].keyword, DHS_DT_INT32, 0, NULL, frameRoiHdr[bufNum][n].value.lval,&status);
	    else if(dhsFrameRoiHeader[n].type==DCADOUBLE)
		dhsBdAttribAdd( frame, dhsFrameRoiHeader[n].keyword, DHS_DT_DOUBLE, 0, NULL, frameRoiHdr[bufNum][n].value.dval,&status);
	    else if(dhsFrameRoiHeader[n].type==DCASTRING)
		dhsBdAttribAdd( frame, dhsFrameRoiHeader[n].keyword, DHS_DT_STRING, 0, NULL, frameRoiHdr[bufNum][n].value.sval,&status);
	    else
		printf ("header type not supported %d for %s\n",dhsFrameRoiHeader[n].type,dhsFrameRoiHeader[n].keyword);
	}
	else 
	{
	    gemLogMsg(6,"header %s [%d] has bad status\n",dhsFrameRoiHeader[n].keyword,n);
	    all_status++;
	}
		
	if(status != DHS_S_SUCCESS) 
	{
	    printf("ERROR: setFrameHeader - unable to set header keyword %s\n",dhsFrameRoiHeader[n].keyword);
	    all_status++;
	}
	n++;
    }
	

    return all_status;
}
