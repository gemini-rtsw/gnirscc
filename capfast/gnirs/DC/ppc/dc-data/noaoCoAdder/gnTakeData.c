static struct {
    void *v;
    char *c;
} rcsid = {
    &rcsid,
    "$Id: gnTakeData.c,v 1.2 2009/05/27 19:32:44 fkraemer Exp $"
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
/* #include "dcvx.h" */  /* include for temporary dhs*/
#include "carRecord.h"
#include "saver.h"

/*extern  globals*/
extern SEM_ID dcaRegLock;
extern SEM_ID coaddSem;/**/
extern saverParams svrP;
extern int noao_intDisable;
extern coAdSems sem;
extern char *headerVals[255][2];
extern int dqDebug;
extern int dq_debug;

/* global variables*/
int firstFrame = 1;
char tmp[80];
int XX = 0,YY = 0;

/* function prototypes*/
int getDsHeader( int timing ,int buf);
void testCoadd(int numCoadds,int fpc);
int clearBuf(int buf);
STATUS getFrameHeader( int timing,int bufnum);

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
  int expTime;
  int nextInBuf;
  int status = (OK);
  int cntCA, cntLNR;
  long binTrue = 1;
  int detector;
#ifdef DEBUG
  int dqDebug = 1;
#endif
  int dx,dy;

#ifdef TRACE
  printf("\n\n gnTakedata start\n");
#endif

/*   printf("dcardd: 1 format = %d\n",svrP.param.dhs.format); */
	status = OK;
  
  
  DPRINT(dqDebug,"DCArdd obsflags\n");
  setObsFlags(FALSE, TRUE, TRUE); /*set prep FALSE, acq TRUE, rdout FALSE */
 
  if((strcmp(A2,detType) == 0) || (strcmp(A3_A,detType) == 0))
  {
    dx = arsize[arSizeIdx];
    dy = arsize[arSizeIdx];
/*        dy = 100; */
    if(strcmp(A2,detType)  == 0)
      detector = 2;
    else
      detector = 3;
  }
  else if (strcmp(A3_B,detType) == 0)
    {
      dx = a3csize[arSizeIdx];
      dy = a3rsize[arSizeIdx];
/*       dy = 2; */
      detector = 3;
    }
  else
    {
      printf("wrong array type\n");
      status = ERROR;
    }	

/*   printf("dcardd get buffer\n"); */
  nextInBuf = getEmptyBuf();
  while (nextInBuf == ERROR)	{
	sleep(0,100000000);
	nextInBuf = getEmptyBuf();
  }
	
/*   printf("dcardd got buffer\n"); */

 
/*   printf("DCArdd: taking coaddSem\n"); */
  semTake(coaddSem,WAIT_FOREVER);
  dcaLoadRegister(CNTRLFLAGS, DCA_ADD | DCA_FIRSTF);
  dcaLoadRegister(PCOADDSTART, captBufAddr[nextInBuf]);
  dcaLoadRegister(PIXELCNT, (dx*dy)); 
  dcaLoadRegister(IVCOADDDONE,COADD_INT_NUM);
  dcaLoadRegister(PDESSTART, captBufTAddr[nextInBuf]); 
  dcaLoadRegister(ROWCNT, dy-1);
  dcaLoadRegister(COLCNT, dx-1);
    
    
  /* go tell Epics/TP side to start*/
  /*     printf("captbuf = %d\n",nextInBuf); */
  if(status == OK) 
  {
	  initHeader(nextInBuf); 
	  /* get and store Header Info */
	  if(getDsHeader(BEFORE,nextInBuf) == ERROR)  
	  {
		  printf("error getting DS header info\n"); 
	  } 
	  if(getFrameHeader(BEFORE,nextInBuf) == ERROR)  
	  { 
		  printf("error getting frame header info\n"); 
	  }
	  printf("start image\n");
	  /*enable interrupts*/
	  noao_intDisable = 0;
	  gnGetEpicsT(dbTop, INT_TIME ".VAL",DCALONG, &intTime); 
	  gnGetEpicsT(dbTop, EXP_TIME ".VAL",DCALONG, &expTime); 
	  gnPutEpicsT(dbTop, DCAREADY ".VAL",DCALONG, &binTrue); 
	  /*printf("Integration  Time = %i\n", intTime); */
	  /* Start the Data taking loop */
  }
  else
	  printf("Error opening socket in dcardd\n");
  cntCA = 0;			/* zero CoAdd counter */
  
  /* this crash DHS - memcorruption of svrP.dhs struct !! - mdcb */
  /*   printf("dcardd: format = %d\n",svrP.param.dhs.format); */
  if (svrP.server == FITS_SAVE)
	  printf("ERROR, no fits server support !!!\n");
  
  while ((cntCA < numCoAdds)&&(status == OK))    
  {
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
		  
#ifdef TRACE
		  printf("waiting for bias frame lnr #%d\n",cntLNR);  
#endif 	
		  
		  
		  if( semTake(sem.coAdFrame,sysClkRateGet()*15 ) != OK)
          {
			  printf("Error getting first frame\n");
			  status = ERROR;
			  break;
          }
		  printf("got coadsem\n");
		  numAcq++;
		  cntLNR++;
      }
	  
	
	  cntLNR = 0;
	  while ((cntLNR < numLNRs - 1)&&(status == OK))
      {
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
		  printf("got coadsem 2\n");
		  /* 	    printf("%d",cntLNR); */
		  numAcq++;
		  cntLNR++;
      }
	  
	  /* decide if this is the last frame for last LNR last Coadded FRAME */
	  if (cntCA < numCoAdds - 1)
      {
		  dcaLoadRegister(CNTRLFLAGS, DCA_SUB );
      } 
	  else
      {
		  /*  getDsHeader(-1,nextInBuf);  */
		  dcaLoadRegister(CNTRLFLAGS, DCA_SUB | DCA_LASTF); 
		  
		  putFullBuf(nextInBuf);
      }
	  
	  /* 	printf("\n\n waiting for last frame to start\n"); */
	  /*     semTake(sem.coAdFrame, WAIT_FOREVER); */
	  
	  
#ifdef TRACE
	  printf(" waiting for last frame to finish\n");
#endif
	  
	  
	  if((status == OK)&&(semTake(sem.coAdFrame,sysClkRateGet()*(15+intTime)) != OK))
      {
		  printf("Error getting last frame %d\n",nextInBuf);
		  status = ERROR;
		  semGive(coaddSem);
		  clearBuf(nextInBuf);
		  break;
      }
	 
	  /* taskDelay(10); */
	  cntCA++; 

	  /* on the last frame this will cause the system to drop out of the 
		 coAdd loop The frame will probably not arrive until later but we 
		 are done here everything else will be taken care of by the DHS 
		 interface which will get the Final headers and off load the Data.*/
  }  
  
  if(status  == OK)
	  semGive(coaddSem);
  /*disable interrupts*/
  noao_intDisable = 1;
  setObsFlags(FALSE, FALSE, FALSE);
  dcaLoadRegister(CNTRLFLAGS, DCA_ADD | DCA_FIRSTF);
  /*   getDsHeader(AFTER,nextInBuf);  */
  
  
