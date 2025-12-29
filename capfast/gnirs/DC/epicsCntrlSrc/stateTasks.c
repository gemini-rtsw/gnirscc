static struct {
        void *v;
        char *c;
} rcsid = {
        &rcsid,
        "$Id: stateTasks.c,v 1.2 2009/05/27 19:32:22 fkraemer Exp $"
};
/*
 * Copyright 1997 Association of Universities for Research in Astronomy, Inc.
 * See the file COPYRIGHT for more details.
 *
 * FILENAME
 * stateTasks.c
 *
 * DESCRIPTION
 * This file contains the VxWorks control tasks for the state CAD
 * records. Note that there are no "doPause" or "doContinue" tasks,
 * because these commands are not currently supported. If they are
 * added in a future upgrade, this is where they should go.
 * 
 * FUNCTION NAME(S)
 * doObserve - performs an observation
 * doPark    - executes the "park" command
 * doAbort   - aborts an observation (drops the data)
 * doStop    - stops an observation (keeps the data)
 *   
 *INDENT-OFF*
 * $Log: stateTasks.c,v $
 * Revision 1.2  2009/05/27 19:32:22  fkraemer
 * fkraemer - copied my complete working dir over trunk
 *
 * Revision 1.2  1998/11/20 17:13:54  pruckle
 * speed up saver, log messages
 *
 * Revision 1.1.1.1  1998/09/30 16:40:27  pruckle
 * Initial Release
 *
 *INDENT-ON* 
 */

/* #define DEBUG */
#include <vxWorks.h>
#include <taskLib.h>
#include <semLib.h>
#include <epCommon.h>
#include <naacTasks.h>
#include <sysLib.h>
#include <rpcLib.h>
#include <car.h>

#include <dataHandling.h>

#include <gnerrno.h>

/* function prototypes*/
int  dqObsSetupRPC();
int dqObserveRPC( );
int dqObsAbortRPC( );
int dqObsStopRPC( );
int dqRpcInit(char *server) ;
STATUS caMonitorT(char *top,char *name,void (*callback)(void *),
		  void *userarg,void **ppevid);
void dcaReady(void *val);
void frameReady(void *val);

/*extern globals*/
extern char *rpcServer; 
extern char *dbTop;
extern int coAdSim;


/* globals*/
static int abortObs=0,stopObs=0;
int readyFlag = 0;
int frameFlag = 0;
int rebootNow();

void dcaReady(void *val)
{
    if(readyFlag)
	semGive (semDcaReady);
    readyFlag = 0;
   
}
void frameReady(void *val)
{
    if(frameFlag)
	semGive (semFrameReady);
    frameFlag = 0;
}
/*
 * FUNCTION NAME:
 * doObserve
 *
 * INVOCATION:
 * int n1,n2,n3,n4,n5,n6,n7,n8,n9,n10;
 * 
 * doObserve( n1, n2, n3, n4, n5, n6, n7, n8, n9, n10)
 *
 * PARAMETERS: (">" input, "!" modified, "<" output)
 * > n1   (int)    integer containing the address of the observe CAD
 * > n2   (int)    not used, 0
 * > n3   (int)    not used, 0
 * > n4   (int)    not used, 0
 * > n5   (int)    not used, 0
 * > n6   (int)    not used, 0
 * > n7   (int)    not used, 0
 * > n8   (int)    not used, 0
 * > n9   (int)    not used, 0
 * > n10  (int)    not used, 0
 *
 * FUNCTION VALUE:
 * long  Status value, although function should never return
 *
 * PURPOSE:
 * Performs all functions necessary to implement the "observe" command
 *
 * DESCRIPTION:
 * This VxWorks task is created (spawned) upon startup.  The task waits
 * for its semaphore and for the completion of any other tasks (if any)
 * which must finish execution before this one begins.  It executes all
 * the functions needed to complete an observation and updates the 
 * associatied CAR record appropriately.  The output parameters of the
 * associated CAD record are available though the n1 input parameter.
 *
 * EXTERNAL VARIABLES:
 * None
 *
 * PRIOR REQUIREMENTS:
 * None
 *
 * DEFICIENCIES:
 * This task will continue running even if an abort or stop is
 * issued.  The manner in which this task is stopped will depend
 * the details that have not been implemented yet. 
 *
 * HISTORY (optional):
 * 17-Apr-1997  Original version.			   J.E. Tvedt
 * 16-Jun-1997  Modified to set the PREP, ACQ and RDOUT
 *              flags.                                     S.M. Beard
 * 19-Jun-1997  Obtain the data label from the CAD record
 *              and call the data handling library to
 *              generate and store fake data.              S.M. Beard
 * 26-Jun-1997  Split calls to data handling library into
 *              those at the beginning and those at the
 *              end of the observation.                    S.M. Beard
 *
 *-
 */
