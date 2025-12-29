static struct {
        void *v;
        char *c;
} rcsid = {
        &rcsid,
        "$Id: stateCad.c,v 1.2 2009/05/27 19:32:22 fkraemer Exp $"
};
/*
 * Copyright 1997 Association of Universities for Research in Astronomy, Inc.
 * See the file COPYRIGHT for more details.
 *
 * FILENAME
 * stateCad.c
 *
 * DESCRIPTION
 * This file contains the source for all the functions used by the
 * NAAC state CAD records. These functions are used to validate the 
 * arguments given to the CAD record.  Each function checks that the 
 * command is acceptable and returns a status. A message is supplied 
 * for rejected commands or error conditions.  The status and message 
 * is subsequently written to the VAL and MESS fields of the CAD record 
 * by the record support routines. 
 * 
 * FUNCTION NAME(S)
 * parkProc       	- Park all components 
 * abortProc      	- Abort observation, toss data
 * observeProc       	- Start observation
 * stopProc     	- Stop observation, keep data
 * pauseProc            - Pause observation (not currently supported)
 * continueProc         - Continue observation if paused
 *   
 * DEPENDENCIES
 * The names of the subroutines in this file should be identical to those
 * declared in the SNAM field of each CAD record. If a change is made to
 * the name of a subroutine, that change should be reflected in the SNAM
 * field, and vice versa.
 *
 * The ordering of the arguments within each CAD record (A, B, C...) is
 * defined in the description of the interface between the CAD database
 * and its clients. Changes to that interface should be reflected in this
 * file.
 *
 * The input arguments for each CAD record (A, B, C...) are all strings
 * and must be converted to their appropriate data types before validation.
 * However, the data type of each output argument (VALA, VALB, VALC...)
 * is determined by the (FTVA, FTVB, FTVC....) fields in the CAD record.
 * The data types assumed here must match those declared in the CAD
 * record properties.
 *
 *INDENT-OFF*
 * $Log: stateCad.c,v $
 * Revision 1.2  2009/05/27 19:32:22  fkraemer
 * fkraemer - copied my complete working dir over trunk
 *
 * Revision 1.1.1.1  1998/09/30 16:40:28  pruckle
 * Initial Release
 *
 *INDENT-ON* 
 */

#include <epCommon.h>
#include <naacTasks.h>
#include <car.h>
#include <symLib.h>
#include <timeLib.h>
#include <slalib.h>
extern char *dbTop;

/*
 *+
 * FUNCTION NAME:
 * parkProc
 *
 * INVOCATION:
 * struct cadRecord *pCad;
 * status = parkProc( pCad );
 *
 * PARAMETERS: (">" input, "!" modified, "<" output)
 * ! pCad   (struct cadRecord *)   pointer to CAD data structure
 *
 * FUNCTION VALUE:
 * long  Status value written to CAD VAL field
 *
 * PURPOSE:
 * User defined function for "park" CAD record
 *
 * DESCRIPTION:
 * This routine is called whenever the park CAD record is processed.
 * park is the command to park all components .  
 * The command has no arguments.
 *
 * EXTERNAL VARIABLES:
 * None
 *
 * PRIOR REQUIREMENTS:
 * It is assumed the CAD record has already been initialized and the
 * directive and any arguments have already been assembled into the cadRecord
 * data structure.
 *
 * DEFICIENCIES:
 * None
 *
 * HISTORY:
 * 17-Jan-1997  Original version adapted from CICS alpha 1.0   J.E. Tvedt
 *
 *-
 */

