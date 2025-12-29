static struct {
    void *v;
    char *c;
} rcsid = {
    &rcsid,
    "$Id: gnTakeData.c,v 1.2 2009/05/27 19:32:30 fkraemer Exp $"
};


/* #define DEBUG */
/*****************************************************************************
 * Copyright 1997 Association of Universities for Research in Astronomy, Inc. 
 * See the file COPYRIGHT for more details.
 *
 * Filename: 	
 * 	gnTakeData.c
 *
 * Description:
 * 	This file contains the routines used to setup to take a specific kind 
 *	    of Data
 *
 * Function name(s)
 * 	DQrdd - calls pipe description routines to set up acquisition and
 *		processing pipes and pats for Reset Read Read code with
 *		possible Fowler (Low Noise Read) Sampling.
 *
 * 	DQrrd - calls pipe description routines to set up acquisition and
 *		processing pipes and pats for Row Reset Read code with only
 *		straight coadds.
 *      DQsep - Starts up the sep readout and processing pipes.
 *      DQtest - Starts up the test readout and processing pipes.
 *      pipetest - used in previous generation
 *      gnrt - used in previous generation
 *
 * Dependencies
 * 	The Datacube Imageflow libraries must be loaded into the IOC.
 * 	ImageFlow Version 2.5.130 or later must be used.  The Imageflow event
 * 	driver for vxWorks should be loaded and running.
 *
 * Orginial Author:
 *	Nick C. Buchholz
 *
 * History:
 *	18-June-1997: Created original version - ncb
 *
 ***************************************************************************/
#include <sys/types.h>
#include <sys/times.h>

#include <stdio.h>
#include <string.h>

#include <vxWorks.h>  
#include <taskLib.h>
#include <semLib.h>
#include <ioLib.h>    
#include <iosLib.h>    
#include <pipeDrv.h>  
#include <sysLib.h> 
#include <time.h>
#include <semLib.h> 

#include <epCommon.h>
#include <gnerrno.h>
#include "gnDCADefs.h"
#undef	MAIN
#include "gnDCAVars.h"
#include "gnDQSocket.h"
#include "dcvx.h"  /* include for temporary dhs*/
#include "car.h"
#include "saver.h"

/*extern  globals*/
extern SEM_ID dcaRegLock;
extern SEM_ID coaddSem;
extern saverParams svrP;
extern int intDisable;
extern coAdSems sem;
extern char *headerVals[255][2];
extern int dqDebug;
extern int dq_debug;

/* global variables*/
int firstFrame = 1;
char tmp[80];
int XX = 0,YY = 0;

/* function prototypes*/
int getFitsName(char *label);
int getDsHeader( int timing ,int buf);
void testCoadd(int numCoadds,int fpc);
int clearBuf(int buf);


/*****************************************************************************
 * Function name:
 * 	DCArdd
 *
 * Invocation:
 * 	status = DCArdd ( );
 *
 * PARAMETERS:
 *	None
 *
 * FUNCTION VALUE:
 * 	int - Status value returned by function 
 *
 * PURPOSE:
 * 	Starts up the RDD readout and processing pipes.
 *
 * DESCRIPTION:
 * 	Uses the standard pipe connection functions to setup Datacube
 *
 * EXTERNAL VARIABLES:
 * 	Lots - see gnaaDQVars.h
 *
 * PRIOR REQUIREMENTS:
 *	Assumes two Datacube boards with six, 4Mb Am memory modules each
 *		board. Assumes the ImageFlow Libraries are loaded.  
 *
 * DEFICIENCIES:
 * 	None known
 *
 * HISTORY:
 * 	Created - 18-June-1997  Original version  N Buchholz
 *  	Modified - 08-December-1999 - changed code to handle noaoCoAdd board - ncb
 *
 *****************************************************************************/