int doObserve( int n1, int n2, int n3, int n4, int n5, int n6, int n7,
		  int n8, int n9, int n10)
{
    int numCoAdds;
    double intTime;
    long n,numImages;
    struct cadRecord *pCad;
    long status = OK;
    long lVal;
    char buf[80];
    long seqRegVal;
    char errMess[MAX_STRING_SIZE];
    char dummy[MAX_STRING_SIZE];
    long binTrue = TRUE;
    long binFalse = FALSE;


    abortObs = 0;
    
    /* initialize rpc for this process*/
    while(dqRpcInit(rpcServer)!= OK)
      {
	printf("waiting for rpc\n");
	sleep (1,0);
      }
    /* set monitors for variables coadder uses for signaling*/
    caMonitorT(dbTop,FRAMEREADY ".VAL",frameReady,NULL,NULL);
    caMonitorT(dbTop,DCAREADY ".VAL",dcaReady,NULL,NULL);


    DPRINT(prdb,"doObserve 1\n");
    /* Sleep until there is something to do */
    while( 1 )
    {	status = putDbInfoT(dbTop, HK_FREEZE ".VAL", dummy, DBF_LONG, 
			    &binFalse);
	/* Wait for semaphore and completion of the arSetup task */
	semTake(semObserve,WAIT_FOREVER);

	stopObs = abortObs = 0;
	 
	/* Set the CAR to BUSY */
	status = setCar(OBSERVE_CAR,CAR_BUSY,OK,"",dummy);
	if(status != OK)
	{
	    printf("error setting car\n");
	    continue;
	}
	/* Get the address of the CAD record so command arguments
	 * are accessible. */	
	pCad = (struct cadRecord *) n1;

        /* First set the PREP flag and clear ACQ and RDOUT, to show the
         * detector is preparing to expose. the rest of this control is done 
	 * from the data coadder side */

        putDaqFlags( 1, 0, 0);
	printf("waiting for ready sem...");
#if 1
	lVal = 0;
	status = putDbInfoT(dbTop, DCAREADY ".VAL", dummy, DBF_LONG, &lVal);
	lVal = 0;
	status = putDbInfoT(dbTop, FRAMEREADY ".VAL", dummy, DBF_STRING, "waiting");
/* 	semTake(semDcaReady,WAIT_FOREVER); */
#endif
        /* do the remote procedure call to setup the Data coadderPrograms and
	 * pipelines */  
   	printf("done\n");
	DPRINT(prdb,"call obssetup rpc\n");
/* 	cicsLogMessage(3,"doOBServe got sem\n"); */
	status = dqObsSetupRPC();
	
	if (status != OK)
	{
	    /* do something to tell the OCS its screwed and continue to wait */
	    sprintf(errMess, "DoObserve Failed coadder setup RPC FAILED\n");
	    /* Set the CAR to BUSY */
	    status = setCar(OBSERVE_CAR,CAR_ERROR,DQ_RPC_FAILED,errMess,dummy);
	    if(status != OK)
	    {
		printf("error setting car\n");
	       
	    }
	    cicsLogMessage(0,"doOBSetupRPC fail\n");
	    putDaqFlags(0,0,0);
	    continue;
	}

	DPRINT(prdb,"doObserve 2\n");
	/* all OK so start the Data coadder observe task */
	/* check busy flag and loop*/
	getDbInfoT(dbTop,  OBSSETUP_CAD ".VALF", dummy, DBF_LONG, &numImages);
	getDbInfoT(dbTop,  OBSSETUP_CAD ".G", dummy, DBF_DOUBLE, &intTime);
	getDbInfoT(dbTop,  OBSSETUP_CAD ".VALE", dummy, DBF_LONG, &numCoAdds);
	n = 0;
	printf("number pics = %d\n",numImages);
	status = putDbInfoT(dbTop, HK_FREEZE ".VAL", dummy, DBF_LONG, 
			    &binTrue);
	while((n < numImages) && (status == OK)) 	
	{ 
	    readyFlag = 1;
	    frameFlag = 1;
	    if ((abortObs == 1)||(stopObs == 1))
	    {
			abortObs = 0;
			stopObs = 0;
			/* 	status = setCar(OBSERVE_CAR,CAR_IDLE,OK,"",dummy); */
			if(status != OK)
			{
				printf("error setting car\n");
			}
			putDaqFlags(0,0,0);
			break;
	    }
#if 0
	    lVal = CAR_BUSY;
	    status = putDbInfoT(dbTop, OBSERVE_CAR ".IVAL", pCad->mess,
							DBF_LONG,&lVal);
#endif	   
		
	    printf("\nTaking image Number %d\n",n);
		/*    tickSet(0); */
	    status = dqObserveRPC( );
	    
		
	    if (status != OK)
	    {
			printf("dqObserveRpc failed\n");
			/* do something to tell them they're screwed */
			sprintf(errMess, "DoObserve Failed  observe RPC FAILED\n");
		
			if	(setCar(OBSERVE_CAR,CAR_ERROR,DQ_RPC_FAILED,errMess,dummy) != OK)
			{
				printf("error setting car\n");
				
			}
			cicsLogMessage(0,"doOBServeRPC fail\n");
			DPRINT(prdb,"observerpc fail\n");
			putDaqFlags(0,0,0);
			break;
	    }
	    DPRINT (prdb,"Wait for dq response\n");
	    /* now wait up to 20 secs for the Data coadder program 
	       to say its ready to go */
	    status = semTake(semDcaReady,20*sysClkRateGet());
		/*   printf("got dcaready flag %d\n",tickGet()); */
	    if(status == ERROR)
	    {
			sprintf(buf,"Timeout waiting for dcaready flag");
			printf("%s\n",buf);
			putDaqFlags(0,0,0);
	    }
		/*   tickGet(); */
	    /* reset signal flag*/
	    lVal = 0;
	    putDbInfoT(dbTop, DCAREADY ".VAL", dummy, DBF_LONG, &lVal);
		
	    if(status != OK)
	    {
			sprintf(errMess, "DoObserve Failed Data coadder Not Ready\n");
			setCar(OBSERVE_CAR,CAR_ERROR,DQNOTREADY,errMess,
				   dummy);
			
			
			putDaqFlags(0,0,0);
			break;
	    }
		
		
	    DPRINT(prdb,"Tell sequencer to send frame\n");
	    /* now the Data coadder routines are running (we hope) and the 
	     *	data can be sent by the sequencer so tell it to send an image
	     * 	ie coadds * lnrs * framesPerRead frames sent in a known order 
	     * 	First stop the housekeeping data gathering */
		
	    /* send the message to the sequencer to start the readout 
	     *	sequence*/
		
	    status = putDbInfoT(dbTop, SEQREG ".VAL", dummy, DBF_LONG,
							&binTrue);/* change to dbProcess ???  */
	    status = getDbInfoT(dbTop, SEQREG ".VAL", dummy, DBF_LONG, 
							&seqRegVal);
	    seqRegVal |= Read_Flag;
		
		
	    /* don't set if sim mode ??? */
#ifndef COADD_TEST
		/*    printf(" sending frame %d\n",tickGet()); */
	    status = putDbInfoT(dbTop, SEQREGSET ".VAL", dummy, DBF_LONG, 
							&seqRegVal);
	    if (status != OK) /* something has gone wrong */
	    {
			/* do something to tell the OCS Its screwed and continue */
			
			sprintf(errMess, "DoObserve write to sequencer Failed\n");
			status = setCar(OBSERVE_CAR,CAR_ERROR,OBSFAILED,errMess,dummy);
			if(status != OK)
			{
				printf("error setting car\n");
			}
			putDaqFlags(0,0,0);
			break;
	    }	 
#endif
		
	    status = getDbInfoT(dbTop, OBSERVE_CAR ".VAL", dummy, DBF_LONG, 
							&lVal);
	    if(lVal == CAR_BUSY)
	    {
			printf("Wait for frame ready idle flag, \n"); 
			/*   printf("intTime = %f, numCoadds = %d %f\n",intTime,numCoAdds,2.5*intTime*numCoAdds); */
			status = semTake(semFrameReady,sysClkRateGet()*(25+2.5*intTime*numCoAdds));
			if(status == ERROR)
			{
				sprintf(buf,"Timeout waiting for frameready flag");
				frameFlag = 0;
				printf("%s\n",buf);
				putDaqFlags(0,0,0);
				break;
			}
#if 0
			status = semTake(semObserveDone,sysClkRateGet()*25);
			if(status == ERROR)
			{
				sprintf(buf,"timeout error waiting for observe car to go idle");
				printf("%s\n",buf);
				putDaqFlags(0,0,0);
			}
#endif
			
	    }
	    lVal = 0;
	    status = putDbInfoT(dbTop, FRAMEREADY ".VAL", dummy, DBF_LONG, 
							&lVal);
	    sprintf(buf,"done with image # %d\n",n);
	    DPRINT(prdb,buf);
	    if ((lVal == CAR_ERROR)||(status != OK))
			status = ERROR;
	    else
			n++;
	}
	
	status = getDbInfoT(dbTop, OBSERVE_CAR ".VAL", dummy, DBF_LONG, &lVal);
	if(lVal == CAR_BUSY)    {
		status = setCar(OBSERVE_CAR,CAR_IDLE,OK,"",dummy);	
		if(status != OK)
		{
			printf("error setting car\n");
			
		}
	    
	}
	
	
#if 0
	
	/* If status is OK, just continue finishing this off will be done by
	 * the Data Coadder program */
	if(status != OK)
	{   /* Otherwise, set CAR and DONE records to ERROR.  Also set
	     * error code and error message of the CAR record */
	    lVal = UNK_ERROR;
	    putDbInfoT(dbTop, OBSERVE_CAR ".IERR", dummy, DBF_LONG, &lVal);
	    
	    sprintf(errMess,"DoDobserve unknown error occurred\n");
	    putDbInfoT(dbTop, OBSERVE_CAR ".IMSS", dummy, DBF_STRING, errMess);

	    lVal = CAR_ERROR;
	    putDbInfoT(dbTop, OBSERVE_CAR ".IVAL", dummy, DBF_LONG, &lVal);
	}
#endif


    }
    /* print errMess to screen and epics if we get here.  */
    /* we never get to here */
    return status;
}

