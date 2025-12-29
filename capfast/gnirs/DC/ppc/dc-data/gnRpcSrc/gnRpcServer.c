
static struct {
        void *v;
        char *c;
} rcsid = {
        &rcsid,
        "$Id: gnRpcServer.c,v 1.2 2009/05/27 19:32:40 fkraemer Exp $"
};


static char dummy[80];
#define DEBUG
int rpcdebug;
int rundebug;
void gemLogMsg(int level, const char *pFormat, ...);
/*****************************************************************************
 * Copyright 1997 Association of Universities for Research in Astronomy, Inc. 
 * See the file COPYRIGHT for more details.
 *
 * Filename: 	
 * 	gnRpcServer.c
 *
 * Description:
 * 	This file contains the routines in the data acquisition IOC which respond to
 *	    the RPC calls from the EPICS IOC.  These routines control the
 *	    gnaac acquisition routines to setup data system components
 *
 * Function name(s)
 * 	gnObsSetup() - uses Channel access to get the appropriate observation 
 *		parameters from the EPICS IOC 
 *
 * Dependencies
 * 	The EPICS IOC must be set up and running.
 *
 * Orginial Author:
 *	Nick C. Buchholz
 *
 * History:
 *	11-Jun-1997: Created original version - ncb
 *
 ***************************************************************************/
#include <stdio.h>
#include <usrLib.h>
#include <sysLib.h>
#include <rpcLib.h>
#include <rpc/rpc.h>	/* always need this here */
#include <epdq.h>	/* need this too: */
#include <carRecord.h>
#include <dataHandling.h>

#include <gnerrno.h>
#include <gnDCADefs.h>

#define TASKINIT
#undef MAIN
#include <gnDCAVars.h>

#include <epCommon.h>
/* #include "dq.h" */
#include "gnDQSocket.h"
#include "saver.h"

extern saverParams svrP;
#define SDEBUG
#define DEBUG 
extern int setObsFlags(int prep, int acq, int rdout); 
extern void sleep (int a, int b);
void dropDhs();
/*****************************************************************************
 * Function name:
 * 	gnObsSetup
 *
 * Invocation:
 * 	invoked by RPC call from other IOC
 *		status = gnObsSetup(  );
 *
 * PARAMETERS:
 *	None
 *
 * FUNCTION VALUE:
 * 	int - Status value returned by function 
 *
 * PURPOSE:
 * 	obtains a full set of parameters, needed by the data acquisition program, from
 *		the EPICS IOC.
 *
 * DESCRIPTION:
 * 	Uses channel access to obtain the values of the important data acquisition
 *		variables stored in the EPICS IOC. This includes ucode
 *		description, operation modes, header data, ROI descriptions, 
 *		etc.  These values are stored in 'C'.
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
void observeTest()
{
    dqobssetuprpc_1();
    dqobserverpc_1();
      
}
#define DEBUG
extern int dq_debug;
int *dqobssetuprpc_1(int i)
{
#ifdef DEBUG
    int t1 = 1;
#endif
    static int result; /* must be static! */
    int status = (OK);
    static int setupNeeded = TRUE,acqSetupNeeded = TRUE;
    char errString[MAX_STRING_SIZE];
    char tempbuf[80], taskName[80];
	int rc;
/* 	printf("dqobssetuprpc: 1  format = %d\n",svrP.param.dhs.format); */
   DCA_Abort = FALSE;
    DCA_Save = TRUE;
    disposition = SAVE;
    result = RPC_OK;
    gemLogMsg(10,"obs Setup\n");
    /* get parameters from EPICS IOC */
    status = gnGetGNAACParams(&setupNeeded,&acqSetupNeeded);
    gemLogMsg(10,"dqobssetuprpc: 1.5  status = %d\n",status);