long parkProc( struct cadRecord *pCad ) 
{
    char dummy[80];
   long status;         /* return status */

   /* Initialise CAD status */
   status = CAD_ACCEPT;

   /* Switch according to the CAD directive in DIR field */
   switch (pCad->dir)
   {
      /* CAD MARK directive detected. Nothing needs to be done.*/
      case CAD_MARK:
         cicsLogMessage( 2, "park - MARK directive.");
         break;

      /* CAD PRESET directive detected.  Check the input argument. 
         If it is acceptable then copy it to the output. 
      */
      case CAD_PRESET:
         cicsLogMessage( 2, "park - PRESET directive.");
         break;

      /* CAD CLEAR directive detected. Nothing needs to be done. */
      case CAD_CLEAR:
         cicsLogMessage( 2, "park - CLEAR directive.");
         break;

      /* CAD START directive detected. Cannot do this if
         an observation is in progress
      */
      case CAD_START:
         cicsLogMessage( 2, "park - START directive.");
	 if(carStatus(pCad->a) == CAR_IDLE)
	 {
	     status = setCar(PARK_CAR,CAR_BUSY,OK,"",dummy);
	   
	     if(status == OK)
		 semGive(semPark);
	     else
		 status = CAD_REJECT;
	 }
	 else
	 {
	     status = CAD_REJECT;
	     strcpy(pCad->mess,"park - cannot be done during observe");
	     cicsLogMessage(1, pCad->mess);
	 }
         break;

      /* CAD STOP directive detected.
      */
      case CAD_STOP:
         cicsLogMessage( 1, "park - STOP directive. Cannot be stopped.");
         strcpy( pCad->mess, "Cannot be stopped");
	 status = CAD_REJECT;
         break;

      /* Unrecognised CAD directive detected. This is regarded as an error. */
      default:
         strcpy( pCad->mess, "Unrecognized CAD directive" );
         status = CAD_REJECT;
         break;
   }
   return status;
}


/*
 *+
 * FUNCTION NAME:
 * abortProc
 *
 * INVOCATION:
 * struct cadRecord *pCad;
 * status = abortProc( pCad );
 *
 * PARAMETERS: (">" input, "!" modified, "<" output)
 * ! pCad   (struct cadRecord *)   pointer to CAD data structure
 *
 * FUNCTION VALUE:
 * long  Status value written to CAD VAL field
 *
 * PURPOSE:
 * User defined function for "abort" CAD record
 *
 * DESCRIPTION:
 * This routine is called whenever the abort CAD record is processed.
 * abort is the command to abort the observation and toss the data .  
 * The command has no arguments.
 *
 * EXTERNAL VARIABLES:
 * None
 *
 * PRIOR REQUIREMENTS:
 * It is assumed the CAD record has already been initialized and the
 * directive and any arguments have already been assembled into the cadRecord
 * data structure.
 *
 * DEFICIENCIES:
 * None
 *
 * HISTORY:
 * 17-Jan-1997  Original version adapted from CICS alpha 1.0   J.E. Tvedt
 *
 *-
 */

long abortProc( struct cadRecord *pCad ) 
{
   long status;         /* return status */

   /* Initialise CAD status */
   status = CAD_ACCEPT;

   /* Switch according to the CAD directive in DIR field */
   switch (pCad->dir)
   {
      /* CAD MARK directive detected. Nothing needs to be done.*/
      case CAD_MARK:
         cicsLogMessage( 2, "abort - MARK directive.");
         break;

      /* CAD PRESET directive detected.  Check the input argument. 
         If it is acceptable then copy it to the output. 
      */
      case CAD_PRESET:
         cicsLogMessage( 2, "abort - PRESET directive.");
         break;

      /* CAD CLEAR directive detected. Nothing needs to be done. */
      case CAD_CLEAR:
         cicsLogMessage( 2, "abort - CLEAR directive.");
         break;

      /* CAD START directive detected. An observation must be
	 in progress to perform an abort.
      */
      case CAD_START:
         cicsLogMessage( 2, "abort - START directive.");
	 if(carStatus(pCad->a) != CAR_IDLE)
	 {
	DPRINT(1,"abort cad\n");
	     semGive(semAbort);
	 }
	 else
	 {
	     status = CAD_REJECT;
	     strcpy(pCad->mess,"abort - no observe in progress");
	     cicsLogMessage(1, pCad->mess);
	 }         
	 break;

      /* CAD STOP directive detected.
      */
      case CAD_STOP:
         cicsLogMessage( 1, "abort - STOP directive. Cannot be stopped.");
         strcpy( pCad->mess, "Cannot be stopped");
         status = CAD_REJECT;
         break;

      /* Unrecognised CAD directive detected. This is regarded as an error. */
      default:
         strcpy( pCad->mess, "Unrecognized CAD directive" );
         status = CAD_REJECT;
         break;
   }
   return status;
}