/*
 *+
 * FUNCTION NAME:
 * doPark
 *
 * INVOCATION:
 * int n1,n2,n3,n4,n5,n6,n7,n8,n9,n10;
 * 
 * doPark( n1, n2, n3, n4, n5, n6, n7, n8, n9, n10)
 *
 * PARAMETERS: (">" input, "!" modified, "<" output)
 * > n1   (int)    integer containing the address of the park CAD
 * > n2   (int)    not used, 0
 * > n3   (int)    not used, 0
 * > n4   (int)    not used, 0
 * > n5   (int)    not used, 0
 * > n6   (int)    not used, 0
 * > n7   (int)    not used, 0
 * > n8   (int)    not used, 0
 * > n9   (int)    not used, 0
 * > n10  (int)    not used, 0
 *
 * FUNCTION VALUE:
 * long  Status value, although function should never return
 *
 * PURPOSE:
 * Performs all functions necessary to implement the "park" command
 *
 * DESCRIPTION:
 * This VxWorks task is created (spawned) upon startup.  The task waits
 * for its semaphore and for the completion of any other tasks (if any)
 * which must finish execution before this one begins.  It executes all
 * the functions needed to perform a "park" of the system and updates the 
 * associatied CAR record appropriately.  The output parameters of the
 * associated CAD record are available though the n1 input parameter.
 *
 * EXTERNAL VARIABLES:
 * None
 *
 * PRIOR REQUIREMENTS:
 * None
 *
 * DEFICIENCIES:
 * None known.
 *
 * HISTORY (optional):
 * 17-Apr-1997  Original version.			   J.E. Tvedt
 *
 *-
 */
