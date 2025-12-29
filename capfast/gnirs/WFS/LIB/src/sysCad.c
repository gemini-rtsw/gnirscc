static char rcsid[]="$Id: sysCad.c,v 1.2 2009/05/27 19:34:57 fkraemer Exp $";

/*
 *   FILENAME
 *   -------- 
 *   syscad.c
 *
 *   PURPOSE
 *   -------
 *   This file contains the source for all the functions used by the
 *   CICS SYSTEM CAD records. These functions are used to validate the arguments
 *   given to the CAD record
 *
 *
 *   FUNCTION NAME(S)
 *   ----------------
 *   CADinit        - Initialise
 *   CADdatum       - Datum
 *   CADpark        - Park
 *   CADobserve     - Observe
 *   CADpause       - Pause
 *   CADcontinue    - Continue
 *   CADstop        - Stop
 *   CADabort       - Abort
 *   CADendObserve  - End Observe
 *   CADdebugInit   - Initialise debug
 *   CADdebug       - Debug
 *   CADreboot      - Reboot
 *
 *   DEPENDENCIES
 *   ------------
 *   The names of the subroutines in this file should be identical to those
 *   declared in the SNAM field of each CAD record. If a change is made to
 *   the name of a subroutine, that change should be reflected in the SNAM
 *   field, and vice versa.
 *
 *   The ordering of the arguments within each CAD record (A, B, C...) is
 *   defined in the description of the interface between the CAD database
 *   and its clients. Changes to that interface should be reflected in this
 *   file.
 *
 *   The input arguments for each CAD record (A, B, C...) are all strings.
 *   However, the data type of each output argument (VALA, VALB, VALC...)
 *   is determined by the (FTVA, FTVB, FTVC....) fields in the CAD record.
 *   The data types assumed here must match those declared in the CAD
 *   record properties
 *
 *   LIMITATIONS
 *   ------------
 *
 *   AUTHOR
 *   ------
 *   Steven Beard  (smb@roe.ac.uk)
 *
 *   HISTORY
 *   -------
 *INDENT-OFF*
 *
 * $Log: sysCad.c,v $
 * Revision 1.2  2009/05/27 19:34:57  fkraemer
 * fkraemer - copied my complete working dir over trunk
 *
 * Revision 1.5  1999/11/14 02:05:43  yamada
 * Reformatted logs.
 *
 *
 *INDENT-ON*
 *   28-Mar-1996: Original version with just CADfilterSelect.        (smb)
 *   16-Apr-1996: Add CADfilterEngMove, CADfilterDatum and
 *                CADfilterPark.                                     (smb)
 *   16-Apr-1996: CADfilterEngMove renamed due to 15 character
 *                subroutine name length restriction in the
 *                CAD record.                                        (smb)
 *   03-May-1996: Modified to use CAD_ACCEPT/REJECT constants.       (smb)
 *   07-May-1996: All subroutine names shortened to reflect shorter
 *                names for CAD records. (I was hitting the 29
 *                character EPICS record name length limit as well
 *                as the 15 character subroutine name limit).        (smb)
 *   15-May-1996: Documentation tidied up.                           (smb)
 *   06-Jun-1996: Headers converted to Gemini standard format, as
 *                described in SPE-C-G0009. Functions CADinit and
 *                CADreset added.                                    (smb)
 *   12-Jun-1996: EPICS 3.12.2.Gem3 installed, plus latest IGPO
 *                extensions. CAD records now have a "MARK"
 *                directive.                                         (smb)
 *   19-Jun-1996: Headers modified to make them compatible with
 *                the "wflman" utility.                              (smb)
 *   25-Jun-1996: CADfiltSel and CADfiltPark modified to generate
 *                engineering position as output (in preparation
 *                for the use of lookup tables later).               (smb)
 *   11-Jul-1996: CAD records are not allowed to report anything
 *                other than reasons for failure through the MESS
 *                field. Diagnostic messages removed.                (smb)
 *   02-Aug-1996: Modified to make use of "lutin" and "lutout"
 *                records, so the CAD records no longer need to
 *                process lookup tables themselves.                  (smb)
 *   11-Oct-1996: Split into "sysCad" and "filtCad". "filtCad" will
 *                eventually be made more generic.                   (smb)
 *   04-Dec-1996: "printf" replaced by "cicsLogMessage" or
 *                "cicsLogString".                                   (smb)
 *   05-Dec-1996: Components Controller and Detector Controller
 *                versions merged and moved to general CICS library,
 *                since both may need to use the same sequence
 *                commands.                                          (smb)
 *   13-Feb-1997: Start removing the "reset" command. Add "datum"
 *                and "park".                                        (smb)
 *   21-Mar-1997: "reboot" and "endObserve" added. Data label added
 *                to "observe".                                      (smb)
 *   16-Apr-1997: Second input argument of CAD records used to
 *                sense whether the instrument is busy and the
 *                sequence command should be rejected.               (smb)
 */

#include  <vxWorks.h>
#include  <types.h>
#include  <math.h>
#include  <time.h>
#include  <stdlib.h>
#include  <stdioLib.h>
#include  <string.h>

#include  <dbDefs.h>
#include  <cadRecord.h>
#include  <dbCommon.h>
#include  <recSup.h>
#include  <cad.h>

#include  <cicsConst.h>
#include  <cicsLib.h>

long checkBusy( struct cadRecord * );

/* ===================================================================== */

