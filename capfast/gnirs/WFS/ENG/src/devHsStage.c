static char rcsid[] = "$Id: devHsStage.c,v 1.2 2009/05/27 19:34:42 fkraemer Exp $";

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
 * focusing stage.
 *
 * FILENAME
 *     devHallStepStage.c
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
#include "recHallStep.h"
#include "ifaErrors.h"

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

DevHallStep devHallStepStage = {
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

static long zeroSen(HallStepRecord *, struct link *, long, long *);



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
 * SCOPE: static (devHsStage.c)
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
 *-
 */

static long
datum(HallStepRecord *phs)
{
	long pos;
	long status = 0;

	if (!status) {
		hsStatus(phs, HS_STATUS, "Zero linear");
		if (phs->en2p) {
			status = zeroSen(phs, &phs->hs2p, phs->dslp, &pos);
			pos += phs->dofp;
		} else if (phs->en2b) {
			status = zeroSen(phs, &phs->hs2b, phs->dslb, &pos);
			pos += phs->dofb;
		} else {
			hsStatus(phs, HS_ERROR, "No valid sensor 2");
			status = S_ifa_hs_MissingSensor;
		}
	}

	if (!status) {
		hsStatus(phs, HS_STATUS, "Search for peak");
		if (phs->en1p) {
			if (!status)
				status = hsMax(phs, &phs->hs1p, &pos, phs->wthp, 1, 0, 0, 0);
		} else if (phs->en1b) {
			pos -= phs->dof1;
			if (!status)
				status = hsMax(phs, &phs->hs1b, &pos, phs->wthb, 1, 0, 0, 0);
			pos += phs->dof1;
		} else {
			hsStatus(phs, HS_ERROR, "No valid sensor 1");
			status = S_ifa_hs_MissingSensor;
		}
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
 *     (!) phs (HallStepRecord *) HallStep record
 *
 * FUNCTION VALUE: (long) status value.  Non-zero indicates an error.
 *
 * SCOPE: static (devHsStage.c)
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
	 * Update expected location (for purposes of verification).
	 */

	phs->vmmp = phs->dest;
	db_post_events(phs, &phs->vmmp, MONITORMASK);

	/*
	 * In raw mode, move directly to the target with no backlash
	 * compensation.
	 *
	 * Otherwise, do an extra move to remove backlash.  Note that
	 * a move to the current position is permitted, and has the
	 * effect of backlash removal.
	 */

	if (!status) {
		if (!phs->raw) {
			status = hsMoveTo(phs, phs->dest - phs->blsh, 1);

			if (!status)
				status = hsMoveTo(phs, phs->dest, 1);
		} else if (phs->raw && motorPos != phs->dest) {
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
 * SCOPE: static (devHsStage.c)
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

	pos = 0;

	if (phs->en1p) {
		if (!status) {
			hsStatus(phs, HS_STATUS, "Narrow search for peak");
			status = hsMax(phs, &phs->hs1p, &pos, phs->wthp, 1, 0, 0, 0);
		}
	} else if (phs->en1b) {
		if (!status) {
			hsStatus(phs, HS_STATUS, "Narrow search for peak");
			pos -= phs->dof1;
			status = hsMax(phs, &phs->hs1b, &pos, phs->wthb, 1, 0, 0, 0);
			pos += phs->dof1;
		}
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
 * INVOCATION: cycleN(phs, nRep)
 *
 * PARAMETERS:
 *
 *     (!) phs (HallStepRecord *) HallStep record
 *     (>) nRep (int) Number of repetitions
 *
 * FUNCTION VALUE: (long) status value (non-zero indicates an error)
 *
 * SCOPE: static (devHsStage.c)
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
 * FUNCTION NAME: zeroSen
 *
 * INVOCATION: zeroSen(phs, pLink, slope, pPos)
 *
 * PARAMETERS:
 *     (!) phs (HallStepRecord *) HallStep record
 *     (!) pLink (struct link *) Link to sensor record
 *     (>) slope (long) Slope of sensor value vs position
 *     (!) pPos (long *) Starting position, changed to zero position
 *
 * FUNCTION VALUE:
 *
 * SCOPE: static (devHsStage.c)
 *
 * PURPOSE: Find a zero value for the linear sensor.
 *
 * DESCRIPTION:
 *     
 *     Read the current value of the linear sensor (or the backup
 *     linear sensor).  Use the slope of the sensor vs. steppermotor
 *     position, and use that to extrapolate to zero.  Move to the
 *     zero position.  Repeat.
 *
 *     This works regardless of whether the sensor is enabled or
 *     disabled.
 *
 *     This will work, even in the region in which the sensor curve
 *     starts to peak, or where the sensor curve flattens out, provided
 *     that the shape of the curve is roughly a straight line.
 *
 * EXTERNAL VARIABLES: none
 *
 * PRIOR REQUIREMENTS:
 *
 *     Must always be called with the database locked (use dbScanLock).
 *
 * DEFICIENCIES:
 *
 *     This will not detect a failing sensor.  If the slope value is
 *     grossly incorrect this will fail to converge to the correct
 *     value.  If the slope value has the wrong sign, this can move
 *     the mechanism in the wrong direction, possibly into a hard stop.
 *
 *     Assumes that after ten iterations, it is close enough to
 *     the home position.  (Ten is probably overkill, but is so fast
 *     that it's not worth worrying about.)
 *-
 */

#define NITER (10)

static long
zeroSen(HallStepRecord *phs, struct link *pLink, long slope, long *pPos)
{
	int i;
	long status = 0;
	long cPos;
	double val;

	hsStatus(phs, 1, ">>>> " __FILE__ ":zeroSen(%p,%p)\n", phs, pPos);

	for (i = 0; i < NITER; i++) {
		if (!status) {
			status = hsGetLinkValueDouble3(phs, pLink, &val);
			hsStatus(phs, 3, "++++ i=%d val=%g\n", i, val);
		}

		hsStatus(phs, 3, "Estimate position at absolute %g\n", val * slope);

		if (!status) {
			status = hsGetLinkValueLong(phs, &phs->imps, &cPos);
			hsStatus(phs, 3, "Current position is %ld\n", cPos);
		}

		*pPos = cPos - val * slope;

		if (!status)
			status = hsMoveTo(phs, *pPos, 1);
	}

	hsStatus(phs, 3, "<<<< " __FILE__ ":zeroSen return %d\n", status);

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
