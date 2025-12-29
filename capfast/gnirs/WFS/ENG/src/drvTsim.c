static char rcsid[] = "$Id: drvTsim.c,v 1.2 2009/05/27 19:34:44 fkraemer Exp $";

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
 *     drvTsim.c
 *
 * FUNCTION NAME(S)
 */

#include <vxWorks.h>
#include <stdioLib.h>
#include <sysLib.h>             /* library for task support */
#include <taskLib.h>
#include <rngLib.h>             /* library for ring buffer support */
#include <semLib.h>
#include <time.h>
#include <string.h>
#include <math.h>
#include <tyLib.h>
#include <usrLib.h>
#include <ioLib.h>
#include <tyLib.h>
#include <usrLib.h>
#include <stdio.h>
#include <ctype.h>
#include <string.h>

#include <dbDefs.h>
#include <drvSup.h>
#include <devSup.h>
#include <recSup.h>
#include <module_types.h>
#include <taskwd.h>

#include "drvTsim.h"

#define DEBUG 0

#if !defined(DEBUG)
#	define DEBUG (0)
#endif

typedef struct { /* derived from struct drvet */
	long number;         /* !REQ! number of support routines */
	DRVSUPFUN report;    /* !REQ! print report */
	DRVSUPFUN init;      /* !REQ! *init support */
	DEVSUPFUN reboot;    /* !REQ! init support for particular record */
/* End of standard fields */
} DrvTsim;

/*
 * Enable simulation update task.
 */

int tsimEnable = 0;

static long init();
#define report ((DEVSUPFUN)0)
#define reboot ((DEVSUPFUN)0)
#define get_ioint_info ((DEVSUPFUN)0)
DrvTsim drvTsim = {
	4,
	report,
	init,
	reboot,
};

/*
 * Nothing about the temperature simulation loop needs to be updated
 * very rapidly, so don't update very often.
 */

#define UPDATE_RATE (1000) /* Update rate, ms */
#define UPDATE_RATE_S (UPDATE_RATE / 1000.0)
#define UPDATE_RATE_NS ((UPDATE_RATE % 1000) * 1000000)

#define DHEAT (30.0)     /* Temperature change per loop */
#define DLOAD (25.0)     /* Heat input per loop */
#define AMBIENT (300.0)  /* Starting point for temperature */
#define CLRATE (0.005)   /* How much cooling per motor step per unit time */

#define MAX_MOTOR (100000000)
#define MIN_MOTOR (-100000000)

/*****************************************************************
 * The temperature-controller data structures
 *****************************************************************/

static TsimValues values;

static void tsimUpdate();



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
 *     tsimEnable
 *
 * PRIOR REQUIREMENTS:
 *
 * DEFICIENCIES:
 *-
 */