/*+
 *   Function name:
 *   CADinit
 *
 *   Purpose:
 *   User defined function for "init" CAD record
 *
 *   Purpose:
 *   This routine is called whenever the init CAD record is processed.
 *   It checks that the command is acceptable and returns a status and a
 *   message, which are written to the VAL and MESS fields of the CAD record.
 *
 *   init is the command for reinitializing the subsystem.
 *   The command has no arguments
 *
 *   Invocation:
 *   struct cadRecord *pcad;
 *   status = CADinit( pcad );
 *
 *   Parameters in:
 *      > pcad->dir   *string     CAD directive
 *      > pcad->b     long        Instrument busy flag. The command will be
 *                                rejected if this flag happens to be exactly
 *                                equal to CICS_BUSY.
 *
 *   Parameters out:
 *      < pcad->mess  *string     failure message written to CAD MESS field
 *
 *   Return value:
 *      < status      long        Status value written to CAD VAL field
 *
 *   Globals:
 *      External functions:
 *      None
 *
 *      External variables:
 *      None
 *
 *   Requirements:
 *   It is assumed the CAD record has already been initialized and the
 *   directive and any arguments have already been assembled into the pcad
 *   structure
 *
 *   Author:
 *   Steven Beard  (smb@roe.ac.uk)
 *
 *   History:
 *   06-Jun-1996: Original version.                        (smb)
 *   16-Apr-1997: Check on instrument being busy added.    (smb)
 *-
 */

long CADinit( struct cadRecord *pcad ) 
{
  long status;         /* return status */

/* Initialise CAD status */

  status = CAD_ACCEPT;

/* Switch according to the CAD directive in DIR field */

  switch (pcad->dir)
  {
    case CAD_MARK:

/* CAD MARK directive detected. Nothing needs to be done.
 */

      cicsLogMessage( CICS_DB_MIN, "init - MARK directive.");
      break;

    case CAD_PRESET:

/* CAD PRESET directive detected. There are no command-specific arguments
 * to be checked.
 */

      cicsLogMessage( CICS_DB_MIN, "init - PRESET directive.");

/*
 * Reject the command if the second argument indicates the instrument is
 * busy.
 */

      status = checkBusy( pcad );
      break;

    case CAD_CLEAR:

/* CAD CLEAR directive detected. */

      cicsLogMessage( CICS_DB_MIN, "init - CLEAR directive.");
      break;

    case CAD_START:

/* CAD START directive detected. Do nothing. The directive is being
 * monitored by the CICS sequence code, which will start the appropriate
 * action.
 */

      cicsLogMessage( CICS_DB_MIN, "init - START directive.");
      break;

    case CAD_STOP:

/* CAD STOP directive detected. It is not possible to stop initialization
 * once it has started, so reject the directive.
 */
      cicsLogMessage( CICS_DB_LOG, "init - STOP directive. Cannot be stopped.");

      strncpy( pcad->mess, "init - Cannot be stopped", MAX_STRING_SIZE );
      status = CAD_REJECT;
      break;

    default:

/* Unrecognised CAD directive detected. This is regarded as an error. */

      strncpy( pcad->mess, "init - Unrecognised CAD directive",
        MAX_STRING_SIZE );
      status = CAD_REJECT;
      break;

  }
  return status;
}

/* ===================================================================== */

/*+
 *   Function name:
 *   CADdatum
 *
 *   Purpose:
 *   This routine is called whenever the datum CAD record is
 *   processed. It checks that the command
 *   is acceptable and returns a status and a message, which are written
 *   to the VAL and MESS fields of the CAD record.
 *
 *   datum is the command for datuming the subsystem (i.e. making all the
 *   components locate their reference or index points).
 *   The command has no arguments.
 *
 *   Invocation:
 *   struct cadRecord *pcad;
 *   status = CADdatum( pcad );
 *
 *   Parameters in:
 *      > pcad->dir   *string     CAD directive
 *      > pcad->b     long        Instrument busy flag. The command will be
 *                                rejected if this flag happens to be exactly
 *                                equal to CICS_BUSY.
 *
 *   Parameters out:
 *      < pcad->mess  *string     failure message written to CAD MESS field
 *
 *   Return value:
 *      < status      long        Status value written to CAD VAL field
 *
 *   Globals:
 *      External functions:
 *      None
 *
 *      External variables:
 *      None
 *
 *   Requirements:
 *   It is assumed the CAD record has already been initialized and the
 *   directive and any arguments have already been assembled into the pcad
 *   structure
 *
 *   Author:
 *   Steven Beard  (smb@roe.ac.uk)
 *
 *   History:
 *   13-Feb-1997: Original version.                        (smb)
 *   16-Apr-1997: Check on instrument being busy added.    (smb)
 *-
 */

long CADdatum( struct cadRecord *pcad ) 
{
  long status;         /* return status */

/* Initialise CAD status */

  status = CAD_ACCEPT;

/* Switch according to the CAD directive in DIR field */

  switch (pcad->dir)
  {
    case CAD_MARK:

/* CAD MARK directive detected. Nothing needs to be done.
 */

      cicsLogMessage( CICS_DB_MIN, "datum - MARK directive.");
      break;

    case CAD_PRESET:

/* CAD PRESET directive detected. There are no command-specific arguments
 * to be checked.
 */

      cicsLogMessage( CICS_DB_MIN, "datum - PRESET directive.");

/*
 * Reject the command if the second argument indicates the instrument is
 * busy.
 */

      status = checkBusy( pcad );
      break;

    case CAD_CLEAR:

/* CAD CLEAR directive detected */

      cicsLogMessage( CICS_DB_MIN, "datum - CLEAR directive.");
      break;

    case CAD_START:

/* CAD START directive detected. Do nothing. The directive is being
 * monitored by the CICS sequence code, which will start the appropriate
 * action.
 */

      cicsLogMessage( CICS_DB_MIN, "datum - START directive.");
      break;

    case CAD_STOP:

/* CAD STOP directive detected. It is not possible to stop a datum
 * once it has started, so reject the directive.
 */
      cicsLogMessage( CICS_DB_LOG,
        "datum - STOP directive. Cannot be stopped.");

      strncpy( pcad->mess, "datum - Cannot be stopped",  MAX_STRING_SIZE );
      status = CAD_REJECT;
      break;

    default:

/* Unrecognised CAD directive detected. This is regarded as an error. */

      strncpy( pcad->mess, "datum - Unrecognised CAD directive",
        MAX_STRING_SIZE );
      status = CAD_REJECT;
      break;

  }
  return status;
}