int pr(int a)
{
  return a;
}
int DCArdd()
{
  int intTime;
  int nextInBuf;
  int status = (OK);
  int cntCA, cntLNR;
  long binTrue = 1;
#ifdef DEBUG
  int dqDebug = 1;
  int t1 = 1;
#endif
  int dx,dy;
  printf("\n\n gnTakedata start\n");

  /*enable interrupts*/
  intDisable = 0;
  
  DPRINT(dqDebug,"DCArdd obsflags\n");
  setObsFlags(FALSE, TRUE, TRUE); /*set prep FALSE, acq TRUE, rdout FALSE */
  /*     timex(setObsFlags,FALSE, TRUE, FALSE); */  
   
    
  dx = arsize[arSizeIdx];
  dy = arsize[arSizeIdx];
	
  nextInBuf = getEmptyBuf();
  while (nextInBuf == ERROR)	{
	sleep(0,100000000);
	nextInBuf = getEmptyBuf();
  }
	

  /* get and store Header Info */
  /*     if(getDsHeader(BEFORE,nextInBuf) == ERROR)  { */
  /* 	printf("error getting header info\n");  */
  /*     } */

  dcaLoadRegister(PCOADDSTART, captBufAddr[nextInBuf]);
  dcaLoadRegister(PIXELCNT,  (dx*dy)-1);
  dcaLoadRegister(CNTRLFLAGS, DCA_ADD | DCA_FIRSTF);
  dcaLoadRegister(IVCOADDDONE,COADD_INT_NUM);
  dcaLoadRegister(PDESSTART, captBufTAddr[nextInBuf]); 
  dcaLoadRegister(ROWCNT, dy-1);
  dcaLoadRegister(COLCNT, dx-1);
    
    
  /* go tell Epics/TP side to start*/
  semTake(coaddSem,WAIT_FOREVER);
  /*     printf("captbuf = %d\n",nextInBuf); */
  /*   status = openEpicsSocket(); */
  if(status == OK) {
	rdBanCom635Time(BEFORE,nextInBuf);
	gnPutEpicsT(dbTop, DCAREADY ".VAL",DCALONG, &binTrue); 
	gnGetEpicsT(dbTop, INT_TIME ".VAL",DCALONG, &intTime); 
	/* Start the Data taking loop */
    /* 	closeEpicsSocket(); */
      
  }
  else
	printf("Error opening socket in dcardd\n");
  cntCA = 0;			/* zero CoAdd counter */
  if(svrP.server == FITS_SAVE)
	  status = getFitsName(svrP.param.fits.filename[nextInBuf]);
  while ((cntCA < numCoAdds)&&(status == OK))    {
	/* 	printf("coadd #%d\n",cntCA); */
	/* run first bias frame capture and processing algorithm */
    if (cntCA > 0)
      dcaLoadRegister(CNTRLFLAGS, DCA_ADD);
      
	/* run bias frame capture and processing algorithm LNRs times*/
	cntLNR = 0;
#ifdef COADD_TEST
	DPRINT(1,"	Calling coadd test function\n");
	testCoadd(numLNRs,framesPerCycle);
#endif 	
	while (cntLNR < numLNRs)
      {
	    if (cntLNR > 0)
          dcaLoadRegister(CNTRLFLAGS, DCA_ADD);
	    printf("waiting for bias frame lnr #%d\n",cntLNR);  
	  
	    if( semTake(sem.coAdFrame,sysClkRateGet()*15 ) != OK)
          {
            printf("Error getting first frame\n");
            status = ERROR;
            break;
          }
  
	    /* 	    printf("%d",cntLNR); */
	    numAcq++;
	    cntLNR++;
      }
	
    /* 	if (DCA_Abort == TRUE)  */
    /* 	{ */
    /* 	    printf("aborting observe \n"); */
    /*   printf("put full buf\n"); */
    /* 	    putFullBuf(nextInBuf); */
    /* 	    break; */
    /* 	} */

	/*     DPRINT(t1,"Take second frame\n"); */
	cntLNR = 0;
	while (cntLNR < numLNRs - 1)
      {
/*  	    dcaLoadRegister(CNTRLFLAGS, DCA_ADD); */
		 dcaLoadRegister(CNTRLFLAGS, DCA_SUB);
	    printf("\n waiting for signalframe lnr = %d\n",cntLNR); 
	  
	    if(semTake(sem.coAdFrame, sysClkRateGet()*(15+intTime)) != OK)
          {
            printf("Error getting second frame lnr = %d\n",cntLNR);
            semGive(coaddSem);
            clearBuf(nextInBuf);
            status = ERROR;
            break;
          }
	    /* 	    printf("%d",cntLNR); */
	    numAcq++;
	    cntLNR++;
      }

	/* decide if this is the last frame for last LNR last Coadded FRAME */
	if (cntCA < numCoAdds - 1)
      {
/* 	    dcaLoadRegister(CNTRLFLAGS, DCA_ADD); */
	    dcaLoadRegister(CNTRLFLAGS, DCA_SUB );
      } 
	else
      {
	    /*       rdBanCom635Time(-1,nextInBuf); */
/* 	    dcaLoadRegister(CNTRLFLAGS, DCA_ADD | DCA_LASTF);  */
	    dcaLoadRegister(CNTRLFLAGS, DCA_SUB | DCA_LASTF); 
	 
	    putFullBuf(nextInBuf);
      }
	
	/* 	printf("\n\n waiting for last frame to start\n"); */
	/*     semTake(sem.coAdFrame, WAIT_FOREVER); */


	printf(" waiting for last frame to finish\n");
	if(semTake(sem.coAdFrame,sysClkRateGet()*(15+intTime)) != OK)
      {
	    printf("Error getting last frame %d\n",nextInBuf);
	    status = ERROR;
	    semGive(coaddSem);
	    clearBuf(nextInBuf);
	    break;
      }
	cntCA++; 
	/* on the last frame this will cause the system to drop out of the 
	   coAdd loop The frame will probably not arrive until later but we 
	   are done here everything else will be taken care of by the DHS 
	   interface which will get the Final headers and off load the Data.*/
  }  
  /*     printf("gnTakeData: release coaddsem!!!!!!!!!!!!!!!!!!!!!!\n"); */
  if(status  == OK)
    semGive(coaddSem);
  dcaLoadRegister(CNTRLFLAGS, DCA_ADD | DCA_FIRSTF); 
  setObsFlags(FALSE, FALSE, FALSE);
  /*     	rdBanCom635Time(AFTER); */
    
  /*disable interrupts*/
  intDisable = 1;

  printf("Observe Finished\n");
   
  return status;
}

