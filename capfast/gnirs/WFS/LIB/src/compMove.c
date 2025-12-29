static char rcsid[]="$Id: compMove.c,v 1.2 2009/05/27 19:34:56 fkraemer Exp $";

/*
 *   FILENAME
 *   --------
 *   compcCad.c
 *
 *   PURPOSE
 *   -------
 *   This file contains the source for all the functions used by the
 *   CICS continuous CAD records. These functions are used to validate
 *   the arguments given to the CAD record
 *
 *   FUNCTION NAME(S)
 *   ----------------
 *   CADcomp1mMove  - Move 1 axis continuous component in real world units
 *   CADcomp2mMove   - Move 2 axis continuous component in rotated units
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
 *   Hubert Yamada (yamada@newton.ifa.hawaii.edu)
 *
 *   HISTORY
 *   -------
 *INDENT-OFF*
 *
 * $Log: compMove.c,v $
 * Revision 1.2  2009/05/27 19:34:56  fkraemer
 * fkraemer - copied my complete working dir over trunk
 *
 *
 *INDENT-ON*
 *   16-Dec-1996: Original version edited from "filtCad.c".          (smb)
 *   10-Jan-1997: Problem with converting CAD outputs to anything
 *                other than a string fixed.                         (smb)
 *   16-May-1997: NIRI version.                                      (smb)
 *   19-May-1997: Read allowed range of mechanism movement from the
 *                "C" and "D" inputs of the CAD record.              (smb)
 *   21-May-1997: Check if the mechanism is datumed from the "B"
 *                input of the CAD record.                           (smb)
 *   29-May-1997: Bret's fix for grabbing the contents of the LUT
 *                record attached to OUTA added. This now becomes a
 *                generic routine, compcCad.c                        (smb)
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
#include  <genSubRecord.h>
#include  <dbCommon.h>
#include  <recSup.h>
#include  <cad.h>

#include  <cicsConst.h>
#include  <cicsLib.h>
#include  <compLib.h>

/* ===================================================================== */

/*+
 *   Function name:
 *   CADcomp1mMove - Check acceptability of "move continuous component" command.
 *
 *   Purpose:
 *   This routine is called whenever the focMove CAD record is
 *   processed. It checks that the arguments are valid and the command
 *   is acceptable and returns a status and a message, which are written
 *   to the VAL and MESS fields of the CAD record.
 *
 *   focMove is the command for positioning the continuous component in real
 *   world coordinates.
 *
 *   Invocation:
 *   struct cadRecord *pCad;
 *   status = CADcomp1mMove(pCad);
 *
 *   Parameters in:
 *      > pCad->name  *string     Name of CAD record
 *      > pCad->dir   *string     CAD directive
 *      > pCad->a     *string     CAD input A (Desired real-world position)
 *      > pCad->e     *string     CAD input E (Ignore datum -- Not used)
 *      > pCad->f     *string     CAD input F (Zero == Mechanism is datumed)
 *      > pCad->g     *string     CAD input G (Lower Eng limit)
 *      > pCad->h     *string     CAD input H (Upper Eng limit)
 *      > pCad->m     *string     CAD input M (Scale)
 *      > pCad->n     *string     CAD input N (Offset)
 *
 *   Parameters out:
 *      < pCad->mess  *string     failure message written to CAD MESS field
 *      < pCad->vala  double      CAD output value A
 *                                = Component real world position demand
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
 *
 *   Author:
 *   Steven Beard  (smb@roe.ac.uk)
 *   Hubert Yamada  (yamada@newton.ifa.hawaii.edu)
 *
 *   History:
 *   29-Mar-1996: Original version, based on the TCS prototype and
 *                the CAD skeleton provided by Andy Foster.          (smb)
 *   16-Dec-1996: Focus version.                                     (smb)
 *   10-Jan-1997: Problem of copying CAD output to anything other
 *                than a string fixed.                               (smb)
 *   19-May-1997: Read allowed range of mechanism movement from the
 *                "C" and "D" inputs of the CAD record.              (smb)
 *-
 */

static long comp1mMove(struct cadRecord *, const CompParms *);

long
CADcomp1mMove(struct cadRecord *pCad)
{
	return compCad(pCad, comp1mMove);
}

/*
 * Check the command arguments and, if acceptable, copy them to the 
 * CAD outputs.
 */