/* ===================================================================== */

/*+
 *   Function name:
 *   CADpark
 *
 *   Purpose:
 *   This routine is called whenever the park CAD record is
 *   processed. It checks that the command
 *   is acceptable and returns a status and a message, which are written
 *   to the VAL and MESS fields of the CAD record.
 *
 *   park is the command for parking all the components in the subsystem
 *   (i.e. moving them to a position where the instrument can be safely
 *   shut down).
 *   The command has no arguments.
 *
 *   Invocation:
 *   struct cadRecord *pcad;
 *   status = CADpark( pcad );
 *
 *   Parameters in:
 *      > pcad->dir   *string     CAD directive
 *      > pcad->b     long        Instrument busy flag. The command will be
 *                                rejected if this flag happens to be exactly
 *                                equal to CICS_BUSY.
 *
 *   Parameters out:
 *      < pcad->mess  *string     failure message written to CAD MESS field
 *
 *   Return value:
 *      < status      long        Status value written to CAD VAL field
 *
 *   Globals:
 *      External functions:
 *      None
 *
 *      External variables:
 *      None
 *
 *   Requirements:
 *   It is assumed the CAD record has already been initialized and the
 *   directive and any arguments have already been assembled into the pcad
 *   structure
 *
 *   Author:
 *   Steven Beard  (smb@roe.ac.uk)
 *
 *   History:
 *   13-Feb-1997: Original version.                        (smb)
 *   16-Apr-1997: Check on instrument being busy added.    (smb)
 *-
 */

long CADpark( struct cadRecord *pcad ) 
{
  long status;         /* return status */

/* Initialise CAD status */

  status = CAD_ACCEPT;

/* Switch according to the CAD directive in DIR field */

  switch (pcad->dir)
  {
    case CAD_MARK:

/* CAD MARK directive detected. Nothing needs to be done.
 */

      cicsLogMessage( CICS_DB_MIN, "park - MARK directive.");
      break;

    case CAD_PRESET:

/* CAD PRESET directive detected. There are no command-specific arguments
 * to be checked.
 */

      cicsLogMessage( CICS_DB_MIN, "park - PRESET directive.");

/*
 * Reject the command if the second argument indicates the instrument is
 * busy.
 */

      status = checkBusy( pcad );
      break;

    case CAD_CLEAR:

/* CAD CLEAR directive detected */

      cicsLogMessage( CICS_DB_MIN, "park - CLEAR directive.");
      break;

    case CAD_START:

/* CAD START directive detected. Do nothing. The directive is being
 * monitored by the CICS sequence code, which will start the appropriate
 * action.
 */

      cicsLogMessage( CICS_DB_MIN, "park - START directive.");
      break;

    case CAD_STOP:

/* CAD STOP directive detected.
 */
      cicsLogMessage( CICS_DB_LOG, "park - STOP directive.");
      break;

    default:

/* Unrecognised CAD directive detected. This is regarded as an error. */

      strncpy( pcad->mess, "park - Unrecognised CAD directive",
        MAX_STRING_SIZE );
      status = CAD_REJECT;
      break;

  }
  return status;
}


/* ===================================================================== */

#if 0
/*+
 *   Function name:
 *   CADobserve
 *
 *   Purpose:
 *   User defined function for "observe" CAD record
 *
 *   Purpose:
 *   This routine is called whenever the observe CAD record is processed.
 *   It checks that the command is acceptable and returns a status and a
 *   message, which are written to the VAL and MESS fields of the CAD record.
 *
 *   observe is the command for starting an observation.
 *   The command has one argument - the data label.
 *
 *   Invocation:
 *   struct cadRecord *pcad;
 *   status = CADobserve( pcad );
 *
 *   Parameters in:
 *      > pcad->dir   *string     CAD directive
 *      > pcad->a     *string     CAD input argument A
 *                                = data label
 *      > pcad->b     long        Instrument busy flag. The command will be
 *                                rejected if this flag happens to be exactly
 *                                equal to CICS_BUSY.
 *
 *   Parameters out:
 *      < pcad->mess  *string     failure message written to CAD MESS field
 *      < pcad->vala  *string     CAD output value A
 *                                = data label
 *
 *   Return value:
 *      < status      long        Status value written to CAD VAL field
 *
 *   Globals:
 *      External functions:
 *      None
 *
 *      External variables:
 *      None
 *
 *   Requirements:
 *   It is assumed the CAD record has already been initialized and the
 *   directive and any arguments have already been assembled into the pcad
 *   structure
 *
 *   Author:
 *   Steven Beard  (smb@roe.ac.uk)
 *
 *   History:
 *   11-Nov-1996: Original version.                              (smb)
 *   21-Mar-1997: Data label and "applyC" arguments added.       (smb)
 *   16-Apr-1997: Check on instrument being busy added.    (smb)
 *-
 */

