static struct {
        void *v;
        char *c;
} rcsid = {
        &rcsid,
        "$Id: gnStartDCA.c,v 1.2 2009/05/27 19:32:46 fkraemer Exp $"
};


/*****************************************************************************
 * Copyright 1997 Assocation of Universities for Research in Astronomy, Inc. 
 * See the file COPYRIGHT for more details.
 *
 * Filename: 	
 * 	gnStartDCA.c
 *
 * Description:
 * 	This file contains the main startup program for the GNAAC data CoAdder
 *	Board routines
 *
 * Function name(s)
 * 	gnDCAStart - creates System Variables, sets up the environment and
 *		starting configuration of the NOAO CoAdder board
 *	initTask - starts an indepent process which waits for instructions to
 *		take data. Uses VxWorks semaphores to wait.
 *	doRDD - task to do Reset, read Data, read Data, data taking 
 *	      - calls DCArdd
 *	doRRD - task to do Reset, read Data, data taking - calls DCArrd
 *	doSEP - Task to take Sepped data - calls DCAsep
 *	doTEST - 
 *
 * Dependencies
 *      The NOAO CoAdder board interupt handler must be loaded and running.
 *
 * Orginial Author:
 *	Nick C. Buchholz
 *
 * History:
 *	12-September-1999: Created original version - ncb
 *
 ***************************************************************************/
#include <sys/types.h>
#include <sys/times.h>

#include <stdio.h>
#include <vxWorks.h>  
#include <taskLib.h>
#include <semLib.h>
#include <ioLib.h>    
#include <sysLib.h>
#include <vme.h>

#include <cadef.h> 
#include <carRecord.h>
#include <cadRecord.h>
#define NODBACCESS
#define DCA_IOC
#include <epCommon.h>
#include "gnDQSocket.h"

/* #define TASKINIT */
#define EPICS
 
#include <gnerrno.h>
#include "gnDCADefs.h"
#undef	MAIN
#include "gnDCAVars.h"


extern coAdSems sem;
extern SEM_ID coaddSem;
extern SEM_ID dcaRegLock;
int saveDataset();
extern int dcadebug ;
static char retString[80];
/* function prototypes*/
int transFrame();
int initTask( SEM_ID *semId, SEM_B_STATE semState, int priority, int options,int stack, FUNCPTR func, char *name);
/*****************************************************************************
 * Function name:
 * 	gnDCAStart
 *
 * Invocation:
 * 	status = gnDCAStart ( );
 *
 * PARAMETERS:
 *	None
 *
 * FUNCTION VALUE:
 * 	int - Status value returned by function 
 *
 * PURPOSE:
 * 	Starts up the Standard DataCoadder system
 *
 * DESCRIPTION:
 * 	Uses the gnCreateStdSys and gnDCASurfInit routines to startup the
 *	data CoAdder system and do standard hardware initialization for
 *	tasks used by GNAAC programs.
 *
 * EXTERNAL VARIABLES:
 * 	Lots - see gnDCAVars.h
 *
 * PRIOR REQUIREMENTS:
 *	Assumes an NOAO CoAdder board and event interupt handler are installed.
 *
 * DEFICIENCIES:
 * 	None known
 *
 * HISTORY:
 * 	12-September-1999  Original version  N Buchholz
 *
 *****************************************************************************/