/*   printf("rpc running\n"); */
    /* take the oportunity to update the DHS status field here */
    if (svrP.health == GOOD) {
      rc = 1;
      if(gnPutEpicsT(dbSadTop, DHSCONNECTED ".VAL", DCALONG, &rc)!= OK)
	gemLogMsg (0,"dqobssetuprpc_1: failed to set DHSCONNECTED\n");
    }
    else {
      rc = 0;
      if(gnPutEpicsT(dbSadTop, DHSCONNECTED ".VAL", DCALONG, &rc)!= OK)
	gemLogMsg (0,"dqobssetuprpc_1: failed to set DHSCONNECTED\n");
    }
    
    
    /* 	printf("dqobssetuprpc: 2  format = %d\n",svrP.param.dhs.format); */
    
    if (status != (OK))
      {
	
	sprintf(errString, "gnObsSetup error: Data acquisition Params not set");
	/* 	setCar(OBSERVE_CAR,menuCarstatesERROR,DQ_GETPARAM_ERR,errString,dummy); */
	gemLogMsg(10,"obs setup rpc error1\n");
	result = RPC_ERROR;
	return (&result);
    } 
 
    if ((setupNeeded == FALSE) &&(acqSetupNeeded == FALSE))
    {
	result = RPC_OK; 
	gemLogMsg(5,"obs setup rpc done setup not needed\n");
	return (&result);
    }

    /* set setupDone to false so if we don't get through all of this we can
     * signal other parts of the system not to start taking data */
    rddSetupDone = FALSE;
    rrdSetupDone = FALSE;
    testSetupDone = FALSE;
    sepSetupDone = FALSE;
  
    /* Setup Processing Pipes of correct type */
/*     cicsLogMessage(3,"gnObsSetup: Entering switch\n"); */ 
    gemLogMsg(10,"obs setup rpc procmode\n");
    switch (svrP.procMode)
    {
      case TEST:   /* Test mode captures data and without further processing
		    * displays it scaled by taking the SqRoot of the data.
		    * This is a fast method to check the integrity of the 
		    * data processing chain */
	  sprintf(tempbuf,"Setting up Test Mode\n");
/* 	  cicsLogMessage(3,tempbuf); */
	  gemLogMsg(10,"obs setup rpc procmode test\n");
	  semGive(semSetupTEST);
	  if ((status = semTake(semSetup, 90000000)) == ERROR)
	  {
	     /*  cicsLogMessage(0,"rpc error: Test Mode Failed\n"); */
	      sprintf(errString, "rpc error: Test Mode Failed");
	     
	      setCar(OBSERVE_CAR,menuCarstatesERROR,TEST_NOT_SETUP,errString,dummy);
	   
	      return (&result);
	  }
	  break;

      case SEP:	   /* SEP mode captures data and attempts to send each frame
		    * to the saver to be stored on disk. The Data is converted
		    * to a float before it is sent. */

    gemLogMsg(5,"obs setup rpc procmode sep\n");
	  sprintf(tempbuf,"Setting up Sep Mode\n");
/* 	  cicsLogMessage(3,tempbuf); */
	  semGive(semSetupSEP);
	  if ((status = semTake(semSetup, 90000000)) == ERROR)
	  {
	    /*   cicsLogMessage(0,"rpc error: SEP Mode Failed\n"); */
	      sprintf(errString, "rpc error: SEP Mode Failed");
	      setCar(OBSERVE_CAR,menuCarstatesERROR,SEP_NOT_SETUP,errString,dummy);
	    
	      return (&result);
	  }
	  break;

      case STARE:   /* Stare mode is the normal data taking mode for the
		     * controller. two uCodeTypes will be supported:
		     * RDD - Global Reset double correlated sampling. This 
		     *	     mode includes the ability to do Low Noise Reads
		     *	     (LNRs) and coadding
		     * RRD - Row Reset single correlated sampling. This mode
		     *	     does not support LNRs but does do coAdding as
		     *       required. (ucode for this has yet to be developed
		     */

	gemLogMsg(5,"obs setup rpc procmode stare\n");
	  sprintf(tempbuf, "Setting up Stare Mode\n");
  
/* 	  cicsLogMessage(3,tempbuf); */
	  if (uCodeType == RDD)
	  {
	      sprintf(taskName, "%s", "RDD");
	      semGive(semSetupRDD);
	  }
	  else
	  {
	      sprintf(taskName, "%s", "RRD");
	      semGive(semSetupRRD);
	  }

	  if ((status = semTake(semSetup, 90000000)) == ERROR)
	  {
	      sprintf(errString, "rpc error: %s Mode Failed", taskName);
	      setCar(OBSERVE_CAR,menuCarstatesERROR,STARE_NOT_SETUP,errString,dummy);
	      return (&result);
	  }
	  break;

      default: 
	gemLogMsg(5,"obs setup rpc procmode default\n");
	 /*  cicsLogMessage(0,"Error: Bad Mode selected\n"); */
	  sprintf(errString, "rpc error: Bad Mode selected\n");
	setCar(OBSERVE_CAR,menuCarstatesERROR,STARE_NOT_SETUP,errString,dummy);
	 
	  return (&result);

    } /* end of the switch on processing mode */
