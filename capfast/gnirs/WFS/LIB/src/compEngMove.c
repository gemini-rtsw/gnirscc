static char rcsid[]="$Id: compEngMove.c,v 1.2 2009/05/27 19:34:55 fkraemer Exp $";

/*
 *   FILENAME
 *   -------- 
 *   compCad.c
 *
 *   PURPOSE
 *   -------
 *   This file contains the source for all the functions used by the
 *   CICS CAD records for any type of component. These functions are used to
 *   validate the arguments given to the CAD record
 *
 *
 *   FUNCTION NAME(S)
 *   ----------------
 *   CADcompEngMove - Move component to specified engineering position
 *   CADcompNop     - Trigger arbitrary SNL code.
 *
 *   REQUIREMENTS
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
 *INDENT-ON
 *
 * $Log: compEngMove.c,v $
 * Revision 1.2  2009/05/27 19:34:55  fkraemer
 * fkraemer - copied my complete working dir over trunk
 *
 * Revision 1.3  1999/11/14 02:05:40  yamada
 * Reformatted logs.
 *
 *
 *INDENT-OFF
 *
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
 *   11-Oct-1996: Split into "syscad" and "filtcad". "filtcad" will
 *                eventually be made more generic.                   (smb)
 *   04-Dec-1996: "printf" replaced by "cicsLogMessage" or
 *                "cicsLogString".                                   (smb)
 *   10-Jan-1997: Problem with converting CAD outputs to anything
 *                other than a string fixed.                         (smb)
 *   07-May-1997: New version for NIRI ("filt" --> "wh0").           (smb)
 *   16-May-1997: NIRI naming changed to reflect NIRI/OCS interface
 *                ("filt" --> "filt1").                              (smb)
 *   19-May-1997: Read allowed range of mechanism movement from the
 *                "C" and "D" inputs of the CAD record.              (smb)
 *   21-May-1997: Check if the mechanism is datumed from the "B"
 *                input of the CAD record.                           (smb)
 *   29-May-1997: Bret's fix for grabbing the contents of the LUT
 *                record attached to OUTA added. This now becomes a
 *                generic routine, compCad.c                         (smb)
 *   24-Nov-1997: Added CADcompDiag and CADcompRdtm to support diagnose
 *                and redatum commands.                             (hty)
 */

#include <vxWorks.h>
#include <types.h>
#include <math.h>
#include <time.h>
#include <stdlib.h>
#include <stdioLib.h>
#include <string.h>

#include <dbDefs.h>
#include <dbAccess.h>
#include <dbEvent.h>
#include <dbFldTypes.h>
#include <errMdef.h>
#include <recSup.h>
#include <devSup.h>
#include <special.h>
#include <cadRecord.h>
#include <dbCommon.h>
#include <cad.h>
#include <lutoutRecord.h>

#include <cicsConst.h>
#include <cicsLib.h>
#include <compLib.h>

/* ===================================================================== */

/*+
 *   Function name:
 *   CADcompEngMove
 *
 *   Purpose:
 *   This routine is called whenever the CADcompEngMove CAD record is
 *   processed. It checks that the arguments are valid and the command
 *   is acceptable and returns a status and a message, which are written
 *   to the VAL and MESS fields of the CAD record.
 *
 *   CADcompEngMove is the command for positioning the component to
 *   a location specified in engineering units.
 *
 *   Invocation:
 *   struct cadRecord *pCad;
 *   status = CADcompEngMove(pCad);
 *
 *   Parameters in:
 *      > pCad->dir   *string     CAD directive
 *      > pCad->a     *string     CAD input A (Desired engineering position)
 *      > pCad->b     *string     CAD input B (Non-zero == Ignore limits)
 *      > pCad->c     *string     CAD input C (Ignored)
 *      > pCad->d     *string     CAD input D (Ignored)
 *      > pCad->e     *string     CAD input E (Ignore datum -- Not used)
 *      > pCad->f     *string     CAD input F (Zero == Mechanism is datumed)
 *      > pCad->g     *string     CAD input G (Lower Eng limit)
 *      > pCad->h     *string     CAD input H (Upper Eng limit)
 *
 *   Parameters out:
 *      < pCad->mess  *string     Failure message written to CAD MESS field
 *      < pCad->vala  *string     CAD output A (SNL Arg == Engineering position)
 *      < pCad->valb  *string     CAD output B (Ignored)
 *      < pCad->valc  *string     CAD output C (Ignored)
 *      < pCad->vald  *string     CAD output D (Ignored)
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
 *   directive and any arguments have already been assembled into the pCad
 *   structure
 *
 *   Limitations:
 *   None known
 *
 *   Authors:
 *   Hubert Yamada  (yamada@newton.ifa.hawaii.edu)
 *
 *   History:
 *   1999-10-13 hty Original Version
 *-
 */

static long engMove(struct cadRecord *, const CompParms *);

long 
CADcompEngMove(struct cadRecord *pCad) 
{
	return compCad(pCad, engMove);
}

static long
engMove(struct cadRecord *pCad, const CompParms *pParms)
{
	long status = CAD_ACCEPT;
	long pos;             /* engineering position */

	/*
	 * Convert the input argument (engineering position) to an integer.
	 */

	if (status == CAD_ACCEPT) {
		status = compLong(pCad->a, &pos);

		if (status != CAD_ACCEPT) {
			strncpy(pCad->mess, "Illegal Engineering Position",
				MAX_STRING_SIZE);
			cicsLogFmt(CICS_DB_ERROR,
				"%s - Illegal Engineering Position", pCad->name);
		} else {
			cicsLogFmt(CICS_DB_FULL, "Requested position = %ld", pos);
		}
	}

	if (status == CAD_ACCEPT) {
		/*
		 * Check the engineering position is within a sensible range.
		 */

		if (pParms->mech[0].cyclic == EPICS_FALSE
				&& (pos < pParms->mech[0].min || pos > pParms->mech[0].max)) {
			cicsLogFmt(CICS_DB_ERROR, "%s - Engineering position out of range",
				pCad->name);
			strncpy(pCad->mess, "Eng posn out of range", MAX_STRING_SIZE);
			status = CAD_REJECT;
		}
	}

	/*
	 * Copy the engineering position to the CAD output.
	 */

	if (status == CAD_ACCEPT)
		sprintf(pCad->vala, "%ld", pos);

	return status;
}