int gnDCAStart( void )
{
    int status=OK;	/* function return status */

    /* create and initialize coadder system and devices */
/*     cicsLogMessage(1,"\n    Initialize coadder Board ....\n"); */

     status = gnCreateStdSys(); 
     /* Calculate and assign interupt vectors */
     if((status = interruptInit()) != OK)
     {
	 printf("gnDCAStart: error in interruptInit\n");
	 return status;
     }
     
	 if((status = taskSpawn("tsaveDataSet",60,VX_FP_TASK,30000,saveDataset,
			    NULL,0,0,0,0,0,0,0,0,0)) == ERROR)
	printf("gnDCAStart: error spawning task saveDataset");
#ifdef COADD_TEST
     if(( status =  taskSpawn("ttransFrame",60,VX_FP_TASK,4000,transFrame,
			     NULL,0,0,0,0,0,0,0,0,0)) == ERROR)
     {
        printf("gnDCAStart: error spawning task transFrame");
	 return status;
     }
#else
    /* calculate and assign base Address */  
     if((status = sysBusToLocalAdrs(VME_AM_SUP_SHORT_IO, 
				    (void *)DCA_BASE,
				    (void *) &oSystem.pDCARegs)) != OK )
     {
	 printf("DCAInit: Cannot convert VME address to local.\n");
	 return( status ) ;
     }
    
     printf("coadd address is %x\n",(unsigned int)oSystem.pDCARegs);
     dcaLoadRegister(IVCOADDDONE,COADD_INT_NUM);
     dcaLoadRegister(IVXFERDONE,TRANS_INT_NUM);
     dcaLoadRegister(IVDESDONE,UNSCRAMBLE_INT_NUM);
#endif	
     
     
     
     status = initTask(&semRDD, SEM_EMPTY, 50, VX_FP_TASK, 6000, doRDD, 
		       "doRDD");
     if(status)
	 status = initTask(&semRRD, SEM_EMPTY, 50, VX_FP_TASK, 6000, doRRD, 
			   "do	RRD");
     if(status)
     status = initTask(&semTEST, SEM_EMPTY, 50, VX_FP_TASK, 6000, doTEST, 
		       "doTest");
     if(status)
	 status =initTask(&semSEP, SEM_EMPTY, 50, VX_FP_TASK, 6000, doSEP, 
			  "doSEP");
     if(status)
	 status =initTask(&semSetupRDD, SEM_EMPTY, 50, VX_FP_TASK, 6000, 
			  gnRDDStareSetup, "RDDSetup");
     if(status)
	 status = initTask(&semSetupRRD, SEM_EMPTY, 50, VX_FP_TASK, 6000, 
			   gnRRDStareSetup,  "RRDSetup");
     if(status)
	 status = initTask(&semSetupTEST, SEM_EMPTY, 50, VX_FP_TASK, 6000, 
			   gnTestSetup, "TestSetup");
     if(status)
	 status =initTask(&semSetupSEP, SEM_EMPTY, 50, VX_FP_TASK, 6000, 
			  gnSepSetup,  "SepSetup");
     if(status == ERROR)
     {
	 printf("gnDCAStart: error in initTask\n");
	 return status;
     }
     
     /* Create semaphore for the setup tasks */
     if((semSetup = semBCreate(SEM_Q_FIFO,SEM_EMPTY)) == NULL)
     {
	 printf("gnDCAStart: error creating semaphore\n");
	 return ERROR;
     }
     if((semDMA = semBCreate(SEM_Q_FIFO,SEM_FULL)) == NULL)
     {
	 printf("gnDCAStart: error creating semaphore\n");
	 return ERROR;
     }
     if(( obsDone = semBCreate(SEM_Q_FIFO,SEM_EMPTY)) == NULL)
     {
	 printf("gnDCAStart: error creating semaphore\n");
	 return ERROR;
     }
     /* semaphores*/
     if((sem.coAdFrame = semCCreate(SEM_Q_FIFO,0)) == NULL)
     {
	 printf("gnDCAStart: error creating semaphore\n");
	 return ERROR;
     }
     if((sem.transFrame = semCCreate(SEM_Q_FIFO,0)) == NULL)
     {
	 printf("gnDCAStart: error creating semaphore\n");
	 return ERROR;
     }
     if((sem.unScrambleFrame = semCCreate(SEM_Q_FIFO,0)) == NULL)
     {
	 printf("gnDCAStart: error creating semaphore\n");
	 return ERROR;
     }
     if((dcaRegLock = semBCreate(SEM_Q_FIFO,0)) == NULL)
     {
	 printf("gnDCAStart: error creating semaphore\n");
	 return ERROR;
     }
     if((dcaRegLock = semBCreate(SEM_Q_FIFO,SEM_FULL)) == NULL)
     {
	 printf("gnDCAStart: error creating semaphore\n");
	 return ERROR;
     }
     if((coaddSem = semBCreate(SEM_Q_FIFO,SEM_FULL)) == NULL)
     {
	 printf("gnDCAStart: error creating semaphore\n");
	 return ERROR;
     }


     /* everything else depends on the configuration at run time and is setup
      * by gnSetupRDD or 	gnSetupRD depending on ucode, etc.
      */
     rddSetupDone = FALSE;
     rrdSetupDone = FALSE;
     testSetupDone = FALSE;
     printf("gnDCAStart finished\n");
     return (status);
}

/*****************************************************************************
 * Function name:
 * initTask
 *
 * Invocation:
 * initTask( SEM_ID *semId, SEM_B_STATE semState, int priority, int options,
	      int stack, FUNCPTR func, char *name)
 * PARAMETERS:
 *      SEM_ID *semId -
 *      SEM_B_STATE semState - 
 *      int priority - 
 *      int options - 
 *      int stack - 
 *      FUNCPTR func - 
 *      char *name - 
 *
 * FUNCTION VALUE:
 * 	int - Status value returned by function 
 *
 * PURPOSE: 
 *       Start the dataCoAddtasks for taking images and creates a semaphore
 *        for comunication.
 * 
 *
 * DESCRIPTION:
 * 
 *
 * EXTERNAL VARIABLES:
 * 	Lots - see gnDCAVars.h
 *
 * PRIOR REQUIREMENTS:
 *	Assumes an NOAO CoAdder board and interupt handler for same are 
 *		installed 
 *
 * DEFICIENCIES:
 * 	None known
 *
 * HISTORY:
 * 	12-June-1997  Original version  N Buchholz
 *
 *****************************************************************************/