static long
comp1mMove(struct cadRecord *pCad, const CompParms *pParms)
{
	long status;            /* return status */
	double wPos;            /* real world position */
	long engPos;            /* Engineering units position */
	const double scale = pParms->inp[0]; /* Convert engineering units */
	const long offset = pParms->inp[1]; /* Offset for engineering units */

	/*
	 * Initialise CAD status
	 */

	status = CAD_ACCEPT;

	/*
	 * Convert the input argument (real world position) to a double
	 * value.
	 */

	if (status == CAD_ACCEPT) {
		status = compDouble(pCad->a, &wPos);
		if (status != CAD_ACCEPT) {
			cicsLogFmt(CICS_DB_ERROR, "%s.A:  invalid real world posn",
				pCad->name);
			strncpy (pCad->mess, "Invalid real world posn", MAX_STRING_SIZE);
		} else {
			cicsLogFmt(CICS_DB_FULL, "%s.A: Requested position = %f",
				pCad->name, wPos);
		}
	}

	/* 
	 * Convert the real-world position into engineering units and check
	 * if it is in range.
	 */

	if (status == CAD_ACCEPT) {
		engPos = wPos * scale + offset;

		if (pParms->mech[0].cyclic == EPICS_FALSE
				&& (engPos < pParms->mech[0].min
					|| engPos > pParms->mech[0].max)) {
			cicsLogFmt(CICS_DB_ERROR, "%s.A - real world posn out of range",
				pCad->name);
			strncpy (pCad->mess, "Out of range", MAX_STRING_SIZE);
			status = CAD_REJECT;
		}
	}

	/*
	 * Copy the real world position to the CAD output.
	 */

	if (status == CAD_ACCEPT)
		sprintf(pCad->vala, "%ld", engPos);

	return status;
}

/* ===================================================================== */

/*+
 *   Function name:
 *   CADcomp2mMove - Check acceptability of "move continuous component"
 *   command.
 *
 *   Purpose:
 *   This routine is called whenever the focMove CAD record is
 *   processed. It checks that the arguments are valid and the command
 *   is acceptable and returns a status and a message, which are written
 *   to the VAL and MESS fields of the CAD record.
 *
 *   focMove is the command for positioning the continuous component in real
 *   world coordinates.
 *
 *   Invocation:
 *   struct cadRecord *pCad;
 *   status = CADcomp2cMove(pCad);
 *
 *   Parameters in:
 *      > pCad->dir   *string     CAD directive
 *      > pCad->a     *string     CAD input argument A
 *                                = Component X coordinate target
 *      > pCad->b     *string     CAD input argument B
 *                                = Component Y coordinate target
 *      > pCad->e     *string     CAD input E (Ignore datum -- Not used)
 *      > pCad->f     *string     CAD input F (Zero == Mechanism is datumed)
 *      > pCad->g     *string     CAD input G (Lower Eng limit axis 1)
 *      > pCad->h     *string     CAD input H (Upper Eng limit axis 1)
 *      > pCad->i     *string     CAD input I (Lower Eng limit axis 2)
 *      > pCad->j     *string     CAD input J (Upper Eng limit axis 2)
 *      > pCad->o     *string     CAD input O (Scale axis 1)
 *      > pCad->q     *string     CAD input R (Scale axis 2)
 *      > pCad->s     *string     CAD input S (Offset axis 1)
 *      > pCad->t     *string     CAD input T (Offset axis 2)
 *
 *   Parameters out:
 *      < pCad->mess  *string     failure message written to CAD MESS field
 *      < pCad->vala  double      CAD output value A
 *                                = Component X coordinate demand
 *      < pCad->valb  double      CAD output value B
 *                                = Component Y coordinate demand
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
 *   The CAD record can only report one message, so if both of the input
 *   arguments are invalid only the second one will be reported. However,
 *   both error messages will be logged
 *
 *   Author:
 *   Steven Beard  (smb@roe.ac.uk)
 *   Hubert Yamada (yamada@newton.ifa.hawaii.edu)
 *
 *   History:
 *   29-Mar-1996: Original version, based on the TCS prototype and
 *                the CAD skeleton provided by Andy Foster.          (smb)
 *   16-Dec-1996: Focus version.                                     (smb)
 *   10-Jan-1997: Problem of copying CAD output to anything other
 *                than a string fixed.                               (smb)
 *   19-May-1997: Read allowed range of mechanism movement from the
 *                "C" and "D" inputs of the CAD record.              (smb)
 *-
 */

static long comp2mMove(struct cadRecord *, const CompParms *pParms);

long
CADcomp2mMove(struct cadRecord *pCad)
{
	return compCad(pCad, comp2mMove);
}