/*     cicsLogMessage(1,"rpc done"); */
    gemLogMsg(10,"obs setup rpc done\n");

    result = RPC_OK; 
    setupNeeded = FALSE; /* pr 6-22*/
    return (&result);
}


/*****************************************************************************
 * Function name:
 * 	gnObserve
 *
 * Invocation:
 * 	invoked by RPC call from other IOC
 *		status = gnObserve(  );
 *
 * PARAMETERS:
 *	None
 *
 * FUNCTION VALUE:
 * 	int - Status value returned by function 
 *
 * PURPOSE:
 * 	uses parameters setup by ObsSetup call 
 *
 * DESCRIPTION:
 * 	starts up the observing routines and pipelines
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
 * 	11-Sep-1997  Original version  N Buchholz
 *
 *****************************************************************************/
extern int dq_debug;
int *dqobserverpc_1(int i)
{
   
    static int result; /* must be static! */
    char errString[80];
#ifdef DEBUG
    int t1=0;
#endif

    DCA_Abort = FALSE;
    DCA_Save = TRUE;
    disposition = SAVE;
    gemLogMsg(5,"gnRpcServer:  observe rpc\n");
    if ((svrP.procMode == STARE) && 
	(((uCodeType == RDD) && (rddSetupDone == FALSE)) ||
	 ((uCodeType == RRD) && (rrdSetupDone == FALSE)) ))
      {
/* 	cicsLogMessage(0,"gnObserve error: DQ Setup not complete. Observe Failed\n"); */
	sprintf(errString, "Error GOBS: DQ Oberve Setup not complete");
	setCar(OBSERVE_CAR,menuCarstatesERROR,OBS_OUTOFORDER,errString,dummy);
	return (&result);
      } 
    else if (((svrP.procMode == TEST) || (svrP.procMode == SEP)) &&
	     (((svrP.procMode == TEST) && (testSetupDone == FALSE)) ||
	      ((svrP.procMode == SEP) && (sepSetupDone == FALSE)) ))
      {
/* 	cicsLogMessage(0,"gnObserve error: DQ Setup not complete. Observe Failed\n"); */
	sprintf(errString, "Error GOBS: DQ Oberve Setup not complete");
	setCar(OBSERVE_CAR,menuCarstatesERROR,OBS_OUTOFORDER,errString,dummy);
	return (&result);
      }
    gemLogMsg(10,"gnRpcServer:  observe rpc clear\n");
	
    if (svrP.procMode == TEST)
      {
	/* start the test task which does all the real work and return */
	semGive(semTEST);
      }
    else if (svrP.procMode == SEP)
      {
	/* start the test task which does all the real work and return */
	semGive(semSEP);
      }
    else
      {
	switch (uCodeType)
	  {
	  case RDD:
	    /* start the task which does all the real work and return */
	    gemLogMsg(5,"starting RDD task\n");
	    semGive(semRDD);	 /* This will start the doRDD_Data task */
	    break;
		
	  case RRD:
	    semGive(semRRD);	 /* This will start the doRRD_Data task */
	    break;
		
	  default:
	    sprintf(errString, "Error GOBS: Invalid uCodeType");
	    setCar(OBSERVE_CAR,menuCarstatesERROR,INVALID_UCODE,errString,dummy);
	    setObsFlags(FALSE, FALSE, FALSE); /* set prep, acq & rdout false */
	    return (&result);
	  }
      }
	 
    gemLogMsg(5,"gnRpcServer:  observe rpc started dq\n"); 
      
    /* OK the task is started and we are done. */
    result = (RPC_OK); 
    return (&result);
}

/* Remote verson of "dqObsStatus"  */
int *dqobsstatusrpc_1(int i)
{
  static int result; /* must be static! */
  char errMess[MAX_STRING];

  strcpy(errMess, "");
/*   cicsLogMessage(3,"dqObsStatus running\n"); */
  return (&result);
}


/*****************************************************************************
 * Function name:
 * 	gnObsAbort
 *
 * Invocation:
 * 	invoked by RPC call from other IOC
 *		status = gnObsAbort(  );
 *
 * PARAMETERS:
 *	None
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
int *dqobsabortrpc_1(int i)
{
#ifdef DEBUG
    int t1 = 1;
#endif
    static int result; /* must be static! */
    int retval;

