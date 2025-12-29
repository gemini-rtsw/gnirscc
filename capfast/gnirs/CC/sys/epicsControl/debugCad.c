static struct {
	void *v;
	char *c;
} rcsid = {
	&rcsid,
	"$Id: debugCad.c,v 1.1 2009/06/10 15:05:10 gemvx Exp $"
};
/*
 * Copyright 1997 Association of Universities for Research in Astronomy, Inc.
 * See the file COPYRIGHT for more details.
 *
 * FILENAME
 * debugCad.c
 *
 * DESCRIPTION 
 * Supports CAD record for "debug" command.
 *
 * 
 * FUNCTION NAME(S)
 * debugCad() - interprets directive and assigns new debugging level
 *   
 * DEPENDENCIES
 * Requires EPICS support library.
 *
 *
 *INDENT-OFF*
 * $Log: debugCad.c,v $
 * Revision 1.1  2009/06/10 15:05:10  gemvx
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
#define DEBUG_LEVEL a
#define DEBUG_LOGGING b
#define DEBUG_DISPLAY c
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
 *     debugCad
 *
 * INVOCATION:
 *     struct cadRecord *pCad;
 *     status = debugCad( pCad );
 *
 * PARAMETERS: (">" input, "!" modified, "<" output)
 *     ! pCad   (struct cadRecord *)   pointer to CAD data structure
 *
 * FUNCTION VALUE:
 *     long  Status value written to CAD VAL field
 *
 * PURPOSE:
 *     User defined function for "debug" CAD record
 *
 * DESCRIPTION:
 *     This routine is called whenever the debug CAD record is processed.
 *     debug is the command for setting the debug level.
 *     The command has one input argument: 
 *   	    debug level - which is restricted to one of the following:  
 *			    QUIET, NONE, MIN,  FULL, MAX.
 *     This function provides two outputs to the CAD record:
 *        VALA - long integer representation of selected debugging level
 *        VALB - string value of selected debugging level
 *
 * EXTERNAL VARIABLES:
 *     debugLevel - in the CICS logging routines
 *
 * PRIOR REQUIREMENTS:
 *     It is assumed the CAD record has already been initialized and the
 *     directive and any arguments have already been assembled into the cadRecord
 *     data structure.
 *
 * DEFICIENCIES:
 * 
 *
 * HISTORY:
 * 25-Jan-1999   Original version.     Janet Tvedt
 *
 *-
 */
long debugInitCad( struct cadRecord *pCad ) 
{
   long status = CAD_ACCEPT; 
   return status;

}

