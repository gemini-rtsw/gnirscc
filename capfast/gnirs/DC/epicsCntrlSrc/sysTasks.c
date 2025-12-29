static struct {
        void *v;
        char *c;
} rcsid = {
        &rcsid,
        "$Id: sysTasks.c,v 1.2 2009/05/27 19:32:23 fkraemer Exp $"
};
/*
 * Copyright 1997 Association of Universities for Research in Astronomy, Inc. 
 * See the file COPYRIGHT for more details.
 *
 * FILENAME
 * sysTasks.c
 *
 * DESCRIPTION
 * This file contains tasks which are spawned on startup and wait for a 
 * semaphore
 * 
 * FUNCTION NAME(S)
 *	doTest - 
 *	doinit -  initialize system, load  values into epics variables
 * 	doSetDhsInfo -
 * DEPENDENCIES
 * 
 *
 *INDENT-OFF*
 * 
 *
 *INDENT-ON* 
 */

#include <vxWorks.h>
#include <vme.h>
#include <taskLib.h>
#include <semLib.h>
#include <epCommon.h>
#include <naacTasks.h>
#include <sysLib.h>
#include <rpcLib.h>
#include <car.h>
#include <gnerrno.h>
extern char *dbTop;
extern char *dbSadTop;
int sysdebug = 0;

extern int dqDhsConnectionRPC(int);

/*
 *+
 * FUNCTION NAME:
 *	doTest
 *
 * INVOCATION: 
 *	spawned from initTasks function
 *
 * PARAMETERS: (">" input, "!" modified, "<" output)
 *	10 ints as defined in taskSpawn function
 *	The first one is a pointer to the cad record.
 *
 * FUNCTION VALUE:
 *	infinite loop, never returns
 *
 * PURPOSE:
 *     	currently none, but can be modified to run a system test
 *
 * DESCRIPTION:
 *
 * EXTERNAL VARIABLES:
 * 	semTest - semaphore which gets released to start processing of this
 *		function
 *	dbTop - database prefix
 *
 * PRIOR REQUIREMENTS:
 *
 * DEFICIENCIES:
 *
 * HISTORY (optional):
 *
 *-
 */
int doTest( int n1, int n2, int n3, int n4, int n5, int n6, int n7,
		  int n8, int n9, int n10)
{
    struct cadRecord *pCad;
    long status;
    long error;
    char errMess[MAX_STRING_SIZE];
    char dummy[MAX_STRING_SIZE];
    
	pCad = (struct cadRecord *) n1;
    /* Sleep until there is something to do */
    while( 1 )
    {
	cicsLogMessage(2,"task tDoTest sleeping ...");
	semTake(semTest,WAIT_FOREVER); 
	cicsLogMessage(2, "task tDoTest awake");

	/* Get the address of the CAD record so command arguments
	   are accessible.  Currently, there are no input arguments
	   for the test command.
	*/	

	/* Test the system */
	cicsLogMessage( 2, "Testing the system");
	sleep(3,0);
	cicsLogMessage( 2, "Test is complete");

	/* The following variables need to be set by the code replacing the sleep
	   functions.  These variable are used to indicate whether these tasks 
	   were successful and what error may have occured.
	*/
	status = OK;
	error = 0;
	strcpy(errMess, "\0");
	/*************************************************************************/

    
	/* If status is OK, set CAR to IDLE */
	if(status == OK)
	{
	    setCar(TEST_CAR,CAR_IDLE,OK,"",dummy);
	 
	}

	/* Otherwise, set CAR and DONE records to ERROR.  Also set
	   error code and error message of the CAR record
	*/
	else
	{
	    setCar(TEST_CAR,CAR_ERROR,ERROR,errMess,dummy);
	
	}
    }
    return status;
}
/*
 *+
 * FUNCTION NAME:
 *	doInit
 *
 * INVOCATION:
 *	spawned from initTasks function
 *
 * PARAMETERS: (">" input, "!" modified, "<" output)
 *	10 ints as defined in taskSpawn function
 *	The first one is a pointer to the cad record.
 *
 * FUNCTION VALUE:
 *	infinite loop, never returns
 *
 * PURPOSE:
 *
 * DESCRIPTION:
 *
 * EXTERNAL VARIABLES:
 * 	semInit - semaphore which gets released to start processing of this
 *		function
 *	dbTop - database prefix
 *	dbSadTop - status and alarm database prefix
 *
 * PRIOR REQUIREMENTS:
 *
 * DEFICIENCIES:
 *
 * HISTORY (optional):
 *
 *-
 */