long CADobserve( struct cadRecord *pcad ) 
{
  long status;         /* return status */

/* Initialise CAD status */

  status = CAD_ACCEPT;

/* Switch according to the CAD directive in DIR field */

  switch (pcad->dir)
  {
    case CAD_MARK:

/* CAD MARK directive detected. Nothing needs to be done.
 */

      cicsLogMessage( CICS_DB_MIN, "observe - MARK directive.");
      break;

    case CAD_PRESET:

/* CAD PRESET directive detected. The data label is assumed valid.
 * IS THERE A WAY OF CHECKING THE DATA LABEL?
 */

      cicsLogMessage( CICS_DB_MIN, "observe - PRESET directive.");

      cicsLogString( CICS_DB_FULL, "Data label = ", pcad->a);

/*
 * Reject the command if the second argument indicates the instrument is
 * busy.
 */

      status = checkBusy( pcad );

/*
 * Only copy the data label to the output if the command has not been
 * rejected
 */

      if ( status != CAD_REJECT ) strcpy( pcad->vala, pcad->a );
      break;

    case CAD_CLEAR:

/* CAD CLEAR directive detected. */

      cicsLogMessage( CICS_DB_MIN, "observe - CLEAR directive.");
      break;

    case CAD_START:

/* CAD START directive detected. Do nothing. The directive is being
 * monitored by the CICS DC sequence code, which will start the appropriate
 * action.
 */

      cicsLogMessage( CICS_DB_MIN, "observe - START directive.");
      break;

    case CAD_STOP:

/* CAD STOP directive detected. The normal way to stop an observation is
 * through the STOP sequence command, so reject this directive.
 */
      cicsLogMessage( CICS_DB_ERROR,
        "observe - STOP directive. Use STOP command instead.");

      strncpy( pcad->mess, "observe - Use STOP command instead.",
        MAX_STRING_SIZE );
      status = CAD_REJECT;
      break;

    default:

/* Unrecognised CAD directive detected. This is regarded as an error. */

      strncpy( pcad->mess, "observe - Unrecognised CAD directive",
        MAX_STRING_SIZE );
      status = CAD_REJECT;
      break;

  }
  return status;
}
#endif

/* ===================================================================== */

/*+
 *   Function name:
 *   CADpause
 *
 *   Purpose:
 *   User defined function for "pause" CAD record
 *
 *   Purpose:
 *   This routine is called whenever the pause CAD record is processed.
 *   It checks that the command is acceptable and returns a status and a
 *   message, which are written to the VAL and MESS fields of the CAD record.
 *
 *   pause is the command for pausing an observation.
 *   The command has no arguments
 *
 *   Invocation:
 *   struct cadRecord *pcad;
 *   status = CADpause( pcad );
 *
 *   Parameters in:
 *      > pcad->dir   *string     CAD directive
 *
 *   Parameters out:
 *      < pcad->mess  *string     failure message written to CAD MESS field
 *
 *   Return value:
 *      < status      long        Status value written to CAD VAL field
 *
 *   Globals:
 *      External functions:
 *      None
 *
 *      External variables:
 *      None
 *
 *   Requirements:
 *   It is assumed the CAD record has already been initialized and the
 *   directive and any arguments have already been assembled into the pcad
 *   structure
 *
 *   Author:
 *   Steven Beard  (smb@roe.ac.uk)
 *
 *   History:
 *   11-Nov-1996: Original version.                        (smb)
 *-
 */

long CADpause( struct cadRecord *pcad ) 
{
  long status;         /* return status */

/* Initialise CAD status */

  status = CAD_ACCEPT;

/* Switch according to the CAD directive in DIR field */

  switch (pcad->dir)
  {
    case CAD_MARK:

/* CAD MARK directive detected. Nothing needs to be done.
 */

      cicsLogMessage( CICS_DB_MIN, "pause - MARK directive.");
      break;

    case CAD_PRESET:

/* CAD PRESET directive detected. There are no command arguments
   to be checked.
 */

      cicsLogMessage( CICS_DB_MIN, "pause - PRESET directive.");
      break;

    case CAD_CLEAR:

/* CAD CLEAR directive detected. */

      cicsLogMessage( CICS_DB_MIN, "pause - CLEAR directive.");
      break;

    case CAD_START:

/* CAD START directive detected. Do nothing. The directive is being
 * monitored by the CICS DC sequence code, which will start the appropriate
 * action.
 *
 * NOTE: This directive should be rejected if an observation is not taking
 * place. This will involve checking the "observeC" CAR record at this point.
 * I need to find out how to do this.
 *
 */

      cicsLogMessage( CICS_DB_MIN, "pause - START directive.");
      break;

    case CAD_STOP:

/* CAD STOP directive detected. The normal way to restart a paused observation
 * is through the CONTINUE sequence command, so reject this directive.
 */
      cicsLogMessage( CICS_DB_ERROR,
        "pause - STOP directive. Use CONTINUE command instead.");

      strncpy( pcad->mess, "pause - Use CONTINUE command instead.",
        MAX_STRING_SIZE );
      status = CAD_REJECT;
      break;

    default:

/* Unrecognised CAD directive detected. This is regarded as an error. */

      strncpy( pcad->mess, "pause - Unrecognised CAD directive",
        MAX_STRING_SIZE );
      status = CAD_REJECT;
      break;

  }
  return status;
}

/* ===================================================================== */

/*+
 *   Function name:
 *   CADcontinue
 *
 *   Purpose:
 *   User defined function for "continue" CAD record
 *
 *   Purpose:
 *   This routine is called whenever the continue CAD record is processed.
 *   It checks that the command is acceptable and returns a status and a
 *   message, which are written to the VAL and MESS fields of the CAD record.
 *
 *   continue is the command for continuing a paused observation.
 *   The command has no arguments
 *
 *   Invocation:
 *   struct cadRecord *pcad;
 *   status = CADcontinue( pcad );
 *
 *   Parameters in:
 *      > pcad->dir   *string     CAD directive
 *      > pcad->b     long        Instrument busy flag. The command will be
 *                                rejected if this flag happens to be exactly
 *                                equal to CICS_BUSY.
 * 
 *   Parameters out:
 *      < pcad->mess  *string     failure message written to CAD MESS field
 *
 *   Return value:
 *      < status      long        Status value written to CAD VAL field
 *
 *   Globals:
 *      External functions:
 *      None
 *
 *      External variables:
 *      None
 *
 *   Requirements:
 *   It is assumed the CAD record has already been initialized and the
 *   directive and any arguments have already been assembled into the pcad
 *   structure
 *
 *   Author:
 *   Steven Beard  (smb@roe.ac.uk)
 *
 *   History:
 *   11-Nov-1996: Original version.                        (smb)
 *   16-Apr-1997: Check on instrument being busy added.    (smb)
 *-
 */