int initTask( SEM_ID *semId, SEM_B_STATE semState, int priority, int options,
	      int stack, FUNCPTR func, char *name)
{
    char taskName[MAX_STRING_SIZE];
    int taskId;
    int iAddr;

    /* Create a semaphore for the task */
    *semId = semBCreate(SEM_Q_FIFO, semState);    
    if(semId == NULL)
	return ERROR;
    sprintf(taskName,"%c%s",'t', name);
    taskId = taskSpawn(taskName, priority, options, stack, func,
		       			iAddr, 0, 0, 0, 0, 0, 0, 0, 0, 0);
    
    return taskId;
    
}

/*****************************************************************************
 * Function name:
 * doRDD
 *
 * Invocation:
 *     invoked by initTask - refer to initTask
 * 
 * PARAMETERS:
 *  none
 *
 * FUNCTION VALUE:
 * never returns
 *
 * PURPOSE:
 *     Error checking front end for taking an image using the rdd format.  Sets 
 * EPICS status after completion
 * 
 *
 * DESCRIPTION:
 * 
 *
 * EXTERNAL VARIABLES:
 * 	Lots - see gnDCAVars.h
 *
 * PRIOR REQUIREMENTS:
 *	Assumes  an NOAO CoAdder board and interupt handler for same are 
 *		installed  
 *
 * DEFICIENCIES:
 * 	None known
 *
 * HISTORY:
 * 	02- Nov-1999  Original version  N Buchholz
 *
 *****************************************************************************/

int doRDD(int n1, int n2, int n3, int n4, int n5, int n6, int n7, int n8,
      	int n9, int n10)
{
    long binTrue = 1;
#ifdef DEBUG
    int t1=0;
#endif
    int status = (OK);
    char errString[MAX_STRING_SIZE];
    
    /* Sleep until there is something to do */
    while( 1 )    {
      DPRINT(t1,"doRDD waiting for semaphore\n");
      semTake(semRDD,WAIT_FOREVER); 
      DPRINT(t1,"doRDD got semaphore\n");
      if (rddSetupDone)
        {
          status = DCArdd(); 
        }

      else
        {
	
          sprintf(errString, "Error DRDD: RDD setup not done ");
          printf(errString);
          /* signal an error to EPICS system */
          setCar(OBSERVE_CAR,menuCarstatesERROR,RDD_NOT_SETUP,errString,retString);
	
          continue;
        }

      if (status != (OK))	{ 
        sprintf(errString, "Error DRDD: DoRDD returned error.");
        printf(errString);
        setCar(OBSERVE_CAR,menuCarstatesERROR,RDD_RUNTIME,errString,retString);
        continue;
      }
      else 	{
        #ifdef TRACE
		printf("frame done\n");
        #endif
		
		 /*  taskDelay(30); */
		 gnPutEpicsT(dbTop, FRAMEREADY ".VAL",DCALONG, &binTrue); 
        
      }
    
    }
}

/*****************************************************************************
 * Function name:
 *       doRRD
 *
 * Invocation:
 *     refer to initTask
 *       
 * PARAMETERS:
 *       none
 *
 * FUNCTION VALUE:
 * 	never returns
 *
 * PURPOSE:
 *       Error checking front end for taking an image using the rrd format. Sets 
 * EPICS status after completion 
 *
 * DESCRIPTION:
 * 
 *
 * EXTERNAL VARIABLES:
 * 	Lots - see gnDCAVars.h
 *
 * PRIOR REQUIREMENTS:
 *	Assumes  an NOAO CoAdder board and interupt handler for same are 
 *		installed 
 *
 * DEFICIENCIES:
 * 	None known
 *
 * HISTORY:
 * 	30-Oct-1999  Original version  N Buchholz
 *
 *****************************************************************************/

int doRRD(int n1, int n2, int n3, int n4, int n5, int n6, int n7, int n8,
      	int n9, int n10)
{
    long binTrue = 1;
    int status =(OK);
    char errString[MAX_STRING_SIZE];


    /* Sleep until there is something to do */
    while( 1 )
    {
     
	semTake(semRRD,WAIT_FOREVER); 

	if (rrdSetupDone)
	{
	    /* start the observation*/
	    status = DCArrd();
	}
	else
	{
	   
	    sprintf(errString, "Error DRRD: RRD setup not done ");
	    setCar(OBSERVE_CAR,menuCarstatesERROR,RRD_NOT_SETUP,errString,retString);
	   
	    continue;
	}

	if (status != (OK))
	{
	   
	    sprintf(errString, "Error DRRD: DoRRD returned error.");
	    setCar(OBSERVE_CAR,menuCarstatesERROR,RRD_RUNTIME,errString,retString);
	    continue;
	}

	else 
      {
	 
#if 0
            /* signal observation done to EPICS */
		    setCar(OBSERVE_CAR,menuCarstatesIDLE,OK,"",dummy);
	
#else
            
			gnPutEpicsT(dbTop, FRAMEREADY ".VAL",DCALONG, &binTrue); 

#endif
    
	    continue;
      }

    }
    
}

