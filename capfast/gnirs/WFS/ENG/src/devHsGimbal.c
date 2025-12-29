static char rcsid[] = "$Id: devHsGimbal.c,v 1.2 2009/05/27 19:34:42 fkraemer Exp $";

/*
 * Copyright 1999, University of Hawaii, Institute for Astronomy.  All
 * rights reserved.
 *
 * The University of Hawaii grants AURA a non-exclusive license to
 * use this software, as stated in [contract].
 *
 * Author: Hubert Yamada, University of Hawaii, Institute for Astronomy
 *
 * Device Support Routines for Hall-Effect/Stepper-motor control record,
 * gimbal mirrors.
 *
 * FILENAME
 *     devHallStepGimbal.c
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

#include <hallStepRecord.h>
#include "ifaErrors.h"
#include "recHallStep.h"

#define MONITORMASK (DBE_VALUE | DBE_LOG)

static long move(HallStepRecord *);
static long datum(HallStepRecord *);
static long redatum(HallStepRecord *);
static long cycleN(HallStepRecord *, int);
static long update(HallStepRecord *);
static long verify(HallStepRecord *);
#define init_record    ((DEVSUPFUN)0)
#define init           ((DEVSUPFUN)0)
#define report         ((DEVSUPFUN)0)
#define get_ioint_info ((DEVSUPFUN)0)
#define directMove     ((DEVSUPFUN)0)

DevHallStep devHallStepGimbal = {
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

static long roughPeak(HallStepRecord *, struct link *, long, long *);

/*
 * TODO: These shouldn't be hard coded.
 *
 * The gimbal is unusual, because the rough calibration requires
 * a search for the translation sensor peak, followed by a finer
 * search for the home sensor peak.
 *
 * The parameters defined below define constants which are used
 * while searching for the peak of the translation sensor.
 *
 * Note:  It is assumed that even if we are at the soft limits, we
 * normally can move a distance, COARSE.  Also, assume
 *
 * WIDTH, COARSE, and MARGIN should not be coded here, but the HallStep
 * record is too complicated as it is.  COARSE should be a lot smaller
 * than WIDTH, and MARGIN should be large enough to insure that the
 * limits set by the approximate zero are within the hard, physical
 * limits.
 */

#define COARSE (10000) /* Step size for initial search */
#define WIDTH (150000) /* Width of peak */
#define MARGIN (30000) /* How close to the limits is is safe to rough search */

/*
 * We need to search a little further than usual when looking for the gear
 * sensor, because the rough position is so uncertain.  This range actually
 * overlaps with itself a little, so that it will include two gear peaks.
 * This could be a problem, if the peak is near the edge of the range.
 * (Note: One full cycle of the gear sensor is about 11,100 steps).
 */

#define SEARCH_RANGE (6000)



/*
 *+
 * FUNCTION NAME: datum
 *
 * INVOCATION: datum(phs)
 *
 * PARAMETERS:
 *
 *     (!) phs (HallStepRecord *) HallStep Record
 *
 * FUNCTION VALUE: (long) status value.  Non-zero indicates an error.
 *
 * SCOPE: static (devHsGimbal.c)
 *
 * PURPOSE: Initiate search for a datum value.
 *
 *     Not intended to be called directly; used to provide device
 *     support.
 *
 * DESCRIPTION:
 *
 *     Look for the zero of the primary linear sensor (or the backup
 *     linear sensor if the primary is disabled).  Offset from the
 *     sensor zero to the expected location of the gear sensor, and
 *     search for the peak value of the gear sensor.
 *
 * EXTERNAL VARIABLES: none
 *
 * PRIOR REQUIREMENTS:
 *
 *     Must always be called with the database locked (use dbScanLock).
 *
 * DEFICIENCIES:
 *
 *     Insufficient error checking.  Will not discover a problem if
 *     it is unable to locate the peak.
 *
 *     TODO: The primary sensors are currently non-functional.  This
 *     needs to change when the hardware becomes available.
 *-
 */