long CADcontinue( struct cadRecord *pcad ) 
{
  long status;         /* return status */

/* Initialise CAD status */

  status = CAD_ACCEPT;

/* Switch according to the CAD directive in DIR field */

  switch (pcad->dir)
  {
    case CAD_MARK:

/* CAD MARK directive detected. Nothing needs to be done.
 */

      cicsLogMessage( CICS_DB_MIN, "continue - MARK directive.");
      break;

    case CAD_PRESET:

/* CAD PRESET directive detected. There are no command-specific arguments
 * to be checked.
 */

      cicsLogMessage( CICS_DB_MIN, "continue - PRESET directive.");

/*
 * Reject the command if the second argument indicates the instrument is
 * busy.
 */

      status = checkBusy( pcad );
      break;

    case CAD_CLEAR:

/* CAD CLEAR directive detected. */

      cicsLogMessage( CICS_DB_MIN, "continue - CLEAR directive.");
      break;

    case CAD_START:

/* CAD START directive detected. Do nothing. The directive is being
 * monitored by the CICS DC sequence code, which will start the appropriate
 * action.
 *
 * NOTE: This directive should be rejected if an observation is not paused.
 * This will involve checking the "observeC" CAR record at this point.
 * I need to find out how to do this.
 *
 */

      cicsLogMessage( CICS_DB_MIN, "continue - START directive.");
      break;

    case CAD_STOP:

/* CAD STOP directive detected. The normal way to stop an observation is
 * through the STOP sequence command, so reject this directive.
 */
      cicsLogMessage( CICS_DB_ERROR,
        "continue - STOP directive. Cannot be stopped.");

      strncpy( pcad->mess, "continue - Cannot be stopped", MAX_STRING_SIZE );
      status = CAD_REJECT;
      break;

    default:

/* Unrecognised CAD directive detected. This is regarded as an error. */

      strncpy( pcad->mess, "continue - Unrecognised CAD directive",
        MAX_STRING_SIZE );
      status = CAD_REJECT;
      break;

  }
  return status;
}

/* ===================================================================== */

/*+
 *   Function name:
 *   CADstop
 *
 *   Purpose:
 *   User defined function for "stop" CAD record
 *
 *   Purpose:
 *   This routine is called whenever the stop CAD record is processed.
 *   It checks that the command is acceptable and returns a status and a
 *   message, which are written to the VAL and MESS fields of the CAD record.
 *
 *   stop is the command for stopping an observation and keeping the data.
 *   The command has no arguments
 *
 *   Invocation:
 *   struct cadRecord *pcad;
 *   status = CADstop( pcad );
 *
 *   Parameters in:
 *      > pcad->dir   *string     CAD directive
 *
 *   Parameters out:
 *      < pcad->mess  *string     failure message written to CAD MESS field
 *
 *   Return value:
 *      < status      long        Status value written to CAD VAL field
 *
 *   Globals:
 *      External functions:
 *      None
 *
 *      External variables:
 *      None
 *
 *   Requirements:
 *   It is assumed the CAD record has already been initialized and the
 *   directive and any arguments have already been assembled into the pcad
 *   structure
 *
 *   Author:
 *   Steven Beard  (smb@roe.ac.uk)
 *
 *   History:
 *   11-Nov-1996: Original version.                        (smb)
 *-
 */

long CADstop( struct cadRecord *pcad ) 
{
  long status;         /* return status */

/* Initialise CAD status */

  status = CAD_ACCEPT;

/* Switch according to the CAD directive in DIR field */

  switch (pcad->dir)
  {
    case CAD_MARK:

/* CAD MARK directive detected. Nothing needs to be done.
 */

      cicsLogMessage( CICS_DB_MIN, "stop - MARK directive.");
      break;

    case CAD_PRESET:

/* CAD PRESET directive detected. There are no command arguments
   to be checked.
 */

      cicsLogMessage( CICS_DB_MIN, "stop - PRESET directive.");
      break;

    case CAD_CLEAR:

/* CAD CLEAR directive detected. */

      cicsLogMessage( CICS_DB_MIN, "stop - CLEAR directive.");
      break;

    case CAD_START:

/* CAD START directive detected. Do nothing. The directive is being
 * monitored by the CICS DC sequence code, which will start the appropriate
 * action.
 *
 * NOTE: This directive should be rejected if an observation is not taking
 * place. This will involve checking the "observeC" CAR record at this point.
 * I need to find out how to do this.
 *
 */

      cicsLogMessage( CICS_DB_MIN, "stop - START directive.");
      break;

    case CAD_STOP:

/* CAD STOP directive detected. The normal way to stop an observation is
 * through the STOP sequence command, so reject this directive.
 */
      cicsLogMessage( CICS_DB_ERROR,
        "stop - STOP directive. STOP cannot be stopped.");

      strncpy( pcad->mess, "stop - Cannot be stopped", MAX_STRING_SIZE );
      status = CAD_REJECT;
      break;

    default:

/* Unrecognised CAD directive detected. This is regarded as an error. */

      strncpy( pcad->mess, "stop - Unrecognised CAD directive",
        MAX_STRING_SIZE );
      status = CAD_REJECT;
      break;

  }
  return status;
}

/* ===================================================================== */