/*****************************************************************************
 * Function name:
 * 	DCArrd
 *
 * Invocation:
 * 	status = DCArrd ( );
 *
 * PARAMETERS:
 *	None
 *
 * FUNCTION VALUE:
 * 	int - Status value returned by function 
 *
 * PURPOSE:
 * 	Starts up the RRD readout and processing pipes.
 *
 * DESCRIPTION:
 * 	Uses the standard pipe connection functions to setup Datacube
 *
 * EXTERNAL VARIABLES:
 * 	Lots - see gnaaDCAVars.h
 *
 * PRIOR REQUIREMENTS:
 *	Assumes two Datacube boards with six, 4Mb Am memory modules each
 *		board. Assumes the ImageFlow Libraries are loaded.  
 *
 * DEFICIENCIES:
 * 	None known
 *
 * HISTORY:
 * 	18-June-1997  Original version  N Buchholz
 *
 *****************************************************************************/
int DCArrd()
{
  int intTime;
    int frames;
    int dx, dy;
    int status = (OK);
    int cntCA;
    long binTrue = 1;
    int nextInBuf;
    /*enable interrupts*/
    intDisable = 0;

    /* get and store Header Info */
    frames = (numLNRs +numCoAdds)*framesPerCycle;

    DPRINT(dqDebug,"DCArrd obsflags\n");
    setObsFlags(FALSE, TRUE, FALSE); /*set prep FALSE, acq TRUE, rdout FALSE */

        nextInBuf = ERROR;
        while (nextInBuf == ERROR)
	{
            sleep(0,100000000);
            nextInBuf = getEmptyBuf();
	}

	
/*         if(getDsHeader(BEFORE,nextInBuf) == ERROR) */
/* 	    return ERROR; */
        dx = arsize[arSizeIdx];
        dy = arsize[arSizeIdx];
        dcaLoadRegister(PIXELCNT,  (dx*dy)-1); 
        dcaLoadRegister(PCOADDSTART, captBufAddr[nextInBuf]);
        dcaLoadRegister(PDESSTART, captBufTAddr[nextInBuf]);
        dcaLoadRegister(CNTRLFLAGS, DCA_ADD | DCA_FIRSTF);
        dcaLoadRegister(ROWCNT, dy-1);
        dcaLoadRegister(COLCNT, dx-1);
    
/*     status = openEpicsSocket(); */
    if(status == OK) {
	rdBanCom635Time(BEFORE,nextInBuf);
	/* tell Epics/TP stuff to Go */
	gnGetEpicsT(dbTop, INT_TIME ".VAL",DCALONG, &intTime); 
	gnPutEpicsT(dbTop, DCAREADY ".VAL", DCALONG, &binTrue);
/* 	closeEpicsSocket(); */
    }
    else 
	printf("Error opening epics socket in dcarrd\n");
    
    cntCA = 0;
    /* run frame capture and processing algorithm Coadds times */
    while (cntCA < frames)
    {
#ifdef COADD_TEST
        DPRINT(1,"	Calling coadd test function\n");
        testCoadd(numLNRs,framesPerCycle);
#endif
        /* run first bias frame capture and processing algorithm */
        if (cntCA < frames - 1)
	{
            if (cntCA > 0)	
		dcaLoadRegister(CNTRLFLAGS, DCA_ADD);

            printf(" waiting for frame ");
            printf("%d", cntCA); 
	} 
        else 
	{
            dcaLoadRegister(CNTRLFLAGS, DCA_ADD | DCA_LASTF);
	}
      
        printf(" \n\nwaiting for last signal frame in group");
        if(semTake(sem.coAdFrame, sysClkRateGet()*15+intTime) != OK)
	{
            printf("Error getting second frame\n");
            status = ERROR;
            break;
	}

        cntCA++;
    }

    putFullBuf(nextInBuf); 
    
    /*      rdBanCom635Time(AFTER); */
  
 
    /*disable interrupts*/
    intDisable = 1;

    printf("Observe Finished\n\n");
    return status;
}