/*
 *+
 * FUNCTION NAME:
 * observeProc 
 *
 * INVOCATION:
 * struct cadRecord *pCad;
 * status = observeProc( pCad );
 *
 * PARAMETERS: (">" input, "!" modified, "<" output)
 * ! pCad   (struct cadRecord *)   pointer to CAD data structure
 *
 * FUNCTION VALUE:
 * long  Status value written to CAD VAL field
 *
 * PURPOSE:
 * User defined function for "observe" CAD record
 *
 * DESCRIPTION:
 * This routine is called whenever the observe CAD record is processed.
 * observe is the command for starting observation.  The command has 
 *  two arguments:
 *	Data label (a) - label identifying the observation to the DHS.
 *                       This parameter should contain the data label
 *                       obtained by the OCS. If this parameter is blank
 *                       GNAAC will obtain its own data label.
 *	File Name  (b) - filename of where to store the data (used only
 *                       when the DHS is not available).
 *
 * EXTERNAL VARIABLES:
 * None
 *
 * PRIOR REQUIREMENTS:
 * It is assumed the CAD record has already been initialized and the
 * directive and any arguments have already been assembled into the cadRecord
 * data structure.
 *
 * DEFICIENCIES:
 * None
 *
 * HISTORY:
 * 17-Jan-1997  Original version adapted from CICS alpha 1.0   J.E. Tvedt
 * 19-Jun-1997  "Observation ID" --> "Data Label"              S.M. Beard
 *
 *-
 */


long observeProc( struct cadRecord *pCad ) 
{
   long   status;         /* return status */
   long   lVal;
   int    mjd[7];
   int    stat;
   double epoch;
   double mjd2;


   /* Initialise CAD status */
   status = CAD_ACCEPT;

   /* Switch according to the CAD directive in DIR field */
   switch (pCad->dir)
   {
      /* CAD MARK directive detected. Nothing needs to be done.*/
      case CAD_MARK:
         cicsLogMessage( 2, "observe - MARK directive.");
         break;

      /* CAD PRESET directive detected.  Assume input arguments are correct.
         Copy them to the output. 
      */
      case CAD_PRESET:
         cicsLogMessage( 2, "observe - PRESET directive.");
      
	 cicsLogString( 3, "DHS Data label    =", pCad->vala);
	 cicsLogString( 3, "Non-DHS File name =", pCad->valb);
	 printf(  "\nDHS Data label    =%s\n", (char *)pCad->vala);
	 printf( "Non-DHS File name =%s\n", (char *)pCad->valb);
         break;

      /* CAD CLEAR directive detected. Nothing needs to be done. */
      case CAD_CLEAR:
         cicsLogMessage( 2, "observe - CLEAR directive.");
         break;

      /* CAD START directive detected. Start an observation only
	 if one is not already in progress and if drRoiSet has
	 been done or is in progress (meaning that arSetup and 
	 obsSetup were also done.
      */
      case CAD_START:
         cicsLogMessage( 2, "observe - START directive.");
	 if((carStatus(pCad->c) == CAR_IDLE) || (carStatus(pCad->c) == CAR_ERROR))
	 {
	     if (carStatus(pCad->c) == CAR_ERROR)
	     {
		 /* clear the error and continue */
	     }
	     /* check to see if the setup routines have been completed */
	     lVal = naacStatus(dbTop,DRROISET_DONE);
	     if((lVal == NAAC_DONE) || (lVal == NAAC_BUSY))
	     {
		 if(status == CAD_ACCEPT)
		 {   strcpy(pCad->vala, pCad->a);
		     strcpy(pCad->valb, pCad->b);
                     stat = timeNowC(UTC,1, mjd);
                     sprintf(pCad->valc,"%02d:%02d:%02d.%d",mjd[3],mjd[4],mjd[5],mjd[6]);
                     sprintf(pCad->vald,"%d-%02d-%02d",mjd[0],mjd[1],mjd[2]);
                     /*printf("***UT = %s\n",pCad->valc);
                     printf("***DATE = %s\n",pCad->vald);*/
  
                     /* put in epics the epoch only at the start of exposure */

                      stat = timeNowD(UTC, &mjd2);
                      epoch =  slaEpj(mjd2);
                      *(double *) pCad->vale = epoch;

                     /* Clean dhs Label */
		     /*strcpy(pCad->a,""); */

		     semGive(semObserve);
		 }
		 else
		     status = CAD_REJECT;
	     }
	     else
	     {
		 status = CAD_REJECT;
		 strcpy(pCad->mess,"observe - Setup procedures not complete");
		 cicsLogMessage(1, pCad->mess);
	     }
	 }
	 else
	 {
	     status = CAD_REJECT;
	     printf("car status = %d, ERROR = %d, idle = %d\n",carStatus(pCad->c),CAR_ERROR,CAR_IDLE);
	     strcpy(pCad->mess,"observe - observe already in progress");
	     cicsLogMessage(1, pCad->mess);
	 }
         break;

      /* CAD STOP directive detected.
      */
      case CAD_STOP:
         cicsLogMessage( 1, "observe - STOP directive. Cannot be stopped.");
         strcpy( pCad->mess, "Cannot be stopped");
         status = CAD_REJECT;
         break;

      /* Unrecognised CAD directive detected. This is regarded as an error. */
      default:
         strcpy( pCad->mess, "Unrecognized CAD directive" );
         status = CAD_REJECT;
         break;
   }
   return status;
}




