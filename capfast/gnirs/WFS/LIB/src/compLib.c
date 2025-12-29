static char rcsid[]="$Id: compLib.c,v 1.2 2009/05/27 19:34:56 fkraemer Exp $";

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
 * $Log: compLib.c,v $
 * Revision 1.2  2009/05/27 19:34:56  fkraemer
 * fkraemer - copied my complete working dir over trunk
 *
 *
 *INDENT-ON*
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
#include <genSubRecord.h>
#include <timeLib.h>

#include <cicsConst.h>
#include <cicsLib.h>
#include <compLib.h>

/*
 * Define this as EPICS_TRUE to use VxWorks time.  This is only necessary
 * for testing the system when it is not connected to the time bus.
 */

static int vxTime(double *);

int compUseVxTime = EPICS_FALSE;

/* ===================================================================== */

/*+
 *   Function name:
 *   compCad
 *
 *   Purpose:
 *   This routine is called whenever the CAD record which needs no
 *   argument validation is processed. It checks that the command is
 *   acceptable and returns a status and a message, which are written
 *   to the VAL and MESS fields of the CAD record.
 *
 *   Invocation:
 *   struct cadRecord *pCad;
 *   status = compCad(pCad);
 *
 *   Parameters in:
 *      > pCad->dir   *string     CAD directive
 *      > pCad->a     *string     CAD input A (Unused)
 *      > pCad->b     *string     CAD input B (Unused)
 *      > pCad->c     *string     CAD input C (Unused)
 *      > pCad->d     *string     CAD input D (Unused)
 *      > pCad->e     *string     CAD input E (Non-zero == Ignore datum field)
 *      > pCad->f     *string     CAD input F (Zero == Mechanism is datumed)
 *      > pCad->g     *string     CAD input G (Lower Eng limit 1, Unused)
 *      > pCad->h     *string     CAD input H (Upper Eng limit 1, Unused)
 *      > pCad->i     *string     CAD input I (Lower Eng limit 2, Unused)
 *      > pCad->j     *string     CAD input J (Upper Eng limit 2, Unused)
 *      > pCad->k     *string     CAD input I (Lower Eng limit 3, Unused)
 *      > pCad->l     *string     CAD input J (Upper Eng limit 3, Unused)
 *
 *   Parameters out:
 *      < pCad->mess  *string     Failure message written to CAD MESS field
 *      < pCad->vala  *string     CAD output A (SNL Arg 1, Unused)
 *      < pCad->valb  *string     CAD output B (SNL Arg 2, Unused)
 *      < pCad->valc  *string     CAD output C (SNL Arg 3, Unused)
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
 *   Authors:
 *   Steven Beard  (smb@roe.ac.uk)
 *   Hubert Yamada (yamada@newton.ifa.hawaii.edu)
 *
 *   History:
 *   16-Apr-1996: Original version.                                    (smb)
 *   25-Jun-1996: Generate a park position in engineering coordinates. (smb)
 *   02-Aug-1996: Modified so a "lutout" record is used to obtain the
 *                park position from a lookup table.                   (smb)
 *   05-Jun-1997: Write "park" to both A and B outputs (so this same
 *                function works with two axis devices).               (smb)
 *-
 */