/*****************************************************************************
 * Function name:
 * 	DCAsep
 *
 * Invocation:
 * 	status = DCAsep ( );
 *
 * PARAMETERS:
 *	None
 *
 * FUNCTION VALUE:
 * 	int - Status value returned by function 
 *
 * PURPOSE:
 * 	Starts up the sep readout and processing pipes.
 *
 * DESCRIPTION:
 * 	Uses the standard pipe connection functions to setup Datacube
 *
 * EXTERNAL VARIABLES:
 * 	Lots - see gnaaDCAVars.h
 *
 * PRIOR REQUIREMENTS:
 *	Assumes two Datacube boards with six, 4Mb Am memory modules each
 *		board. Assumes the ImageFlow Libraries are loaded.  
 *
 * DEFICIENCIES:
 * 	None known
 *
 * HISTORY:
 * 	22-July-1998  Original version  N Buchholz
 *
 *****************************************************************************/
SEM_ID xxx;
#define GNDEBUG0 
int DCAsep()
{
  int intTime;
    int cntCA = 0;
    int status = (OK);
    long binTrue = 1;
    static int j = 0;
    int dx,dy;
    int frames;
    int nextInBuf = ERROR;
    DPRINT(dqDebug,"DCAsep begin\n");
    DPRINT(dqDebug,"DCAsep getheader\n");
    xxx = sem.coAdFrame;
    /*enable interrupts*/
    intDisable = 0;

    /* get and store Header Info */



	    /* calculate number of frames to expect */
    frames = (numLNRs * numCoAdds) * framesPerCycle;
     
	if(frames > emptyBuffers())
	  {
		printf("!!!!!!!!!!!!!!!!!!!!!there are not currently %d frames free\n",frames);
		return ERROR;
	  }
    DPRINT(dqDebug,"DCAsep obsflags\n");
    semTake(coaddSem,WAIT_FOREVER);
    setObsFlags(FALSE, TRUE, FALSE); /*set prep FALSE, acq TRUE, rdout FALSE */

    dx = arsize[arSizeIdx];
    dy = arsize[arSizeIdx];
    dcaLoadRegister(PIXELCNT,  (dx*dy)-1);
    dcaLoadRegister(ROWCNT, dy-1);
    dcaLoadRegister(COLCNT, dx-1);

  
    dcaLoadRegister(CNTRLFLAGS, DCA_ADD | DCA_FIRSTF | DCA_LASTF);
	if ((nextInBuf = getEmptyBuf()) != ERROR) 
	{ 
		printf("bufnum = %d\n",nextInBuf);
  if(svrP.server == FITS_SAVE)
			status = getFitsName(svrP.param.fits.filename[nextInBuf]);
		dcaLoadRegister(PCOADDSTART, captBufAddr[nextInBuf]);
		dcaLoadRegister(PDESSTART, captBufTAddr[nextInBuf]);
		putFullBuf(nextInBuf);
		if(getDsHeader(BEFORE,nextInBuf) == ERROR)
		{
			printf("error in getDsHeader\n");
			return ERROR; 
		}
	}
	else	{ 
		/*data will be lost the previous frame will be trashed by */
		printf("too many frames\n");
	}

	rdBanCom635Time(BEFORE,nextInBuf); 


	gnGetEpicsT(dbTop, INT_TIME ".VAL",DCALONG, &intTime); 
    gnPutEpicsT(dbTop, DCAREADY ".VAL",DCALONG, &binTrue); 

    while  (cntCA < frames)
      {
		  if(cntCA > 0)
		  {
			  if ((nextInBuf = getEmptyBuf()) != ERROR) 
			  { 
				  printf("bufnum = %d\n",nextInBuf);
  if(svrP.server == FITS_SAVE)
					  status = getFitsName(svrP.param.fits.filename[nextInBuf]);
				  dcaLoadRegister(PCOADDSTART, captBufAddr[nextInBuf]);
				  dcaLoadRegister(PDESSTART, captBufTAddr[nextInBuf]);
				  putFullBuf(nextInBuf);
				 /*  if(getDsHeader(BEFORE,nextInBuf) == ERROR) */
/* 				  { */
/* 					  printf("error in getDsHeader\n"); */
/* 					  return ERROR;  */
/* 				  } */
			  }
			  else	{ 
				  /*data will be lost the previous frame will be trashed by */
				  printf("too many frames\n");
			  }
		  }
		
	/* 	printf("gnTakeData: numRois = %d\n",svrP.roi.numRois); */
	/* 	printf("gnTakeData: numRois = %d\n",svrP.roi.numRois); */
	/* 	rdBanCom635Time(BEFORE,nextInBuf);  */ 
	
	if (semTake(sem.coAdFrame,sysClkRateGet()*(15+intTime)) != OK)
	  {
	    printf("Error getting  frame %d\n",cntCA);
	    status = ERROR;
	    break;
	  }
	cntCA++;
      }   
    semGive(coaddSem);
    /*   rdBanCom635Time(AFTER); */
  
  setObsFlags(FALSE, FALSE, FALSE);
 
    /*disable interrupts*/
    intDisable = 1;

    printf("Observe Finished\n\n");
    return OK;

}