long debugCad( struct cadRecord *pCad ) 
{
	char temp[200];
   long status = CAD_ACCEPT;         /* return status */
   static long dbg;
   static long log;
   static long disp;


   /* Switch according to the CAD directive in DIR field */
   switch (DIRECTIVE)
   {
      /* CAD MARK directive detected. Nothing needs to be done.*/
      case CAD_MARK:
         LOG_MSG(DEBUG2_MSG, "debug - MARK directive.");
         break;

	 /* CAD PRESET directive detected.  Check the input argument. 
         If it is acceptable then copy it to the output.    */
	
     case CAD_PRESET:
       LOG_MSG(DEBUG2_MSG, "debug - PRESET directive.");
       cicsLogString( 3, "Requested Debug Level =", cadInput(DEBUG_LEVEL));

	   log = atoi(cadInput(DEBUG_LOGGING));
 	   disp = atoi(cadInput(DEBUG_DISPLAY)) ;
       if(strcmp(cadInput(DEBUG_LEVEL),"QUIET") == 0)
		   dbg = DBG_QUIET;
       else if(strcmp(cadInput(DEBUG_LEVEL),"NONE") == 0)
		   dbg = DBG_NONE;
       else if(strcmp(cadInput(DEBUG_LEVEL),"MIN") == 0)
		   dbg = DBG_MIN;
       else if(strcmp(cadInput(DEBUG_LEVEL),"FULL") == 0)
		   dbg = DBG_FULL;
       else if(strcmp(cadInput(DEBUG_LEVEL),"MAX") == 0)
		   dbg = DBG_MAX;
       else
       {
		   
		   sprintf(temp,  "Unrecognized debug level %s ",cadInput(DEBUG_LEVEL));
		   strncpy(MESSAGE,temp,MAX_STRING_SIZE - 1);
		   cicsLogMessage(0, MESSAGE);
		   status = CAD_REJECT;
       }

     
        	 
       break;

	   /* CAD CLEAR directive detected. Nothing needs to be done. */
     case CAD_CLEAR:
       LOG_MSG(DEBUG2_MSG, "debug - CLEAR directive.");
       break;

       /* CAD START directive detected. Set CAR to BUSY by processing the CAR
	 interface record, then call function to set the debug level.  The CAR
	 will be set back to IDLE automatically by a seq record in the database. 
      */ 
       break;
     case CAD_START:
       LOG_MSG(DEBUG2_MSG, "debug - START directive.");
	    carVal = CAR_BUSY;
	    /*valh is connected to car record*/
	    *(long *)pCad->valh = carVal;

       status = assignVal(type(DEBUG_LEVEL),&dbg,cadOutput(DEBUG_LEVEL),
			      MESSAGE);
       status = assignVal(type(DEBUG_LOGGING),&log,cadOutput(DEBUG_LOGGING),
			      MESSAGE);
       status = assignVal(type(DEBUG_DISPLAY),&disp,cadOutput(DEBUG_DISPLAY),
			      MESSAGE);
       semGive( semDebug );

   
       cicsSetDebug(*(long *) cadOutput(DEBUG_LEVEL)); 

       break;

       /* CAD STOP directive detected. 
      */
     case CAD_STOP:
       cicsLogMessage( 1, "debugCad: Cannot be stopped.");
       status = CAD_REJECT;
       strncpy( MESSAGE, "debugCad: Cannot be stopped",MAX_STRING_SIZE - 1  );
       break;

       /* Unrecognised CAD directive detected. This is regarded as an error. */
     default:
       strncpy( MESSAGE, "debugCad: Unrecognized CAD directive",MAX_STRING_SIZE - 1 );
       status = CAD_REJECT;
       break;
   }
  
       
 
   return status;
}


/*
 *+
 * FUNCTION NAME:
 * debugCtrl
 *
 * INVOCATION:  spawned by initTasks()
 *  struct cadRecord *pCad;
 *  debugCtrl( pCad, 0,0,0,0,0,0,0,0,0);
 *
 * PARAMETERS: (">" input, "!" modified, "<" output)
 * >pCad  (struct cadRecord *) Address of associated CAD record
 * others not used but are required by VxWorks for spawned tasks
 *
 * FUNCTION VALUE:
 * int -  except the function never returns
 *
 * PURPOSE:

 *
 * DESCRIPTION:

 *
 *
 * EXTERNAL VARIABLES:
 * None
 *
 * PRIOR REQUIREMENTS:
 *
 *
 * DEFICIENCIES:
 *
 *
 * HISTORY (optional):
 
 *
 *-
 */
/* #define LOG_MSG(a,b) printf(b) */
int debugCtrl( int n1, int n2, int n3, int n4, int n5, int n6, int n7,
	      int n8, int n9, int n10 )
{
    long            status;
    char           dummy[40];
    struct         cadRecord* pCad;
    pCad = (struct cadRecord *) n1;
  
    while( 1 )
    {
		status = OK;
		LOG_MSG(DEBUG2_MSG, "Task tdebugCtrl sleeping...");
      
		semTake(semDebug, WAIT_FOREVER);
		LOG_MSG(DEBUG2_MSG, "Task tdebugCtrl awake...");
		sleep(1,0);	




	    /* Set CAR record to IDLE or ERROR depending on init DONE status */
	    carVal = CAR_IDLE;
	    if(setCar(DEBUG_CAR,CAR_IDLE,OK,"",dummy)!= OK)
			DPRINT(DPdebug,ERROR_MSG,dummy);


    }  /* End of while(1) */
}
 
