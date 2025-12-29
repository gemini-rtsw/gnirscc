static char rcsid[] = "$Id: devHsBinary.c,v 1.2 2009/05/27 19:34:42 fkraemer Exp $";

/*
 * Copyright 1997 University of Hawaii, Institute for Astronomy.  All
 * rights reserved.
 *
 * The University of Hawaii grants AURA a non-exclusive license to
 * use this software, as stated in [contract].
 *
 * Author: Hubert Yamada, University of Hawaii, Institute for Astronomy
 *
 * Device Support Routines for Hall-Effect/Stepper-motor control record,
 * binary motion component.  This record is used to control the
 * pupil viewer, which immediately moves to one of two stable locations
 * when motor power is removed.
 *
 * FILENAME
 *     devHallStepBinary.c
 *
 * FUNCTION NAME(S)
 */

/*
 * Note: For binary mechanisms, the range of travel is deliberately
 * set to be a little larger than the actual physical range of travel,
 * to insure that the mechanism is forced into the stop.  Also, WTHP
 * and WTHB and PKB are the width and height of the peak, if we
 * assume that the peak is reflected around the origin.  It is assumed
 * that it is acceptable to overrun the endpoints by a significant
 * fraction of the width of the peak.  Since there is no comparator
 * PKB is the minimum of the values of both the primary and
 * backup peaks.  BLSH is not normally used, except during verification,
 * where it is used to determine how much overshoot to use to insure
 * that the mechanism is actually in its home position.
 *
 * Note:  It is important to datum or cycle this mechanism after it is
 * powered on, if there is any chance that the mechanism has been moved,
 * to insure that the mechanism is forced into a hard stop.
 */

#include <dbCommon.h>
#include <dbEvent.h>
#include <recSup.h>
#include <devSup.h>
#include <time.h>
#include <string.h>
#include <limits.h>
#include <sys/types.h>

#include <hallStepRecord.h>
#include "recHallStep.h"
#include "ifaErrors.h"

#define MONITORMASK (DBE_VALUE | DBE_LOG)

/*
 * The redatum command isn't provided for this mechanism, because
 * there is no peak search.  We just run into the hard stop.
 */

static long move(HallStepRecord *);
static long datum(HallStepRecord *);
static long update(HallStepRecord *);
static long verify(HallStepRecord *);
static long cycleN(HallStepRecord *, int);
static long directMove(HallStepRecord *);
#define init_record    ((DEVSUPFUN)0)
#define init           ((DEVSUPFUN)0)
#define report         ((DEVSUPFUN)0)
#define redatum        ((DEVSUPFUN)0)
#define get_ioint_info ((DEVSUPFUN)0)

DevHallStep devHallStepBinary = {
	HS_DEVNUM,
	report,
	init,
	init_record,
	get_ioint_info,
	move,
	datum,
	redatum,
	cycleN,
	update,
	verify,
	directMove,
};



/*
 *+
 * FUNCTION NAME: datum
 *
 * INVOCATION: datum(phs)
 *
 * PARAMETERS:
 *
 *     (!) phs (HallStepRecord *)
 *
 * FUNCTION VALUE:
 *
 * PURPOSE:
 *
 * DESCRIPTION:
 *
 * EXTERNAL VARIABLES:
 *
 * PRIOR REQUIREMENTS:
 *
 * DEFICIENCIES:
 *-
 */

static long
datum(HallStepRecord *phs)
{
	long pos;
	long limit;
	long status = 0;
	double hs1p;
	double hs1b;

	/*
	 * Get the current location.
	 */

	if (!status) {
		status = hsGetLinkValueLong(phs, &phs->imps, &pos);
		limit = pos + phs->hopr - phs->lopr + phs->blsh + phs->wthp;
	}

	if (!status) {
		if (phs->en1p) {
			while (!status && pos <= limit) {
				status = hsGetLinkValueDouble3(phs, &phs->hs1p, &hs1p);

				if (!status) {
					if (hs1p > phs->thrp) { /* Found the home sensor */
						break;
					} else { /* Move, and try again */
						pos += phs->wthp / 3.0;
						status = hsMoveTo(phs, pos, 1);
					}
				}
			}

			/*
			 * Force it into the hard stop.  The backlash parameter
			 * is handled a little differently for this mechanism.
			 * It is used as an overshoot to insure that we have
			 * moved into the hard stop.
			 */

			if (!status) {
				pos += phs->wthp + phs->blsh; 
				status = hsMoveTo(phs, pos, 1);
			}
		} else if (phs->en1b) {
			while (!status && pos <= limit) {
				status = hsGetLinkValueDouble3(phs, &phs->hs1b, &hs1b);

				if (!status) {
					if (hs1b > phs->thrb) { /* Found the home sensor */
						break;
					} else { /* Move, and try again */
						pos += phs->wthb / 3.0;
						status = hsMoveTo(phs, pos, 1);
					}
				}
			}

			/*
			 * Force it into the hard stop.  The backlash parameter
			 * is handled a little differently for this mechanism.
			 * It is used as an overshoot to insure that we have
			 * moved into the hard stop.
			 */

			if (!status) {
				pos += phs->wthp + phs->blsh; 
				status = hsMoveTo(phs, pos, 1);
			}
		} else {
			hsStatus(phs, HS_ERROR, "No valid sensor");
			status = S_ifa_hs_MissingSensor;
		}
	}

	if (pos > limit) {
		hsStatus(phs, HS_ERROR, "Unable to find home");
		status = S_ifa_hs_MissingHome;
	}

	if (!status)
		status = hsSetHome(phs, pos);

	return status;
}



