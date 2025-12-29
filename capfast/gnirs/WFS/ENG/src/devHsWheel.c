static char rcsid[] = "$Id: devHsWheel.c,v 1.2 2009/05/27 19:34:42 fkraemer Exp $";

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
 * wheel-like component.
 *
 * FILENAME
 *     devHallStepWheel.c
 *
 * FUNCTION NAME(S)
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

static long move(HallStepRecord *);
static long datum(HallStepRecord *);
static long redatum(HallStepRecord *);
static long update(HallStepRecord *);
static long verify(HallStepRecord *);
static long cycleN(HallStepRecord *, int);
#define init_record    ((DEVSUPFUN)0)
#define init           ((DEVSUPFUN)0)
#define report         ((DEVSUPFUN)0)
#define get_ioint_info ((DEVSUPFUN)0)
#define directMove     ((DEVSUPFUN)0)

DevHallStep devHallStepWheel = {
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

static long smartMoveTo(HallStepRecord *, long);



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
	long status = 0;

	if (phs->en1p) {
		/*
		 * Issue a HOMF, to get us in the vicinity of the peak.  After
		 * the move, we will _not_ be where we were sent; that's how 
		 * HOMF works.
		 *
		 * The comparator will trigger someplace on the peak.  If we were 
		 * already on the peak, the location will be arbitrary; we need to 
		 * refine our value carefully. 
		 */

		if (!status) {
			hsStatus(phs, HS_STATUS, "Rough datum search");
			status = hsSeekHome(phs, phs->tout);
		}
		pos = 0;

		/*
		 * Now that we have a reproducable starting point, refine our
		 * location.
		 */

		if (!status) {
			hsStatus(phs, HS_STATUS, "Narrow search for peak");
			status = hsMax(phs, &phs->hs1p, &pos, phs->wthp, 1, 0, 0, 0);
		}

		if (!status)
			status = hsSetHome(phs, pos);
	} else if (phs->en1b) {
		double val;
		int found;
		long i;
		long imps;

		/*
		 * Only one sensor can be connected to the home trigger on the
		 * oms board.  If the primary sensor is dead, we use our
		 * backup sensor with a slower, hunt and find algorithm.
		 */

		hsStatus(phs, HS_STATUS, "Rough datum search (slow)");

		found = 0;

		/*
		 * Start from current position.  Take steps that are small enough
		 * to insure that we will see the peak.  I assume that the
		 * peak is fairly reasonably shaped, so if I take steps which
		 * are 1/8 of the full-width at half maximum, I will find
		 * at lease one point that is half of the maximum height.  This
		 * is conservative, since I should be able to take steps that
		 * are four times this size, but the peak height and width
		 * are only rough numbers (also, the height changes a little
		 * with temperature), so this gives me a little room for error.
		 */

		if (!status)
			status = hsGetLinkValueLong(phs, &phs->imps, &imps);

		for (i = imps; 
				i < imps + phs->hopr - phs->lopr + phs->wthb * 2 && !status;
				i += phs->wthb / 8.0) {
			if (!status)
				status = hsMoveTo(phs, i, 1);

			if (!status)
				status = hsGetLinkValueDouble3(phs, &phs->hs1b, &val);

			if (!status && val > phs->thrb) {
				pos = i;
				found = 1;
				break;
			}
		}
		if (!status && !found) {
        	hsStatus(phs, HS_ERROR, "Cannot find rough home");
			status = S_ifa_hs_TimeOut;
		}

		if (!status) {
			hsStatus(phs, HS_STATUS, "Narrow search for peak");
			status = hsMax(phs, &phs->hs1b, &pos, phs->wthb, 1, 0, 0, 0);
		}

		if (!status)
			status = hsSetHome(phs, pos + phs->dof1);
	} else {
        hsStatus(phs, HS_ERROR, "No valid sensor 1");
        status = S_ifa_hs_MissingSensor;
    }

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

	/*
	 * In raw mode, we move to the specified coordinate value, without
	 * considering coordinate wraparound, and without worrying about
	 * backlash correction.
	 *
	 * Otherwise, we take advantage of coordinate wraparound and
	 * do backlash correction.  We always move, so that backlash
	 * correction is always done.
	 */

	if (phs->raw && !status && motorPos != phs->dest) {
		if (phs->dest > HS_MAX_STEP || phs->dest < -HS_MAX_STEP) {
        	hsStatus(phs, HS_ERROR, "Maximum step count exceeded");
			status = S_ifa_hs_OutOfRange;
		} else {
			phs->vmmp = phs->dest;
			db_post_events(phs, &phs->vmmp, MONITORMASK);

			status = hsMoveTo(phs, phs->dest, 1);
		}
	} else if (!phs->raw && !status) {
		phs->vmmp = phs->dest % phs->hopr;
		if (phs->vmmp < 0)
			phs->vmmp += phs->hopr;
		db_post_events(phs, &phs->vmmp, MONITORMASK);

		status = smartMoveTo(phs, motorPos);
	}

	hsStatus(phs, 1, "<<<< " __FILE__ ":move return %d", status);

	return status;
}



