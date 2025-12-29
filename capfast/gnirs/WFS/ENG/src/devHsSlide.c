static char rcsid[] = "$Id: devHsSlide.c,v 1.2 2009/05/27 19:34:42 fkraemer Exp $";

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
 * slide component.  This record is used to control the window cover,
 * which has no Hall-effect sensors.  Its two positions are defined
 * by limit switches.
 *
 * NOTE: These routines return a lot of spurious warning messages,
 * which should be supressed.
 *
 * FILENAME
 *     devHallStepSlide.c
 *
 * FUNCTION NAME(S)
 */

/*
 * Note: For slide mechanisms, the range of travel is deliberately
 * set to be a little larger than the actual physical range of travel,
 * to insure that the mechanism is forced into the limit.  None of the
 * peak parameters are relevant, because this mechanism has no
 * Hall-effect sensors.  BLSH is not used.
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

#define CYCLE_POINTS (3)
static const char *const cycle_names[] = { "A", "B", "C" };

/*
 * The redatum command isn't provided for this mechanism, because
 * there is no peak search.  We just run into the limit switches.
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

DevHallStep devHallStepSlide = {
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
	long limit;
	long status = 0;
	int inLimit;

	/*
	 * Get the current location.  Figure out a range of values
	 * in which we should find the lower limit.  The factor of
	 * 2 is just a bit of overkill.
	 */

	if (!status) {
		status = hsGetLinkValueLong(phs, &phs->imps, &pos);
		limit = pos - 2 * (phs->hopr - phs->lopr);
	}

	/*
	 * Drive it into the lower limit.
	 */

	if (!status)
		status = smartMoveTo(phs, limit);

	/*
	 * At this point, we should be at the lower limit.  If not,
	 * we have a problem.
	 */
	
	if (!status)
		status = hsCheckLimits(phs, &inLimit);

	if (!status) {
		if (inLimit == HS_LIMIT_HIGH) {
			hsStatus(phs, HS_ERROR, "Jammed at upper limit");
			status = S_ifa_hs_MissingHome;
		} else if (inLimit != HS_LIMIT_LOW) {
			hsStatus(phs, HS_ERROR, "Not at lower limit");
			status = S_ifa_hs_MissingHome;
		}
	}

	/*
	 * Where are we now, accounting for the variability in the
	 * home position (depending on when the limit switch was
	 * actuated).
	 */

	if (!status)
		status = hsGetLinkValueLong(phs, &phs->imps, &pos);

	/*
	 * Set the new home position.
	 */

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

	if (!status) {
		if (phs->raw)
			status = hsMoveTo(phs, phs->dest, 1);
		else
			status = smartMoveTo(phs, phs->dest);
	}

	/* 
	 * Where are we now?
	 */

	if (!status) {
		status = hsGetLinkValueLong(phs, &phs->imps, &motorPos);
	}

	/*
	 * Set our new position.
	 */

	if (!status) {
		phs->vmmp = motorPos;
		db_post_events(phs, &phs->vmmp, MONITORMASK);
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
 *     This is different from the generic hsCycleN(), because
 *     it must support the slightly-different way that this
 *     mechanism handles limits.
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

long
cycleN(HallStepRecord *phs, int nRep)
{
	long status = 0;
	int pass = 0;
	int i;
	long dest[CYCLE_POINTS];
	int inLimit;

	hsStatus(phs, 1, ">>>> " __FILE__ ":cycleN(%p,%d)\n", phs, nRep);

	phs->cstp = 0;
	if (MONITORMASK)
		db_post_events(phs, &phs->cstp, MONITORMASK);

	/*
	 * Save the current destination, just in case some idiot changes
	 * them to an out-of-range value while we are in the middle of 
	 * a pass.
	 */

	for (i = 0; i < CYCLE_POINTS; i++) {
		dest[i] = (&phs->cdsa)[i];
		printf("dest[%d]=%ld\n", i, dest[i]);
	}

	/*
	 * Move to the last point, so that we always have a reproducable
	 * first data point.
	 */

	if (!status)
		status = smartMoveTo(phs, dest[CYCLE_POINTS - 1]);

	/*
	 * Cycle through the three points.  Note that we always complete the
	 * full cycle, to be consistent with the usual hsCycleN() function.
	 */

	for (pass = 0; !status && !phs->cstp && pass < nRep && !phs->stop; pass++) {
		for (i = 0; i < CYCLE_POINTS && !status; i++) {
			hsStatus(phs, HS_STATUS, "Cycle/Pass %d%s", 
				pass, cycle_names[i]);

			if (!status)
				status = smartMoveTo(phs, dest[i]);

			/*
			 * Since there are no Hall-effect sensors, and since there
			 * are limit switches, we treat the limits as if they
			 * like hall sensors.
			 */

			if (!status)
				hsCheckLimits(phs, &inLimit);

			if (!status) {
				(&phs->v1pa)[i] = (&phs->v1ba)[i] = inLimit;
				(&phs->v2pa)[i] = (&phs->v2ba)[i] = 0;
			}

			if (MONITORMASK) {
				db_post_events(phs, &phs->v1pa + i, MONITORMASK);
				db_post_events(phs, &phs->v1ba + i, MONITORMASK);
				db_post_events(phs, &phs->v2pa + i, MONITORMASK);
				db_post_events(phs, &phs->v2ba + i, MONITORMASK);
			}

			if (!status && phs->cset) {
				(&phs->e1pa)[i] = (&phs->v1pa)[i];
				(&phs->e1ba)[i] = (&phs->v1ba)[i];
				(&phs->e2pa)[i] = (&phs->v2pa)[i];
				(&phs->e2ba)[i] = (&phs->v2ba)[i];
				if (MONITORMASK) {
					db_post_events(phs, &phs->e1pa + i, MONITORMASK);
					db_post_events(phs, &phs->e1ba + i, MONITORMASK);
					db_post_events(phs, &phs->e2pa + i, MONITORMASK);
					db_post_events(phs, &phs->e2ba + i, MONITORMASK);
				}
			}

			if (!status 
					&& (fabs((&phs->v1pa)[i] - (&phs->e1pa)[i]) > phs->tol
					|| fabs((&phs->v1ba)[i] - (&phs->e1ba)[i]) > phs->tol
					|| fabs((&phs->v2pa)[i] - (&phs->e2pa)[i]) > phs->tol
					|| fabs((&phs->v2ba)[i] - (&phs->e2ba)[i]) > phs->tol)) {
				status = S_ifa_hs_CycleFailed;
				hsStatus(phs, HS_ERROR, "Cycle/Failed %d%s",
					pass, cycle_names[i]);
			}
		}

		if (phs->cset) {
			phs->cset = 0;
			if (MONITORMASK)
				db_post_events(phs, &phs->cset, MONITORMASK);
		}
	}

	/*
	 * If we exited without carrying out the full pass, we may not
	 * have carried out the appropriate backlash correction.
	 */

	if (!status && pass == 0)
		(void)hsPutLinkValueLong(phs, &phs->pser, 1L);

	hsStatus(phs, 1, "<<<< " __FILE__ ":cycle return %d\n", status);

	return status;
}


/*
 * This mechanism doesn't have any hall sensors.  Instead, we return
 * HS_LIMIT_LOW if it is in the negative limit and HS_LIMIT_HIGH if
 * it is in the positive limit.
 */

static long
update(HallStepRecord *phs)
{
	long status;
	long imps;
	int inLimit;

	status = 0;

	/*
	 * See if a limit switch is set.
	 */

	if (!status)
		status = hsCheckLimits(phs, &inLimit);

	/*
	 * Read current motor position.
	 */

	if (!status)
		status = hsGetLinkValueLong(phs, &phs->imps, &imps);

	/*
	 * Write values to output fields, for use by the inverse lut
	 */

	if (!status) {
		phs->rvlp = inLimit;
		db_post_events(phs, &phs->rvlp, MONITORMASK);

		phs->rvlb = inLimit;
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

/*
 * A normal move except that if an appropriate limit is encountered,
 * it is not treated as an error.  If the a limit is encountered,
 * then the origin is treated appropriately.
 *
 * The soft limits are set so that they are wider than the physical
 * limits, because we always want to move into the limit switches
 * when we move to a defined position.  (The only defined positions
 * are at the endpoints.) 
 *
 * Note that setting the limits clears the limit flags.  Yuck.  To
 * insure that the limits are set to a non-misleading value, an
 * additional move is executed which should always fail, but which
 * should set the limit flag.
 */

static long
smartMoveTo(HallStepRecord *phs, long dest)
{
	long status = 0;
	long pos;
	int inLimit;

	/* Get the current position */

	if (!status) {
		status = hsGetLinkValueLong(phs, &phs->imps, &pos);
	}

	if (!status) {
		status = hsMoveTo(phs, dest, 1);

		if (status == S_ifa_hs_MoveFailed && !hsCheckLimits(phs, &inLimit)) {
			if (dest > pos && inLimit == HS_LIMIT_HIGH) {
				status = 0;
				if (!status)
					status = hsGetLinkValueLong(phs, &phs->imps, &pos);
				if (!status)
					status = hsSetHome(phs, pos - phs->hopr);
				if (!status)
					(void)hsMoveTo(phs, phs->hopr + 1, 1);
				if (!status)
					hsStatus(phs, HS_ERROR, "");
			} else if (dest < pos && inLimit == HS_LIMIT_LOW) {
				status = 0;
				if (!status)
					status = hsGetLinkValueLong(phs, &phs->imps, &pos);
				if (!status)
					status = hsSetHome(phs, pos - phs->lopr);
				if (!status)
					(void)hsMoveTo(phs, phs->lopr - 1, 1);
				if (!status)
					hsStatus(phs, HS_ERROR, "");
			}
		}
	}

	return status;
}