static long
comp2mMove(struct cadRecord *pCad, const CompParms *pParms)
{
	long status;          /* return status */
	char message[STRBUF]; /* descriptive message */
	double pos1;          /* real world axis-1 position */
	double pos2;          /* real world axis-2 position */
	const double a11 = pParms->inp[0]; /* Coordinate transformation */
	const double a12 = pParms->inp[1]; /* Coordinate transformation */
	const double a21 = pParms->inp[2]; /* Coordinate transformation */
	const double a22 = pParms->inp[3]; /* Coordinate transformation */
	const long c1 = pParms->inp[4]; /* Offset for axis 1 */
	const long c2 = pParms->inp[5]; /* Offset for axis 2 */
	long eng1;            /* Axis-1 position in motor steps */
	long eng2;            /* Axis-2 position in motor steps */

	/*
	 * Initialise CAD status
	 */

	status = CAD_ACCEPT;

	/*
	 * Convert the first input argument (real world axis-1 coordinate)
	 * to a double value, then check that the input argument was
	 * successfully converted.
	 */

	if (status == CAD_ACCEPT) {
		status = compDouble(pCad->a, &pos1);

		if (status == CAD_ACCEPT) {
			cicsLogFmt(CICS_DB_FULL, "%s: Requested axis-1 position = %f",
				pCad->name, pos1);
		} else {
			sprintf(message, "%s.A - Invalid real world axis-1 coord",
				pCad->name);
			cicsLogMessage(CICS_DB_ERROR, message);
			strncpy (pCad->mess, "First coordinate invalid",
				MAX_STRING_SIZE);
		}
	}

	/*
	 * Convert the second input argument (real world axis-2 coordinate)
	 * to a double value, then check that the input argument was
	 * successfully converted.
	 */

	if (status == CAD_ACCEPT) {
		status = compDouble(pCad->b, &pos2);

		if (status == CAD_ACCEPT) {
			cicsLogFmt(CICS_DB_FULL, "%s: Requested axis-2 position = %f",
				pCad->name, pos2);
		} else {
			sprintf(message, "%s.B - Second coordinate invalid",
				pCad->name);
			cicsLogMessage(CICS_DB_ERROR, message);
			strncpy (pCad->mess, "Invalid real world axis-2 coord",
				MAX_STRING_SIZE);
		}
	}

	/*
	 * Convert the input values into motor coordinates.
	 */

	if (status == CAD_ACCEPT) {
		eng1 = a11 * pos1 + a12 * pos2 + c1;
		eng2 = a21 * pos1 + a22 * pos2 + c2;
		cicsLogFmt(CICS_DB_FULL, "%s: eng1 = %f", pCad->name, eng1);
		cicsLogFmt(CICS_DB_FULL, "%s: eng2 = %f", pCad->name, eng2);
	}

	/*
	 * Check the axis-1 coordinate is within a sensible range.
	 */

	if (status == CAD_ACCEPT) {
		if (pParms->mech[0].cyclic == EPICS_FALSE
				&& (eng1 <= pParms->mech[0].min 
					|| eng1 >= pParms->mech[0].max)) {
			cicsLogFmt(CICS_DB_ERROR,
				"%s.A - Real world axis-1 coord out of range", pCad->name);
			strncpy(pCad->mess, "First coordinate out of range",
				MAX_STRING_SIZE);
			status = CAD_REJECT;
		}
	}

	/*
	 * Check that the axis-2 coordinate is within a sensible range. 
	 */

	if (status == CAD_ACCEPT) {
		if (pParms->mech[1].cyclic == EPICS_FALSE
				&& (eng2 <= pParms->mech[1].min
					|| eng2 >= pParms->mech[1].max)) {
			cicsLogFmt(CICS_DB_ERROR,
				"%s.B - Real world axis-2 coord out of range", pCad->name);
			strncpy (pCad->mess, "Second coordinate out of range",
				MAX_STRING_SIZE);
			status = CAD_REJECT;
		}
	}

	/*
	 * Now copy the computed values to the output.
	 */

	if (status == CAD_ACCEPT) {
		sprintf(pCad->vala, "%ld", eng1);
		sprintf(pCad->valb, "%ld", eng2);
	}

	return status;
}

int
comp2mAltInvCalc(struct genSubRecord *pGenSub)
{
	const double i11 = *(double *)pGenSub->c;
	const double i12 = *(double *)pGenSub->d;
	const double i21 = *(double *)pGenSub->e;
	const double i22 = *(double *)pGenSub->f;
	const double c1 = *(double *)pGenSub->g;
	const double c2 = *(double *)pGenSub->h;
	const long engX = *(long *)pGenSub->a - c1;
	const long engY = *(long *)pGenSub->b - c2;
	double x;
	double y;

	x = i11 * engX + i12 * engY;
	y = i21 * engX + i22 * engY;

	*(double *)pGenSub->vala = x;
	*(double *)pGenSub->valb = y;

	return 0;
}