int doInit( int n1, int n2, int n3, int n4, int n5, int n6, int n7,
		  int n8, int n9, int n10)
{
  char buf[80];
    struct cadRecord *pCad;
    long status;
    long error;
    long val;
    char errMess[MAX_STRING_SIZE];
    char dummy[MAX_STRING_SIZE];
    

    cicsLogMessage(3, "doInit: \n");
    
	pCad = (struct cadRecord *) n1;
    /* Sleep until there is something to do */
    while( 1 )
    {
	cicsLogMessage(2,"task tDoInit sleeping ...");
	semTake(semInit,WAIT_FOREVER); 
	cicsLogMessage(2, "task tDoInit awake");

	cicsLogMessage(3, "doInit: got semaphore\n");
	/* Get the address of the CAD record so command arguments
	   are accessible.  
	*/	
	printCadVals(3, pCad, 1);

	/* Initialize the system */
	cicsLogMessage( 2,"Initializing the system");

	/*  any low level epics initialization
	*/
	/* pvloads*/
	sprintf(buf,"top=%s,sadtop=%s",dbTop,dbSadTop);
	status = pvload(INIT_VALS,	buf);
	if(status == OK)
	    status = pvload(CAR_VALS, 	buf);
	if(status == OK)
	    status = pvload(CAD_VALS,	buf);
	if(status == OK)
	    cicsLogMessage(3, "Initialization complete\n"); 

	/* If status is OK, set CAR to IDLE */
	if(status == OK)
	{
	    error = 0;
	    strcpy(errMess, "\0");
	    setCar(INIT_CAR,CAR_IDLE,OK,"",dummy);
	    putDbInfoT(dbSadTop, STATE ".VAL", dummy, DBF_STRING, "RUNNING");
	    val = 1;
	    putDbInfoT(dbTop, "masterEnable" ".VAL", dummy, DBF_LONG, &val);
	}
	else	/* Otherwise, set CAR and DONE records to ERROR.  Also set 
		   error code  and error message of the CAR record 	*/
	{
	    error = ERROR;
	    strcpy(errMess,"doInit: pvload error\n");
	    setCar(INIT_CAR,CAR_ERROR,error,errMess,dummy);
	}
    }
    return status;
}

/* 26-Jun-1997: Function below modified to set the quick look stream
 *              Steven Beard.
 */
/*
 *+
 * FUNCTION NAME:
 *	doSetDhsInfo
 *
 * INVOCATION:
 *	spawned from initTasks function
 *
 * PARAMETERS: (">" input, "!" modified, "<" output)
 *	10 ints as defined in taskSpawn function
 *	The first one is a pointer to the cad record.
 *
 * FUNCTION VALUE:
 *	infinite loop, never returns
 *
 * PURPOSE:
 *
 * DESCRIPTION:
 *
 * EXTERNAL VARIABLES:
 * None
 *
 * PRIOR REQUIREMENTS:
 *
 * DEFICIENCIES:
 *
 * HISTORY (optional):
 *
 *-
 */