/*
 *+
 * FUNCTION NAME:
 * stopProc 
 *
 * INVOCATION:
 * struct cadRecord *pCad;
 * status = stopProc( pCad );
 *
 * PARAMETERS: (">" input, "!" modified, "<" output)
 * ! pCad   (struct cadRecord *)   pointer to CAD data structure
 *
 * FUNCTION VALUE:
 * long  Status value written to CAD VAL field
 *
 * PURPOSE:
 * User defined function for "stop" CAD record
 *
 * DESCRIPTION:
 * This routine is called whenever the stop CAD record is processed.
 * stop is the command for stopping the observation and keeping the data.  
 * The command has has two arguments:
 *	Data label (a) - ????
 *	File name (b) - ????
 *
 * EXTERNAL VARIABLES:
 * None
 *
 * PRIOR REQUIREMENTS:
 * It is assumed the CAD record has already been initialized and the
 * directive and any arguments have already been assembled into the cadRecord
 * data structure.
 *
 * DEFICIENCIES:
 * None
 *
 * HISTORY:
 * 17-Jan-1997  Original version adapted from CICS alpha 1.0   J.E. Tvedt
 * 19-Jun-1997  "Observation ID" --> "Data Label"              S.M. Beard
 *
 *-
 */

long stopProc( struct cadRecord *pCad ) 
{
   long status;         /* return status */

   /* Initialise CAD status */
   status = CAD_ACCEPT;

   /* Switch according to the CAD directive in DIR field */
   switch (pCad->dir)
   {
      /* CAD MARK directive detected. Nothing needs to be done.*/
      case CAD_MARK:
         cicsLogMessage( 2, "stop - MARK directive.");
         break;

      /* CAD PRESET directive detected.  Check the input argument. 
         If it is acceptable then copy it to the output. 
      */
      case CAD_PRESET: /*????*/
         cicsLogMessage( 2, "stop - PRESET directive.");
	 cicsLogString( 3, "New DHS Data label    =", pCad->vala);
	 cicsLogString( 3, "New Non-DHS File name =", pCad->valb);
         break;

      /* CAD CLEAR directive detected. Nothing needs to be done. */
      case CAD_CLEAR:
         cicsLogMessage( 2, "stop - CLEAR directive.");
         break;

      /* CAD START directive detected. Do nothing. The directive is being
         monitored by the sequence code, which will start the appropriate
         action. 
      */
      case CAD_START:
         cicsLogMessage( 2, "stop - START directive.");
	 if(carStatus(pCad->c) != CAR_IDLE)
	 {
	     strcpy(pCad->vala, pCad->a);
	     strcpy(pCad->valb, pCad->b);
	     semGive(semStop);
	 }
	 else
	 {
	     status = CAD_REJECT;
	     strcpy(pCad->mess,"stop - no observe in progress");
	     cicsLogMessage(1, pCad->mess);
	 }         
	 break;

      /* CAD STOP directive detected. 
      */
      case CAD_STOP:
         cicsLogMessage( 1, "stop - STOP directive. Cannot be stopped.");
         strcpy( pCad->mess, "Cannot be stopped");
         status = CAD_REJECT;
         break;

      /* Unrecognised CAD directive detected. This is regarded as an error. */
      default:
         strcpy( pCad->mess, "Unrecognized CAD directive" );
         status = CAD_REJECT;
         break;
   }
   return status;
}



