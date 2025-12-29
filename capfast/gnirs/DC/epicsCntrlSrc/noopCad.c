static struct {
        void *v;
        char *c;
} rcsid = {
        &rcsid,
        "$Id: noopCad.c,v 1.2 2009/05/27 19:32:21 fkraemer Exp $"
};
/*
 * Copyright 1997 Association of Universities for Research in Astronomy, Inc.
 * See the file COPYRIGHT for more details.
 *
 * FILENAME
 * noopCad.c
 *
 * DESCRIPTION
 * This file contains the source for all the functions used by the
 * NAAC noop CAD records. These functions are used to validate the 
 * arguments given to the CAD record.  Each function checks that the 
 * command is acceptable and returns a status. A message is supplied 
 * for rejected commands or error conditions.  The status and message 
 * is subsequently written to the VAL and MESS fields of the CAD record 
 * by the record support routines. 
 * 
 * FUNCTION NAME(S)
 * verifyProc       	- Begin verifying 
 * endVerifyProc        - Stop verifying
 * guideProc      	- Begin guiding
 * endGuideProc       	- Stop Guiding
 * datumProc      	- Datum
 * endObserve           - End of observation confirmation
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
 * $Log: noopCad.c,v $
 * Revision 1.2  2009/05/27 19:32:21  fkraemer
 * fkraemer - copied my complete working dir over trunk
 *
 * Revision 1.1.1.1  1998/12/15 16:18:51  buchholz
 * Imported gnaacSrc into CVS
 *
 *INDENT-ON* 
 */

#include "epCommon.h"
extern char *dbTop;
/*
 *+
 * FUNCTION NAME:
 * verifyProc
 *
 * INVOCATION:
 * struct cadRecord *pCad;
 * status = verifyProc( pCad );
 *
 * PARAMETERS: (">" input, "!" modified, "<" output)
 * ! pCad   (struct cadRecord *)   pointer to CAD data structure
 *
 * FUNCTION VALUE:
 * long  Status value written to CAD VAL field
 *
 * PURPOSE:
 * User defined function for "verify" CAD record
 *
 * DESCRIPTION:
 * This routine is called whenever the verify CAD record is processed.
 * verify is the command to begin verifying.  
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
 * 09-Jun-1997  Do not reject the "pause" and "continue"
 *              commands (I suspect this has been causing
 *              the APPLY "memory" problem).                   S.M. Beard
 *
 *-
 */

