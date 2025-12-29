static struct {
	void *v;
	char *c;
} rcsid = {
	&rcsid,
	"$Id: diagnoseCad.c,v 1.1 2009/06/10 15:05:11 gemvx Exp $"
};

/*
 * Copyright 1997 Association of Universities for Research in Astronomy, Inc.
 * See the file COPYRIGHT for more details.
 *
 * FILENAME
 * 	diagnose.c
 *
 * DESCRIPTION 
 * 	Implements Gemini "diagnose" command.
 *
 * 
 * FUNCTION NAME(S)
 *	diagnoseCad
 *
 *   
 * DEPENDENCIES
 * 	EPICS support libraries
 *
 *
 *INDENT-OFF*
 * $Log: diagnoseCad.c,v $
 * Revision 1.1  2009/06/10 15:05:11  gemvx
 * Added Files:
 * epicsControl diectory populated.
 *
 *INDENT-ON* 
 */


/* VxWorks include files */
#include <vxWorks.h>
#include <taskLib.h>
#include <semLib.h>

/* EPICS specific include files */
#include "epCommon.h"
#include "epicsNames.h"
#include "gnirsTasks.h"

/* Include file needed for control tasks */
#include <sysLib.h>
#include <car.h>

/* this value will have the current state of the car*/
static carVal = CAR_IDLE;

/*
 *+
 * FUNCTION NAME:
 *	diagnoseCad
 *
 * INVOCATION:
 *	struct cadRecord *pCad;
 * 	status = diagnoseCad( pCad );
 *
 * PARAMETERS: (">" input, "!" modified, "<" output)
 * 	> pCad   (struct cadRecord *)   pointer to CAD data structure
 *
 * FUNCTION VALUE:
 * 	long  Status value written to CAD VAL field
 *
 * PURPOSE:
 * 	User defined function for "diagnose" CAD record
 *
 *
 *
 * DESCRIPTION:
 *	This function is triggered by the diagnose cad record.  When start
 * 	is received, it releases a semaphore to allow the diagnoseCtrl routine
 *	to run.
 *
 *
 * EXTERNAL VARIABLES:
 *	none
 *
 * PRIOR REQUIREMENTS:
 *
 *
 * DEFICIENCIES:
 *
 *
 * HISTORY (optional):
 *
 *
 *
 *-
 */
int diagnoseInitCad(struct cadRecord *pCad)

{

   long status = CAD_ACCEPT; 
   return status;
}


int diagnoseCad(struct cadRecord *pCad)

{
    long status = CAD_ACCEPT;

    /* Action to be taken depends on directive received		*/
    switch (DIRECTIVE)
    {
      case CAD_MARK:	       	/* No action required		*/
	LOG_MSG(DEBUG2_MSG, "diagnoseCad - MARK directive");
	break;

      case CAD_PRESET:
	LOG_MSG(DEBUG2_MSG, "diagnoseCad - PRESET directive");

	break;
      case CAD_CLEAR:		/* No action required		*/
	LOG_MSG(DEBUG2_MSG, "diagnoseCad - CLEAR directive");
	break;

      case CAD_START:		/* Execute diagnose command	*/
	LOG_MSG(DEBUG2_MSG, "diagnoseCad - START directive");
	
	
	    carVal = CAR_BUSY;
	    /*valh is connected to car record*/
	    *(long *)pCad->valh = carVal;
	     /* release semaphore to diagnoseCtrl function*/
	     semGive (semDiagnose);
	break;

      case CAD_STOP:		   /* Really can't stop	       */
	LOG_MSG(DEBUG2_MSG, "diagnoseCad - STOP directive");
	strcpy(MESSAGE, "Cannot stop");
	status = CAD_REJECT;
	break;

      default:		   /* Unknown directive		*/
	strcpy(MESSAGE, "Unrecongized directive");
	status = CAD_REJECT;
	break;
    }
   
    return status;
 
}

/*
 *+
 * FUNCTION NAME:
 * 	diagnoseCtrl
 *
 * INVOCATION:
 *	Spawned from initTasks routine.
 *
 * PARAMETERS: (">" input, "!" modified, "<" output)
 *	 Set of 10 integer variables, most of which mean nothing, some of which
 *      	are interpreted in various ways.
 *
 * FUNCTION VALUE:
 *	int             Result of function...should never be seen.
 *
 * PURPOSE:
 *	perform a diagnose.
 *
 * DESCRIPTION:
 *	Started by initTasks, when system boots up.  Waits on a semaphore
 *	which is released by the diagnoseCad routine.  Tells low level software
 *	to perform a diagnose.
 *
 *
 * EXTERNAL VARIABLES:
 *	none
 *
 * PRIOR REQUIREMENTS:
 *
 *
 * DEFICIENCIES:
 *
 *
 * HISTORY (optional):
 *
 *
 *
 *-
 */
int diagnoseCtrl( int n1, int n2, int n3, int n4, int n5, int n6, int n7, int n8,
				  int n9, int n10 )
{
    char errMess[80];
    char err[80];
    struct cadRecord* pCad;
    long status = OK;


    /* Convert first parameter to CAD address	*/
    pCad = (struct cadRecord *) n1;

    /* Repeat as inifinite loop	*/
    while(1)
    {
		status = OK;
		LOG_MSG(DEBUG2_MSG, "Task tdiagnoseCtrl sleeping...\n");

		/* Wait for our semaphore   */
		semTake(semDiagnose, WAIT_FOREVER);
		LOG_MSG(DEBUG2_MSG, "Task tdiagnoseCtrl awake..."); 


		
		if(status == OK)
		{
			carVal = CAR_IDLE;
			/* set car back to idle*/
			if(setCar(DIAGNOSE_CAR,CAR_IDLE,OK,"",err) != OK)
			{
				LOG_MSG(ERROR_MSG,err);
			}
		}
		else
		{

			carVal = CAR_ERROR;
			/* set car back to idle*/
			if(setCar(DIAGNOSE_CAR,CAR_ERROR,ERROR,errMess,err) != OK)
			{
				LOG_MSG(ERROR_MSG,err);
			}
		
		}
	}
    return status;
}