long 
compCad(struct cadRecord *pCad, CompCadFunction preset)
{
	long lParms;
	CompParms *pParms;
	long status = CAD_ACCEPT;          /* return status */

	/*
	 * Make sure that we have a valid data buffer.
	 */
	
	if (status == CAD_ACCEPT) {
		status = compLong(pCad->t, &lParms);
		if (status == CAD_REJECT) {
			strcpy(pCad->mess, "Invalid parameters");
			cicsLogFmt(CICS_DB_ERROR, "%s.T: Invalid parameters", pCad->name);
		}
	}

	if (status == CAD_ACCEPT)
		pParms = (CompParms *)lParms;

	if (status == CAD_ACCEPT && pParms == NULL) {
		status = CAD_REJECT;
		strcpy(pCad->mess, "No parameters");
		cicsLogFmt(CICS_DB_ERROR, "%s.T: No parameters", pCad->name);
	}

	/*
	 * Read the ignore field.
	 */
	
	if (status == CAD_ACCEPT) {
		status = compLong(pCad->r, &pParms->ignore);
		if (status == CAD_REJECT) {
			strcpy(pCad->mess, "Invalid ignore field");
			cicsLogFmt(CICS_DB_ERROR, "%s.R: Invalid ignore field", pCad->name);
		}
	}

	/*
	 * Read the datumed field.
	 */
	
	if (status == CAD_ACCEPT && !pParms->ignore) {
		status = compLong(pCad->s, &pParms->datumed);
		if (status == CAD_REJECT) {
			strcpy(pCad->mess, "Invalid datumed field");
			cicsLogFmt(CICS_DB_ERROR, "%s.S: Invalid datumed field",
				pCad->name);
		}
	}

	/*
	 * Switch according to the CAD directive in DIR field
	 */

	if (status == CAD_ACCEPT) {
		switch (pCad->dir) {
		case CAD_MARK:
			/*
			 * CAD MARK directive detected. Nothing needs to be done.
			 */

			cicsLogFmt(CICS_DB_MIN, "%s: MARK directive", pCad->name);

			break;

		case CAD_PRESET:
			/*
			 * CAD PRESET directive detected. There are no command arguments
			 * to be checked.
			 */

			cicsLogFmt(CICS_DB_MIN, "%s - PRESET directive", pCad->name);

			/*
			 * Reject the command if the mechanism has not been datumed.
			 */

			if (status == CAD_ACCEPT
					&& !pParms->ignore
					&& pParms->datumed == EPICS_FALSE) {
				cicsLogFmt(CICS_DB_ERROR, "%s: Not datumed", pCad->name);
				strncpy(pCad->mess, "OIWFS Not datumed", MAX_STRING_SIZE);
				status = CAD_REJECT;
			}

			/*
			 * Now execute a record-specific routine.
			 */

			if (status == CAD_ACCEPT && preset != COMP_CAD_NULL)
				status = preset(pCad, pParms);

			break;

		case CAD_CLEAR:
			/* 
			 * CAD CLEAR directive detected.
			 */

			cicsLogFmt(CICS_DB_MIN, "%s: CLEAR directive", pCad->name);

			break;

		case CAD_START:
			/*
			 * CAD START directive detected. Do nothing. The directive is being
			 * monitored by the CICS sequence code, which will start the
			 * appropriate action.
			 */

			cicsLogFmt(CICS_DB_MIN, "%s: START directive", pCad->name);

			break;

		case CAD_STOP:
			/*
			 * CAD STOP directive detected. Do nothing. The directive is being
			 * monitored by the CICS sequence code, which will stop the appropriate
			 * action.
			 */

			cicsLogFmt(CICS_DB_MIN, "%s: STOP directive", pCad->name);

			break;

		default:
			/* 
			 * Unrecognised CAD directive detected. This is regarded as an error.
			 */

			cicsLogFmt(CICS_DB_ERROR,
				"%s - Unrecognised CAD directive", pCad->name);
			strncpy(pCad->mess, "Unrecognized CAD directive", MAX_STRING_SIZE);
			status = CAD_REJECT;

			break;
		}
	}

	return status;
}

/* ===================================================================== */

/*+
 *   Function name:
 *   CADcompInit - Initialise "select position" command.
 *
 *   Purpose:
 *   This routine is called whe compdSel CAD record is initialized.
 *   It ensures that the A output of the CAD record is of type STRING.
 *
 *   Invocation:
 *   struct cadRecord *pcad;
 *   status = CADcompInit( pcad );
 *
 *   Parameters in:
 *
 *   Parameters out:
 *      < pcad->ftva  *string     Type of CAD output A.
 *      < pcad->ftvb  *string     Type of CAD output B.
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
 *
 *   Limitations:
 *
 *   Authors:
 *   Bret Goodrich (goodrich@gemini.edu)
 *   Steven Beard  (smb@roe.ac.uk)
 *   Hubert Yamada (yamada@newton.ifa.hawaii.edu)
 *
 *   History:
 *   01-Oct-1996: Original version.                            (goodrich)
 *   29-May-1997: Standard prologue and comments added.        (smb)
 *   15-Oct-1999: Increased number of input and output fields. (yamada)
 *   08-Nov-1999: Can't change data type in init subroutine    (yamada)
 *   08-Nov-1999: Added initialization for parameter array     (yamada)
 *-
 */

long
CADcompInit(struct cadRecord *pCad)
{
	STATUS status = CAD_ACCEPT;

	/*
	 * Make sure that the inputs/outputs are of the expected data type.
	 */

    if (status == CAD_ACCEPT && pCad->ftva != DBF_STRING) {
		status = CAD_REJECT;
		cicsLogFmt(CICS_DB_ERROR,
			"%s.FTVA: Output type not string", pCad->name);
	}

    if (status == CAD_ACCEPT && pCad->ftvb != DBF_STRING) {
		status = CAD_REJECT;
		cicsLogFmt(CICS_DB_ERROR,
			"%s.FTVB: Output type not string", pCad->name);
	}

    if (status == CAD_ACCEPT && pCad->ftvc != DBF_STRING) {
		status = CAD_REJECT;
		cicsLogFmt(CICS_DB_ERROR,
			"%s.FTVC: Output type not string", pCad->name);
	}

    if (status == CAD_ACCEPT && pCad->ftvc != DBF_STRING) {
		status = CAD_REJECT;
		cicsLogFmt(CICS_DB_ERROR,
			"%s.FTVC: Output type not string", pCad->name);
	}

    return status;
}