int doPark( int n1, int n2, int n3, int n4, int n5, int n6, int n7,
		  int n8, int n9, int n10)
{
    struct cadRecord *pCad;
    long status;
    long lVal;
    char errMess[MAX_STRING_SIZE];
    char dummy[MAX_STRING_SIZE];
   


      while(dqRpcInit(rpcServer)!= OK)
	;
    
    /* Sleep until there is something to do */
    while( 1 )
    {
	semTake(semPark,WAIT_FOREVER); 

	/* Get the address of the CAD record so command arguments
	   are accessible.  Currently, there are no input arguments
	   for the park command.
	*/	
	pCad = (struct cadRecord *) n1;
	status = setCar(PARK_CAR,CAR_BUSY,OK,"",dummy);	
	if(status != OK)
	{
	    printf("error setting car\n");
	    continue;
	}

	/* just deactivate the array */
	lVal = FALSE;
	status = putDbInfoT(dbTop, ACTIVATE ".VAL", dummy, DBF_LONG, &lVal);
    
	/* If status is OK, set CAR to IDLE */
	if(status == OK)
	{
	    status = setCar(PARK_CAR,CAR_IDLE,OK,"",dummy);
	    if(status != OK)
	    {
		printf("error setting car\n");
		
	    }
	}
	else
	{   /* Otherwise, set CAR and DONE records to ERROR.  Also set
	     * error code and error message of the CAR record */
	    sprintf(errMess,"DoPark putdbinfo to activate record failed");
	    status = setCar(PARK_CAR,CAR_ERROR,PARKFAILED,"",dummy);
	    if(status != OK)
	    {
		printf("error setting car\n");
		
	    }
	}
	
    }
    return status;
}