/*
 *+
 * FUNCTION NAME: redatum
 *
 * INVOCATION: redatum(phs)
 *
 * PARAMETERS:
 *     phs (HallStepRecord *)
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
 *-
 */

static long
redatum(HallStepRecord *phs)
{
	long pos;
	long status = 0;

	pos = 0.0;
	if (phs->en1p) {
		if (!status) {
			hsStatus(phs, HS_STATUS, "Search for peak");
			status = hsMax(phs, &phs->hs1p, &pos, phs->wthp, 1, 0, 0, 0);
		}
	} else if (phs->en1b) {
		if (!status) {
			hsStatus(phs, HS_STATUS, "Search for peak");
			status = hsMax(phs, &phs->hs1b, &pos, phs->wthb, 1, 0, 0, 0);
		}

		pos += phs->dof1;
	} else {
		hsStatus(phs, HS_ERROR, "No valid sensor 1");
		status = S_ifa_hs_MissingSensor;
	}

	if (!status) {
		hsStatus(phs, HS_MESS, "Peak at %ld", pos);
		phs->rdtm = pos;
	}

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
	return hsCycleN(phs, nRep, 1);
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

		phs->mmps = imps % phs->hopr;
		if (phs->mmps < 0)
			phs->mmps += phs->hopr;
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

/*
 * Move to the target, taking advantage of coordinate wraparound, and
 * doing backlash compenstaion.
 */

static long
smartMoveTo(HallStepRecord *phs, long motorPos)
{
	long delta;
	long dest;
	long status;

	status = 0;

	/*
	 * Figure out how much we need to move, taking advantage of
	 * the coordinate wraparound.  (The modulus is taken
	 * twice, because ansi C doesn't guarantee that the
	 * modulus is positive.)  It is assumed that lopr is 0.
	 */

	delta = (phs->dest - motorPos) % phs->hopr;
	if (delta < 0)
		delta +=phs->hopr;

	/*
	 * Find the most efficient way to move: forward or
	 * reverse.
	 */

	if (delta < phs->hopr / 2)
		dest = motorPos + delta;
	else
		dest = motorPos + delta - phs->hopr;

	/*
	 * If the destination is out of the legal range of the
	 * step counter, adjust the value.  Note that we assume that
	 * we are always starting from a legal position, so we
	 * will never have to adjust by more than one rotation of the
	 * wheel.
	 */

	if (dest > HS_MAX_STEP)
		dest -= phs->hopr;

	if (dest < -HS_MAX_STEP)
		dest += phs->hopr;

	/*
	 * Note that the direction of backlash correction must be consistent 
	 * with the direction in which home is found.  The final motion
	 * is always in an increasing direction.  Note that we always
	 * move, even if it is unnecessary, so that a move to the current
	 * position will remove any backlash.
	 */

	if (!status && phs->blsh)
		status = hsMoveTo(phs, dest - phs->blsh, 1);

	if (!status)
		status = hsMoveTo(phs, dest, 1);

	if (!status && phs->dest != dest) {
		phs->dest = dest;
		db_post_events(phs, &phs->dest, MONITORMASK);
	}

	return status;
}