static long
init()
{
	long status = 0;
	int i;

	values.sem = semMCreate(SEM_Q_FIFO);

	values.ts.temperature = 0.0;

	for (i = 0; i < TSIM_NTC; i++) {
		values.tc[i].setPoint = 0.0;
		values.tc[i].temperature = 0.0;
	}

	for (i = 0; i < TSIM_NMOTOR; i++) {
		values.motor[i].pos = 0;
		values.motor[i].speed = 0;
		values.motor[i].moving = 0;
		values.motor[i].dir = 1;
		values.motor[i].dest = 0;
		values.motor[i].event = 0;
		values.motor[i].callback = 0;
		values.motor[i].callbackArg = 0;
	}

	if (tsimEnable) {
		taskSpawn("tsimUpdate", 42, VX_FP_TASK, 8000, (FUNCPTR)tsimUpdate,
			0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
	}

	return status;
}



/*
 *+
 * FUNCTION NAME: tsimInit
 *
 * INVOCATION: tsimInit(pRec, id, pDpvt)
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
tsimRecInit(void *pRec, void **pDpvt)
{
	long status = 0;

#if DEBUG > 1
	printf("tsimRecInit(%p,%p)\n", pRec, pDpvt);
#endif

	*pDpvt = &values;

	return status;
}



/*
 *+
 * FUNCTION NAME: tsimUpdate
 *
 * INVOCATION: tsimUpdate()
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
 *     Units must be K
 *
 *     This is not intended to be an accurate model of a temperature-
 *     control loop.  Only enough of a simulation so that I can
 *     test all of the high-level software.
 */

static void
tsimUpdate()
{
	for (;;) {
		int i;
		double temperature;
		double basetmp;

		semTake(values.sem, WAIT_FOREVER);

		basetmp = AMBIENT;
		for (i = 0; i < TSIM_NCOOL; i++) {
			if (values.motor[i].speed > 0 && values.motor[i].moving)
				basetmp -= values.motor[i].speed * CLRATE;
		}

		temperature = 0;

		/*
		 * Update the temperature simulation loop.
		 */
			
		for (i = 0; i < TSIM_NTC; i++) {
			if (values.tc[i].temperature < basetmp - DLOAD)
				values.tc[i].temperature += DLOAD;
			else if (values.tc[i].temperature > basetmp + DLOAD)
				values.tc[i].temperature -= DLOAD;
			else
				values.tc[i].temperature = basetmp;

			if (values.tc[i].temperature < values.tc[i].setPoint - DHEAT) {
				values.tc[i].heating = 100.0;
				values.tc[i].temperature += DHEAT;
			} else if (values.tc[i].temperature < values.tc[i].setPoint) {
				values.tc[i].heating
					= (values.tc[i].setPoint - values.tc[i].temperature) /
						(double)DHEAT * 100.0;
				values.tc[i].temperature = values.tc[i].setPoint;
			} else {
				values.tc[i].heating = 0.0;
			}

			temperature += values.tc[i].temperature;
		}

		values.ts.temperature = temperature / (double)TSIM_NTC;

		/*
		 * If there is a callback function, then we call it whenever
		 * we receive a command from the device layer, and
		 * we call it periodically while we're in motion.
		 *
		 * We give up the semaphore, just before processing, because
		 * the callback function might call a semaphore-protected
		 * function.
		 */

		for (i = 0; i < TSIM_NMOTOR; i++) {
			if (values.motor[i].moving) {
				int step = (UPDATE_RATE * values.motor[i].speed) / 1000;

				if (values.motor[i].jog && values.motor[i].dir > 0) {
					values.motor[i].pos += step;
					values.motor[i].event = 1;
				} else if (values.motor[i].jog) {
					values.motor[i].pos -= step;
					values.motor[i].event = 1;
				} else if (values.motor[i].pos > values.motor[i].dest + step) {
					values.motor[i].pos -= step;
					values.motor[i].event = 1;
				} else if (values.motor[i].pos < values.motor[i].dest - step) {
					values.motor[i].pos += step;
					values.motor[i].event = 1;
				} else {
					values.motor[i].moving = 0;
					values.motor[i].pos = values.motor[i].dest;
					values.motor[i].event = 1;
				}

				/*
				 * The motor record has problems with large numbers
				 */

				if (values.motor[i].pos > MAX_MOTOR)
					values.motor[i].pos -= MAX_MOTOR - MIN_MOTOR; 
				else if (values.motor[i].pos < MIN_MOTOR)
					values.motor[i].pos += MAX_MOTOR - MIN_MOTOR; 
			}

			if (values.motor[i].callback
					&& values.motor[i].callbackArg && values.motor[i].event) {
				values.motor[i].event--;
				semGive(values.sem);
				values.motor[i].callback(values.motor[i].callbackArg);
				semTake(values.sem, WAIT_FOREVER);
			}
		}

		semGive(values.sem);

		/*
		 * Don't update too frequently
		 */

		{
			struct timespec ts;
			ts.tv_sec = UPDATE_RATE_S;
			ts.tv_nsec = UPDATE_RATE_NS;
			nanosleep(&ts, NULL);
		}
	}
}

/*
 *+
 * FUNCTION NAME: tsimMotorSetSpeed
 *
 * INVOCATION: tsimMotorSetSpeed(pMotor, speed)
 *
 * PARAMETERS:
 *
 *     (>) speed (int)
 *     (!) pMotor (TsimMotor *)
 *
 * INVOCATION: tsimMotorSetSpeed(pMotor, speed)
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
tsimMotorSetSpeed(TsimMotor *pMotor, int speed)
{
#if DEBUG > 1
	printf("tsimMotorSetSpeed(%p,%d)\n", pMotor, speed);
#endif

	semTake(values.sem, WAIT_FOREVER);
	pMotor->speed = speed;
	semGive(values.sem);

	return 0;
}

/*
 *+
 * FUNCTION NAME: tsimMotorSetPos
 *
 * INVOCATION: tsimMotorSetPos(pMotor, pos)
 *
 * PARAMETERS:
 *     (>) pos (int)
 *     (!) pMotor (TsimMotor *)
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
tsimMotorSetPos(TsimMotor *pMotor, int pos)
{
#if DEBUG > 1
	printf("tsimMotorSetPos(%p,%d)\n", pMotor, pos);
#endif

	semTake(values.sem, WAIT_FOREVER);
	pMotor->pos = pos;
	pMotor->event = 1;
	semGive(values.sem);

	return 0;
}



/*
 *+
 * FUNCTION NAME: tsimMotorStop
 *
 * INVOCATION: tsimMotorStop(pMotor)
 *
 * PARAMETERS:
 *
 *     (!) pMotor (TsimMotor *)
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
tsimMotorStop(TsimMotor *pMotor)
{
#if DEBUG > 1
	printf("tsimStop(%p)\n", pMotor);
#endif

	semTake(values.sem, WAIT_FOREVER);
	pMotor->moving = 0;
	pMotor->jog = 0;
	pMotor->event = 1;
	semGive(values.sem);

	return 0;
}



/*
 *+
 * FUNCTION NAME: tsimStop
 *
 * INVOCATION: tsimStop(pMotor)
 *
 * PARAMETERS:
 *
 *     (!) pMotor (TsimMotor *)
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
tsimMotorStart(TsimMotor *pMotor)
{
#if DEBUG > 1
	printf("tsimStart(%p)\n", pMotor);
#endif

	semTake(values.sem, WAIT_FOREVER);
	pMotor->moving = 1;
	pMotor->event = 1;
	semGive(values.sem);

	return 0;
}



/*
 *+
 * FUNCTION NAME: tsimStop
 *
 * INVOCATION: tsimStop(pMotor)
 *
 * PARAMETERS:
 *
 *     (!) pMotor (TsimMotor *)
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
tsimMotorJog(TsimMotor *pMotor)
{
#if DEBUG > 1
	printf("tsimMotorJog(%p)\n", pMotor);
#endif

	semTake(values.sem, WAIT_FOREVER);
	pMotor->jog = 1;
	semGive(values.sem);

	return 0;
}



/*
 *+
 * FUNCTION NAME: tsimMotorSetCallback
 *
 * INVOCATION: tsimMotorSetCallback(pMotor, callback, void *)
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
tsimMotorSetCallback(TsimMotor *pMotor, long (*callback)(void *), void *arg)
{
#if DEBUG > 1
	printf("tsimMotorSetCallback(%p,%p,%p)\n", pMotor, callback, arg);
#endif

	pMotor->callback = callback;
	pMotor->callbackArg = (void *)arg;
}



/*
 *+
 * FUNCTION NAME: tsimMotorSetDest
 *
 * INVOCATION: tsimMotorSetDest(pMotor, dest, rel)
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
tsimMotorSetDest(TsimMotor *pMotor, int dest, int rel)
{
	long status = 0;

#if DEBUG > 1
	fprintf(stderr, "tsimMotorSetDest(%p, %d, %d)\n",
		pMotor, dest, rel);
#endif

	semTake(values.sem, WAIT_FOREVER);
	if (rel)
		pMotor->dest = pMotor->pos + dest;
	else
		pMotor->dest = dest;
	semGive(values.sem);

	return status;
}



/*
 *+
 * FUNCTION NAME: tsimMotorSetDir
 *
 * INVOCATION: tsimMotorSetDir(pMotor, dir)
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
tsimMotorSetDir(TsimMotor *pMotor, int dir)
{
	long status = 0;

#if DEBUG > 1
	fprintf(stderr, "tsimMotorSetDir(%p,%d)\n", pMotor, dir);
#endif

	semTake(values.sem, WAIT_FOREVER);
	pMotor->dir = dir;
	semGive(values.sem);

	return status;
}



/*
 *+
 * FUNCTION NAME: tsimMotorCallback
 *
 * INVOCATION: tsimMotorCallback(pMotor)
 *
 * PARAMETERS:
 *
 *     (!) pMotor (TsimMotor *)
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
tsimMotorCallback(TsimMotor *pMotor)
{
	long status;

#if DEBUG > 1
	fprintf(stderr, "tsimMotorCallback(%p)\n", pMotor);
#endif

	status = pMotor->callback(pMotor->callbackArg);

	return status;
}



/*
 *+
 * FUNCTION NAME: tsimMotorInit
 *
 * INVOCATION: tsimMotorInit(pRec, mnum, pDpvt)
 *
 * PARAMETERS:
 *
 *     (>) pRec  (void *)
 *     (>) mnum  (const char [])
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
 *     Behavior is undefined if mnum is not between 0 and TSIM_NMOTOR - 1,
 *     inclusive.
 *-
 */

long
tsimMotorInit(void *pRec, int mnum, void **pDpvt)
{
	long status = 0;

#if DEBUG > 1
	fprintf(stderr, "tsimMotorInit(%p,%d,%p)\n", pRec, mnum, pDpvt);
#endif

	semTake(values.sem, WAIT_FOREVER);

	if (!status)
		*pDpvt = &values.motor[mnum];

	semGive(values.sem);

	return status;
}
