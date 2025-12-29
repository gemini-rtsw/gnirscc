static struct {
	void *v;
	char *c;
} rcsid = {
	&rcsid,
	"$Id: testCad.c,v 1.1 2009/06/10 15:05:14 gemvx Exp $"
};

/*
 * Copyright 1997 Association of Universities for Research in Astronomy, Inc.
 * See the file COPYRIGHT for more details.
 *
 * FILENAME
 * 	test.c
 *
 * DESCRIPTION 
 * 	Implements Gemini "test" command.
 *
 * 
 * FUNCTION NAME(S)
 *	testCad
 *
 *   
 * DEPENDENCIES
 * 	EPICS support libraries
 *
 *
 *INDENT-OFF*
 * $Log: testCad.c,v $
 * Revision 1.1  2009/06/10 15:05:14  gemvx
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
#include "gnirsCC.h"    /* added by rjw for VME_OK */

/* Include file needed for control tasks */
#include <sysLib.h>
#include <car.h>

/* static global variables*/
/* this value will have the current state of the car*/
static carVal = CAR_IDLE;

/*
 *+
 * FUNCTION NAME:
 *	testCad
 *
 * INVOCATION:
 *	struct cadRecord *pCad;
 * 	status = testCad( pCad );
 *
 * PARAMETERS: (">" input, "!" modified, "<" output)
 * 	> pCad   (struct cadRecord *)   pointer to CAD data structure
 *
 * FUNCTION VALUE:
 * 	long  Status value written to CAD VAL field
 *
 * PURPOSE:
 * 	User defined function for "test" CAD record
 *
 *
 *
 * DESCRIPTION:
 *	This function is triggered by the test cad record.  When start
 * 	is received, it releases a semaphore to allow the testCtrl routine
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

int testInitCad(struct cadRecord *pCad)
{

   long status = CAD_ACCEPT; 
   return status;
}

int testCad(struct cadRecord *pCad)

{
    long status = CAD_ACCEPT;

    /* Action to be taken depends on directive received		*/
    switch (DIRECTIVE)
    {
      case CAD_MARK:	       	/* No action required		*/
		LOG_MSG(DEBUG1_MSG, "testCad - MARK directive");
		break;

      case CAD_PRESET:
		LOG_MSG(DEBUG1_MSG, "testCad - PRESET directive");

		break;
      case CAD_CLEAR:		/* No action required		*/
		LOG_MSG(DEBUG1_MSG, "testCad - CLEAR directive");
		break;

      case CAD_START:		/* Execute test command	*/
		LOG_MSG(DEBUG1_MSG, "testCad - START directive");
	
		carVal = CAR_BUSY;
		/* release semaphore to testCtrl function*/
		/*valh is connected to car record*/
		*(long *)pCad->valh = carVal;
		semGive (semTest);
		break;

      case CAD_STOP:		   /* Really can't stop	       */
		LOG_MSG(DEBUG1_MSG, "testCad - STOP directive");
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
 * 	testCtrl
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
 *	perform a test.
 *
 * DESCRIPTION:
 *	Started by initTasks, when system boots up.  Waits on a semaphore
 *	which is released by the testCad routine.  Tells low level software
 *	to perform a test.
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
int testCtrl( int n1, int n2, int n3, int n4, int n5, int n6, int n7, int n8,
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
		LOG_MSG(DEBUG1_MSG, "Task ttestCtrl sleeping...\n");

		/* Wait for our semaphore   */
		semTake(semTest, WAIT_FOREVER);
		LOG_MSG(DEBUG1_MSG, "Task ttestCtrl awake..."); 
		carVal = CAR_BUSY;
	
		/*tell low level to do a test*/
	    
		status = testHardware(); 
		
		if (status != VME_OK)
		{
			status = CAR_ERROR	;
			strncpy(errMess,gnirsErrorMessage,79);
			
		}
		
	
		if(status == OK)
		{
			carVal = CAR_IDLE;
			if(setCar(TEST_CAR,CAR_IDLE,OK,"",err) != OK)
			{
				LOG_MSG(ERROR_MSG,err);
			}
		}
		else
		{
			carVal = CAR_ERROR;
			if(setCar(TEST_CAR,CAR_ERROR,ERROR,errMess,err) != OK)
			{
				LOG_MSG(ERROR_MSG,err);
				LOG_MSG(ERROR_MSG,errMess);
			}
			
		}
    }
    return status;
}
