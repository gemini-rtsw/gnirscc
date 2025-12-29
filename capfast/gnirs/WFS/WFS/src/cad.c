#include <cad.h>
#include <cadRecord.h>

#include <cicsLib.h>
#include <cicsConst.h>
#include <compLib.h>

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

static long wfsSetTolerance(struct cadRecord *, const CompParms *pParms);

long 
CADwfsSetTolerance(struct cadRecord *pCad) 
{
	long status = CAD_ACCEPT;

	/*
	 * Not implemented yet
	 */
	
	if (status == CAD_ACCEPT)
		status = compCad(pCad, wfsSetTolerance);

	return status;
}

static long
wfsSetTolerance(struct cadRecord *pCad, const CompParms *pParms)
{
	double a;
	double b;
	double c;
	STATUS status = CAD_ACCEPT;

	if (status == CAD_ACCEPT) {
		status = compDouble(pCad->a, &a);
		if (status == CAD_REJECT) {
			cicsLogFmt(CICS_DB_ERROR, "%s.A:  Illegal value", pCad->desc);
			strcpy(pCad->mess, "A: Illegal values");
		}
	}

	if (status == CAD_ACCEPT) {
		a = a * pParms->inp[0] + pParms->inp[1];
		sprintf(pCad->valm, "%ld", (long)a);
	}

	if (status == CAD_ACCEPT) {
		status = compDouble(pCad->b, &b);
		if (status == CAD_REJECT) {
			cicsLogFmt(CICS_DB_ERROR, "%s.B:  Illegal value", pCad->desc);
			strcpy(pCad->mess, "B: Illegal values");
		}
	}

	if (status == CAD_ACCEPT) {
		b = b * pParms->inp[2] + pParms->inp[3];
		sprintf(pCad->valo, "%ld", (long)b);
	}

	if (status == CAD_ACCEPT) {
		status = compDouble(pCad->c, &c);
		if (status == CAD_REJECT) {
			cicsLogFmt(CICS_DB_ERROR, "%s.C:  Illegal value", pCad->desc);
			strcpy(pCad->mess, "C: Illegal values");
		}
	}

	if (status == CAD_ACCEPT) {
		c = c * pParms->inp[4] + pParms->inp[5];
		sprintf(pCad->valq, "%ld", (long)c);
	}
	
	return status;
}