/*+
 *   Function name:
 *   CADcompNop
 *
 *   Purpose:
 *   This routine is called whenever the CAD record which needs no
 *   argument validation is processed. It checks that the command is
 *   acceptable and returns a status and a message, which are written
 *   to the VAL and MESS fields of the CAD record.
 *
 *   Invocation:
 *   struct cadRecord *pCad;
 *   status = CADcompNop(pCad);
 *
 *   Parameters in:
 *      > pCad->dir   *string     CAD directive
 *      > pCad->a     *string     CAD input A (Ignored)
 *      > pCad->b     *string     CAD input B (Ignored)
 *      > pCad->c     *string     CAD input C (Ignored)
 *      > pCad->d     *string     CAD input D (Ignored)
 *      > pCad->e     *string     CAD input E (Non-zero == Ignore datum field)
 *      > pCad->f     *string     CAD input F (Zero == Mechanism is datumed)
 *      > pCad->g     *string     CAD input G (Lower Eng limit, ignored)
 *      > pCad->h     *string     CAD input H (Upper Eng limit, ignored)
 *
 *   Parameters out:
 *      < pCad->mess  *string     Failure message written to CAD MESS field
 *      < pCad->vala  *string     CAD output A (SNL Arg, Unused)
 *      < pCad->valb  *string     CAD output B (Unused)
 *      < pCad->valc  *string     CAD output C (Unused)
 *      < pCad->vald  *string     CAD output D (Unused)
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
 *       Hubert Yamada (yamada@newton.ifa.hawaii.edu)
 *
 *   History:
 *       1999-10-13 hty Original version
 *-
 */

long 
CADcompNop(struct cadRecord *pCad) 
{
	long status = CAD_ACCEPT;

	if (status == CAD_ACCEPT)
		status = compCad(pCad, COMP_CAD_NULL);

	return status;
}

/* Gensub */

/* u is reserved for a long containing a pointer to the data array */

/* ugly code */

long
compParmsInit(struct genSubRecord *pSub)
{
	STATUS status = PASS;
	int i;
	const unsigned short *const pFt = &pSub->fta;
	
	for (i = 0; i <= 'L' - 'A'; i++) {
		if (status == PASS && pFt[i] != DBF_LONG) {
			status = FAIL;
			cicsLogFmt(CICS_DB_ERROR,
				"%s.FT%c: Input type not long", pSub->name, 'A' + i);
		}
	}

	for (i = 'M' - 'A'; i <= 'T' - 'A'; i++) {
		if (status == PASS && pFt[i] != DBF_DOUBLE) {
			status = FAIL;
			cicsLogFmt(CICS_DB_ERROR,
				"%s.FT%c: Input type not double", pSub->name, 'A' + i);
		}
	}

	/* 
	 * Create parameter storage area.
	 *
	 * This is evil: It assumes that a pointer will fit in a long.
	 */

	if (status == PASS && pSub->ftvj != DBF_LONG) {
		status = FAIL;
		cicsLogFmt(CICS_DB_ERROR,
			"%s.FTVJ: Input type not long", pSub->name);
	}

	if (status == PASS) {
		*(long *)pSub->valj = (long)calloc(1, sizeof(CompParms));
		if (*(long *)pSub->valj == NULL) {
			cicsLogFmt(CICS_DB_ERROR,
				"%s.FTVJ: Could not allocate parameter buffer", pSub->name);
			status = FAIL;
		}
	}

	return status;
}

long
compParms(struct genSubRecord *pSub)
{
	STATUS status = PASS;
	CompParms *const pParms = (CompParms *)*(long *)(pSub->valj);

	if (pParms == NULL) {
		status = FAIL;
	}

	if (status == PASS) {
		pParms->mech[0].cyclic = *(long *)pSub->a;
		pParms->mech[0].min = *(long *)pSub->b;
		pParms->mech[0].max = *(long *)pSub->c;
		pParms->mech[1].cyclic = *(long *)pSub->d;
		pParms->mech[1].min = *(long *)pSub->e;
		pParms->mech[1].max = *(long *)pSub->f;
		pParms->mech[2].cyclic = *(long *)pSub->g;
		pParms->mech[2].min = *(long *)pSub->h;
		pParms->mech[2].max = *(long *)pSub->i;
		pParms->mech[3].cyclic = *(long *)pSub->j;
		pParms->mech[3].min = *(long *)pSub->k;
		pParms->mech[3].max = *(long *)pSub->l;
		pParms->inp[0] = *(double *)pSub->m;
		pParms->inp[1] = *(double *)pSub->n;
		pParms->inp[2] = *(double *)pSub->o;
		pParms->inp[3] = *(double *)pSub->p;
		pParms->inp[4] = *(double *)pSub->q;
		pParms->inp[5] = *(double *)pSub->r;
		pParms->inp[6] = *(double *)pSub->s;
		pParms->inp[7] = *(double *)pSub->t;
	}
	
	pSub->val = status;

	return status;
}

double
compTime()
{
	double curTime;

	if ((compUseVxTime == EPICS_FALSE && timeNow(&curTime) == OK)
			|| (compUseVxTime == EPICS_TRUE && vxTime(&curTime) == OK))
		return curTime;
	else
		return 0.0;
}

static int
vxTime(double *pTime)
{
	struct timespec ts;
	int status = OK;

	if (status == OK && clock_gettime(CLOCK_REALTIME, &ts) != 0)
		status = ERROR;

	if (status == OK)
		*pTime = ts.tv_sec + ts.tv_nsec * 1.0E-9;

	return status;
}