/*
 *+
 * FUNCTION NAME:
 * doAbort
 *
 * INVOCATION:
 * int n1,n2,n3,n4,n5,n6,n7,n8,n9,n10;
 * 
 * doAbort( n1, n2, n3, n4, n5, n6, n7, n8, n9, n10)
 *
 * PARAMETERS: (">" input, "!" modified, "<" output)
 * > n1   (int)    integer containing the address of the abort CAD
 * > n2   (int)    not used, 0
 * > n3   (int)    not used, 0
 * > n4   (int)    not used, 0
 * > n5   (int)    not used, 0
 * > n6   (int)    not used, 0
 * > n7   (int)    not used, 0
 * > n8   (int)    not used, 0
 * > n9   (int)    not used, 0
 * > n10  (int)    not used, 0
 *
 * FUNCTION VALUE:
 * long  Status value, although function should never return
 *
 * PURPOSE:
 * Performs all functions necessary to implement the "abort" command
 *
 * DESCRIPTION:
 * This VxWorks task is created (spawned) upon startup.  The task waits
 * for its semaphore and for the completion of any other tasks (if any)
 * which must finish execution before this one begins.  It executes all
 * the functions needed to abort an observation and updates the 
 * associatied CAR record appropriately.  The output parameters of the
 * associated CAD record are available though the n1 input parameter.
 *
 * EXTERNAL VARIABLES:
 * None
 *
 * PRIOR REQUIREMENTS:
 * None
 *
 * DEFICIENCIES:
 * This task will set the observeC CAR to IDLE, but does not
 * actually kill or deactivate the doObserve task.  The details
 * of how to do this are TBD.
 *
 * HISTORY (optional):
 * 17-Apr-1997  Original version.			   J.E. Tvedt
 *
 *-
 */

int doAbort( int n1, int n2, int n3, int n4, int n5, int n6, int n7,
		  int n8, int n9, int n10)
{
   
    long status, error;
    long lVal, seqRegVal;
    char errMess[MAX_STRING_SIZE];
    char dummy[MAX_STRING_SIZE];
  

     while(dqRpcInit(rpcServer)!= OK)
       ;

    /* Sleep until there is something to do */
    while( 1 )
    {
	semTake(semAbort,WAIT_FOREVER); 
 	cicsLogMessage(2, "task tDoAbort awake"); 
 	DPRINT(1, "task tDoAbort awake\n"); 

	status = OK;
	error = GNAAC_OK;
	strcpy(errMess, "\0");

	/* Abort the observation */
	cicsLogMessage(3,"doAbort got sem\n");
	abortObs = 1;
	status = dqObsAbortRPC();

	if(status != OK)
	{	 
	    sprintf(errMess,"doAbort Failed; RPC did not complete");
	    status = setCar(OBSERVE_CAR,CAR_ERROR,DQ_RPC_FAILED,errMess,dummy);
	    if(status != OK)
	    {
		printf("error setting car\n");
	      
	    }
	    continue;
	}

	getDbInfoT(dbTop, SEQREG ".VAL", dummy, DBF_LONG, &seqRegVal);
	seqRegVal |= ABORT_INT;
	status = putDbInfoT(dbTop, SEQREGSET ".VAL", dummy, DBF_LONG, 
			    &seqRegVal);
	    
	if(status != OK)
	{
	    sprintf(errMess,"doAbort Failed; could not signal Sequencer");
	    status = setCar(OBSERVE_CAR,CAR_ERROR,ABORTFAILED,errMess,dummy);
	    if(status != OK)
	    {
		printf("error setting car\n");
		
	    }
	}

 	cicsLogMessage( 2, "Observation aborted"); 
	    
	/* If status is OK, set CAR to IDLE */
	if(status == OK)
	{
	    lVal = CAR_IDLE;
	}
	else
	{   /* Otherwise, set CAR and DONE records to ERROR.  Also set
	     * error code and error message of the CAR record 	*/
	    sprintf(errMess, "doAbort FAILED unknown error");
	    status = setCar(OBSERVE_CAR,CAR_ERROR,error,errMess,dummy);
	    if(status != OK)
	    {
		printf("error setting car\n");
		
	    }

	}

    }

    /* we never should get to here */
    return status;
}