/*
 *+
 * FUNCTION NAME:
 * pauseProc
 *
 * INVOCATION:
 * struct cadRecord *pCad;
 * status = pauseProc( pCad );
 *
 * PARAMETERS: (">" input, "!" modified, "<" output)
 * ! pCad   (struct cadRecord *)   pointer to CAD data structure
 *
 * FUNCTION VALUE:
 * long  Status value written to CAD VAL field
 *
 * PURPOSE:
 * User defined function for "pause" CAD record
 *
 * DESCRIPTION:
 * This routine is called whenever the pause CAD record is processed.
 * pause is the command to pause the observation temporarily.
 *
 * NOTE - This command is not currently supported and it will be rejected.
 *        Hooks have been included in the code to allow the command to be
 *        supported by a future version of GNAAC.
 *
 * The command has no arguments.
 *
 * EXTERNAL VARIABLES:
 * None
 *
 * PRIOR REQUIREMENTS:
 * It is assumed the CAD record has already been initialized and the
 * directive and any arguments have already been assembled into the cadRecord
 * data structure.
 *
 * DEFICIENCIES:
 * None
 *
 * HISTORY:
 * 17-Jan-1997  Original version adapted from CICS alpha 1.0   J.E. Tvedt
 * 10-Jun-1997  Modified to make a new "pauseProc" function.   S.M. Beard
 *
 *-
 */

long pauseProc( struct cadRecord *pCad ) 
{
   long status;         /* return status */

   /* Initialise CAD status */
   status = CAD_ACCEPT;

   /* Switch according to the CAD directive in DIR field */
   switch (pCad->dir)
   {
      /* CAD MARK directive detected. Nothing needs to be done.*/
      case CAD_MARK:
         cicsLogMessage( 2, "pause - MARK directive.");
         break;

      /* CAD PRESET directive detected.  Check the input argument. 
         If it is acceptable then copy it to the output. 
      */
      case CAD_PRESET:
         cicsLogMessage( 2, "pause - PRESET directive.");
         break;

      /* CAD CLEAR directive detected. Nothing needs to be done. */
      case CAD_CLEAR:
         cicsLogMessage( 2, "pause - CLEAR directive.");
         break;

      /* CAD START directive detected. An observation must be
	 in progress to perform an pause. This command is not currently
         supported and will be rejected.
      */
      case CAD_START:
         cicsLogMessage( 2, "pause - START directive. Not supported.");
	 if(carStatus(pCad->a) == CAR_BUSY)
	 {
	     status = CAD_REJECT;
	     strcpy(pCad->mess,"pause - command not supported");
	     cicsLogMessage(1, pCad->mess);

/* NOTE: If the pause command needs to be implemented, replace this "if"
 * branch with the following line, and add a suitable doPause() task to
 * receive the semaphore.
 *
 *           semGive(semPause); <-- Uncomment to implement pause command.
 */
	 }
	 else
	 {
	     status = CAD_REJECT;
	     strcpy(pCad->mess,"pause - no observe in progress");
	     cicsLogMessage(1, pCad->mess);
	 }         
	 break;

      /* CAD STOP directive detected.
      */
      case CAD_STOP:
         cicsLogMessage( 1, "pause - STOP directive. Cannot be stopped.");
         strcpy( pCad->mess, "Cannot be stopped");
         status = CAD_REJECT;
         break;

      /* Unrecognised CAD directive detected. This is regarded as an error. */
      default:
         strcpy( pCad->mess, "Unrecognized CAD directive" );
         status = CAD_REJECT;
         break;
   }
   return status;
}