static long
datum(HallStepRecord *phs)
{
	long pos;
	long status = 0;

	/*
	 * Begin by finding a very rough peak position by brute force.  Set the
	 * home to the rough peak position.  This will make it possible to
	 * do a more sophisticated search without worrying about hitting
	 * a limit switch.  Assumes that the gear-sensor peak is close enough
	 * to the true home so that we can travel a significant distance on
	 * both sides.
	 */

	if (!status) {
		hsStatus(phs, HS_STATUS, "Rough peak");
		if (phs->en2p) {
			status = roughPeak(phs, &phs->hs2p, phs->dofp, &pos);
		} else if (phs->en2b) {
			status = roughPeak(phs, &phs->hs2b, phs->dofb, &pos);
		} else {
			hsStatus(phs, HS_ERROR, "No valid sensor 2");
			status = S_ifa_hs_MissingSensor;
		}
	}

	if (!status) {
		hsStatus(phs, HS_STATUS, "Search for peak");
		if (phs->en1p) {
			if (!status) {
				status = hsMax(phs, &phs->hs1p, &pos, phs->wthp, 1, 0, 0,
					SEARCH_RANGE);
			}
		} else if (phs->en1b) {
			pos -= phs->dof1;
			if (!status) {
				status = hsMax(phs, &phs->hs1b, &pos, phs->wthb, 1, 0, 0,
					SEARCH_RANGE);
			}
			pos += phs->dof1;
		} else {
			hsStatus(phs, HS_ERROR, "No valid sensor 1");
			status = S_ifa_hs_MissingSensor;
		}
	}

	if (!status)
		status = hsSetHome(phs, pos - phs->zero);

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
 *     (!) phs (HallStepRecord *) HallStep record
 *
 * FUNCTION VALUE: (long) status value.  Non-zero indicates an error.
 *
 * SCOPE: static (devHsGimbal.c)
 *
 * PURPOSE: Move to a new location.
 *
 *     Not intended to be called directly; used to provide device
 *     support.
 *
 * DESCRIPTION:
 *
 *     Read a desired destination value and move there.
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
move(HallStepRecord *phs)
{
	long status = 0;
	long motorPos;

	hsStatus(phs, 1, ">>>> " __FILE__ ":move(%p)\n", phs);

	if (!status && !phs->raw) {
		if (phs->dest < phs->lopr)
			phs->dest = phs->lopr;
		else if (phs->dest > phs->hopr)
			phs->dest = phs->hopr;
	}

	if (!status)
		status = hsGetLinkValueLong(phs, &phs->imps, &motorPos);

	/*
	 * Raw moves include no backlash compensation.
	 *
	 * Otherwise, there is backlash compensation.  To force backlash
	 * removal without changing the nominal position, a move of 0
	 * counts is permitted.
	 */

	if (!status) {
		if (!phs->raw) {
			status = hsMoveTo(phs, phs->dest - phs->blsh, 1);

			if (!status)
				status = hsMoveTo(phs, phs->dest, 1);
		} else if (motorPos != phs->dest) {
			status = hsMoveTo(phs, phs->dest, 1);
		}
	}

	hsStatus(phs, 1, "<<<< " __FILE__ ":move return %d\n", status);

	return status;
}



/*
 *+
 * FUNCTION NAME: redatum
 *
 * INVOCATION: redatum(phs)
 *
 * PARAMETERS:
 *
 *     (!) phs (HallStepRecord *) HallStep record
 *
 * FUNCTION VALUE: (long) status value.  Non-zero indicates an error.
 *
 * SCOPE: static (devHsGimbal.c)
 *
 * PURPOSE: Look for positioning errors.
 *
 *     Not intended to be called directly; used to provide device
 *     support.
 *
 * DESCRIPTION:
 *
 *     Move to the current zero value and carry out the peak finding
 *     algorithm.  Display the error.
 *
 * EXTERNAL VARIABLES: none
 *
 * PRIOR REQUIREMENTS:
 *
 *     Must always be called with the database locked (use dbScanLock).
 *
 * DEFICIENCIES:
 *
 *     None known.
 *-
 */

static long
redatum(HallStepRecord *phs)
{
	int status = 0;
	long pos;

	/*
	 * Begin by finding a very rough peak position by brute force.  Set the
	 * home to the rough peak position.  This will make it possible to
	 * do a more sophisticated search without worrying about hitting
	 * a limit switch.  Assumes that the gear-sensor peak is close enough
	 * to the true home so that we can travel a significant distance on
	 * both sides.
	 */

	if (!status) {
		hsStatus(phs, HS_STATUS, "Rough peak");
		if (phs->en2p) {
			status = roughPeak(phs, &phs->hs2p, phs->dofp, &pos);
		} else if (phs->en2b) {
			status = roughPeak(phs, &phs->hs2b, phs->dofb, &pos);
		} else {
			hsStatus(phs, HS_ERROR, "No valid sensor 2");
			status = S_ifa_hs_MissingSensor;
		}
	}

	if (!status) {
		hsStatus(phs, HS_MESS, "Rough home at %ld", pos);
		hsStatus(phs, 0, "Rough home at %ld\n", pos);
	}

	if (phs->en1p) {
		if (!status) {
			hsStatus(phs, HS_STATUS, "Narrow search for peak");
			status = hsMax(phs, &phs->hs1p, &pos, phs->wthp, 1, 0, 0, 
				SEARCH_RANGE);
		}
	} else if (phs->en1b) {
		if (!status) {
			pos -= phs->dof1;
			hsStatus(phs, HS_STATUS, "Narrow search for peak");
			status = hsMax(phs, &phs->hs1b, &pos, phs->wthb, 1, 0, 0, 
				SEARCH_RANGE);
			pos += phs->dof1;
		}
	} else {
		hsStatus(phs, HS_ERROR, "No valid sensor 1");
		status = S_ifa_hs_MissingSensor;
    }

	if (!status) {
		hsStatus(phs, HS_MESS, "Home at %ld", pos - phs->zero);
		phs->rdtm = pos - phs->zero;
	}

	return status;
}



/*
 *+
 * FUNCTION NAME: cycleN
 *
 * INVOCATION: cycleN(phs, nRep)
 *
 * PARAMETERS:
 *
 *     (!) phs (HallStepRecord *) HallStep record
 *     (>) nRep (int) Number of repetitions
 *
 * FUNCTION VALUE: (long) status value (non-zero indicates an error)
 *
 * SCOPE: static (devHsGimbal.c)
 *
 * PURPOSE: Exercise a piece of hardware 'nRep' times.
 *
 *     This is a diagnostic routine which is used to verify that a
 *     piece of hardware is fully operational, to verify that stepper
 *     motors are working properly without losing counts, and to
 *     insure that a mechanism will work properly for an extended
 *     period of time.
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
 *
 *     None known.
 *-
 */



static long
cycleN(HallStepRecord *phs, int nRep)
{
	long status = 0;

	if (!phs->raw
			&& (   phs->cdsa < phs->lopr || phs->cdsa > phs->hopr
			    || phs->cdsb < phs->lopr || phs->cdsb > phs->hopr
			    || phs->cdsc < phs->lopr || phs->cdsc > phs->hopr)) {
		status = S_ifa_hs_OutOfRange;
		hsStatus(phs, HS_ERROR, "Cycle out of range");
	}

	if (!status)
		status = hsCycleN(phs, nRep, 1);

	return status;
}



/*
 *+
 * FUNCTION NAME: roughPeak
 *
 * INVOCATION: roughPeak(phs, pLink, pPos)
 *
 * PARAMETERS:
 *     (!) phs (HallStepRecord *) HallStep record
 *     (!) pLink (struct link *) Link to sensor record
 *     (>) dof (long) Rough estimate of home position relative to peak
 *     (!) pPos (long *) Starting position, changed to zero position
 *
 * FUNCTION VALUE:
 *
 * SCOPE: static (devHsGimbal.c)
 *
 * PURPOSE: Find a home position for the translation sensor.
 *
 * DESCRIPTION:
 *
 *     Read the current value of the translation sensor (or the backup
 *     translation sensor).  Find a rough peak, refine it, and return
 *     an estimate of the home position.
 *
 * EXTERNAL VARIABLES: none
 *
 * PRIOR REQUIREMENTS:
 *
 *     Must always be called with the database locked (use dbScanLock).
 *
 * DEFICIENCIES:
 *
 *     This will not detect a failed sensor.  A failed sensor could
 *     drive the hardware into a hard stop.
 *-
 */

static long
roughPeak(HallStepRecord *phs, struct link *pLink, long dof, long *pPos)
{
	int i;
	long status = 0;
	double val0;
	double val1;
	int incr;

	hsStatus(phs, 1, ">>>> " __FILE__ ":roughPeak(%p,%p)\n", phs, pPos);

	incr = COARSE;

	if (!status) {
		status = hsGetLinkValueDouble3(phs, pLink, &val0);
		hsStatus(phs, 2, "++++ i=%d val0=%g\n", i, val0);
	}

	if (!status) {
		status = hsGetLinkValueLong(phs, &phs->imps, pPos);
		hsStatus(phs, 2, "Current position is %ld\n", *pPos);
	}

	if (!status) {
		*pPos += COARSE;
		status = hsMoveTo(phs, *pPos, 1);

		if (status) { /* Hit a limit switch?  Reverse direction. */
			hsStatus(phs, 0, "Hit a limit: val0 -> val1\n");
			hsStatus(phs, HS_STATUS, "Try to back out of limit");

			*pPos -= COARSE * 2;
			status = hsMoveTo(phs, *pPos, 1);

			if (!status) {
				hsStatus(phs, HS_ERROR, "");
				val1 = val0;

				status = hsGetLinkValueDouble3(phs, pLink, &val0);
				hsStatus(phs, 2, "++++ i=%d val0=%g\n", i, val0);
			}
		} else {
			status = hsGetLinkValueDouble3(phs, pLink, &val1);
			hsStatus(phs, 2, "++++ i=%d val1=%g\n", i, val1);
		}
	}

	/*
	 * Figure out which direction is increasing
	 */


	if (val1 < val0)
		incr = -COARSE;
	else
		incr = COARSE;

	/*
	 * Step until we pass the peak
	 */

	do {
		val0 = val1;

		if (!status) {
			*pPos += incr;
			status = hsMoveTo(phs, *pPos, 1);
		}

		if (!status) {
			status = hsGetLinkValueDouble3(phs, pLink, &val1);
			hsStatus(phs, 2, "++++ COARSE *pPos=%ld val0=%g val1=%g\n",
				*pPos, val0, val1);
		}
	} while (val1 > val0 && !status);

	/*
	 * Begin finer approach from a consistent direction, then use
	 * the standard peak finding routine.
	 */

	if (!status) {
		*pPos -= 2 * COARSE;
		status = hsMoveTo(phs, *pPos, 1);
	}

	/*
	 * Now we can use the standard peak finding routine
	 */

	if (!status) {
		hsStatus(phs, HS_STATUS, "Refine peak");
		status = hsMax(phs, pLink, pPos, WIDTH, 1,
			*pPos + dof + phs->lopr + MARGIN,
			*pPos + dof + phs->hopr - MARGIN, 0);
	}

	/*
	 * Now add the offset of the home position from the sensor peak.
	 */

	*pPos += dof;

	return status;
}

static long
update(HallStepRecord *phs)
{
	double hs2p, hs2b;
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
		if (phs->en2p)
			status = hsGetLinkValueDouble3(phs, &phs->hs2p, &hs2p);
		else
			hs2p = phs->vvlp;
	}
	
	if (!status) {
		if (phs->en2b)
			status = hsGetLinkValueDouble3(phs, &phs->hs2b, &hs2b);
		else
			hs2b = phs->vvlb;
	}

	if (!status)
		status = hsGetLinkValueLong(phs, &phs->imps, &imps);

	/*
	 * Write values to output fields, for use by the inverse lut
	 */

	if (!status) {
		phs->rvlp = hs2p;
		db_post_events(phs, &phs->rvlp, MONITORMASK);

		phs->rvlb = hs2b;
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
	double hs2p, hs2b;
	long status;

	status = 0;

	/*
	 * Verify that the sensor value matches the expected value, within
	 * the specified tolerance.  (A zero tolerance means that the
	 * value should not be checked.)
	 */

	if (!status && phs->vtol > 0.0 && fabs(hs2p - phs->vvlp) > phs->vtol
			&& phs->en2p) {
		hsStatus(phs, HS_ERROR, "Position verify fail (p)");
		status = S_ifa_hs_VerifyFailed;
	}

	if (!status && phs->vtol > 0.0 && fabs(hs2b - phs->vvlb) > phs->vtol
			&& phs->en2b) {
		hsStatus(phs, HS_ERROR, "Position verify fail (b)");
		status = S_ifa_hs_VerifyFailed;
	}

	return status;
}