/*****************************************************************************
 * Function name:
 * 	DCAtest
 *
 * Invocation:
 * 	status = DCAtest ( );
 *
 * PARAMETERS:
 *	None
 *
 * FUNCTION VALUE:
 * 	int - Status value returned by function 
 *
 * PURPOSE:
 * 	Starts up the test readout and processing pipes.
 *
 * DESCRIPTION:
 * 	Uses the standard pipe connection functions to setup Datacube
 *
 * EXTERNAL VARIABLES:
 * 	Lots - see gnaaDCAVars.h
 *
 * PRIOR REQUIREMENTS:
 *	Assumes two Datacube boards with six, 4Mb Am memory modules each
 *		board. Assumes the ImageFlow Libraries are loaded.  
 *
 * DEFICIENCIES:
 * 	None known
 *
 * HISTORY:
 * 	28-June-1997  Original version  N Buchholz
 *
 *****************************************************************************/
int DCAtest()
{
    int intTime;
    int nextInBuf;
    int status = (OK);
    int cntCA, cntLNR;
    long binTrue = 1;

#ifdef DEBUG
    int dqDebug = 1;
    int t1 = 1;
#endif
    int dx,dy;
    printf("\n\n gnTakedata test starting\n");

    /*enable interrupts*/
    intDisable = 0;
  
    DPRINT(dqDebug,"DCAtest obsflags\n");
    setObsFlags(FALSE, TRUE, FALSE); /*set prep FALSE, acq TRUE, rdout FALSE */
    
    dx = arsize[arSizeIdx];
    dy = arsize[arSizeIdx];
	
    nextInBuf = getEmptyBuf();
    while (nextInBuf == ERROR)	{
	sleep(0,100000000);
	nextInBuf = getEmptyBuf();
    }

    dcaLoadRegister(PCOADDSTART, captBufAddr[nextInBuf]);
    dcaLoadRegister(PIXELCNT, (dx*dy)-1);
    dcaLoadRegister(CNTRLFLAGS, DCA_ADD | DCA_FIRSTF);
    dcaLoadRegister(IVCOADDDONE,COADD_INT_NUM);
    dcaLoadRegister(PDESSTART, captBufTAddr[nextInBuf]); 
    dcaLoadRegister(ROWCNT, dy-1);
    dcaLoadRegister(COLCNT, dx-1);
    
    /* go tell Epics/TP side to start*/
    semTake(coaddSem,WAIT_FOREVER);

    if(status == OK) 
    {
	rdBanCom635Time(BEFORE,nextInBuf);
	gnPutEpicsT(dbTop, DCAREADY ".VAL",DCALONG, &binTrue); 
	gnGetEpicsT(dbTop, INT_TIME ".VAL",DCALONG, &intTime); 
    }
    else
	printf("Error opening socket in dcardd\n");

    cntCA = 0;			/* zero CoAdd counter */
  
  if(svrP.server == FITS_SAVE)
		status = getFitsName(svrP.param.fits.filename[nextInBuf]);
    while ((cntCA < numCoAdds)&&(status == OK))    
    {
	/* run first bias frame capture and processing algorithm */
	if (cntCA > 0)
	    dcaLoadRegister(CNTRLFLAGS, DCA_ADD);
      
	/* run bias frame capture and processing algorithm LNRs times*/
	cntLNR = 0;

	while (cntLNR < numLNRs)
	{
	    if (cntLNR > 0)
		dcaLoadRegister(CNTRLFLAGS, DCA_ADD);
	    printf("waiting for bias frame lnr #%d\n",cntLNR);  
	  
	    if( semTake(sem.coAdFrame,sysClkRateGet()*(15 +intTime)) != OK)
	    {
		printf("Error getting first frame\n");
		status = ERROR;
		break;
	    }
  
	    numAcq++;
	    cntLNR++;
	}
	
	cntLNR = 0;
	while (cntLNR < numLNRs - 1)
	{
	    dcaLoadRegister(CNTRLFLAGS, DCA_ADD);
	    printf("\n waiting for signalframe lnr = %d\n",cntLNR); 
	  
	    if(semTake(sem.coAdFrame, sysClkRateGet()*(15+intTime)) != OK)
	    {
		printf("Error getting second frame lnr = %d\n",cntLNR);
		semGive(coaddSem);
		clearBuf(nextInBuf);
		status = ERROR;
		break;
	    }

	    numAcq++;
	    cntLNR++;
	}

	/* decide if this is the last frame for last LNR last Coadded FRAME */
	if (cntCA < numCoAdds - 1)
	{
	    dcaLoadRegister(CNTRLFLAGS, DCA_ADD );
	} 
	else
	{
	    dcaLoadRegister(CNTRLFLAGS, DCA_ADD | DCA_LASTF); 
	    putFullBuf(nextInBuf);
	}
	
	printf(" waiting for last TEST frame to finish\n");
	if(semTake(sem.coAdFrame,sysClkRateGet()*(15+intTime)) != OK)
	{
	    printf("Error getting last TEST frame %d\n",nextInBuf);
	    status = ERROR;
	    semGive(coaddSem);
	    clearBuf(nextInBuf);
	    break;
	}
	cntCA++; 
	/* on the last frame this will cause the system to drop out of the 
	   coAdd loop The frame will probably not arrive until later but we 
	   are done here everything else will be taken care of by the DHS 
	   interface which will get the Final headers and off load the Data.*/
    }  

    if(status  == OK)
	semGive(coaddSem);
    dcaLoadRegister(CNTRLFLAGS, DCA_ADD | DCA_FIRSTF);
    
    /*disable interrupts*/
    intDisable = 1;

    printf("Test Observe Finished\n");
   
    return status;

}