/*
 *+
 * FUNCTION NAME:
 * doStop
 *
 * INVOCATION:
 * int n1,n2,n3,n4,n5,n6,n7,n8,n9,n10;
 * 
 * doStop( n1, n2, n3, n4, n5, n6, n7, n8, n9, n10)
 *
 * PARAMETERS: (">" input, "!" modified, "<" output)
 * > n1   (int)    integer containing the address of the stop CAD
 * > n2   (int)    not used, 0
 * > n3   (int)    not used, 0
 * > n4   (int)    not used, 0
 * > n5   (int)    not used, 0
 * > n6   (int)    not used, 0
 * > n7   (int)    not used, 0
 * > n8   (int)    not used, 0
 * > n9   (int)    not used, 0
 * > n10  (int)    not used, 0
 *
 * FUNCTION VALUE:
 * long  Status value, although function should never return
 *
 * PURPOSE:
 * Performs all functions necessary to implement the "stop" command
 *
 * DESCRIPTION:
 * This VxWorks task is created (spawned) upon startup.  The task waits
 * for its semaphore and for the completion of any other tasks (if any)
 * which must finish execution before this one begins.  It executes all
 * the functions needed to stop an observation and updates the 
 * associatied CAR record appropriately.  The output parameters of the
 * associated CAD record are available though the n1 input parameter.
 *
 * EXTERNAL VARIABLES:
 * None
 *
 * PRIOR REQUIREMENTS:
 * None
 *
 * DEFICIENCIES:
 * This task will set the observeC CAR to IDLE, but does not
 * actually kill or deactivate the doObserve task.  The details
 * of how to do this are TBD.
 *
 * HISTORY (optional):
 * 17-Apr-1997  Original version.			   J.E. Tvedt
 *
 *-
 */

int doStop( int n1, int n2, int n3, int n4, int n5, int n6, int n7,
		  int n8, int n9, int n10)
{
    struct cadRecord *pCad;
    long status;
    long error;
    long  seqRegVal;
    char errMess[MAX_STRING_SIZE];
    char dummy[MAX_STRING_SIZE];
    

     while(dqRpcInit(rpcServer)!= OK)
       ;
 
    /* Sleep until there is something to do */
    while( 1 )
    {
	semTake(semStop,WAIT_FOREVER); 

	status = OK;
	error = GNAAC_OK;
	strcpy(errMess, "\0");

	/* Get the address of the CAD record so command arguments
	   are accessible.
	*/	
	pCad = (struct cadRecord *) n1;

	/* Stop the observation */
	cicsLogMessage( 2, "Stopping observation"); 

	stopObs = 1;
	status = dqObsStopRPC();
	 	
	
	if(status != OK)
	{
	    sprintf(errMess,"doStop Failed; RPC did not complete");
	    status = setCar(OBSERVE_CAR,CAR_ERROR,DQ_RPC_FAILED,errMess,dummy);
	    if(status != OK)
	    {
		printf("error setting car\n");
		
	    }
	    continue;
	}

	cicsLogMessage(3,"doStop 3"); 
	getDbInfoT(dbTop, SEQREG ".VAL", dummy, DBF_LONG, &seqRegVal);
	seqRegVal |= ABORT_INT;
	status = putDbInfoT(dbTop, SEQREGSET ".VAL", dummy, DBF_LONG, 
			    &seqRegVal);
	    
	if(status != OK)
	{
	    sprintf(errMess,"doAbort Failed; could not signal Sequencer");
	    status = setCar(OBSERVE_CAR,CAR_ERROR,ABORTFAILED,errMess,dummy);
	    if(status != OK)
	    {
		printf("error setting car\n");
		
	    }
	    continue;
	}


	if(status != OK)

	{ /* Otherwise, set CAR and DONE records to ERROR.  Also set
	     error code and error message of the CAR record	  */

	    status = setCar(OBSERVE_CAR,CAR_ERROR,error,errMess,dummy);
	    if(status != OK)
	    {
		printf("error setting car\n");
		
	    }

	}  	

    }	
  
    return status;
}