/*
 *+
 * FUNCTION NAME:
 * continueProc
 *
 * INVOCATION:
 * struct cadRecord *pCad;
 * status = continueProc( pCad );
 *
 * PARAMETERS: (">" input, "!" modified, "<" output)
 * ! pCad   (struct cadRecord *)   pointer to CAD data structure
 *
 * FUNCTION VALUE:
 * long  Status value written to CAD VAL field
 *
 * PURPOSE:
 * User defined function for "continue" CAD record
 *
 * DESCRIPTION:
 * This routine is called whenever the continue CAD record is processed.
 * continue is the command to continue an observation that has been
 * temporarily paused.
 *
 * NOTE - This command is not currently supported and it will be rejected.
 *        Hooks have been included in the code to allow the command to be
 *        supported by a future version of GNAAC.
 *
 * The command has no arguments.
 *
 * EXTERNAL VARIABLES:
 * None
 *
 * PRIOR REQUIREMENTS:
 * It is assumed the CAD record has already been initialized and the
 * directive and any arguments have already been assembled into the cadRecord
 * data structure.
 *
 * DEFICIENCIES:
 * None
 *
 * HISTORY:
 * 17-Jan-1997  Original version adapted from CICS alpha 1.0     J.E. Tvedt
 * 10-Jun-1997  Modified to make a new "continueProc" function.  S.M. Beard
 *
 *-
 */

long continueProc( struct cadRecord *pCad ) 
{
   long status;         /* return status */

   /* Initialise CAD status */
   status = CAD_ACCEPT;

   /* Switch according to the CAD directive in DIR field */
   switch (pCad->dir)
   {
      /* CAD MARK directive detected. Nothing needs to be done.*/
      case CAD_MARK:
         cicsLogMessage( 2, "continue - MARK directive.");
         break;

      /* CAD PRESET directive detected.  Check the input argument. 
         If it is acceptable then copy it to the output. 
      */
      case CAD_PRESET:
         cicsLogMessage( 2, "continue - PRESET directive.");
         break;

      /* CAD CLEAR directive detected. Nothing needs to be done. */
      case CAD_CLEAR:
         cicsLogMessage( 2, "continue - CLEAR directive.");
         break;

      /* CAD START directive detected. An observation must be
	 paused to perform an continue.
      */
      case CAD_START:
         cicsLogMessage( 2, "continue - START directive. Not supported.");
	 if(carStatus(pCad->a) == CAR_PAUSED)
	 {
	     status = CAD_REJECT;
	     strcpy(pCad->mess,"continue - command not supported");
	     cicsLogMessage(1, pCad->mess);

/* NOTE: If the continue command needs to be implemented, replace this "if"
 * branch with the following line, and add a suitable doContinue() task to
 * receive the semaphore.
 *
 *           semGive(semContinue); <-- Uncomment to implement continue command.
 */
	 }
	 else
	 {
	     status = CAD_REJECT;
	     strcpy(pCad->mess,"continue - observe not paused");
	     cicsLogMessage(1, pCad->mess);
	 }         
	 break;

      /* CAD STOP directive detected.
      */
      case CAD_STOP:
         cicsLogMessage( 1, "continue - STOP directive. Cannot be stopped.");
         strcpy( pCad->mess, "Cannot be stopped");
         status = CAD_REJECT;
         break;

      /* Unrecognised CAD directive detected. This is regarded as an error. */
      default:
         strcpy( pCad->mess, "Unrecognized CAD directive" );
         status = CAD_REJECT;
         break;
   }
   return status;
}