long verifyProc( struct cadRecord *pCad ) 
{
    long status;         /* return status */

    /* Initialise CAD status */
    status = CAD_ACCEPT;

    /* Switch according to the CAD directive in DIR field */
    switch (pCad->dir)
    {
	/* CAD MARK directive detected. Nothing needs to be done.*/
      case CAD_MARK:
	  cicsLogMessage( 2, "verify - MARK directive.");
	  break;

	  /* CAD PRESET directive detected.  Check the input argument. 
	     If it is acceptable then copy it to the output. 
	  */
      case CAD_PRESET:
	  cicsLogMessage( 2, "verify - PRESET directive.");
	  break;

	  /* CAD CLEAR directive detected. Nothing needs to be done. */
      case CAD_CLEAR:
	  cicsLogMessage( 2, "verify - CLEAR directive.");
	  break;

	  /* CAD START directive detected. Process the associated sequencer
	     record to toggle the CAR record to BUSY and back to idle.
	  */
      case CAD_START:
	  cicsLogMessage( 2, "verify - START directive.");
	  processRec(dbTop,"verifySeq");
	  break;

	  /* CAD STOP directive detected. 
	   */
      case CAD_STOP:
	  cicsLogMessage( 2, "verify - STOP directive.");
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
 * endVerifyProc
 *
 * INVOCATION:
 * struct cadRecord *pCad;
 * status = endVerifyProc( pCad );
 *
 * PARAMETERS: (">" input, "!" modified, "<" output)
 * ! pCad   (struct cadRecord *)   pointer to CAD data structure
 *
 * FUNCTION VALUE:
 * long  Status value written to CAD VAL field
 *
 * PURPOSE:
 * User defined function for "endVerify" CAD record
 *
 * DESCRIPTION:
 * This routine is called whenever the endVerify CAD record is processed.
 * endVerify is the command to stop verifying (this command is ignored).  
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

long endVerifyProc( struct cadRecord *pCad ) 
{
    long status;         /* return status */

    /* Initialise CAD status */
    status = CAD_ACCEPT;

    /* Switch according to the CAD directive in DIR field */
    switch (pCad->dir)
    {
	/* CAD MARK directive detected. Nothing needs to be done.*/
      case CAD_MARK:
	  cicsLogMessage( 2, "endVerify - MARK directive.");
	  break;

	  /* CAD PRESET directive detected.  Check the input argument. 
	     If it is acceptable then copy it to the output. 
	  */
      case CAD_PRESET:
	  cicsLogMessage( 2, "endVerify - PRESET directive.");
	  break;

	  /* CAD CLEAR directive detected. Nothing needs to be done. */
      case CAD_CLEAR:
	  cicsLogMessage( 2, "endVerify - CLEAR directive.");
	  break;

	  /* CAD START directive detected. Process the associated sequencer
	     record to toggle the CAR record to BUSY and back to idle.
	  */
      case CAD_START:
	  cicsLogMessage( 2, "endVerify - START directive.");
	  processRec(dbTop,"endVerifySeq");
	  break;

    
      case CAD_STOP:
	  cicsLogMessage( 2, "endVerify - STOP directive.");
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
 * guideProc
 *
 * INVOCATION:
 * struct cadRecord *pCad;
 * status = guideProc( pCad );
 *
 * PARAMETERS: (">" input, "!" modified, "<" output)
 * ! pCad   (struct cadRecord *)   pointer to CAD data structure
 *
 * FUNCTION VALUE:
 * long  Status value written to CAD VAL field
 *
 * PURPOSE:
 * User defined function for "guide" CAD record
 *
 * DESCRIPTION:
 * This routine is called whenever the guide CAD record is processed.
 * guide is the command to begin guiding (this command is ignored.  
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

long guideProc( struct cadRecord *pCad ) 
{
    long status;         /* return status */

    /* Initialise CAD status */
    status = CAD_ACCEPT;

    /* Switch according to the CAD directive in DIR field */
    switch (pCad->dir)
    {
	/* CAD MARK directive detected. Nothing needs to be done.*/
      case CAD_MARK:
	  cicsLogMessage( 2, "guide - MARK directive.");
	  break;

	  /* CAD PRESET directive detected.  Check the input argument. 
	     If it is acceptable then copy it to the output. 
	  */
      case CAD_PRESET:
	  cicsLogMessage( 2, "guide - PRESET directive.");
	  break;

	  /* CAD CLEAR directive detected. Nothing needs to be done. */
      case CAD_CLEAR:
	  cicsLogMessage( 2, "guide - CLEAR directive.");
	  break;

	  /* CAD START directive detected. Process the associated sequencer
	     record to toggle the CAR record to BUSY and back to idle.
	  */
      case CAD_START:
	  cicsLogMessage( 2, "guide - START directive.");
	  processRec(dbTop,"guideSeq");
	  break;

	  /* CAD STOP directive detected.     */
      case CAD_STOP:
	  cicsLogMessage( 2, "guide - STOP directive.");
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
 * endGuideProc
 *
 * INVOCATION:
 * struct cadRecord *pCad;
 * status = endGuideProc( pCad );
 *
 * PARAMETERS: (">" input, "!" modified, "<" output)
 * ! pCad   (struct cadRecord *)   pointer to CAD data structure
 *
 * FUNCTION VALUE:
 * long  Status value written to CAD VAL field
 *
 * PURPOSE:
 * User defined function for "endGuide" CAD record
 *
 * DESCRIPTION:
 * This routine is called whenever the endGuide CAD record is processed.
 * endGuide is the command stop guiding (this command is ignored).
 *  The command has no arguments.
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

long endGuideProc( struct cadRecord *pCad ) 
{
    long status;         /* return status */

    /* Initialise CAD status */
    status = CAD_ACCEPT;

    /* Switch according to the CAD directive in DIR field */
    switch (pCad->dir)
    {
	/* CAD MARK directive detected. Nothing needs to be done.*/
      case CAD_MARK:
	  cicsLogMessage( 2, "endGuide - MARK directive.");
	  break;

	  /* CAD PRESET directive detected.  Check the input argument. 
	     If it is acceptable then copy it to the output. 
	  */
      case CAD_PRESET:
	  cicsLogMessage( 2, "endGuide - PRESET directive.");
	  break;

	  /* CAD CLEAR directive detected. Nothing needs to be done. */
      case CAD_CLEAR:
	  cicsLogMessage( 2, "endGuide - CLEAR directive.");
	  break;

	  /* CAD START directive detected. Process the associated sequencer
	     record to toggle the CAR record to BUSY and back to idle.
	  */
      case CAD_START:
	  cicsLogMessage( 2, "endGuide - START directive.");
	  processRec(dbTop,"endGuideSeq");
	  break;

	  /* CAD STOP directive detected.      */
      case CAD_STOP:
	  cicsLogMessage( 2, "endGuide - STOP directive.");
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
 * datumProc
 *
 * INVOCATION:
 * struct cadRecord *pCad;
 * status = datumProc( pCad );
 *
 * PARAMETERS: (">" input, "!" modified, "<" output)
 * ! pCad   (struct cadRecord *)   pointer to CAD data structure
 *
 * FUNCTION VALUE:
 * long  Status value written to CAD VAL field
 *
 * PURPOSE:
 * User defined function for "datum" CAD record
 *
 * DESCRIPTION:
 * This routine is called whenever the datum CAD record is processed.
 * datum is the command for reinitializing the subsystem.  
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

long datumProc( struct cadRecord *pCad ) 
{
    long status;         /* return status */

    /* Initialise CAD status */
    status = CAD_ACCEPT;

    /* Switch according to the CAD directive in DIR field */
    switch (pCad->dir)
    {
	/* CAD MARK directive detected. Nothing needs to be done.*/
      case CAD_MARK:
	  cicsLogMessage( 2, "datum - MARK directive.");
	  break;

	  /* CAD PRESET directive detected.  Check the input argument. 
	     If it is acceptable then copy it to the output. 
	  */
      case CAD_PRESET:
	  cicsLogMessage( 2, "datum - PRESET directive.");
	  break;

	  /* CAD CLEAR directive detected. Nothing needs to be done. */
      case CAD_CLEAR:
	  cicsLogMessage( 2, "datum - CLEAR directive.");
	  break;

	  /* CAD START directive detected. Process the associated sequencer
	     record to toggle the CAR record to BUSY and back to idle.
	  */
      case CAD_START:
	  cicsLogMessage( 2, "datum - START directive.");
	  processRec(dbTop,"datumSeq");
	  break;

	  /* CAD STOP directive detected. 
	   */
      case CAD_STOP:
	  cicsLogMessage( 2, "datum - STOP directive.");
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
 * endObserveProc
 *
 * INVOCATION:
 * struct cadRecord *pCad;
 * status = endObserveProc( pCad );
 *
 * PARAMETERS: (">" input, "!" modified, "<" output)
 * ! pCad   (struct cadRecord *)   pointer to CAD data structure
 *
 * FUNCTION VALUE:
 * long  Status value written to CAD VAL field
 *
 * PURPOSE:
 * User defined function for "endObserve" CAD record
 *
 * DESCRIPTION:
 * This routine is called whenever the endObserve CAD record is processed.
 * endObserve is the command for acknowledging the end of observation.  
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
 * 10-Jun-1997  Copied to make "endObserveProc"                S.M. Beard
 *
 *-
 */

long endObserveProc( struct cadRecord *pCad ) 
{
    long status;         /* return status */

    /* Initialise CAD status */
    status = CAD_ACCEPT;

    /* Switch according to the CAD directive in DIR field */
    switch (pCad->dir)
    {
	/* CAD MARK directive detected. Nothing needs to be done.*/
      case CAD_MARK:
	  cicsLogMessage( 2, "endObserve - MARK directive.");
	  break;

	  /* CAD PRESET directive detected.  Check the input argument. 
	     If it is acceptable then copy it to the output. 
	  */
      case CAD_PRESET:
	  cicsLogMessage( 2, "endObserve - PRESET directive.");
	  break;

	  /* CAD CLEAR directive detected. Nothing needs to be done. */
      case CAD_CLEAR:
	  cicsLogMessage( 2, "endObserve - CLEAR directive.");
	  break;

	  /* CAD START directive detected. Process the associated sequencer
	     record to toggle the CAR record to BUSY and back to idle.
	  */
      case CAD_START:
	  cicsLogMessage( 2, "endObserve - START directive.");
	  processRec(dbTop,"endObserveSeq");
	  break;

	  /* CAD STOP directive detected. 
	   */
      case CAD_STOP:
	  cicsLogMessage( 2, "endObserve - STOP directive.");
	  break;

	  /* Unrecognised CAD directive detected. This is regarded as an error. */
      default:
	  strcpy( pCad->mess, "Unrecognized CAD directive" );
	  status = CAD_REJECT;
	  break;
    }
    return status;
}