/*+
 *   Function name:
 *   CADabort
 *
 *   Purpose:
 *   User defined function for "abort" CAD record
 *
 *   Purpose:
 *   This routine is called whenever the abort CAD record is processed.
 *   It checks that the command is acceptable and returns a status and a
 *   message, which are written to the VAL and MESS fields of the CAD record.
 *
 *   abort is the command for stopping an observation without keeping the data.
 *   The command has no arguments
 *
 *   Invocation:
 *   struct cadRecord *pcad;
 *   status = CADabort( pcad );
 *
 *   Parameters in:
 *      > pcad->dir   *string     CAD directive
 *
 *   Parameters out:
 *      < pcad->mess  *string     failure message written to CAD MESS field
 *
 *   Return value:
 *      < status      long        Status value written to CAD VAL field
 *
 *   Globals:
 *      External functions:
 *      None
 *
 *      External variables:
 *      None
 *
 *   Requirements:
 *   It is assumed the CAD record has already been initialized and the
 *   directive and any arguments have already been assembled into the pcad
 *   structure
 *
 *   Author:
 *   Steven Beard  (smb@roe.ac.uk)
 *
 *   History:
 *   11-Nov-1996: Original version.                        (smb)
 *-
 */

long CADabort( struct cadRecord *pcad ) 
{
  long status;         /* return status */

/* Initialise CAD status */

  status = CAD_ACCEPT;

/* Switch according to the CAD directive in DIR field */

  switch (pcad->dir)
  {
    case CAD_MARK:

/* CAD MARK directive detected. Nothing needs to be done.
 */

      cicsLogMessage( CICS_DB_MIN, "abort - MARK directive.");
      break;

    case CAD_PRESET:

/* CAD PRESET directive detected. There are no command arguments
   to be checked.
 */

      cicsLogMessage( CICS_DB_MIN, "abort - PRESET directive.");
      break;

    case CAD_CLEAR:

/* CAD CLEAR directive detected. */

      cicsLogMessage( CICS_DB_MIN, "abort - CLEAR directive.");
      break;

    case CAD_START:

/* CAD START directive detected. Do nothing. The directive is being
 * monitored by the CICS DC sequence code, which will start the appropriate
 * action.
 *
 * NOTE: This directive should be rejected if an observation is not taking
 * place. This will involve checking the "observeC" CAR record at this point.
 * I need to find out how to do this.
 *
 */

      cicsLogMessage( CICS_DB_MIN, "abort - START directive.");
      break;

    case CAD_STOP:

/* CAD STOP directive detected. The normal way to abort an observation is
 * through the ABORT sequence command, so reject this directive.
 */
      cicsLogMessage( CICS_DB_ERROR,
        "abort - STOP directive. ABORT cannot be stopped.");

      strncpy( pcad->mess, "abort - Cannot be stopped", MAX_STRING_SIZE );
      status = CAD_REJECT;
      break;

    default:

/* Unrecognised CAD directive detected. This is regarded as an error. */

      strncpy( pcad->mess, "abort - Unrecognised CAD directive",
        MAX_STRING_SIZE );
      status = CAD_REJECT;
      break;

  }
  return status;
}

/* ===================================================================== */

/*+
 *   Function name:
 *   CADendObserve
 *
 *   Purpose:
 *   User defined function for "endObserve" CAD record
 *
 *   Purpose:
 *   This routine is called whenever the endObserve CAD record is processed.
 *   It checks that the command is acceptable and returns a status and a
 *   message, which are written to the VAL and MESS fields of the CAD record.
 *
 *   endObserve is the command for signalling the end of an observation,
 *   but is ignored by Instrument Control Systems.
 *   The command has no arguments
 *
 *   Invocation:
 *   struct cadRecord *pcad;
 *   status = CADendObserve( pcad );
 *
 *   Parameters in:
 *      > pcad->dir   *string     CAD directive
 *
 *   Parameters out:
 *      < pcad->mess  *string     failure message written to CAD MESS field
 *
 *   Return value:
 *      < status      long        Status value written to CAD VAL field
 *
 *   Globals:
 *      External functions:
 *      None
 *
 *      External variables:
 *      None
 *
 *   Requirements:
 *   It is assumed the CAD record has already been initialized and the
 *   directive and any arguments have already been assembled into the pcad
 *   structure
 *
 *   Author:
 *   Steven Beard  (smb@roe.ac.uk)
 *
 *   History:
 *   18-Mar-1997: Original version.                        (smb)
 *-
 */

long CADendObserve( struct cadRecord *pcad ) 
{
  long status;         /* return status */

/* Initialise CAD status */

  status = CAD_ACCEPT;

/* Switch according to the CAD directive in DIR field */

  switch (pcad->dir)
  {
    case CAD_MARK:

/* CAD MARK directive detected. Nothing needs to be done.
 */

      cicsLogMessage( CICS_DB_MIN, "endObserve - MARK directive.");
      break;

    case CAD_PRESET:

/* CAD PRESET directive detected. There are no command arguments
   to be checked.
 */

      cicsLogMessage( CICS_DB_MIN, "endObserve - PRESET directive.");
      break;

    case CAD_CLEAR:

/* CAD CLEAR directive detected. */

      cicsLogMessage( CICS_DB_MIN, "endObserve - CLEAR directive.");
      break;

    case CAD_START:

/* CAD START directive detected. Do nothing. The directive is being
 * monitored by the CICS DC sequence code, which will start the appropriate
 * action.
 *
 * NOTE: This directive should be rejected if an observation is not taking
 * place. This will involve checking the "observeC" CAR record at this point.
 * I need to find out how to do this.
 *
 */

      cicsLogMessage( CICS_DB_MIN, "endObserve - START directive (command ignored).");
      break;

    case CAD_STOP:

/* CAD STOP directive detected. The normal way to endObserve an observation is
 * through the ABORT sequence command, so reject this directive.
 */
      cicsLogMessage( CICS_DB_ERROR, "endObserve - STOP directive - cannot be stopped.");

      strncpy( pcad->mess, "endObserve - Cannot be stopped", MAX_STRING_SIZE );
      status = CAD_REJECT;
      break;

    default:

/* Unrecognised CAD directive detected. This is regarded as an error. */

      strncpy( pcad->mess, "endObserve - Unrecognised CAD directive",
        MAX_STRING_SIZE );
      status = CAD_REJECT;
      break;

  }
  return status;
}

