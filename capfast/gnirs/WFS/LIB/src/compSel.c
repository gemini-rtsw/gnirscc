static char rcsid[]="$Id: compSel.c,v 1.2 2009/05/27 19:34:56 fkraemer Exp $";

/*
 *   FILENAME
 *   -------- 
 *   compdCad.c
 *
 *   PURPOSE
 *   -------
 *   This file contains the source for all the functions used by the
 *   CICS discrete component CAD records. These functions are used to
 *   validate the arguments given to the CAD record
 *
 *
 *   FUNCTION NAME(S)
 *   ----------------
 *   CADcompSel     - Select position
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
 *   AUTHORS
 *   ------_
 *   Steven Beard  (smb@roe.ac.uk)
 *   Hubert Yamada (yamada@newton.ifa.hawaii.edu)
 *
 *   HISTORY
 *   -------
 *INDENT-OFF*
 *
 * $Log: compSel.c,v $
 * Revision 1.2  2009/05/27 19:34:56  fkraemer
 * fkraemer - copied my complete working dir over trunk
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
 *                generic routine, compdCad.c                        (smb)
 *   04-Apr-1998: Copies its input to both A and B outputs           (hty)
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
 *   CADcompSel - Check acceptability of "select position" command.
 *
 *   Purpose:
 *   This routine is called whenever the compSel CAD record is
 *   processed. It checks that the arguments are valid and the command
 *   is acceptable and returns a status and a message, which are written
 *   to the VAL and MESS fields of the CAD record.
 *
 *   compSel is the command for moving a discrete mechanism to a named
 *   position.
 *
 *   Invocation:
 *   struct cadRecord *pCad;
 *   status = CADcompSel(pCad);
 *
 *   Parameters in:
 *      > pCad->name  *string     Name of CAD record
 *      > pCad->desc  *string     Descriptive name of mechanism
 *      > pCad->dir   *string     CAD directive
 *      > pCad->a     *string     CAD input argument A
 *                                = Position name target
 *
 *   Parameters out:
 *      < pCad->mess  *string     failure message written to CAD MESS field
 *      < pCad->vala  *string     CAD output value A
 *                                = Position name demand
 *
 *   Return value:
 *      < status      long        Status value written to CAD VAL field
 *
 *   Globals 
 *      External functions:
 *      None
 *
 *      External variables:
 *      None
 *
 *   Requirements:
 *   It is assumed the CAD record has already been initialized and the
 *   directive and any arguments have already been assembled into the pCad
 *   structure.
 *
 *   It is assumed there is a "lutout" record connected to the OUTA link
 *   of the CAD record.
 *
 *   Limitations:
 *   The method of checking that the string given is present in the
 *   lookup table assumes that the "lutout" record is directly connected
 *   to the OUTA link of the CAD record.
 *
 *   Author:
 *   Steven Beard  (smb@roe.ac.uk)
 *
 *   History:
 *   29-Mar-1996: Original version, based on the TCS prototype and
 *                the CAD skeleton provided by Andy Foster.          (smb)
 *   15-Apr-1996: Fix syntax errors.                                 (smb)
 *   07-May-1996: Relax command acceptance criteria.                 (smb)
 *   25-Jun-1996: Convert string given to engineering position as if
 *                translated by a lookup table.                      (smb)
 *   02-Aug-1996: The position name is now converted to an engineering
 *                position using a "lutout" record.                  (smb)
 *   29-May-1997: Bret Goodrich's fix used to obtain the list of
 *                allowed positions from the "lutout" record,
 *                assuming the record is connected to the OUTA field
 *                of the CAD record.                                 (smb)
 *-
 */

static long compSel(struct cadRecord *, const CompParms *);

long
CADcompSel(struct cadRecord *pCad)
{
	return compCad(pCad, compSel);
}

static long
compSel(struct cadRecord *pCad, const CompParms *pParms)
{
	char message[STRBUF];   /* descriptive message */
	extern struct rset lutoutRSET;
	struct link *plink;
	struct dbAddr *paddr;
	struct dbr_enumStrs es;
	int i;
	long status = CAD_ACCEPT;

	/*
	 * CAD PRESET directive detected. Check the position name given with the
	 * command and, if acceptable, convert this to an engineering position
	 * and write it to the CAD output.
	 */

	cicsLogFmt(CICS_DB_FULL, "%s - Requested position = \"%s\"",
		pCad->name, pCad->a);

	/*
	 * Check the string provided with all the tags contained in the "lutout"
	 * record attached to the OUTA field of the CAD record.
	 */

	/*
	 * test the inputs are in the LUT configuration
	 */

	if (pCad->outd.type == DB_LINK) {
		plink = (struct link *) &pCad->outd;
		paddr = (struct dbAddr *) plink->value.db_link.pdbAddr;
		(lutoutRSET.get_enum_strs)(paddr, &es);

		printf("input = %s\n",pCad->a);
		for (i = 0; i < es.no_str; i++) {
			printf("struct [%d] = %s\n",i,es.strs[1]);
			if (strncmp (pCad->a, es.strs[i], MAX_STRING_SIZE) == 0)
				
				break;
			
		}
		if (i == es.no_str) {
			sprintf(message, "%s - invalid position name.", pCad->name);
			cicsLogMessage(CICS_DB_ERROR, message);
			strncpy (pCad->mess, "Invalid position name", MAX_STRING_SIZE);
			status = CAD_REJECT;
		}
	}

	/*
	 * Copy the input argument to the output.
	 */

	strncpy(pCad->vala, pCad->a, MAX_STRING_SIZE);

	return status;
}