/*****************************************************************************
 * Function name:
 *    doTEST
 *
 * Invocation:
 *     refer to initTask
 *
 * PARAMETERS:
 *     none
 *
 * FUNCTION VALUE:
 * 	never returns
 *
 * PURPOSE:
 * Error checking front end for taking a test image.  Sets 
 * EPICS status after completion
 *
 * DESCRIPTION:
 * 
 *
 * EXTERNAL VARIABLES:
 * 	Lots - see gnDCAVars.h
 *
 * PRIOR REQUIREMENTS:
 *	
 *
 * DEFICIENCIES:
 * 	None known
 *
 * HISTORY:
 * 	12-June-1997  Original version  N Buchholz
 *
 *****************************************************************************/

int doTEST(int n1, int n2, int n3, int n4, int n5, int n6, int n7, int n8,
      	int n9, int n10)
{
    long binTrue = 1;
    int status =(OK);
    char errString[MAX_STRING_SIZE];
    
    /* Sleep until there is something to do */
    while( 1 )
    {
     
	semTake(semTEST, WAIT_FOREVER); 
	if (testSetupDone)
	{
	    status |= DCAtest();
	}
	else
	{
	  
	    sprintf(errString, "Error DTEST: TEST setup not done ");
	    setCar(OBSERVE_CAR,menuCarstatesERROR,TEST_NOT_SETUP,errString,retString);
	

	    /* signal an error to EPICS system */
	    continue;
	}

	if (status != (OK))
	{
	    sprintf(errString, "Error DTEST: DoTEST returned 	error.");
	    setCar(OBSERVE_CAR,menuCarstatesERROR,TEST_RUNTIME,errString,retString);
	    continue;
	}

	else 
	{
	   /*  if(openEpicsSocket() == OK) { */
		/* signal observation done to EPICS */
#if 0
		setCar(OBSERVE_CAR,menuCarstatesERROR,TEST_NOT_SETUP,errString,dummy);
	
#else
		
		gnPutEpicsT(dbTop, FRAMEREADY ".VAL",DCALONG, &binTrue); 	
#endif
	/* 	closeEpicsSocket(); */
/* 	    } */
/* 	    else */
/* 		printf("error opening epics socket in dotest\n"); */


	    continue;
	}

    }

}

/*****************************************************************************
 * Function name:
 *   doSEP
 *
 * Invocation:
 *     refer to initTask
 * PARAMETERS:
 *     none
 *
 * FUNCTION VALUE:
 * 	never returns
 *
 * PURPOSE:
 * Error checking front end for taking an image using the sep format. Sets 
 * EPICS status after completion
 *
 * DESCRIPTION:
 * 
 *
 * EXTERNAL VARIABLES:
 * 	Lots - see gnDCAVars.h
 *
 * PRIOR REQUIREMENTS:
 *
 *
 * DEFICIENCIES:
 * 	None known
 *
 * HISTORY:
 * 	12-June-1997  Original version  N Buchholz
 *
 *****************************************************************************/

int doSEP(int n1, int n2, int n3, int n4, int n5, int n6, int n7, int n8,
      	int n9, int n10)
{
    long binTrue = 1;
    int status =(OK);
    char errString[MAX_STRING_SIZE];
    
    /* Sleep until there is something to do */
    while( 1 )
    {
      
	semTake(semSEP, WAIT_FOREVER); 
	if (sepSetupDone)
	{
	    status |= DCAsep();
	}
	else	{
	    sprintf(errString, "Error DSEP: SEP setup not done	 ");
	    setCar(OBSERVE_CAR,menuCarstatesERROR,SEP_NOT_SETUP,errString,retString);
	    continue;
	}

	if (status != (OK))	{
	    sprintf(errString, "Error DSEP: DCAsep returned error.");
	    setCar(OBSERVE_CAR,menuCarstatesERROR,SEP_RUNTIME,errString,retString);
	    continue;
	}
	else 
	{
	
#if 0
		/* signal observation done to EPICS */
	setCar(OBSERVE_CAR,menuCarstatesERROR,TEST_NOT_SETUP,errString,dummy);
	
#else
	
	gnPutEpicsT(dbTop, FRAMEREADY ".VAL",DCALONG, &binTrue); 

#endif

	    continue;
	}

    }

}