/* ===================================================================== */


/*+
 *   Function name:
 *   CADdebugInit
 *
 *   Purpose:
 *   Initialisation function for "debug" CAD record
 *
 *   Purpose:
 *   This routine is called whenever the debug CAD record is processed.
 *   It checks that the command is acceptable and returns a status and a
 *   message, which are written to the VAL and MESS fields of the CAD record.
 *   If a valid debug level is given the internal debug level is altered.
 *
 *   debug is the command for setting the desired debugging level.
 *   The command has no arguments
 *
 *   Invocation:
 *   struct cadRecord *pcad;
 *   status = CADdebugInit( pcad );
 *
 *   Parameters in:
 *      None
 *
 *   Parameters out:
 *      None
 *
 *   Return value:
 *      < status      long        Status value written to CAD VAL field
 *
 *   Globals:
 *      External functions:
 *      None
 *
 *      External variables:
 *      None
 *
 *   Requirements:
 *   It is assumed the CAD record has already been initialized and the
 *   directive and any arguments have already been assembled into the pcad
 *   structure
 *
 *   Author:
 *   Steven Beard  (smb@roe.ac.uk)
 *
 *   History:
 *   16-Jan-1997: Original version.                        (smb)
 *-
 */

long CADdebugInit( struct cadRecord *pcad ) 
{
  long status;         /* return status */
  static long debug;   /* Required init debug level */

/* Initialise CAD status */

  status = CAD_ACCEPT;

/* Set the inital debug level to NONE. */

  debug = CICS_DB_NONE;
  cicsSetDebug( debug );

  return status;
}

/* ===================================================================== */


/*+
 *   Function name:
 *   CADdebug
 *
 *   Purpose:
 *   User defined function for "debug" CAD record
 *
 *   Purpose:
 *   This routine is called whenever the debug CAD record is processed.
 *   It checks that the command is acceptable and returns a status and a
 *   message, which are written to the VAL and MESS fields of the CAD record.
 *   If a valid debug level is given the internal debug level is altered.
 *
 *   debug is the command for setting the desired debugging level.
 *
 *   Invocation:
 *   struct cadRecord *pcad;
 *   status = CADdebug( pcad );
 *
 *   Parameters in:
 *      > pcad->dir   *string     CAD directive
 *      > pcad->a     *string     CAD input argument A
 *                                = Requested debug level
 *
 *   Parameters out:
 *      < pcad->mess  *string     failure message written to CAD MESS field
 *      < pcad->vala  *string     CAD output value A
 *                                = Set debug level
 *
 *   Return value:
 *      < status      long        Status value written to CAD VAL field
 *
 *   Globals:
 *      External functions:
 *      None
 *
 *      External variables:
 *      None
 *
 *   Requirements:
 *   It is assumed the CAD record has already been initialized and the
 *   directive and any arguments have already been assembled into the pcad
 *   structure
 *
 *   Author:
 *   Steven Beard  (smb@roe.ac.uk)
 *
 *   History:
 *   17-Mar-1997: Original version.                        (smb)
 *-
 */

long CADdebug( struct cadRecord *pcad ) 
{
  long status;         /* return status */
  static long debug;   /* Required debug level */

/* Initialise CAD status */

  status = CAD_ACCEPT;

/* Switch according to the CAD directive in DIR field */

  switch (pcad->dir)
  {
    case CAD_MARK:

/* CAD MARK directive detected. Nothing needs to be done.
 */

      cicsLogMessage( CICS_DB_MIN, "debug - MARK directive.");
      break;

    case CAD_PRESET:

/* CAD PRESET directive detected. Check the debug level given is valid.
 */

      cicsLogMessage( CICS_DB_MIN, "debug - PRESET directive.");
      cicsLogString( CICS_DB_FULL, "Requested debug level =", pcad->a);

      if ( strcmp( pcad->a, "NOLOG" ) == 0 )
      {
        debug = CICS_DB_NOLOG;
        strcpy( pcad->vala, pcad->a );
      }
      else
      {
        if ( strcmp( pcad->a, "NONE" ) == 0 )
        {
          debug = CICS_DB_NONE;
          strcpy( pcad->vala, pcad->a );
        }
        else
        {
          if ( strcmp( pcad->a, "MIN" ) == 0 )
          {
            debug = CICS_DB_MIN;
            strcpy( pcad->vala, pcad->a );
          }
          else
          {
            if ( strcmp( pcad->a, "FULL" ) == 0 )
            {
              debug = CICS_DB_FULL;
              strcpy( pcad->vala, pcad->a );
            }
            else
            {

              strncpy( pcad->mess, "debug - Invalid debug level",
                MAX_STRING_SIZE );
              status = CAD_REJECT;
            }
          }
        }
      }
      break;

    case CAD_CLEAR:

/* CAD CLEAR directive detected. */

      cicsLogMessage( CICS_DB_MIN, "debug - CLEAR directive.");
      break;

    case CAD_START:

/* CAD START directive detected. Set the required debug level.
 *
 */

      cicsLogLong( CICS_DB_MIN, "debug - START directive. New level=", debug );

      cicsSetDebug( debug );
      break;

    case CAD_STOP:

/* CAD STOP directive detected. The normal way to debug an observation is
 * through the ABORT sequence command, so reject this directive.
 */
      cicsLogMessage( CICS_DB_ERROR,
        "debug - STOP directive. debug cannot be stopped.");

      strncpy( pcad->mess, "debug - Cannot be stopped", MAX_STRING_SIZE );
      status = CAD_REJECT;
      break;

    default:

/* Unrecognised CAD directive detected. This is regarded as an error. */

      strncpy( pcad->mess, "debug - Unrecognised CAD directive",
        MAX_STRING_SIZE );
      status = CAD_REJECT;
      break;

  }
  return status;
}