/* old test code replced by add add test */
#if 0
int DCAtest()
{
    int cntCA=0;
    int status = (OK);
    long binTrue = 1;
  
    int frames;
    int nextInBuf = ERROR;

    /*enable interrupts*/
    intDisable = 0;

    /* get and store Header Info */

/*     if(getDsHeader(BEFORE,nextInBuf) == ERROR) */
/* 	return ERROR;  */


    DPRINT(dqDebug,"DCAsep obsflags\n");
    setObsFlags(FALSE, TRUE, FALSE); /*set prep FALSE, acq TRUE, rdout FALSE */

    /* calculate number of frames to expect */
    frames = (numLNRs * numCoAdds) * framesPerCycle;
    cntCA = 0;
    /* loop to start each frame*/
    while (DCA_Abort == FALSE && (cntCA < frames))    {
	if ((nextInBuf = getEmptyBuf()) != ERROR) /* no buffer left you've been a bad nomer */
	{
	    dcaLoadRegister(PCOADDSTART, captBufAddr[nextInBuf]);
	    dcaLoadRegister(PDESSTART, captBufTAddr[nextInBuf]);
	    dcaLoadRegister(CNTRLFLAGS, DCA_ADD | DCA_FIRSTF | DCA_LASTF);
	    putFullBuf(nextInBuf);
	}
	else	{ 
	    /* data will be lost the previous frame will be trashed by coadding */
	    dcaLoadRegister(CNTRLFLAGS, DCA_ADD | DCA_FIRSTF );
	}
	
	if (cntCA == 0) /* if this is the first frame tell Epics stuff to Go */
	{
	   /*  status = openEpicsSocket(); */
	    if(status == OK) {
	      gnGetEpicsT(dbTop, INT_TIME ".VAL",DCALONG, &intTime); 	
	      gnPutEpicsT(dbTop, DCAREADY ".VAL",DCALONG, &binTrue); 
	/* 	closeEpicsSocket(); */
	    }
	    else 
		printf("Error opening socket in dcatest\n");
	    
	}
	
	if(semTake(sem.coAdFrame, sysClkRateGet()*(15+intTime)) != OK)
	{
	    printf("Error getting  frame\n");
	    status = ERROR;
	    break;
	}
	/* 	  rdBanCom635Time(AFTER); */
	cntCA++;
    }    

    /*disable interrupts*/
    intDisable = 1;
    return OK;

}
#endif