/*     cicsLogMessage(3, "dqObsAbort running\n"); */

    result = RPC_OK;
    /* on "abort", abort image taking and discard current image */
    DCA_Abort = TRUE;
    DCA_Save = FALSE;
    disposition = TOSS;
    retval = 2;

    gemLogMsg(0,"\nabort observation\n");

    return (&result);

}


/*****************************************************************************
 * Function name:
 * 	gnObsStop
 *
 * Invocation:
 * 	invoked by RPC call from other IOC
 *		status = gnObsStop(  );
 *
 * PARAMETERS:
 *	None
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
int *dqobsstoprpc_1(int i)
{
#ifdef DEBUG
    int t1;
#endif
    static int result; /* must be static! */
    int retval;

/*     cicsLogMessage(3, "dqObsStop running\n"); */

    result = RPC_OK;
    /* on "stop", abort image taking & save current image */
    DCA_Abort = TRUE;
    DCA_Save = TRUE;
    disposition = SAVE;
    retval=3;
    gemLogMsg(0,"\nstop observation\n");


    return (&result);
}



/*****************************************************************************
 * Function name:
 * 	gnrReboot
 *
 * Invocation:
 * 	invoked by RPC call from other IOC
 *		status = gnReboot(  );
 *
 * PARAMETERS:
 *	None
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
 * 	Lots - see gnaaDCAVars.h
 *
 * PRIOR REQUIREMENTS:
 *
 *
 * DEFICIENCIES:
 * 	None known
 *
 * HISTORY:
 * 	
 *
 *****************************************************************************/

#define BUS_RESET_BIT_MV167   0x01800000
#define BUS_RESET_REG_MV167   0xfff40060 
#define HW_REG32 volatile unsigned long
#define BIT_SET(p, d)      { __typeof__ (* (p)) __temp = (* (p));        \
                             * (p) = __temp | (d); }

#include "rebootLib.h"

void wfsBusReset(void)
{

    gemLogMsg(0,"wfsBusReset: BUS RESET - SYSTEM WILL REBOOT.\n");

    /* Brief pause to allow message to flush..   */
  sleep(1,0);

    /* ..then waggle the hardware bits            */
/*
 * mbec 3/19/2001
 * this code is specific to the mv167
 *
 *     BIT_SET((HW_REG32 *) BUS_RESET_REG_MV167, BUS_RESET_BIT_MV167);
 */
 	reboot (2);
}


int rebootProc()
{

   
    taskSpawn("suicide", 20, VX_NO_STACK_FILL, 2000, (FUNCPTR) wfsBusReset,
              0, 0, 0, 0, 0, 0, 0, 0, 0, 0);

    return OK;
}


int *dqrebootrpc_1(int i)
{
#ifdef DEBUG
    int t1=1;
#endif
    static int result; /* must be static! */
    int retval;

/*     cicsLogMessage(3, "dqObsStop running\n"); */

    result = RPC_OK;
    /* disconnect from dhs*/  
    dropDhs();
	closeSocket();
	taskDelay(120);
	
    /*reboot*/
    rebootProc();


  
    retval=3;
    gemLogMsg(0,"\n reboot\n");


    return (&result);
}


/*****************************************************************************
 * Function name:
 *      gnDhsConnection
 *
 * Invocation:
 *      invoked by RPC call from other IOC
 *              status = gnDhsConnection(  );
 *
 * PARAMETERS:
 *      None
 *
 * FUNCTION VALUE:
 *      int - Status value returned by function
 *
 * PURPOSE:
 *
 *
 * DESCRIPTION:
 *
 *
 * EXTERNAL VARIABLES:
 *      Lots - see gnaaDCAVars.h
 *
 * PRIOR REQUIREMENTS:
 *
 *
 * DEFICIENCIES:
 *      None known
 *
 * HISTORY:
 *      11-June-1997  Original version  N Buchholz
 *
 *****************************************************************************/

extern int servP_DhsConnect();
extern int servP_DhsDisConnect();

int *dqdhsconnectionrpc_1( dqargument *pconn )
{
#ifdef DEBUG
    int t1;
#endif
    static int result; /* must be static! */

printf("en dqdhsconnectionrpc_1\n");
    if (pconn->dqdhsconnectionrpc_1_arg)
    {
     printf("to servP_DhsConnect \n");
     servP_DhsConnect();
     printf("from servP_DhsConnect \n"); 
    }
    else
    {
     printf("to servP_DhsDisConnect \n");
     servP_DhsDisConnect();
     printf("from servP_DhsDisConnect \n"); 
    }

    result = RPC_OK;
    return (&result);
}

