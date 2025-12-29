static char rcsid[] = "$Id: drvSoftHS.c,v 1.2 2009/05/27 19:34:44 fkraemer Exp $";

/*
 * Copyright 1997 University of Hawaii, Institute for Astronomy.  All
 * rights reserved.
 *
 * The University of Hawaii grants AURA a non-exclusive license to
 * use this software, as stated in [contract].
 *
 * Author: Hubert Yamada, University of Hawaii, Institute for Astronomy
 *
 * EPICS Device support software simulation system for NIRI mechanism. 
 *
 * FILENAME
 *     drvSoftHS.c
 *
 * FUNCTION NAME(S)
 *
 *     shsUpdate
 *     init
 *     report
 *     shsDoCallback
 *     shsMotorInit
 *     shsSensorInit
 *     shsSensorRead
 *     shsSetCallback
 *     shsSetDest
 *     shsSetPos
 *     shsSetSpeed
 *     shsStop
 *     shsStop
 *     sigGimbal
 *     sigBinary
 *     sigStage
 */

#include <vxWorks.h>
#include <stdioLib.h>
#include <sysLib.h>             /* library for task  support */
#include <taskLib.h>
#include <rngLib.h>             /* library for ring buffer support */
#include <semLib.h>
#include <time.h>
#include <string.h>
#include <math.h>

#include <dbDefs.h>
#include <drvSup.h>
#include <devSup.h>
#include <recSup.h>
#include <module_types.h>
#include <taskwd.h>

#include "recHmotor.h"
#include "drvHoms.h"
#include "drvSoftHS.h"
#include "ifaErrors.h"

#define UPDATE_RATE (100) /* Update rate, ms */
#define UPDATE_RATE_S (UPDATE_RATE / 1000.0)
#define UPDATE_RATE_NS ((UPDATE_RATE % 1000) * 1000000)

typedef struct { /* derived from struct drvet */
	long number;         /* !REQ! number of support routines */
	DRVSUPFUN report;    /* !REQ! print report */
	DRVSUPFUN init;      /* !REQ! *init support */
	DEVSUPFUN reboot;    /* !REQ! init support for particular record */
/* End of standard fields */
} DrvSoftHS;

/*
 * Enable simulation
 */

int shsEnable = 0;

/*
 * Error term, for debugging
 */

double shsError = 0;

static long init();
static long report(int);
#define reboot ((DEVSUPFUN)0)
#define get_ioint_info ((DEVSUPFUN)0)
DrvSoftHS drvSoftHS = {
	4,
	report,
	init,
	reboot,
};

/*
 * TODO:  The simulated mechanisms should not be hard-coded here. They
 * should be read in from a table at boot time
 */

static void sigStage(ShsParms *);
static void sigGimbal(ShsParms *);
static void sigBinary(ShsParms *);

static SEM_ID semParms; /* Protect the parms data structure */
static ShsParms parms[] = {
	{ "cc:cov",   sigBinary, 180.0,  2,  -1000, 10000,   0 },
	{ "cc:filt1", 0,         400.0,  12, 13250, 24000,  288000 },
	{ "cc:filt2", 0,         400.0,  12, 13250, 24000,  288000 },
	{ "cc:filt3", 0,         400.0,  12, 13250, 24000,  288000 },
	{ "cc:foc",   sigStage,  180.0,  0,  4000,  6000,   0 },
	{ "cc:fopl",  0,         500.0,  12, 2300,  37714,  452571 },
	{ "cc:puvw",  sigBinary, 180.0,  2,  -3140, 10000,  0 },
	{ "cc:splt",  0,         1000.0, 4,  4580,  156735, 626939 },
	{ "cc:ster1", 0,         100.0,  4,  6250,  10285,  41143 },
	{ "cc:ster2", 0,         100.0,  4,  6250,  10285,  41143 },
	{ "wfs:filt", 0,         1500.0, 12, 2786,  9428,   113143 },
	{ "wfs:foc",  sigStage,  100.0,  0,  4000,  6000,   0 },
	{ "wfs:prbx", sigGimbal, 180.0,  0,  4000,  6000,   0 },
	{ "wfs:prby", sigGimbal, 180.0,  0,  4000,  6000,   0 },
};
#define NPARM (sizeof(parms) / sizeof(*parms))