/* ===================================================================== */


/*+
 *   Function name:
 *   CADreboot
 *
 *   Purpose:
 *   User defined function for "reboot" CAD record
 *
 *   Purpose:
 *   This routine is called whenever the reboot CAD record is processed.
 *   It checks that the command is acceptable and returns a status and a
 *   message, which are written to the VAL and MESS fields of the CAD record.
 *
 *   reboot is the command for rebooting the IOC.
 *   The command has no arguments
 *
 *   Invocation:
 *   struct cadRecord *pcad;
 *   status = CADreboot( pcad );
 *
 *   Parameters in:
 *      > pcad->dir   *string     CAD directive
 *
 *   Parameters out:
 *      < pcad->mess  *string     failure message written to CAD MESS field
 *
 *   Return value:
 *      < status      long        Status value written to CAD VAL field
 *
 *   Globals:
 *      External functions:
 *      None
 *
 *      External variables:
 *      None
 *
 *   Requirements:
 *   It is assumed the CAD record has already been initialized and the
 *   directive and any arguments have already been assembled into the pcad
 *   structure.
 *
 *   It is also assumed some other subroutine will do the actual rebooting.
 *
 *   Author:
 *   Steven Beard  (smb@roe.ac.uk)
 *
 *   History:
 *   16-Mar-1997: Original version.                        (smb)
 *-
 */

long CADreboot( struct cadRecord *pcad ) 
{
  long status;         /* return status */

/* Initialise CAD status */

  status = CAD_ACCEPT;

/* Switch according to the CAD directive in DIR field */

  switch (pcad->dir)
  {
    case CAD_MARK:

/* CAD MARK directive detected. Nothing needs to be done.
 */

      cicsLogMessage( CICS_DB_MIN, "reboot - MARK directive.");
      break;

    case CAD_PRESET:

/* CAD PRESET directive detected. Check that a reboot is allowed at this time.
 * AT THE MOMENT IT IS ASSUMED A REBOOT IS ALWAYS ALLOWED.
 */

      cicsLogMessage( CICS_DB_MIN, "reboot - PRESET directive.");

    case CAD_CLEAR:

/* CAD CLEAR directive detected. */

      cicsLogMessage( CICS_DB_MIN, "reboot - CLEAR directive.");
      break;

    case CAD_START:

/*
 * CAD START directive detected.
 */

      cicsLogMessage( CICS_DB_MIN, "reboot - START directive." );

/* Note that "reboot()" is not called here. It is assumed a subroutine record
 * will be activated to do the actual rebooting once the CAR record has been
 * set BUSY.
 */

      break;

    case CAD_STOP:

      cicsLogMessage( CICS_DB_ERROR,
        "reboot - STOP directive. Reboot cannot be stopped.");

      strncpy( pcad->mess, "reboot - Cannot be stopped", MAX_STRING_SIZE );
      status = CAD_REJECT;
      break;

    default:

/* Unrecognised CAD directive detected. This is regarded as an error. */

      strncpy( pcad->mess, "reboot - Unrecognised CAD directive",
        MAX_STRING_SIZE );
      status = CAD_REJECT;
      break;

  }
  return status;
}

/* ===================================================================== */


/*+
 *   Function name:
 *   checkBusy
 *
 *   Purpose:
 *   Check whether instrument is busy, based on second CAD argument.
 *
 *   Purpose:
 *   This is a utility function, called by the above CAD routines, which
 *   looks at the second CAD argument, pcad->b, and determines whether
 *   a command should be rejected due to the instrument being busy.
 *
 *   Invocation:
 *   struct cadRecord *pcad;
 *   status = CADreboot( pcad );
 *
 *   Parameters in:
 *      > pcad->dir   *string     CAD directive
 *      > pcad->b     long        Instrument busy flag. The command will be
 *                                rejected if this flag happens to be exactly
 *                                equal to CICS_BUSY.
 *
 *   Parameters out:
 *      < pcad->mess  *string     failure message written to CAD MESS field
 *
 *   Return value:
 *      < status      long        Status value written to the CAD VAL field.
 *                                Returned CAD_REJECT is instrument is busy.
 *
 *   Globals:
 *      External functions:
 *      None
 *
 *      External variables:
 *      None
 *
 *   Requirements:
 *   It is assumed the CAD record has already been initialized and the
 *   directive and any arguments have already been assembled into the pcad
 *   structure.
 *
 *   Author:
 *   Steven Beard  (smb@roe.ac.uk)
 *
 *   History:
 *   16-Apr-1997: Original version.                        (smb)
 *   16-Apr-1997: Check on instrument being busy added.    (smb)
 *-
 */

long checkBusy( struct cadRecord *pcad ) 
{
  long status;         /* return status */
  long cvstat;         /* conversion status */
  long busyFlag;       /* busy flag */

/* Initialise CAD status */

  status = CAD_ACCEPT;

/* Convert the second input argument to an integer. */

  cvstat = sscanf( pcad->b, "%d", &busyFlag );

  cicsLogLong( CICS_DB_FULL, "Busy flag =", busyFlag );

/*
 * Check the input argument was successfully converted. (cvstat contains the
 * number of items that "sscanf" has successfully converted, and there
 * should be 1).
 * If the argument cannot be converted, just assume the command can go ahead.
 */

  if ( cvstat == 1 )
  {

/*
 * If the argument contains "CICS_BUSY" the instrument is busy and the
 * command should be rejected.
 */

    if ( busyFlag == CICS_BUSY )
    {

      cicsLogMessage( CICS_DB_LOG, "Instrument is busy - command rejected." );
      strncpy( pcad->mess, "Instrument is busy", MAX_STRING_SIZE );
      status = CAD_REJECT;
    }
  }

  return status;
}

/* ===================================================================== */