#ifdef TRACE
   printf("Observe Finished\n");
#endif
  
    
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
    int frames;
    int dx, dy;
    int status = (OK);
    int cntCA;
    long binTrue = 1;
    int nextInBuf;
    int detector;

 
 
    /*enable interrupts*/
    noao_intDisable = 0;
    /* get and store Header Info */
    frames = (numLNRs +numCoAdds)*framesPerCycle;


	status = OK;

    DPRINT(dqDebug,"DCArrd obsflags\n");
    setObsFlags(FALSE, TRUE, TRUE); /*set prep FALSE, acq TRUE, rdout TRUE */

	nextInBuf = ERROR;
	while (nextInBuf == ERROR)
	{
		/* 	  printf("gnTakeData waiting for buffer\n"); */
		sleep(0,100000000);
		nextInBuf = getEmptyBuf();
	}
	
	
	if(getDsHeader(BEFORE,nextInBuf) == ERROR) 
		return ERROR;  
	if(getFrameHeader(BEFORE,nextInBuf) == ERROR)  
	  { 
		  printf("error getting frame header info\n"); 
	  }
	if((strcmp(A2,detType) == 0) || (strcmp(A3_A,detType) == 0))
	{
	    dx = arsize[arSizeIdx];
	    dy = arsize[arSizeIdx];
	    detector = 2;
	}
	else if (strcmp(A3_B,detType) == 0)
	{
	    dx = a3csize[arSizeIdx];
	    dy = a3rsize[arSizeIdx];
	    detector = 3;
	}
	else
	{
	    printf("wrong array type\n");
	    status = ERROR;
	}	
	/*         dx = arsize[arSizeIdx]; */
	/*         dy = arsize[arSizeIdx]; */
	dcaLoadRegister(PIXELCNT, (dx*dy)-1);
	dcaLoadRegister(PCOADDSTART, captBufAddr[nextInBuf]);
	dcaLoadRegister(PDESSTART, captBufTAddr[nextInBuf]);
	dcaLoadRegister(CNTRLFLAGS, DCA_ADD | DCA_FIRSTF);
	dcaLoadRegister(ROWCNT, dy-1);
	dcaLoadRegister(COLCNT, dx-1);
    
    if(status == OK) 
	{
		initHeader(nextInBuf);
		/* tell Epics/TP stuff to Go */
		gnPutEpicsT(dbTop, DCAREADY ".VAL", DCALONG, &binTrue);
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
        if(semTake(sem.coAdFrame, sysClkRateGet()*15) != OK)
	{
            printf("Error getting second frame\n");
            status = ERROR;
            break;
	}
	printf("got coad int\n");
		/* taskDelay(10); */
        cntCA++;
    }

    putFullBuf(nextInBuf); 
    
    /*      getDsHeader(AFTER); */
  
 
    /*disable interrupts*/
    noao_intDisable = 1;

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
#define GNDEBUG0 
int DCAsep()
{
    int cntCA = 0;
    long binTrue = 1;
    int dx,dy;
    int frames;
    int nextInBuf = ERROR;
    int detector;
    int status;


    DPRINT(dqDebug,"DCAsep begin\n");
    DPRINT(dqDebug,"DCAsep getheader\n");



    /*enable interrupts*/
    noao_intDisable = 0;

    /* get and store Header Info */

	if(getDsHeader(BEFORE,nextInBuf) == ERROR) 
	{ 
		printf("error in getDsHeader\n"); 
		return ERROR;  
	} 
	  if(getFrameHeader(BEFORE,nextInBuf) == ERROR)  
	  { 
		  printf("error getting frame header info\n"); 
	  }

    DPRINT(dqDebug,"DCAsep obsflags\n");
    semTake(coaddSem,WAIT_FOREVER);
    setObsFlags(FALSE, TRUE, TRUE); /*set prep FALSE, acq TRUE, rdout TRUE */
  
    if((strcmp(A2,detType) == 0) || (strcmp(A3_A,detType) == 0))
      {
	dx = arsize[arSizeIdx];
	dy = arsize[arSizeIdx];
	detector = 2;
      }
    else if (strcmp(A3_B,detType) == 0)
      {
	dx = a3csize[arSizeIdx];
	dy = a3rsize[arSizeIdx];
	detector = 3;
      }
    else
      {
	printf("wrong array type\n");
	status = ERROR;
      }	
   /*  dx = arsize[arSizeIdx]; */
/*     dy = arsize[arSizeIdx]; */
    dcaLoadRegister(PIXELCNT, (dx*dy - 1)); 
    dcaLoadRegister(ROWCNT, dy-1);
    dcaLoadRegister(COLCNT, dx-1);

    /* calculate number of frames to expect */
    frames = (numLNRs * numCoAdds) * framesPerCycle;
     
    dcaLoadRegister(CNTRLFLAGS, DCA_ADD | DCA_FIRSTF | DCA_LASTF);
    gnPutEpicsT(dbTop, DCAREADY ".VAL",DCALONG, &binTrue); 



    while  (cntCA < frames)
    {
	if ((nextInBuf = getEmptyBuf()) != ERROR)
	{
	    dcaLoadRegister(PCOADDSTART, captBufAddr[nextInBuf]);
	    dcaLoadRegister(PDESSTART, captBufTAddr[nextInBuf]);
	    putFullBuf(nextInBuf);
	}
	else	{ 
	    /* data will be lost the previous frame will be trashed by  */
	    printf("too many frames\n");
	}
	
/* 	printf("gnTakeData: numRois = %d\n",svrP.roi.numRois); */
	
	/* not seen yet using the DHS, but probably same problem
	and not needed by DHS hence do check too - mdcb */
	if (svrP.server == FITS_SAVE)
			printf("ERROR, no fits server support !!!\n");
/* 	printf("gnTakeData: numRois = %d\n",svrP.roi.numRois); */
initHeader(nextInBuf);

	if (semTake(sem.coAdFrame,sysClkRateGet()*15) != OK)
	{
	    printf("Error getting  frame %d\n",cntCA);
	    break;
	}
	printf("got coadsem sep\n");
	/* taskDelay(10); */

	cntCA++;
    }   

    semGive(coaddSem);   
	setObsFlags(FALSE, FALSE, FALSE);
    /*   getDsHeader(AFTER); */
  
 
    /*disable interrupts*/
    noao_intDisable = 1;

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
int DCAtest_orig()
{
    int cntCA=0;
    int status = (OK);
    long binTrue = 1;
  
    int frames;
    int nextInBuf = ERROR;

    /*enable interrupts*/
    noao_intDisable = 0;

    /* get and store Header Info */

     if(getDsHeader(BEFORE,nextInBuf) == ERROR) 
 	return ERROR;  

	  if(getFrameHeader(BEFORE,nextInBuf) == ERROR)  
	  { 
		  printf("error getting frame header info\n"); 
	  }

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
		gnPutEpicsT(dbTop, DCAREADY ".VAL",DCALONG, &binTrue); 
	/* 	closeEpicsSocket(); */
	    }
	    else 
		printf("Error opening socket in dcatest\n");
	    
	}
	
	if(semTake(sem.coAdFrame, sysClkRateGet()*15) != OK)
	{
	    printf("Error getting  frame\n");
	    status = ERROR;
	    break;
	}
	
	/* taskDelay(10); */
	
	/* 	  getDsHeader(AFTER); */
	cntCA++;
    }    

    /*disable interrupts*/
    noao_intDisable = 1;

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
 * 	transfert the 16 coadder buffers - no frames taken
 *
 * DESCRIPTION:
 *
 * EXTERNAL VARIABLES:
 *
 * PRIOR REQUIREMENTS:
 *
 * DEFICIENCIES:
 *
 * HISTORY:
 * 	12 April 2001  Original version  Matthieu Bec
 *
 *****************************************************************************/