/*
 *+
 * FUNCTION NAME:
 * doReboot
 *
 * INVOCATION:
 * int n1,n2,n3,n4,n5,n6,n7,n8,n9,n10;
 * 
 * doReboot( n1, n2, n3, n4, n5, n6, n7, n8, n9, n10)
 *
 * PARAMETERS: (">" input, "!" modified, "<" output)
 * > n1   (int)    integer containing the address of the reboot CAD
 * > n2   (int)    not used, 0
 * > n3   (int)    not used, 0
 * > n4   (int)    not used, 0
 * > n5   (int)    not used, 0
 * > n6   (int)    not used, 0
 * > n7   (int)    not used, 0
 * > n8   (int)    not used, 0
 * > n9   (int)    not used, 0
 * > n10  (int)    not used, 0
 *
 * FUNCTION VALUE:
 * long  Status value, although function should never return
 *
 * PURPOSE:
 * Performs all functions necessary to implement the "reboot" command
 *
 * DESCRIPTION:
 * This VxWorks task is created (spawned) upon startup.  The task waits
 * for its semaphore and for the completion of any other tasks (if any)
 * which must finish execution before this one begins.  It executes all
 * the functions needed to perform a "reboot" of the system and updates the 
 * associatied CAR record appropriately.  The output parameters of the
 * associated CAD record are available though the n1 input parameter.
 *
 * EXTERNAL VARIABLES:
 * None
 *
 * PRIOR REQUIREMENTS:
 * None
 *
 * DEFICIENCIES:
 * None known.
 *
 * HISTORY (optional):
 * 17-Apr-1997  Original version.			   J.E. Tvedt
 *
 *-
 */
int doReboot( int n1, int n2, int n3, int n4, int n5, int n6, int n7,
		  int n8, int n9, int n10)
{
    struct cadRecord *pCad;
    long status;
    long lVal;
    char errMess[MAX_STRING_SIZE];
    char dummy[MAX_STRING_SIZE];
   


      while(dqRpcInit(rpcServer)!= OK)
	;
    
    /* Sleep until there is something to do */
    while( 1 )
    {
	semTake(semReboot,WAIT_FOREVER);  

	/* Get the address of the CAD record so command arguments
	   are accessible.  Currently, there are no input arguments
	   for the reboot command.
	*/	
	pCad = (struct cadRecord *) n1;

	status = setCar(REBOOT_CAR,CAR_BUSY,OK,"",dummy);
	if(status != OK)
	{
	    printf("error setting car\n");
	    continue;
	}


	/* just deactivate the array */
	lVal = FALSE;
	status = putDbInfoT(dbTop, ACTIVATE ".VAL", dummy, DBF_LONG, &lVal);

	/* If status is OK, set CAR to IDLE */
	if(status == OK)
	{
	  
	    printf("REBOOTING\n");
	    sleep(1,0);
	    rebootNow();
	    status = setCar(REBOOT_CAR,CAR_IDLE,OK,"",dummy);
	    if(status != OK)
	    {
		printf("error setting car\n");
	       
	    }
	}
	else
	{   /* Otherwise, set CAR and DONE records to ERROR.  Also set
	     * error code and error message of the CAR record */
	   
	    sprintf(errMess,"DoReboot putdbinfo to activate record failed");
	    status = setCar(REBOOT_CAR,CAR_ERROR,REBOOTFAILED,errMess,dummy);
	    if(status != OK)
	    {
		printf("error setting car\n");
		
	    }
	}

    }
    return status;
}