#define NLEVEL (4)  /* Number of distinguishable magnet levels */
#define SCALE (1.0) /* Scale factor for magnet field strength */
#define HOMESTR (4.0) /* Strength of home magnet */

long lockFast = 0; /* Don't rename, accessed via longin/longout record */

static int shsUpdate();



/*
 *+
 * FUNCTION NAME: report
 *
 * INVOCATION: report(level) 
 *
 * PARAMETERS:
 *
 *     (>) level (int)
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
report(int level)
{
	fprintf(stderr, "SoftHS Report\n");
	return 0;
}



/*
 *+
 * FUNCTION NAME: init
 *
 * INVOCATION: init()
 *
 * PARAMETERS:
 *
 *    None
 *
 * FUNCTION VALUE:
 *
 * PURPOSE:
 *
 * DESCRIPTION:
 *
 * EXTERNAL VARIABLES:
 *
 *     shsEnable: If non-zero, then simulator will be started.
 *
 * PRIOR REQUIREMENTS:
 *
 * DEFICIENCIES:
 *-
 */

static long
init()
{
	semParms = semBCreate(SEM_Q_FIFO, SEM_FULL);

	if (shsEnable) {
		taskSpawn("shsUpdate", 42, VX_FP_TASK, 8000, (FUNCPTR)shsUpdate,
			0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
	}

	return 0;
}



/*
 *+
 * FUNCTION NAME: shsMotorInit
 *
 * INVOCATION: shsMotorInit(pRec, id, pDpvt)
 *
 * PARAMETERS:
 *
 *     (>) pRec  (void *)
 *     (>) id    (const char [])
 *     (<) pDpvt (void **)
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

long
shsMotorInit(void *pRec, const char id[], void **pDpvt)
{
	int i;
	long status = 0;

	semTake(semParms, WAIT_FOREVER);

	for (i = 0; i < NPARM; i++) {
		if (strcmp(id, parms[i].id) == 0)
			break;
	}
	if (i == NPARM) {
		char buf[128];
		sprintf(buf,
			"drvSoftHS (shsMotorInit) Unknown simulated mechanism %s",
			id);
		recGblRecordError(S_db_badField, pRec, buf);
		status = S_db_badField;
	}

	if (!status)
		*pDpvt = &parms[i];

	semGive(semParms);

	return status;
}



/*
 *+
 * FUNCTION NAME: shsSensorInit
 *
 * INVOCATION: shsSensorInit(pRec, info, pDpvt)
 *
 * PARAMETERS:
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

long
shsSensorInit(void *pRec, const char info[], void **pDpvt)
{
	int i;
	long status = 0;
	char id[256];
	char typ[256];

	semTake(semParms, WAIT_FOREVER);

	if (sscanf(info, "%s %s", id, typ) == 2) {
		for (i = 0; i < NPARM; i++) {
			if (strcmp(id, parms[i].id) == 0)
				break;
		}
		if (i == NPARM) {
			char buf[256];
			sprintf(buf,
				"drvSoftHS (shsSensorInit) Unknown simulated mechanism %s",
				id);
			recGblRecordError(S_db_badField, pRec, buf);
			status = S_db_badField;
		}
	} else {
		recGblRecordError(S_db_badField, pRec,
			"drvSoftHS.c (shsSensorInit) Illegal device type");
		status = S_db_badField;
	}

	if (!status) {
		if (strcmp(typ, "1p") == 0) {
			*pDpvt = &parms[i].hs1p;
		} else if (strcmp(typ, "1b") == 0) {
			*pDpvt = &parms[i].hs1b;
		} else if (strcmp(typ, "2p") == 0) {
			*pDpvt = &parms[i].hs2p;
		} else if (strcmp(typ, "2b") == 0) {
			*pDpvt = &parms[i].hs2b;
		} else {
			recGblRecordError(S_db_badField, pRec,
				"drvSoftHS (shsSensorInit) Invalid simulated sensor");
			status = S_db_badField;
		}
	}

	semGive(semParms);

	return status;
}



/*
 *+
 * FUNCTION NAME: shsSetCallback
 *
 * INVOCATION: shsSetCallback(pParms, callback, void *)
 *
 * PARAMETERS:
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

void
shsSetCallback(ShsParms *pParms, long (*callback)(void *), void *arg)
{
	semTake(semParms, WAIT_FOREVER);
	pParms->callback = callback;
	pParms->callbackArg = (void *)arg;
	pParms->event = 1;
	semGive(semParms);
}



/*
 *+
 * FUNCTION NAME: shsSetDest
 *
 * INVOCATION: shsSetDest(pParms, dest, rel)
 *
 * PARAMETERS:
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
 *     Warning: Do not pass in a NULL pointer
 *-
 */

long
shsSetDest(ShsParms *pParms, int dest, int rel)
{
	long status = 0;

	semTake(semParms, WAIT_FOREVER);
	if (rel)
		pParms->motorDest = pParms->motorPos + dest;
	else
		pParms->motorDest = dest;
	semGive(semParms);

	return status;
}



/*
 *+
 * FUNCTION NAME: shsSetPos
 *
 * INVOCATION: shsSetPos(pParms, pos)
 *
 * PARAMETERS:
 *     (>) pos (int)
 *     (!) pParms (ShsParms *)
 *
 * FUNCTION VALUE: (long) Error value (non-zero indicates an error)
 *
 * PURPOSE:
 *
 * DESCRIPTION:
 *
 * EXTERNAL VARIABLES:
 *
 *      semParms
 *
 * PRIOR REQUIREMENTS:
 *
 * DEFICIENCIES:
 *
 *      Warning: Do not pass in a NULL pointer
 */

long
shsSetPos(ShsParms *pParms, int pos)
{
	semTake(semParms, WAIT_FOREVER);
	pParms->motorPos = pos;
	pParms->event = 1;
	semGive(semParms);

	return 0;
}



/*
 *+
 * FUNCTION NAME: shsSetSpeed
 *
 * INVOCATION: shsSetSpeed(pParms, speed)
 *
 * PARAMETERS:
 *
 *     (>) speed (int)
 *     (!) pParms (ShsParms *)
 *
 * INVOCATION: shsSetSpeed(pParms, speed)
 *
 * FUNCTION VALUE: (long) Error value (non-zero indicates an error)
 *
 * PURPOSE:
 *
 * DESCRIPTION:
 *
 * EXTERNAL VARIABLES:
 *
 *      semParms
 *
 * PRIOR REQUIREMENTS:
 *
 * DEFICIENCIES:
 *
 *      Warning: Do not pass in a NULL pointer
 */

long
shsSetSpeed(ShsParms *pParms, int speed)
{
	semTake(semParms, WAIT_FOREVER);
	pParms->motorSpeed = speed;
	semGive(semParms);

	return 0;
}



/*
 *+
 * FUNCTION NAME: shsStop
 *
 * INVOCATION: shsStop(pParms)
 *
 * PARAMETERS:
 *
 *     (!) pParms (ShsParms *)
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
 *      Warning: Do not pass in a NULL pointer
 */

long
shsStop(ShsParms *pParms)
{
	semTake(semParms, WAIT_FOREVER);
	pParms->motorMoving = 0;
	pParms->event = 1;
	semGive(semParms);

	return 0;
}



/*
 *+
 * FUNCTION NAME: shsStop
 *
 * INVOCATION: shsStop(pParms)
 *
 * PARAMETERS:
 *
 *     (!) pParms (ShsParms *)
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
 *      Warning: Do not pass in a NULL pointer
 */



long
shsStart(ShsParms *pParms)
{
	semTake(semParms, WAIT_FOREVER);
	pParms->motorMoving = 1;
	pParms->event = 1;
	semGive(semParms);

	return 0;
}



/*
 *+
 * FUNCTION NAME: shsDoCallback
 *
 * INVOCATION: shsDoCallback(pParms)
 *
 * PARAMETERS:
 *
 *     (!) pParms (ShsParms *)
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
 *      Warning: Do not pass in a NULL pointer
 */

long
shsDoCallback(ShsParms *pParms)
{
	long status;

	status = pParms->callback(pParms->callbackArg);

	return status;
}



/*
 *+
 * FUNCTION NAME: shsSensorRead
 *
 * INVOCATION: shsSensorRead(pSensor)
 *
 * PARAMETERS:
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
 *      Warning: Do not pass in a NULL pointer
 */

long
shsSensorRead(ShsSensor *pSensor, double *pVal)
{
	long status = 0;

	*pVal = pSensor->val;

	return status;
}



/*
 *+
 * FUNCTION NAME: shsUpdate
 *
 * INVOCATION: shsUpdate()
 *
 * PARAMETERS: none
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
 *      Warning: Do not pass in a NULL pointer
 */

static int
shsUpdate()
{
	int firstPass = 1;

	for (;;) {
		struct timespec ts;
		double mult;
		double val;
		int i;
		int x;
		int pos;

		semTake(semParms, WAIT_FOREVER);

		for (i = 0; i < NPARM; i++) {
			if (parms[i].motorMoving || firstPass) {
				int inc;

				/*
				 * Update the motor position
				 */

				inc = UPDATE_RATE_S * parms[i].motorSpeed;
				if (inc == 0)
					inc = 1;

				parms[i].event = 1;
				if (abs(parms[i].motorPos - parms[i].motorDest) <= inc
						|| lockFast) {
					parms[i].motorPos = parms[i].motorDest;
					parms[i].motorMoving = 0;
				} else if (parms[i].motorPos > parms[i].motorDest) {
					parms[i].motorPos -= inc;
					parms[i].motorDir = 1;
				} else if (parms[i].motorPos < parms[i].motorDest) {
					parms[i].motorPos += inc;
					parms[i].motorDir = 0;
				}

				/*
				 * The current position, taking account of wraparound. 
				 */

				pos = parms[i].motorPos;
				if (parms[i].modulus > 0) {
					pos = pos % parms[i].modulus;
					if (pos < 0)
						pos += parms[i].modulus;
				}

				/*
				 * Update the primary sensor, home position.  Note that
				 * we have to consider the effects of wraparound.  I
				 * simplify the computation, by assuming that the width of
				 * the peak is much narrower than the wraparound length.
				 */

				mult = 2.0 / (parms[i].width * parms[i].width);

				x = pos;
				val = HOMESTR / (1 + mult * x * x);
				if (parms[i].modulus > 0) {
					x = pos - parms[i].modulus;

					val = val + HOMESTR / (1 + mult * x * x);
				}

				parms[i].hs1b.val = parms[i].hs1p.val = val;

				/*
				 * Now add the primary sensor, filter magnets
				 *
				 * We assume that there are N, evenly spaced magnets,
				 * and compute the signal from the closest magnet.
				 */

				if (parms[i].nMagnet) {
					int a;
					int b;
					int c;
					double relSig;

					x = pos - parms[i].firstMagnet;

					a = floor((x + (double)parms[i].incMagnet/2.0)
							/ (double)parms[i].incMagnet);

					x -= a * (double)parms[i].incMagnet;

					relSig = 1.0 / (1 + mult * x * x);

					a = a % parms[i].nMagnet;
					if (a < 0)
						a += parms[i].nMagnet;

					b = a % NLEVEL + 1;
					c = (a / NLEVEL) % NLEVEL + 1;

					parms[i].hs1p.val -= relSig * (double)b * SCALE;
					parms[i].hs1b.val -= relSig * (double)c * SCALE;
				}

				/*
				 * Update the secondary sensor value and/or override the
				 * computed primary sensor value.
				 */

				if (parms[i].updateSig)
					parms[i].updateSig(&parms[i]);

				/*
				 * Add in an error term, to aid in debugging.
				 */

				parms[i].hs1b.val += shsError;
				parms[i].hs1p.val += shsError;
				parms[i].hs2p.val += shsError;
				parms[i].hs2b.val += shsError;
			}

			/*
			 * If there is a callback function, then we call it whenever
			 * we receive a command from the device layer, and
			 * we call it periodically while we're in motion.
			 *
			 * We give up the semaphore, just before processing, because
			 * the callback function might call a semaphore-protected
			 * function.
			 */

			if (parms[i].callback && parms[i].callbackArg && parms[i].event) {
				parms[i].event = 0;
				semGive(semParms);
				parms[i].callback(parms[i].callbackArg);
				semTake(semParms, WAIT_FOREVER);
			}
		}

		/*
		 * No need to recalculate, unless the motor is moving.
		 */

		firstPass = 0;

		semGive(semParms);

		ts.tv_sec = 0;
		ts.tv_nsec = UPDATE_RATE_NS;
		nanosleep(&ts, NULL);
	}
}



/*
 *+
 * FUNCTION NAME: sigStage
 *
 * INVOCATION: sigStage(pParms)
 *
 * PARAMETERS:
 *
 *     (!) pParms (ShsParms *)
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
 *      Warning: Do not pass in a NULL pointer
 */

static void
sigStage(ShsParms *pParms)
{
	int x;

	/*
	 * Update the linear sensor
	 */

	x = pParms->motorPos;

	pParms->hs2p.val = (double)x / -1000.0;
	if (pParms->hs2p.val > 5.0)
		pParms->hs2p.val = 5.0;
	else if (pParms->hs2p.val < -5.0)
		pParms->hs2p.val = -5.0;
	pParms->hs2b.val = pParms->hs2p.val;
}



/*
 *+
 * FUNCTION NAME: sigGimbal
 *
 * INVOCATION: sigGimbal(pParms)
 *
 * PARAMETERS:
 *
 *     (!) pParms (ShsParms *)
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
 *      Warning: Do not pass in a NULL pointer
 */

static void
sigGimbal(ShsParms *pParms)
{
	int x;
	const double width = 200000.0;
	const double mult = 2.0 / (width * width);

	/*
	 * Update the linear sensor
	 */

	x = pParms->motorPos;

	pParms->hs2b.val = pParms->hs2p.val = HOMESTR / (1 + mult * x * x)
		- HOMESTR / (1 + mult * (x - 300000) * (x - 300000))
		- HOMESTR / (1 + mult * (x + 300000) * (x + 300000));
}



/*
 *+
 * FUNCTION NAME: sigBinary
 *
 * INVOCATION: sigBinary(pParms)
 *
 * PARAMETERS:
 *
 *     (!) pParms (ShsParms *)
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
 *      Warning: Do not pass in a NULL pointer
 *
 *      This is not a true simulation of the pupil viewer.  It doesn't
 *      take into account the motor slippage when the mechanism hits
 *      a hard stop; it just sets the sensor to the limiting value.
 *      It's good enough for testing purposes, though.
 */

static void
sigBinary(ShsParms *pParms)
{
	if (pParms->motorPos > 0) {
		pParms->hs1p.val = HOMESTR;
		pParms->hs1b.val = HOMESTR;
	}

	if (pParms->motorPos < pParms->firstMagnet) {
		pParms->hs1p.val = -SCALE;
		pParms->hs1b.val = -SCALE;
	}
}