int doSetDhsInfo( int n1, int n2, int n3, int n4, int n5, int n6, int n7,
		  int n8, int n9, int n10)
{
    struct cadRecord *pCad;
    long status;
    long error;
    char errMess[MAX_STRING_SIZE];
    char dummy[MAX_STRING_SIZE];
    char name[MAX_STRING_SIZE];
    
    /* Sleep until there is something to do */
    while( 1 )
    {
	cicsLogMessage(2,"task tDoSetDhsInfo sleeping ...");
	semTake(semSetDhsInfo,WAIT_FOREVER); 
	cicsLogMessage(2, "task tDoSetDhsInfo awake");

        /* Initialise the status and error message */
	status = OK;
	error = 0;
	strcpy(errMess, "\0");

	/* Get the address of the CAD record so command arguments
	   are accessible.  
	*/	
	pCad = (struct cadRecord *) n1;
	printCadVals(3,pCad,1);

        /* Call the data handling function to set the quick look stream,
         * and update the current quick look stream in the Status Alarm
         * database.
         */

	cicsLogMessage( 2, "Setting quick look stream");

/*        status = dataSetDhsInfo( pCad->a ); */

        sprintf( name, "%sqlStream.VAL", dbSadTop );
        status = putDbInfo( name, errMess, DBF_STRING, pCad->vala );

	cicsLogMessage( 2, "Quick look stream set");


    
	/* If status is OK, set CAR to IDLE */
	if(status == OK)
	{
	    setCar(GSYS_CAR,CAR_IDLE,OK,"",dummy);
	 
	}

	/* Otherwise, set CAR and DONE records to ERROR.  Also set
	   error code and error message of the CAR record
	*/
	else
	{
	    setCar(GSYS_CAR,CAR_ERROR,error,errMess,dummy);

	}
    }
    return status;
}

/*
 *+
 * FUNCTION NAME:
 *      doDhsConnect
 *
 * INVOCATION:
 *      spawned from initTasks function
 *
 * PARAMETERS: (">" input, "!" modified, "<" output)
 *      10 ints as defined in taskSpawn function
 *      The first one is a pointer to the cad record.
 *
 * FUNCTION VALUE:
 *      infinite loop, never returns
 *
 * PURPOSE:
 *
 * DESCRIPTION:
 *
 * EXTERNAL VARIABLES:
 * None
 *
 * PRIOR REQUIREMENTS:
 *
 * DEFICIENCIES:
 *
 * HISTORY (optional):
 *
 *-
 */
/*extern globals*/
extern char *rpcServer; 
int dqRpcInit(char *server) ;

int doDhsConnect( int n1, int n2, int n3, int n4, int n5, int n6, int n7,
                  int n8, int n9, int n10)
{
    struct cadRecord *pCad;
    long status;
    long error;
    char errMess[MAX_STRING_SIZE];
    char dummy[MAX_STRING_SIZE];
    char name[MAX_STRING_SIZE];


     printf("en doDhsConnect\n");
      while(dqRpcInit(rpcServer)!= OK)
        ;

     printf("dqRpcIni OK\n");
    /* Sleep until there is something to do */
    while( 1 ) {
      cicsLogMessage(2,"task tdoDhsConnect sleeping ...");
      semTake(semDhsConnect,WAIT_FOREVER);
      cicsLogMessage(2, "task tDoSetDhsInfo awake");
      printf("task tDoSetDhsInfo awake\n");

      /* Initialise the status and error message */
      status = OK;
      error = 0;
      strcpy(errMess, "\0");

      /* Get the address of the CAD record so command arguments
         are accessible.
       */
      pCad = (struct cadRecord *) n1;
      printCadVals(0,pCad,1);

      if (*(long*)pCad->vala == 1) {
         /* request a connection */
         printf("request a connection\n"); 
         status = dqDhsConnectionRPC(1);
         }
      else if (*(long*)pCad->vala == 0) {
         /* request a disconnection */
         printf("request a disconnection\n"); 
         status = dqDhsConnectionRPC(0); 
         }

      if(status == OK) {
         /* we only send a request (which might fail)
          * let sad update from dc-data
          * sprintf( name, "%s%s", dbSadTop ,SAD_DHSCONNECTED);
          * putDbInfo(name, errMess, DBF_LONG, *(long*)pCad->vala);
          */
         setCar(GSYS_CAR,CAR_IDLE,OK,"",dummy);
         }
      else {
         /* we only send a request (which might fail)
          * let sad update from dc-data
          * sprintf( name, "%s%s", dbSadTop ,SAD_DHSCONNECTED);
          * putDbInfo(name, errMess, DBF_LONG, *(long*)pCad->vala);
          */
         printf("dqDhsConnectionRPC failed\n");
         /*setCar(GSYS_CAR,CAR_ERROR,DQ_RPC_FAILED,errMess,dummy); */
         }
      }
   return status;
}