/*
 *+
 * FUNCTION NAME: move
 *
 * INVOCATION: move(phs)
 *
 * PARAMETERS:
 *
 *     (!) phs (HallStepRecord *)
 *
 * FUNCTION VALUE:
 *
 * PURPOSE:
 *
 * DESCRIPTION:
 *
 * EXTERNAL VARIABLES:
 *
 * PRIOR REQUIREMENTS:
 *
 * DEFICIENCIES:
 *
 *     static function
 *     ignores lopr
 *-
 */

static long
move(HallStepRecord *phs)
{
	long status = 0;
	long motorPos;

	hsStatus(phs, 1, ">>>> " __FILE__ ":move(%p)\n", phs);

	if (!status)
		status = hsGetLinkValueLong(phs, &phs->imps, &motorPos);

	if (!status) {
		phs->vmmp = phs->dest;
		db_post_events(phs, &phs->vmmp, MONITORMASK);

		status = hsMoveTo(phs, phs->dest, 1);
	}

	hsStatus(phs, 1, "<<<< " __FILE__ ":move return %d\n", status);

	return status;
}



/*
 *+
 * FUNCTION NAME: cycleN
 *
 * INVOCATION: cycleN(phs, nPass)
 *
 * PARAMETERS:
 *     (!) phs (HallStepRecord)
 *     (>) nPass (int)
 *
 * FUNCTION VALUE:
 *     (long) status value (non-zero indicates an error)
 *
 * PURPOSE:  
 *     Exercise a piece of hardware
 *
 *     This is a diagnostic routine which is used to verify that a
 *     piece of hardware is fully operational and for lifetime testing.  
 *
 * DESCRIPTION:
 *
 *     This routine monitors the Hall effect sensors and will shut 
 *     itself off automatically if any values have changed.  This 
 *     makes it useful for detecting infrequent, lost stepper-motor
 *     counts.  It is safe for unattended operation, because it 
 *     should stop within a short time if the hardware hits a 
 *     limit or is otherwise unable to move.
 *
 *     This subroutine moves the hardware to three pre-defined positions
 *     (normally, two limits and the home position, or two filters and 
 *     the home position).  It records the three sensor values.  It then
 *     repeatedly moves the hardware to each position, and verifies that
 *     the sensor values have not changed.
 *
 * EXTERNAL VARIABLES: none
 *
 * PRIOR REQUIREMENTS:
 *
 *     Must always be called with the database locked (use dbScanLock).
 *
 * DEFICIENCIES:
 *-
 */

static long
cycleN(HallStepRecord *phs, int nRep)
{
	return hsCycleN(phs, nRep, 0);
}

static long
update(HallStepRecord *phs)
{
	double hs1p, hs1b;
	long status;
	long imps;

	status = 0;

	/*
	 * Read values from other records.  If the sensor is disabled
	 * (normally, because of a hardware problem), pretend that
	 * we read the expected value, so that no error will be
	 * detected.
	 */

	if (!status) {
		if (phs->en1p)
			status = hsGetLinkValueDouble3(phs, &phs->hs1p, &hs1p);
		else
			hs1p = phs->vvlp;
	}
	
	if (!status) {
		if (phs->en1b)
			status = hsGetLinkValueDouble3(phs, &phs->hs1b, &hs1b);
		else
			hs1b = phs->vvlb;
	}

	if (!status)
		status = hsGetLinkValueLong(phs, &phs->imps, &imps);

	/*
	 * Write values to output fields, for use by the inverse lut
	 */

	if (!status) {
		phs->rvlp = hs1p;
		db_post_events(phs, &phs->rvlp, MONITORMASK);

		phs->rvlb = hs1b;
		db_post_events(phs, &phs->rvlb, MONITORMASK);

		phs->mmps = imps;
		db_post_events(phs, &phs->mmps, MONITORMASK);
	}

	return status;
}

/*
 * Assumes that update has already been called.
 */

static long
verify(HallStepRecord *phs)
{
	double hs1p, hs1b;
	long status;

	status = 0;

	/*
	 * Verify that the sensor value matches the expected value, within
	 * the specified tolerance.  (A zero tolerance means that the
	 * value should not be checked.)
	 */

	if (!status && phs->vtol > 0.0 && fabs(hs1p - phs->vvlp) > phs->vtol
			&& phs->en1p) {
		hsStatus(phs, HS_ERROR, "Position verify fail (p)");
		status = S_ifa_hs_VerifyFailed;
	}

	if (!status && phs->vtol > 0.0 && fabs(hs1b - phs->vvlb) > phs->vtol
			&& phs->en1b) {
		hsStatus(phs, HS_ERROR, "Position verify fail (b)");
		status = S_ifa_hs_VerifyFailed;
	}

	return status;
}

static long
directMove(HallStepRecord *phs)
{
	long status = 0;

	/*
     * This mechanism is not intended to be left in an intermediate
	 * position.
     */

	(void)hsPutLinkValueLong(phs, &phs->pser, 1L);

	return status;
}