int DCAtest()
{
	int nextInBuf;
    long binTrue = 1;
	
    /* enable interrupts */
    noao_intDisable = 0;
	
	semTake(coaddSem,WAIT_FOREVER);
	for (nextInBuf=0;nextInBuf<numBufs;nextInBuf++) {
		setObsFlags(FALSE, TRUE, TRUE); /*set prep FALSE, acq TRUE, rdout TRUE */	
		/* tell saver task it's okay to start */
		printf("dumping buffer %d [%d]\n",nextInBuf,numBufs);
		putFullBuf(nextInBuf);
		semGive(coaddSem);
		/* tell the saver task to save buffer */
		unscrambleIntHndlr(0);
		/* let the other task start */
		printf("let saver task start\n");
		taskDelay(300);
		/* wait for completion */
		printf("waiting for saver task\n");
		semTake(coaddSem,WAIT_FOREVER);
		}
	
	/*disable interrupts*/
    semGive(coaddSem);   
	setObsFlags(FALSE, FALSE, FALSE);
	noao_intDisable = 1;
	gnPutEpicsT(dbTop, DCAREADY ".VAL",DCALONG, &binTrue);
	gnPutEpicsT(dbTop, FRAMEREADY ".VAL",DCASTRING, "ready");
    return OK;

}

/*****************************************************************************/

