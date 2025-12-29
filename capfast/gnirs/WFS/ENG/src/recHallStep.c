static char rcsid[]="$Id: recHallStep.c,v 1.2 2009/05/27 19:34:45 fkraemer Exp $";

/* 
 * Copyright 1997 University of Hawaii, Institute for Astronomy.  All
 * rights reserved.
 *
 * The University of Hawaii grants AURA a non-exclusive license to
 * use this software, as stated in [contract].
 *
 * Author: Hubert Yamada, University of Hawaii, Institute for Astronomy
 *
 * Record Support Routines for Hall-Effect/Stepper-motor control record
 *
 * FILENAME
 *     recHallStep.c
 *
 * FUNCTION NAME(S)
 */

#include <vxWorks.h>
#include <types.h>
#include <stdioLib.h>
#include <stdlib.h>
#include <lstLib.h>
#include <string.h>
#include <symLib.h>
#include <sysSymTbl.h> /* for sysSymTbl*/
#include <a_out.h> /* for N_TEXT */
#include <taskLib.h>
#include <time.h>
#include <limits.h>
#include <stdarg.h>

#include <alarm.h>
#include <dbDefs.h>
#include <dbEvent.h>
#include <dbAccess.h>
#include <dbRecDes.h>
#include <dbFldTypes.h>
#include <errMdef.h>
#include <recSup.h>
#include <devSup.h>
#include <special.h>
#include <car.h>

#include <hallStepRecord.h>
#include <choiceHmotor.h>
#include "recHallStep.h"
#include "ifaErrors.h"

volatile int hsDebug = 0;

volatile int hsLockCfg = 0;
volatile int hsLockObs = 0;
volatile int hsLockGen = 0;
volatile int hsLockTmp = 0;
volatile int hsForceLockTmp = 0;
volatile int hsLockInit = 1;
volatile int hsActive = 0;
volatile int hsLockTmpHB = 0;

SEM_ID semMoving = 0;
volatile int hsMoving = 0;
volatile int hsMaxMoving = 0; /* Actual value set by pvload */

/* Create RSET - Record Support Entry Table*/

static long get_precision();
static long get_value();
static long init_record();
static long process();
static long special();

#define cvt_dbaddr         NULL
#define get_alarm_double   NULL
#define get_array_info     NULL
#define get_control_double NULL
#define get_enum_str       NULL
#define get_enum_strs      NULL
#define get_graphic_double NULL
#define get_units          NULL
#define initialize         NULL
#define put_array_info     NULL
#define put_enum_str       NULL
#define report             NULL

struct rset hallStepRSET = {
	RSETNUMBER,
	report,
	initialize,
	init_record,
	process,
	special,
	get_value,
	cvt_dbaddr,
	get_array_info,
	put_array_info,
	get_units,
	get_precision,
	get_enum_str,
	get_enum_strs,
	put_enum_str,
	get_graphic_double,
	get_control_double,
	get_alarm_double
};

#define MSTART 5  /* How long (in units of MMON) for the motor to start? */
#define MWAIT 300 /* How long (ms) for the motor to start? */
#define MMON  200 /* How often (ms) should we check if the motor is running? */

/*
 * How far away do we have to go from the peak in order to be able
 * to assume that there is no signal.
 */

#define ZEROSIG (5)

/*
 * When we are doing an initial coarse search for the peak, how
 * fine should the steps be (in units of the peak width)?
 */

#define PEAKSTEP (0.05)

/* 
 * When we are trying to fit the peak, how far out should we go from
 * the rough peak (Should be at least twice PEAKSTEP, but not
 * too large)?
 */

#define FITRANGE (0.1)

/*
 * When we are trying to fit the peak, how big should our steps be?
 * Should be much less than FITRANGE.
 */

#define FITSTEP (0.01)

#define CYCLE_POINTS (3)
static const char *const cycle_names[] = { "A", "B", "C" };

#define MONITORMASK (DBE_VALUE | DBE_LOG)

typedef struct quad_fit_ {
	double xzero;
	double xscale;
	double syx2;
	double syx1;
	double syx0;
	double sx4;
	double sx3;
	double sx2;
	double sx1;
	double sx0;
} QuadFit;

/*
 * TODO: The maximum number of instances of this record should not
 *       be hard coded.
 */

#define MAXHSRECS (32)

typedef struct {
	HallStepRecord *phs;
	SEM_ID sem;
	long (*asyn)(HallStepRecord *);
	int moveCommand;
	int noLock;
} DevInfo;

static DevInfo devInfo[MAXHSRECS];
static int devCount = 0;

static long move(HallStepRecord *);
static long moveAndVerify(HallStepRecord *);
static long update(HallStepRecord *);
static long cycle(HallStepRecord *);
static long datum(HallStepRecord *);
static long datumDiag(HallStepRecord *);
static long redatum(HallStepRecord *);
static long diagnose(HallStepRecord *);
static long nop(HallStepRecord *);

static void taskUpdate(int);
static void monitor(HallStepRecord *, int);
static double median3(double[3]);
static long moveTo(HallStepRecord *, long, int);
static void qf_init(QuadFit *, double, double);
static void qf_addpoint(QuadFit *, double, double);
static double qf_getpeak(QuadFit *);



/************************************************************************
 * Standard interface to EPICS
 ************************************************************************/

/*
 *+
 * FUNCTION NAME: init_record
 *
 * INVOCATION: init_record(phs, pass)
 *
 * PARAMETERS:
 *
 *     (!) phs    (HallStepRecord *)  EPICS record
 *     (>) pass   (int)               Initialization pass
 *
 * FUNCTION VALUE: (long) Error status (nonzero indicates an error)
 *
 * SCOPE: static (recHallStep.c)
 *
 * PURPOSE: Standard EPICS record initialization
 *
 *     This performs the initialization which is required for an EPICS
 *     record.
 *
 *     This is intended to be called by the EPICS initialization
 *     procedure.  This should not be called directly.
 *
 * DESCRIPTION:
 *
 *     Creates task and corresponding semaphore.
 *
 * EXTERNAL VARIABLES: devInfo devCount
 *
 * PRIOR REQUIREMENTS:
 *
 *     EPICS pre-initialization of phs.
 *
 * DEFICIENCIES:
 *-
 */

static long
init_record(HallStepRecord *phs, int pass)
{
	long status = 0;
	DevHallStep *pdset;

	hsStatus(phs, 1, ">>>> " __FILE__ ":init_record(%p,%d)\n", phs, pass);

	if (semMoving == 0)
		semMoving = semBCreate(SEM_Q_FIFO, SEM_FULL);

	if (!(pdset = (DevHallStep *)(phs->dset))) {
		recGblRecordError(S_dev_noDSET, (void *)phs, "hallStep: init_record");
		status = S_dev_noDSET;
	}

	if (pass == 0) {
		if (!status) {
			if (devCount >= MAXHSRECS) {
				fprintf(stderr, 
					"HallStep init_record():  MAXHSRECS exceeded\n");
			} else {
				phs->dpvt = &devInfo[devCount];
				devInfo[devCount].asyn = 0;
				devInfo[devCount].phs = phs;
				devInfo[devCount].sem = semBCreate(SEM_Q_FIFO, SEM_EMPTY);

				/* TODO: check return value from taskSpawn */

				taskSpawn("HSTaskUpdate", 42, VX_FP_TASK, 8000, 
					(FUNCPTR)taskUpdate, devCount, 0, 0, 0, 0, 0, 0, 0, 0, 0);
				devCount++;
			}
		}
	} else if (pass == 1) {
		hsStatus(phs, HS_STATUS, "Idle");
		if (!status && pdset->init_record)
			status = (*pdset->init_record)(phs);

		/*
		 * Make sure that records which receive values from this record
		 * are properly initialized
		 */

		(void)hsPutLinkValueLong(phs, &phs->done, 0L);
		(void)hsPutLinkValueLong(phs, &phs->busy, 0L);
		(void)hsPutLinkValueLong(phs, &phs->pser, 1L);
	}

	hsStatus(phs, 1, "<<<< " __FILE__ ":init_record return %d\n", status);

	return status;
}



/*
 *+
 * FUNCTION NAME: process
 *
 * INVOCATION: process(phs)
 *
 * PARAMETERS:
 *     (!) phs    (HallStepRecord *)  EPICS record
 *
 * FUNCTION VALUE: (long) Error status (nonzero indicates an error)
 *
 * SCOPE: static (recHallStep.c)
 *
 * PURPOSE: Standard EPICS record processing
 *
 *     This performs the processing which is required for an EPICS
 *     record.
 *
 * DESCRIPTION:
 *     
 *     Inspects the op (operation) field, and stores a pointer to a
 *     function in the asyn field.  Then releases the semaphore, 
 *     so that the processing routine can handle it.
 *
 * EXTERNAL VARIABLES: devInfo
 *
 * PRIOR REQUIREMENTS:
 *
 *     EPICS initialization of phs.
 *
 * DEFICIENCIES:
 *
 *     This is intended to be called by the EPICS processing 
 *     procedure.  This should not be called directly.
 *-
 */

static long
process(HallStepRecord *phs)
{
	long status = 0;
	long options;

	hsStatus(phs, 1, ">>>> " __FILE__ ":process(%p)\n", phs);

	phs->pact = TRUE;
	options = 0;

	/* Call the device support routines */

	hsStatus(phs, 1, "**** Calling device support (%p)\n", phs);

	if (!status && ((DevInfo *)phs->dpvt)->asyn) {
		hsStatus(phs, HS_ERROR, "Busy");
		status = S_ifa_hs_Busy;
	}

	/*
	 * If we are enabling automatic power-off, make sure that the
	 * system is turned off.
	 */

	if (!status && phs->op == HS_OPNONE && phs->poff != phs->lpof) {
		phs->lpof = phs->poff;
		if (MONITORMASK)
			db_post_events(phs, &phs->lpof, MONITORMASK);
		if (phs->poff)
			phs->op = HS_OPNOP;
	}

	/*
	 * Process the operation command.
	 */

	if (!status) {
		if (phs->op != HS_OPNONE) {
			switch (phs->op) {
			case HS_OPMOVE:
				((DevInfo *)phs->dpvt)->asyn = move;
				((DevInfo *)phs->dpvt)->noLock = 0;
				((DevInfo *)phs->dpvt)->moveCommand = 1;
				break;

			case HS_OPCYCLE:
				((DevInfo *)phs->dpvt)->asyn = cycle;
				((DevInfo *)phs->dpvt)->noLock = 0;
				((DevInfo *)phs->dpvt)->moveCommand = 1;
				break;

			case HS_OPDIAGNOSE:
				((DevInfo *)phs->dpvt)->asyn = diagnose;
				((DevInfo *)phs->dpvt)->noLock = 0;
				((DevInfo *)phs->dpvt)->moveCommand = 1;
				break;

			case HS_OPREDATUM:
				((DevInfo *)phs->dpvt)->asyn = redatum;
				((DevInfo *)phs->dpvt)->noLock = 0;
				((DevInfo *)phs->dpvt)->moveCommand = 1;
				break;
				
			case HS_OPDATUM:
				((DevInfo *)phs->dpvt)->asyn = datum;
				((DevInfo *)phs->dpvt)->noLock = 0;
				((DevInfo *)phs->dpvt)->moveCommand = 1;
				break;

			case HS_OPDATUMDIAG:
				((DevInfo *)phs->dpvt)->asyn = datumDiag;
				((DevInfo *)phs->dpvt)->noLock = 0;
				((DevInfo *)phs->dpvt)->moveCommand = 1;
				break;

			case HS_OPNOP:
				((DevInfo *)phs->dpvt)->asyn = nop;
				((DevInfo *)phs->dpvt)->noLock = 1;
				((DevInfo *)phs->dpvt)->moveCommand = 0;
				break;

			case HS_OPMOVEVER:
				((DevInfo *)phs->dpvt)->asyn = moveAndVerify;
				((DevInfo *)phs->dpvt)->noLock = 0;
				((DevInfo *)phs->dpvt)->moveCommand = 1;
				break;

			case HS_OPUPDATE:
				((DevInfo *)phs->dpvt)->asyn = update;
				((DevInfo *)phs->dpvt)->noLock = 1;
				((DevInfo *)phs->dpvt)->moveCommand = 0;
				break;

			default:
				hsStatus(phs, HS_ERROR, "Illegal operation");
				status = S_ifa_hs_BadOp;
			}
			phs->stop = 0;
			phs->op = HS_OPNONE;
		}
	}

	/* 
	 * Put the values on the output links
	 */

	if (!status) {
		tsLocalTime(&phs->time);
		monitor(phs, 1);
		recGblFwdLink(phs);
	} 

	/*
	 * Start processing
	 */

	if (((DevInfo *)phs->dpvt)->asyn && !status)
		semGive(((DevInfo *)phs->dpvt)->sem);
	else
		phs->pact = FALSE;

	hsStatus(phs, 1, "<<<< " __FILE__ ":process return %d\n", status);

	return status;
}



/*
 *+
 * FUNCTION NAME: get_precision
 *
 * INVOCATION: get_precision(pAddr, pPrecision)
 *
 * PARAMETERS:
 *     (!) pAddr (struct dbAddr *) EPICS Record
 *     (>) pPrecision (long *)
 *
 * FUNCTION VALUE: (long) Error status (nonzero indicates an error)
 *
 * SCOPE: static (recHallStep.c)
 *
 * PURPOSE: ???
 *
 * DESCRIPTION: ???
 *
 * EXTERNAL VARIABLES: none
 *
 * PRIOR REQUIREMENTS: none
 *
 * DEFICIENCIES:
 *
 *     None known.
 *-
 */

static long
get_precision(struct dbAddr *pAddr, long *pPrecision)
{
	HallStepRecord *phs;

	hsStatus(phs, 1, ">>>> " __FILE__ ":get_precision(%p,%p)\n", 
		phs, pPrecision);

	phs = (HallStepRecord *)pAddr->precord;
	*pPrecision = phs->prec;

	if (pAddr->pfield != (void *)&phs->val) 
		recGblGetPrec(pAddr, pPrecision);

	return 0;
}



/*
 *+
 * FUNCTION NAME: get_value
 *
 * INVOCATION: get_value(phs, pvdes)
 *
 * PARAMETERS:
 *     (!) phs (HallStepRecord *) EPICS Record
 *     (!) pvdes (struct valueDes *)
 *
 * FUNCTION VALUE: (long) Error status (nonzero indicates an error)
 *
 * SCOPE: static (recHallStep.c)
 *
 * PURPOSE: ???
 *
 * DESCRIPTION: ???
 *
 * EXTERNAL VARIABLES: none
 *
 * PRIOR REQUIREMENTS: none 
 *
 * DEFICIENCIES:
 *
 *     None known.
 *-
 */

static long
get_value(HallStepRecord *phs, struct valueDes *pvdes)
{
	hsStatus(phs, 2, ">>>> " __FILE__ ":get_value(%p,%p)\n", phs, pvdes);

	pvdes->no_elements = 1;
	pvdes->pvalue = (void *)phs->val;
	pvdes->field_type = DBF_LONG;

	return 0;
}



/*
 *+
 * FUNCTION NAME: monitor
 *
 * INVOCATION: monitor(phs, reset)
 *
 * PARAMETERS:
 *
 *     (!) phs (HallStepRecord *) EPICS Record
 *     (>) reset (int)
 *
 * FUNCTION VALUE: none
 *
 * SCOPE: static (recHallStep.c)
 *
 * PURPOSE: ???
 *
 * DESCRIPTION: ???
 *
 * EXTERNAL VARIABLES: none
 *
 * PRIOR REQUIREMENTS: none 
 *
 * DEFICIENCIES:
 *
 *     None known.
 *-
 */

static void
monitor(HallStepRecord *phs, int reset)
{
	unsigned short monitorMask;

	hsStatus(phs, 1, ">>>> " __FILE__ ":monitor(%p,%d)\n", phs, reset);

	if (reset)
		monitorMask = recGblResetAlarms(phs);
	else
		monitorMask = 0;
	monitorMask |= MONITORMASK;

	/* 
	 * Post events for VAL and output fields when this routine called from 
	 * process 
	 */

	if (reset && monitorMask) {
		if (phs->val != phs->oval) {
			db_post_events(phs, &phs->val, monitorMask);
			phs->oval = phs->val;
		}
	}

	hsStatus(phs, 1, "<<<< " __FILE__ ":monitor return\n");
}



/*
 *+
 * FUNCTION NAME: special
 *
 * INVOCATION: special(pAddr, after)
 *
 * PARAMETERS:
 *
 *     (!) pAddr (struct dbAddr *)
 *     (!) after (int)
 *
 * FUNCTION VALUE: (long) Error status (nonzero indicates an error)
 *
 * SCOPE: static (recHallStep.c)
 *
 * PURPOSE: ???
 *
 * DESCRIPTION: ???
 *
 * EXTERNAL VARIABLES: none
 *
 * PRIOR REQUIREMENTS: none
 *
 * DEFICIENCIES:
 *
 *     None known.
 *-
 */

static long
special(struct dbAddr *pAddr, int after)
{
	HallStepRecord *phs = (HallStepRecord *)pAddr->precord;

	hsStatus(phs, 1, ">>>> " __FILE__ ":special(%p,%d)\n", pAddr, after);

	return 0;
}



/************************************************************************
 * Asynchronous commands
 ************************************************************************/

/*
 *+
 * FUNCTION NAME: taskUpdate
 *
 * INVOCATION: taskUpdate(phs)
 *
 * PARAMETERS:
 *
 *     (!) phs (HallStepRecord) HallStep record
 *
 * FUNCTION VALUE: (long) status value (non-zero indicates an error)
 *
 * SCOPE: static (recHallStep.c)
 *
 * PURPOSE: Asynchronous processing 
 *
 * DESCRIPTION: Process asynchronous command.
 *
 *     This routine normally is sleeping in an endless loop, waiting for
 *     a semaphore to be released.  When the semaphore is released, this 
 *     routine calls the appropriate asynchronous command.
 *
 * EXTERNAL VARIABLES: none
 *
 * PRIOR REQUIREMENTS:
 *
 *     Semaphore must be initialized.
 *     Must always be called with the database locked (use dbScanLock).
 *     Must always be called with phs->pact TRUE.
 *
 * DEFICIENCIES:
 *
 *     None known.
 *-
 */

static void
taskUpdate(int recNo)
{
	long status = 0;
	long motorPos;
	HallStepRecord *phs = devInfo[recNo].phs;
	DevHallStep *const pdset = (DevHallStep *)phs->dset;

	hsStatus(phs, 1, ">>>> " __FILE__ ":taskUpdate(%p)\n", phs);

	for (;;) {
		semTake(devInfo[recNo].sem, WAIT_FOREVER);

		status = 0;

		/*
		 * We're processing.  Don't accept any new commands and don't
		 * accept any new EPICS instructions.
		 */

		dbScanLock((struct dbCommon *)phs);
		(void)hsPutLinkValueLong(phs, &phs->busy, 1L);

		hsStatus(phs, 2, "**** " __FILE__ ":taskUpdate(%d) is now active\n", 
			recNo);

		hsStatus(phs, HS_ERROR, "");

		if (!((DevInfo *)phs->dpvt)->noLock &&
				(hsLockGen || hsLockCfg || hsLockObs || hsLockInit
				|| hsLockTmp == HS_TMP_CHANGING)) {
			
			/*
			 * Command was rejected because of interlocks.  Display
			 * error message and return error status.
			 */

			if (hsLockInit) {
				hsStatus(phs, HS_ERROR, "Error: Initializing");
			} else if (hsLockTmp == HS_TMP_CHANGING) {
				hsStatus(phs, HS_ERROR, "Error: Temperature changing");
			} else if (hsLockGen) {
				hsStatus(phs, HS_ERROR, "Error: Locked");
			} else if (hsLockCfg) {
				hsStatus(phs, HS_ERROR, "Error: Configuring");
			} else if (hsLockObs) {
				hsStatus(phs, HS_ERROR, "Error: Observing");
			}
			status = S_ifa_hs_Locked;
			(void)hsPutLinkValueLong(phs, &phs->done, (long)status);
			hsMSPause(phs, 500); /* Give SNL time to respond */

			/*
			 * Clear the pending operation.
			 */

			((DevInfo *)phs->dpvt)->asyn = 0;
		} else if (((DevInfo *)phs->dpvt)->asyn) {
			/*
			 * We have a valid command.  Process it.
			 */

			/*
			 * Is this motor excluded from the limit on the number of
			 * running motors?  Store the value in a local variable,
			 * so that we don't get confused if the value changes
			 * while we are processing a command.
			 */

			const int notm = phs->notm;

			/*
			 * At this point, we have no reason to expect positioning
			 * problems.  Clear any existing errors, before we move
			 * the mechanism.  If we aren't going to move the mechanism,
			 * leave the error in place.
			 */
			
			if (((DevInfo *)phs->dpvt)->moveCommand)
				(void)hsPutLinkValueLong(phs, &phs->pser, 0L);

			/*
			 * Don't overload the power supply.
			 */

			semTake(semMoving, WAIT_FOREVER);
			if (!notm)
				hsActive++;
			semGive(semMoving);

			while (hsMoving >= hsMaxMoving && !phs->stop && !notm) {
				hsStatus(phs, HS_STATUS, "Waiting");
				hsMSPause(phs, 1000);
			}

			if (phs->stop) {
				status = S_ifa_hs_Stop;
				(void)hsPutLinkValueLong(phs, &phs->done, (long)status);
				hsStatus(phs, HS_ERROR, "User cancelled move");
				hsStatus(phs, HS_STATUS, "Idle");
			} else {
				/*
				 * We have assured that the power supply can support
				 * us.  Begin processing the command.
				 */

				semTake(semMoving, WAIT_FOREVER);
				if (!notm)
					hsMoving++;
				semGive(semMoving);

				/*
				 * Keep the motor on for the entire sequence of operations.
				 */

				if (!status)
					status = hsPutLinkValueLong(phs, &phs->opwr, HMOTOR_PWR_ON);

				if (!status)
					status = (((DevInfo *)phs->dpvt)->asyn)(phs);
				(void)hsPutLinkValueLong(phs, &phs->done, (long)status);
				hsStatus(phs, HS_STATUS, "Idle");

				/*
				 * Now turn off the motor.  Note that we turn off the motor,
				 * even if an earlier operation failed.
				 */

				if (phs->poff && !status) {
					status = hsPutLinkValueLong(phs, &phs->opwr,
						HMOTOR_PWR_OFF);
				} else if (phs->poff) {
					hsPutLinkValueLong(phs, &phs->opwr, HMOTOR_PWR_OFF);
				}

				/*
				 * We're no longer using the power supply.
				 */

				semTake(semMoving, WAIT_FOREVER);
				if (!notm)
					hsMoving--;
				semGive(semMoving);

				/*
				 * If there was an error or if we stopped, even without
				 * an error, we may not have performed the
				 * appropriate backlash correction.  Flag this
				 * with a warning.  Note that we do not clear the
				 * warning, if the status does not show a
				 * problem, because the processing subroutine may
				 * have output a warning, for its own reasons.
				 * Also
				 */

				if (status || phs->stop)
					(void)hsPutLinkValueLong(phs, &phs->pser, 1L);
			}

			semTake(semMoving, WAIT_FOREVER);
			if (!notm)
				hsActive--;
			semGive(semMoving);

			if (!status || status == S_ifa_hs_Stop)
				status = hsGetLinkValueLong(phs, &phs->imps, &motorPos);
			if (!status || status == S_ifa_hs_Stop)
				status = hsPutLinkValueLong(phs, &phs->odst, motorPos);

			/*
			 * The pending operation is complete.  Clear it, so that
			 * another operation can be initiated.
			 */

			((DevInfo *)phs->dpvt)->asyn = 0;
		} else {
			hsStatus(phs, HS_ERROR, "Missing command");
			status = S_ifa_Impossible;
		}

		/*
		 * Update output values.
		 */

		if (pdset->update)
			(void)pdset->update(phs);

		/*
		 * End of processing.  Now we're ready to accept new EPICS
		 * processing.
		 */

		phs->pact = FALSE;
		(void)hsPutLinkValueLong(phs, &phs->busy, 0L);
		dbScanUnlock((struct dbCommon *)phs);
	}

	hsStatus(phs, 1, "<<<< " __FILE__ ":taskUpdate return\n");
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
 * FUNCTION VALUE: (long) Error status (nonzero indicates an error)
 *
 * SCOPE: static (recHallStep.c)
 *
 * PURPOSE: Calls the low level move function to move hardware.
 *
 * DESCRIPTION:
 *
 *     Calls the device-specific, low level move function to move
 *     the hardware.  Writes the current motor position and makes
 *     sure that the move-status link is fired off.
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
move(HallStepRecord *phs)
{
	DevHallStep *const pdset = (DevHallStep *)phs->dset;
	long status = 0;

	hsStatus(phs, 1, ">>>> " __FILE__ ":move(%p)\n", phs);

	/*
	 * A raw move is done without backlash correction, and can overrun
	 * the limits.
	 */

	if (phs->raw)
		(void)hsPutLinkValueLong(phs, &phs->pser, 1L);
	
	/*
	 * The 'directMove' function is called just before the actual move,
	 * because some mechanisms may have problems if they are moved to
	 * arbitrary positions.  This makes it possible for a device to issue
	 * a warning.  Or to return an error; 
	 */
	
	if (!status && pdset->directMove)
		status = pdset->directMove(phs);

	/*
	 * Call the device-type-specifict move command.
	 */

	if (!status) {
		if (!pdset->move) {
			hsStatus(phs, HS_ERROR, "Move function not defined");
			status = S_ifa_Undefined;
		} else {
			hsStatus(phs, HS_STATUS, "Move started");
			status = pdset->move(phs);
		}
	}

	hsStatus(phs, 1, "<<<< " __FILE__ ":move return %d\n", status);

	return status;
}

/*
 *+
 * FUNCTION NAME: moveAndVerify
 *
 * INVOCATION: moveAndVerify(phs)
 *
 * PARAMETERS:
 *
 *     (!) phs (HallStepRecord *) HallStep record
 *
 * FUNCTION VALUE: (long) Error status (nonzero indicates an error)
 *
 * SCOPE: static (recHallStep.c)
 *
 * PURPOSE: Calls the low level move function to move hardware, then
 *     checks Hall effect sensors to see if the appropriate final
 *     location was reached.
 *
 * DESCRIPTION:
 *
 *     Calls the device-specific, low level move function to move
 *     the hardware.  Writes the current motor position and makes
 *     sure that the move-status link is fired off.  After moving,
 *     it checks to make sure that the hall sensors have the
 *     expected value.
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
moveAndVerify(HallStepRecord *phs)
{
	DevHallStep *const pdset = (DevHallStep *)phs->dset;
	long status = 0;
	long verror;

	hsStatus(phs, 1, ">>>> " __FILE__ ":moveAndVerify(%p)\n", phs);

	/*
	 * A raw move is done without backlash correction, and can overrun
	 * the limits.
	 */

	if (phs->raw)
		(void)hsPutLinkValueLong(phs, &phs->pser, 1L);

	/*
	 * Make sure that a valid position was passed to the LUT
	 * record.
	 */

	if (!status && (phs->iver.type == DB_LINK || phs->iver.type == CA_LINK)) {
		status = hsGetLinkValueLong(phs, &phs->iver, &verror);

		if (!status && verror != NO_ALARM) {
			hsStatus(phs, HS_ERROR, "Invalid position");
			status = S_ifa_hs_OutOfRange;
		}
	}

	/*
	 * Perform the actual move
	 */

	if (!status && !pdset->move) {
		hsStatus(phs, HS_ERROR, "Move function not defined");
		status = S_ifa_Undefined;
	} else if (!status) {
		hsStatus(phs, HS_STATUS, "Move started");
		status = pdset->move(phs);
	}

	hsPutLinkValueLong(phs, &phs->opwr, HMOTOR_PWR_OFF);

	/*
	 * Read sensor values and compare to the expected values
	 */

	if (!status && pdset->update)
		status = pdset->update(phs);

	if (!status && pdset->verify)
		status = pdset->verify(phs);

	hsStatus(phs, 1, "<<<< " __FILE__ ":moveAndVerify return %d\n", status);

	return status;
}

/*
 *+
 * FUNCTION NAME: update
 *
 * INVOCATION: update(phs)
 *
 * PARAMETERS:
 *
 *     (!) phs (HallStepRecord *) HallStep record
 *
 * FUNCTION VALUE: (long) Error status (nonzero indicates an error)
 *
 * SCOPE: static (recHallStep.c)
 *
 * PURPOSE: Rereads the hall sensors.
 *
 * DESCRIPTION:
 *
 *     Calls the device-specific, routines to update the hall sensor
 *     values (for position verification).
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
update(HallStepRecord *phs)
{
	DevHallStep *const pdset = (DevHallStep *)phs->dset;
	long status = 0;

	hsStatus(phs, 1, ">>>> " __FILE__ ":update(%p)\n", phs);

	if (!status && pdset->update)
		status = pdset->update(phs);

	hsStatus(phs, 1, "<<<< " __FILE__ ":update return %d\n", status);

	return status;
}



/*
 *+
 * FUNCTION NAME: nop
 *
 * INVOCATION: nop(phs)
 *
 * PARAMETERS:
 *
 *     (!) phs (HallStepRecord *) HallStep record
 *
 * FUNCTION VALUE: (long) Error status (nonzero indicates an error)
 *
 * SCOPE: static (recHallStep.c)
 *
 * PURPOSE: Dummy function 
 *
 * DESCRIPTION:
 *
 *     This is a dummy function, which does nothing.  However,
 *     when this is used as any asynchronous function, it has
 *     the side effect of turning off the power, if automatic
 *     power off is enabled (i.e., the poff field is non-zero).
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
nop(HallStepRecord *phs)
{
	long status = 0;

	hsStatus(phs, 1, ">>>> " __FILE__ ":nop(%p)\n", phs);
	hsStatus(phs, 1, "<<<< " __FILE__ ":nop return %d\n", status);

	return status;
}



/*
 *+
 * FUNCTION NAME: datum
 *
 * INVOCATION: datum(phs)
 *
 * PARAMETERS:
 *
 *     (!) phs (HallStepRecord *) HallStep record
 *
 * FUNCTION VALUE: (long) Error status (nonzero indicates an error)
 *
 * SCOPE: static (recHallStep.c)
 *
 * PURPOSE:  Calls the low-level datum function
 *
 * DESCRIPTION:
 *
 *     Calls the device-specific, low-level datum function to move
 *     the hardware.  Writes the current motor position and makes
 *     sure that the move-status link is fired off.
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
datum(HallStepRecord *phs)
{
	DevHallStep *const pdset = (DevHallStep *)phs->dset;
	long status = 0;

	hsStatus(phs, 1, ">>>> " __FILE__ ":datum(%p)\n", phs);

	hsStatus(phs, HS_STATUS, "Datum");

	if (phs->datm != 0) {
		phs->datm = 0;
		if (MONITORMASK)
			db_post_events(phs, &phs->datm, MONITORMASK);
	}

	if (!pdset->datum) {
		hsStatus(phs, HS_ERROR, "Datum function not defined");
		status = S_ifa_Undefined;
	} else if (!status) {
		status = pdset->datum(phs);
	}

	if (!status) {
		phs->datm = 1;
		if (MONITORMASK)
			db_post_events(phs, &phs->datm, MONITORMASK);
	}

	hsStatus(phs, 1, "<<<< " __FILE__ ":datum return %d\n", status);

	return status;
}



/*
 *+
 * FUNCTION NAME: datumDiag
 *
 * INVOCATION: datumDiag(phs)
 *
 * PARAMETERS:
 *
 *     (!) phs (HallStepRecord *) HallStep record
 *
 * FUNCTION VALUE: (long) Error status (nonzero indicates an error)
 *
 * SCOPE: static (recHallStep.c)
 *
 * PURPOSE:  Calls the low-level datum function, then calls diagnose
 *
 * DESCRIPTION:
 *
 *     Calls the device-specific, low-level datum function to move
 *     the hardware.  Writes the current motor position and makes
 *     sure that the move-status link is fired off.  If datum finished
 *     correctly, calls the low-level diagnose function.  Strictly
 *     speaking, this function is not necessary, but it is convenient.
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
datumDiag(HallStepRecord *phs)
{
	DevHallStep *const pdset = (DevHallStep *)phs->dset;
	long status = 0;

	hsStatus(phs, 1, ">>>> " __FILE__ ":datum(%p)\n", phs);

	hsStatus(phs, HS_STATUS, "DatumDiag");

	if (phs->datm != 0) {
		phs->datm = 0;
		if (MONITORMASK)
			db_post_events(phs, &phs->datm, MONITORMASK);
	}

	if (!pdset->datum || !pdset->cycleN) {
		hsStatus(phs, HS_ERROR, "Datum or cycleN function not defined");
		status = S_ifa_Undefined;
	} else if (!status) {
		status = pdset->datum(phs);
		if (!status)
			status = pdset->cycleN(phs, 1);
	}

	if (!status) {
		phs->datm = 1;
		if (MONITORMASK)
			db_post_events(phs, &phs->datm, MONITORMASK);
	}

	hsStatus(phs, 1, "<<<< " __FILE__ ":datum return %d\n", status);

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
 * FUNCTION VALUE: (long) Error status (nonzero indicates an error)
 *
 * SCOPE: static (recHallStep.c)
 *
 * PURPOSE:  Look for lost counts or calibration problems.
 *
 * DESCRIPTION:
 *
 *     Calls the device-specific, low-level datum function to move
 *     the hardware.  Writes the current motor position and makes
 *     sure that the move-status link is fired off.
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
	DevHallStep *const pdset = (DevHallStep *)phs->dset;
	long status = 0;

	hsStatus(phs, 1, ">>>> " __FILE__ ":redatum(%p)\n", phs);

	if (!pdset->redatum) {
		hsStatus(phs, HS_ERROR, "Redatum function not defined");
		status = S_ifa_Undefined;
	} else if (!status) {
		status = pdset->redatum(phs);
	}

	hsStatus(phs, 1, "<<<< " __FILE__ ":datum return %d\n", status);

	return status;
}



/*
 *+
 * FUNCTION NAME: cycle
 *
 * INVOCATION: cycle(phs)
 *
 * PARAMETERS:
 *
 *     (!) phs (HallStepRecord) HallStep record
 *
 * FUNCTION VALUE: (long) status value (non-zero indicates an error)
 *
 * SCOPE: static (recHallStep.c)
 *
 * PURPOSE:  
 *
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
 *     should stop within a long time if the hardware hits a 
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
cycle(HallStepRecord *phs)
{
	DevHallStep *const pdset = (DevHallStep *)phs->dset;
	long status = 0;

	if (!pdset->cycleN) {
		hsStatus(phs, HS_ERROR, "CycleN function not defined");
		status = S_ifa_Undefined;
	} else if (!status) {
		hsStatus(phs, HS_STATUS, "Cycle started");

		status = pdset->cycleN(phs, LONG_MAX);
	}

	return status;
}



/*
 *+
 * FUNCTION NAME: diagnose
 *
 * INVOCATION: diagnose(phs)
 *
 * PARAMETERS:
 *
 *     (!) phs (HallStepRecord) HallStep record
 *
 * FUNCTION VALUE:
 *
 *     (long) status value (non-zero indicates an error)
 *
 * SCOPE: static (recHallStep.c)
 *
 * PURPOSE: Exercise a piece of hardware
 *
 *     This is a diagnostic routine which is used to verify that a
 *     piece of hardware is fully operational.
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
diagnose(HallStepRecord *phs)
{
	DevHallStep *const pdset = (DevHallStep *)phs->dset;
	long status = 0;

	hsStatus(phs, 1, ">>>> " __FILE__ ":diagnose(%p)\n", phs);

	if (!pdset->cycleN) {
		hsStatus(phs, HS_ERROR, "CycleN function not defined");
		status = S_ifa_Undefined;
	} else if (!status) {
		hsStatus(phs, HS_STATUS, "Diagnose started");

		status = pdset->cycleN(phs, 1);
	}

	hsStatus(phs, 1, "<<<< " __FILE__ ":diagnose return %d\n", status);

	return status;
}



/************************************************************************
 * Bottom level commands
 ************************************************************************/

/*
 *+
 * FUNCTION NAME: hsSetHome
 *
 * INVOCATION: hsSetHome(phs, pos)
 *
 * PARAMETERS:
 *
 *     (!) phs (HallStepRecord *) HallStep record
 *     (>) pos (long) New home position
 *
 * FUNCTION VALUE: (long) Error status (nonzero indicates an error)
 *
 * SCOPE: global
 *
 * PURPOSE: Set the home position.
 *
 * DESCRIPTION:
 *
 *     Set the datum to a new location
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
hsSetHome(HallStepRecord *phs, long pos)
{
	long status = 0;
	long cpos;

	hsStatus(phs, 1, ">>>> hsSetHome(%p,%ld)", phs, pos);

	hsStatus(phs, HS_STATUS, "Set new datum");

	if (pos > HS_MAX_STEP || pos < -HS_MAX_STEP) {
		recGblRecordError(S_ifa_hs_OutOfRange, (void *)phs,
			"hsSetHome: Illegal value");
		hsStatus(phs, HS_ERROR, "Could not set home to %ld", pos);
		status = S_ifa_hs_OutOfRange;
	}

	if (!status)
		status = hsGetLinkValueLong(phs, &phs->imps, &cpos);

	/*
	 * Switch to counter setting / offset setting mode.
	 */

	if (!status)
		status = hsPutLinkValueLong(phs, &phs->oset, 1);

	/*
	 * Adjust the hardware counter for the new zero position
	 */

	if (!status)
		status = hsPutLinkValueLong(phs, &phs->odmv, cpos - pos);

	/*
	 * Zero the offsets
	 */

	if (!status)
		status = hsPutLinkValueLong(phs, &phs->omvl, cpos - pos);

	/*
	 * Repeat -- the simulated database doesn't set the value correctly
	 * the first time, because it takes it a little bit of time
	 * to update its parameters.
	 */

	if (!status)
		status = hsPutLinkValueLong(phs, &phs->omvl, cpos - pos);

	/*
	 * What a kludge -- add a delay to make sure that the previous
	 * operation completes and the motor parameters are updated, before
	 * we return.  Otherwise the next move operation will fail.
	 */

	hsMSPause(phs, 1000);

	/*
	 * Return to normal operation
	 */

	if (!status)
		status = hsPutLinkValueLong(phs, &phs->oset, 0);

	hsStatus(phs, 1, "<<<< hsSetHome");

	return status;
}

/*
 *+
 * FUNCTION NAME: hsSetOffset
 *
 * INVOCATION: hsSetOffset(phs, pos)
 *
 * PARAMETERS:
 *
 *     (!) phs (HallStepRecord *) HallStep record
 *     (>) pos (long) New offset position
 *
 * FUNCTION VALUE: (long) Error status (nonzero indicates an error)
 *
 * SCOPE: global
 *
 * PURPOSE: Redefines the coordinates of the current position. 
 *
 * DESCRIPTION:
 *
 *     Set the datum to a new location
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
hsSetOffset(HallStepRecord *phs, long pos)
{
	long status = 0;

	hsStatus(phs, 1, ">>>> hsSetOffset(%p,%ld)", phs, pos);

	/*
	 * Switch to counter setting / offset setting mode.
	 */

	if (!status)
		status = hsPutLinkValueLong(phs, &phs->oset, 1);

	/*
	 * Adjust the hardware counter for the new zero position
	 */

	if (!status)
		status = hsPutLinkValueLong(phs, &phs->odmv, pos);

	/*
	 * Zero the offsets
	 */

	if (!status)
		status = hsPutLinkValueLong(phs, &phs->omvl, pos);

	/*
	 * Repeat -- the simulated database doesn't set the value correctly
	 * the first time, because it takes it a little bit of time
	 * to update its parameters.
	 */

	if (!status)
		status = hsPutLinkValueLong(phs, &phs->omvl, pos);

	/*
	 * What a kludge -- add a delay to make sure that the previous
	 * operation completes and the motor parameters are updated, before
	 * we return.  Otherwise the next move operation will fail.
	 */

	hsMSPause(phs, 1000);

	/*
	 * Return to normal operation
	 */

	if (!status)
		status = hsPutLinkValueLong(phs, &phs->oset, 0);

	hsStatus(phs, 1, "<<<< hsSetOffset");

	return status;
}



/*
 *+
 * FUNCTION NAME: hsMax
 *
 * INVOCATION: hsMax(phs, pLink, pPos, width)
 *
 * PARAMETERS:
 *
 *     (!) phs (HallStepRecord *) HallStep record
 *     (!) pLink (struct link *pLink) Link to sensor
 *     (!) pPos (long *) Starting position / maximum
 *     (>) width (int) Width of peak
 *     (>) canStop (int) Can interrupt a move command
 *     (>) searchWidth (int) Initial search range (0 -> ZEROSIG * width)
 *
 * FUNCTION VALUE: (long) Error status (nonzero indicates an error)
 *
 * SCOPE: global
 *
 * PURPOSE:
 *
 *     Search for a peak value
 *
 *     This routine will move the hardware and monitor the Hall effect
 *     sensors to locate a peak value.
 *
 * DESCRIPTION:
 *
 *     This is used by both datum and redatum, so must _not_ do anything
 *     to the home position
 *
 * EXTERNAL VARIABLES: none
 *
 * PRIOR REQUIREMENTS:
 *
 *     Must always be called with the database locked (use dbScanLock).
 *
 * DEFICIENCIES:
 *
 *     This will fail (silently) if it is not actually on a peak.
 *
 *     This assumes that the sensor is not too close to a physical limit,
 *     and assumes the that peak is well defined (i.e., symmetric, and
 *     falls to zero within a few FWHM's (full-width at half maximum).
 *-
 */

long
hsMax(HallStepRecord *phs, struct link *pLink, long *pPos, long width,
	int canStop, long minPos, long maxPos, int searchWidth)
{
	int pos;
	long status = 0;
	double bestval;
	double val;
	int step;
	int initPos;
	QuadFit qf;
	long start;
	long end;
	long preStart;

	hsStatus(phs, 1, ">>>> " __FILE__ ":datum(%p)\n", phs);

	/*
	 * Do a rough search, which will insure that our starting point is 
	 * somewhere near the peak.
	 */

	bestval = -1.0E10; /* Guarantee that the peak position will be set */

	if (searchWidth == 0)
		searchWidth = ZEROSIG * width;

	step = (int)(PEAKSTEP * width);
	if (step < 1)
		step = 1;

	initPos = *pPos;

	start = *pPos - searchWidth;
	if (minPos != maxPos && start < minPos)
		start = minPos;

	end = *pPos + searchWidth;
	if (minPos != maxPos && end > maxPos)
		end = maxPos;

	preStart = *pPos - searchWidth - phs->blsh;
	if (minPos != maxPos && preStart < minPos)
		preStart = minPos;

	if (!status)
		status = hsMoveTo(phs, preStart, canStop);

	for (pos = start; pos <= end && !status; pos += step) {
		if (phs->stop)
			status = S_ifa_hs_Stop;

		if (!status)
			status = hsMoveTo(phs, pos, canStop);

		if (!status) {
			status = hsGetLinkValueDouble3(phs, pLink, &val);
			hsStatus(phs, 3, "++++ i=%d val=%g\n", pos, val);
		}

		if (val > bestval) {
			bestval = val;
			*pPos = pos;
		}
	}

	hsStatus(phs, 2, "**** Rough Searched %ld +- %ld\n", initPos, searchWidth);
	hsStatus(phs, 2, "**** Rough Peak was %g at %d\n", bestval, *pPos);

	/*
	 * Now fit a curve to the peak, and use that to figure out the
	 * location of the maximum.  This assumes that the peak is
	 * not grossly asymmetric, and that we start close enough to
	 * the peak that we can approximate it by a quadratic.
	 */

	qf_init(&qf, *pPos, width);

	step = width * FITSTEP;
	if (step < 1)
		step = 1;

	if (!status)
		status = hsMoveTo(phs, *pPos - width * FITRANGE - phs->blsh, canStop);

	initPos = *pPos;

	for (pos = initPos - width * FITRANGE;
			pos <= *pPos + width * FITRANGE && !status; pos += step) {
		if (phs->stop)
			status = S_ifa_hs_Stop;

		if (!status)
			status = hsMoveTo(phs, pos, canStop);

		if (!status) {
			status = hsGetLinkValueDouble3(phs, pLink, &val);
			hsStatus(phs, 3, "++++ i=%d val=%g\n", pos, val);
		}

		qf_addpoint(&qf, pos, val);
	}

	*pPos = qf_getpeak(&qf); 

	hsStatus(phs, 2, "**** Fit %ld +- %ld\n", initPos, width * FITRANGE);
	hsStatus(phs, 2, "**** Peak was %d\n", *pPos);

	hsStatus(phs, 1, "<<<< " __FILE__ ":datum return %d\n", status);

	return status;
}



/*
 *+
 * FUNCTION NAME: hsMoveTo
 *
 * INVOCATION: hsMoveTo(phs, dest, canStop, pInLimit)
 *
 * PARAMETERS:
 *
 *     (!) phs (HallStepRecord *) HallStep record
 *     (>) dest (long) destination to move to
 *     (>) canStop (int) can interrupt a move command
 *     (<) pInLimit (int *) In limit (+1 = high, -1 = low, 0 = no)
 *
 * FUNCTION VALUE: (long) Error status (nonzero indicates an error)
 *
 * SCOPE: global
 *
 * PURPOSE: Move to a specified location.  
 *
 * DESCRIPTION:
 *
 *     Move to a specified location, with automatic retry on
 *     error.
 *
 *     If STOP is set while this is running, this will cancel the 
 *     operation, and will return a non-zero status.   Otherwise, 
 *     it will return 0.
 *
 * EXTERNAL VARIABLES: none
 *
 * PRIOR REQUIREMENTS:
 *
 *     Must always be called with the database locked (use dbScanLock).
 *
 * DEFICIENCIES:
 *
 *     Does no backlash removal.  Only retries once.
 *-
 */

long
hsMoveTo(HallStepRecord *phs, long dest, int canStop)
{
	long status = 0;

	status = moveTo(phs, dest, canStop);

	/*
	 * Move failed.  Retry it once.
	 */

	if (status && status != S_ifa_hs_Stop) {
		hsStatus(phs, HS_ERROR, "Retrying move");
		status = moveTo(phs, dest, canStop);
		if (!status) /* Clear the warning messages */
			hsStatus(phs, HS_ERROR, "");
	}

	return status;
}



/*
 *+
 * FUNCTION NAME: moveTo
 *
 * INVOCATION: moveTo(phs, dest, canStop)
 *
 * PARAMETERS:
 *
 *     (!) phs (HallStepRecord *) HallStep record
 *     (>) dest (long) destination to move to
 *     (<) canStop (int) Can interrupt a move command
 *
 * FUNCTION VALUE: (long) Error status (nonzero indicates an error)
 *
 * SCOPE: global
 *
 * PURPOSE: Move to a specified location.  
 *
 * DESCRIPTION:
 *
 *     Move to a specified location.  
 *
 *     If STOP is set while this is running, this will cancel the 
 *     operation, and will return a non-zero status.   Otherwise, 
 *     it will return 0.
 *
 * EXTERNAL VARIABLES: none
 *
 * PRIOR REQUIREMENTS:
 *
 *     Must always be called with the database locked (use dbScanLock).
 *
 * DEFICIENCIES:
 *
 *     Does no backlash removal.   Sometimes fails unexpectedly (even
 *     with a large retry count for the motor record).
 *-
 */

static long
moveTo(HallStepRecord *phs, long dest, int canStop)
{
	DevHallStep *const pdset = (DevHallStep *)phs->dset;
	long status = 0;
	long motionDone;
	long imps;
	long init;
	int stop;
	int retry;

	hsStatus(phs, 1, ">>>> hsMoveTo(%p,%ld,%d)\n", phs, dest, canStop);

	stop = 0;

	/*
	 * Get current position.  We want to do this before we start moving,
	 * just in case the move is a very small one.
	 */

	if (!status)
		status = hsGetLinkValueLong(phs, &phs->imps, &init);

	/*
	 * Now order hardware to move
	 */

	if (!status)
		status = hsPutLinkValueDouble(phs, &phs->omvl, dest);

	/*
	 * Make sure that the motion starts before we start checking the
	 * motor DMOV field to see that motion has finished.  Of course,
	 * we don't want to do this if our destination is the same as
	 * our current point.  That would cause us to loop forever 
	 */

	if (init != dest) {
		for (retry = 0; !status && retry < MSTART; retry++) {
			status = hsGetLinkValueLong(phs, &phs->imps, &imps);

			if (init != imps)
				break;

			hsMSPause(phs, MMON);
		}

		if (retry == MSTART) {
			status = S_ifa_hs_MoveFailed;
			hsStatus(phs, HS_ERROR, "Move to %ld never started", dest);
		}
	}

	if (!status && init != dest) {
		do {
			/*
			 * This delay serves four purposes
			 *
			 * 1) It keeps us from wasting cpu cycles.
			 * 2) It gives the motor record a chance to process (for some
			 *    reason, the lock on this record keeps the motor update
			 *    task from updating).  
			 * 3) It lets this record respond to a request to set the
			 *    STOP field.
			 * 4) It provides an extra delay to give the motor time to
			 *    move before we start to monitor DMOV (see the previous
			 *    delay)
			 */

			hsMSPause(phs, MMON);

			/*
			 * Update the mod status display.
			 */

			if (pdset->update)
				(void)pdset->update(phs);

			/*
			 * If STOP is set.  Then interrupt the motion. 
			 */

			if (canStop && phs->stop) {
				stop = 1; /* We want to reset phs->stop later */
				phs->stop = 0; /* Don't process this again */

				/*
				 * Stop the motor.  This will eventually cause the motor
				 * DMOV field to become set, which will terminate this
				 * loop.  We don't set the error status yet, because
				 * we want to come to a gradual stop, without any
				 * more errors.
				 */

				hsStatus(phs, HS_ERROR, "User cancelled move");
				if (!status)
					status = hsPutLinkValueLong(phs, &phs->ostp, 1);
			}

			if (!status)
				status = hsGetLinkValueLong(phs, &phs->idmv, &motionDone);
		} while (motionDone != 1 && !status);
	}

	/*
	 * See if we are where we expect to be.  This will set the status 
	 * to -1 if there was an error or if the movement was cancelled.
	 * Note that we always return an error if the motion is
	 * interrupted by an error/cancel, even if appear to have arrived
	 * at the correct final location.  This is a bit over cautious, but
	 * harmless, and will prevent problems if we have to do a sequence
	 * of short moves (such as for the slow datum search for wheel-like
	 * mechanisms).
	 */

	if (!status) {
		if (stop)
			status = S_ifa_hs_Stop;

		if (!status)
			status = hsGetLinkValueLong(phs, &phs->imps, &imps);
		if (!status && dest != imps) {
			hsStatus(phs, HS_ERROR, "Move to %ld failed (got %d)",
				dest, imps);
			status = S_ifa_hs_MoveFailed;
		}
	}

	/*
	 * If we processed the stop flag, we cleared it.  This can cause
	 * confusion, so reset it.
	 */

	if (!phs->stop)
		phs->stop = stop;

	hsStatus(phs, 1, "<<<< " __FILE__ ":hsMoveTo return %d\n", status);

	return status;
}



/*
 *+
 * FUNCTION NAME: hsMSPause
 *
 * INVOCATION: hsMSPause(phs, msec)
 *
 * PARAMETERS:
 *
 *     (!) phs (HallStepRecord *) HallStep record
 *     (>) msec (int) Delay interval
 *
 * FUNCTION VALUE: none
 *
 * SCOPE: global
 *
 * PURPOSE: Wait for specified time.
 *
 *    This yields the processor and unlocks the database for the
 *    specified time.  As a side effect, other processes may write
 *    to fields of the HallStep record while this record is processing.
 *
 * DESCRIPTION:
 *
 *    Unlock the record, sleeps, then locks the record.
 *
 * EXTERNAL VARIABLES: none
 *
 * PRIOR REQUIREMENTS:
 *
 *     Must always be called with the database locked (use dbScanLock).
 *
 * DEFICIENCIES:
 *
 *     Fails (silently) if msec < 0.
 *-
 */

void
hsMSPause(HallStepRecord *phs, int msec)
{
	struct timespec ts;

	dbScanUnlock((struct dbCommon *)phs);
	ts.tv_sec = msec / 1000;
	ts.tv_nsec = (msec % 1000) * 1000000;
	nanosleep(&ts, NULL);
	dbScanLock((struct dbCommon *)phs);
}



/*
 *+
 * FUNCTION NAME: hsGetLinkValueLong
 *
 * INVOCATION: hsGetLinkValueLong(phs, pLink, pVal)
 *
 * PARAMETERS:
 *
 *     (!) phs (HallStepRecord *) HallStep record
 *     (!) pLink (struct link *) link to record containing value
 *     (<) pVal (long *) value
 *
 * FUNCTION VALUE: (long) Error status (nonzero indicates an error)
 *
 * SCOPE: global
 *
 * PURPOSE:
 *
 *     Read an EPICS database link as a long value
 *
 * DESCRIPTION:
 *
 *     This routine does nothing but call the standard EPICS
 *     recGblGetLinkValue.  It is provided only as protection against
 *     accidents which may occur if the data types are mismatched.
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

long 
hsGetLinkValueLong(HallStepRecord *phs, struct link *pLink, long *pVal)
{
	long options;
	long nRequest;
	long status = 0;

	options = 0;
	nRequest = 1; /* Only getting 1 element */

	status = recGblGetLinkValue(pLink, phs, DBF_LONG, pVal, &options, 
		&nRequest);
	if (status) {
		fprintf(stderr, 
			"hsGetLinkValueLong(%p,%p,%p): recGblGetLinkValue returned %d\n",
			pLink, phs, pVal, status);
	}

	return status;
}



/*
 *+
 * FUNCTION NAME: hsPutLinkValueLong
 *
 * INVOCATION: hsPutLinkValueLong(phs, pLink, val)
 *
 * PARAMETERS:
 *
 *     (!) phs (HallStepRecord *) HallStep Record
 *     (!) pLink (struct link *) Link to process variable which destination
 *     (>) val (long) New value
 *
 * FUNCTION VALUE: (long) Error status (nonzero indicates an error)
 *
 * SCOPE: global
 *
 * PURPOSE:
 *
 *     Write an EPICS database link as a long value
 *
 * DESCRIPTION:
 * 
 *     This routine does nothing but call the standard EPICS
 *     recGblPutLinkValue.  It is provided only as protection against
 *     accidents which may occur if the data types are mismatched.
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

long
hsPutLinkValueLong(HallStepRecord *phs, struct link *pLink, long val)
{
	long nRequest;
	long status = 0;

	nRequest = 1; /* Only getting 1 element */

	status = recGblPutLinkValue(pLink, phs, DBF_LONG, &val, &nRequest);
	if (status) {
		fprintf(stderr, 
			"hsPutLinkValueLong(%p,%p,%ld): recGblPutLinkValue returned %d\n",
			pLink, phs, val, status);
	}

	return status;
}



/*
 *+
 * FUNCTION NAME: hsPutLinkValueDouble
 *
 * INVOCATION: hsPutLinkValueDouble(phs, pLink, val)
 *
 * PARAMETERS:
 *
 *     (!) phs (HallStepRecord *) HallStep Record
 *     (!) pLink (struct link *) Link to process variable which destination
 *     (>) val (double) New value
 *
 * FUNCTION VALUE: (long) Error status (nonzero indicates an error)
 *
 * SCOPE: global
 *
 * PURPOSE:
 *
 *     Write an EPICS database link as a long value
 *
 * DESCRIPTION:
 * 
 *     This routine does nothing but call the standard EPICS
 *     recGblPutLinkValue.  It is provided only as protection against
 *     accidents which may occur if the data types are mismatched.
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

long
hsPutLinkValueDouble(HallStepRecord *phs, struct link *pLink, double val)
{
	long nRequest;
	long status = 0;

	nRequest = 1; /* Only getting 1 element */

	status = recGblPutLinkValue(pLink, phs, DBF_DOUBLE, &val, &nRequest);
	if (status) {
		fprintf(stderr, 
			"hsPutLinkValueDouble(%p,%p,%f): recGblPutLinkValue returned %d\n",
			pLink, phs, val, status);
	}

	return status;
}



/*
 *+
 * FUNCTION NAME: hsGetLinkValueDouble
 *
 * INVOCATION: hsGetLinkValueDouble(phs, pLink, pVal)
 *
 * PARAMETERS:
 *     (!) phs (HallStepRecord *) HallStep record
 *     (!) pLink (struct link *) Link to process variable containing value
 *     (!) pVal (double *) Destination of value
 *
 * FUNCTION VALUE: (long) Error status (nonzero indicates an error)
 *
 * SCOPE: global
 *
 * PURPOSE: Read an EPICS database link as a double value
 *
 * DESCRIPTION:
 * 
 *     This routine does nothing but call the standard EPICS
 *     recGblGetLinkValue.  It is provided only as protection against
 *     accidents which may occur if the data types are mismatched.
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

long 
hsGetLinkValueDouble(HallStepRecord *phs, struct link *pLink, double *pVal)
{
	long options;
	long nRequest;
	long status = 0;

	options = 0;
	nRequest = 1; /* Only getting 1 element */

	status = recGblGetLinkValue(pLink, phs, DBF_DOUBLE, pVal, &options, 
		&nRequest);

	return status;
}



/*
 *+
 * FUNCTION NAME: hsGetLinkValueDouble3
 *
 * INVOCATION: hsGetLinkValueDouble3(phs, pLink, pVal)
 *
 * PARAMETERS:
 *     (!) phs (HallStepRecord *) HallStep record
 *     (!) pLink (struct link *) Link to process variable containing value
 *     (!) pVal (double *) Destination of value
 *
 * FUNCTION VALUE: (long) Error status (nonzero indicates an error)
 *
 * SCOPE: global
 *
 * PURPOSE: Read an EPICS database link as a double value
 *
 * DESCRIPTION:
 * 
 *     Read a value from a link three times, and takes the median,
 *     to get rid of an occasional noisy data value.
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

long 
hsGetLinkValueDouble3(HallStepRecord *phs, struct link *pLink, double *pVal)
{
	int i;
	long status = 0;
	double val[3];

	for (i = 0; i < 3; i++) {
		hsMSPause(phs, 20); /* Don't read the same value */
		status = hsGetLinkValueDouble(phs, pLink, &val[i]);
	}

	*pVal = median3(val);

	return status;
}



/*
 *+
 * FUNCTION NAME: hsSeekHome
 *
 * INVOCATION: hsSeekHome(phs)
 *
 * PARAMETERS:
 *
 *     (!) phs (HallStepRecord *) HallStep record
 *     (!) phs->ohmf (struct link *) Output link for home command
 *     (!) phs->ostp (struct link *) Output link for stop command
 *     (!) phs->idmv (struct link *) Input link for motion completion
 *     (>) phs->stop (int) Interrupt command
 *     (>) timeout (int) Seconds before aborting
 *
 * FUNCTION VALUE: (long) Error status (nonzero indicates an error)
 *
 * SCOPE: global
 *
 * PURPOSE:
 *
 *     Issue a steppermotor home forward (HOMF) command
 *
 *     This function takes care of various setup that is required to
 *     use the HOMF field of the steppermotor record.
 *
 * DESCRIPTION:
 *
 *     This function takes care of various setup that is required to
 *     use the HOMF field of the steppermotor record.
 *
 * EXTERNAL VARIABLES: none
 *
 * PRIOR REQUIREMENTS:
 *
 *     Must always be called with the database locked (use dbScanLock).
 *
 * DEFICIENCIES:
 *
 *     Does no backlash correction or peak finding.
 *-
 */

long
hsSeekHome(HallStepRecord *phs, int timeout)
{
	long status = 0;
	long motionDone;
	int passed;
	long stopTime;
	int didStop = 0;
	int didTimeOut = 0;

	passed = 1;

	hsStatus(phs, 1, ">>>> " __FILE__ ":hsSeekHome(%p)\n", phs);

	/*
	 * When should we worry that the action has failed?  The + 2 is
	 * a safety factor, to protect against the granularity of the
	 * time() call.
	 */

	stopTime = time(NULL) + timeout + 2;

	if (!status) {
		/*
		 * Trigger the move.  This may not cause an actual motion,
		 * if we are already close to home.
		 */

		status = hsPutLinkValueLong(phs, &phs->ohmf, 1);
	}

	/*
	 * Make sure that we have time for the move to begin, before we
	 * start checking the motor DMOV field to see that motion has
	 * finished.  This initial delay is, of course, added to the
	 * next delay on the first pass.
	 */

	if (MWAIT > MMON)
		hsMSPause(phs, MWAIT - MMON);

	if (!status) {
		do {
			/*
			 * This delay serves four purposes
			 *
			 * 1) It keeps us from wasting cpu cycles.
			 * 2) It gives the motor record a chance to process (for some
			 *    reason, the lock on this record keeps the motor update
			 *    task from updating).  
			 * 3) It lets this record respond to a request to set the
			 *    STOP field.
			 * 4) It provides an extra delay to give the motor time to
			 *    move before we start to monitor DMOV (see the previous
			 *    delay)
			 */

			hsMSPause(phs, MMON);

			/*
			 * If STOP is set.  Then interrupt the motion. 
			 */

			if (!status && !didStop && !didTimeOut && phs->stop) {
				phs->stop = 0; /* Don't process this again */
				didStop = 1;

				/*
				 * Stop the motor.  This will eventually cause the motor
				 * DMOV field to become set, which will terminate this
				 * loop
				 */

				hsStatus(phs, HS_ERROR, "User cancelled move");
				if (!status)
					status = hsPutLinkValueLong(phs, &phs->ostp, 1);
			}

			/* 
			 * If exceeded timeout
			 */

			if (!status && !didStop && !didTimeOut && time(NULL) > stopTime) {
				didTimeOut = 1;

				/*
				 * Stop the motor.  This will eventually cause the motor
				 * DMOV field to become set, which will terminate this
				 * loop
				 */

				hsStatus(phs, HS_ERROR, "Search timed out");
				if (!status)
					status = hsPutLinkValueLong(phs, &phs->ostp, 1);
			}

			if (!status)
				status = hsGetLinkValueLong(phs, &phs->idmv, &motionDone);
		} while (motionDone != 1 && !status);
	}

	/*
	 * Return an error code if this was interrupted.
	 */

	if (!status && didTimeOut)
		status = S_ifa_hs_TimeOut;

	if (!status && didStop)
		status = S_ifa_hs_Stop;

	hsStatus(phs, 1, "<<<< " __FILE__ ":hsSeekHome return %d\n", status);

	return status;
}



/*
 *+
 * FUNCTION NAME: hsCycleN
 *
 * INVOCATION: hsCycleN(phs, nRep)
 *
 * PARAMETERS:
 *
 *     (!) phs (HallStepRecord *) HallStep record
 *     (>) nRep (int) How many times to repeat
 *     (<) canStop (int) Can interrupt a move command
 *
 * FUNCTION VALUE: (long) status value (non-zero indicates an error)
 *
 * SCOPE: global
 *
 * PURPOSE:  Exercise a piece of hardware
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
 *
 *     Assumes that various fields which are adjacent in memory may
 *     be treated as if they were an array.  This is not safe programming,
 *     but appears to work with the EPICS-generated data structures.
 *-
 */

long
hsCycleN(HallStepRecord *phs, int nRep, int canStop)
{
	long status = 0;
	int pass = 0;
	int i;
	long dest[CYCLE_POINTS];

	hsStatus(phs, 1, ">>>> " __FILE__ ":hsCycleN(%p,%d)\n", phs, nRep);

	phs->cstp = 0;
	if (MONITORMASK)
		db_post_events(phs, &phs->cstp, MONITORMASK);

	/*
	 * Save the current destination, just in case some idiot changes
	 * them to an out-of-range value while we are in the middle of 
	 * a pass.
	 */

	for (i = 0; i < CYCLE_POINTS; i++)
		dest[i] = (&phs->cdsa)[i];

	/*
	 * Move to the last point, so that we always have a reproducable
	 * first data point.
	 */

	if (!status)
		status = hsMoveTo(phs, dest[CYCLE_POINTS - 1], canStop);

	/*
	 * Cycle through the three points.  Note that we always complete the
	 * full cycle, to insure that we ultimately end up at a
	 * backlash-corrected position.
	 */

	for (pass = 0; !status && !phs->cstp && pass < nRep && !phs->stop; pass++) {
		for (i = 0; i < CYCLE_POINTS && !status; i++) {
			hsStatus(phs, HS_STATUS, "Cycle/Pass %d%s", 
				pass, cycle_names[i]);

			if (!status)
				status = hsMoveTo(phs, dest[i], canStop);

			if (!status) {
				if (phs->en1p) {
					status = hsGetLinkValueDouble3(phs, &phs->hs1p, 
						&phs->v1pa + i);
				} else {
					(&phs->v1pa)[i] = (&phs->e1pa)[i];
				}
			}

			if (!status) {
				if (phs->en1b) {
					status = hsGetLinkValueDouble3(phs, &phs->hs1b, 
						&phs->v1ba + i);
				} else {
					(&phs->v1ba)[i] = (&phs->e1ba)[i];
				}
			}

			if (!status) {
				if (phs->en2p) {
					status = hsGetLinkValueDouble3(phs, &phs->hs2p, 
						&phs->v2pa + i);
				} else {
					(&phs->v2pa)[i] = (&phs->e2pa)[i];
				}
			}

			if (!status) {
				if (phs->en2b) {
					status = hsGetLinkValueDouble3(phs, &phs->hs2b, 
						&phs->v2ba + i);
				} else {
					(&phs->v2ba)[i] = (&phs->e2ba)[i];
				}
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
 *+
 * FUNCTION NAME: median3
 *
 * INVOCATION: median3(val)
 *
 * PARAMETERS:
 *
 *     (!) val (double[3]) Raw data values
 *
 * FUNCTION VALUE: (double) median of three values (no error possible)
 *
 * SCOPE: global
 *
 * PURPOSE:  
 *
 *     Compute the median of 3 values
 *
 * DESCRIPTION:
 *
 *     Side effects:  Leaves 'val' sorted.
 *
 * EXTERNAL VARIABLES: none
 *
 * PRIOR REQUIREMENTS: none
 *
 * DEFICIENCIES:
 *
 *     Behavior is undefined 'val' is null or if val is has fewer than 3
 *     elements.  More than 3 elements are ignored.
 *-
 */

static double
median3(double val[3])
{
	double temp;

	if (val[0] > val[1]) {
		temp = val[0];
		val[0] = val[1];
		val[1] = temp;
	}

	if (val[0] > val[2]) {
		temp = val[0];
		val[0] = val[2];
		val[2] = temp;
	}

	if (val[1] > val[2]) {
		temp = val[1];
		val[1] = val[2];
		val[2] = temp;
	}

	return val[1];
}



/*
 *+
 * FUNCTION NAME: hsStatus
 *
 * INVOCATION: hsStatus(phs, sev, fmt, ...)
 *
 * PARAMETERS:
 *
 *     (!) phs (HallStepRecord *) HallStep record
 *     (>) sev (int) severity -- smaller numbers are more important
 *     (>) fmt (const char fmt[]) printf-like format
 *     (>) [Optional arguments] (arbitrary) printf-like arguments
 *
 * FUNCTION VALUE: none
 *
 * SCOPE: global
 *
 * PURPOSE: Set the status (opst) field of the HallStep record.  
 *
 * Printf-type formats and arguments may be used.
 *
 * DESCRIPTION:
 *     
 *     If the severity (0-255) is greater than or equal to hsDebug,
 *     prints the string.
 *
 *     If the HS_MESS bit is set, generates a string from the
 *     printf type arguments, sets the message field, and posts the
 *     appropriate monitor.  If hsDebug is > 0, then it also prints
 *     the message.
 *
 *     If the HS_ERROR bit is set, generates a string from the
 *     printf type arguments, sets the message field, posts the
 *     appropriate monitor, and prints the message.  
 *
 *     If the HS_STATUS bit is set, generates a string from the
 *     printf type arguments, sets the status field, posts the
 *     appropriate monitor.  If hsDebug is > 0, then it also prints
 *     the message.
 *
 * EXTERNAL VARIABLES: hsDebug
 *
 * PRIOR REQUIREMENTS:
 *
 *     Must always be called with the database locked (use dbScanLock).
 *
 * DEFICIENCIES:
 *
 *     It is possible to pass this an argument that will overwrite the
 *     scratch area, and corrupt memory.
 *-
 */

void
hsStatus(HallStepRecord *phs, int sev, const char fmt[], ...)
{
	va_list ap;

	va_start(ap, fmt);
	if (strcmp(phs->opst, fmt) != 0) {
/* fkraemer - testing
		if (sev == HS_ERROR || sev == HS_MESS || sev == HS_STATUS) {*/
			char buf[1024];

			vsprintf(buf, fmt, ap);

			if (sev == HS_ERROR || sev == HS_MESS) {
				strncpy(phs->mess, buf, MAX_STRING_SIZE);
				phs->mess[MAX_STRING_SIZE - 1] = '\0';
				if (MONITORMASK)
					db_post_events(phs, phs->mess, MONITORMASK);
			} 
			
			if (sev == HS_STATUS) {
				strncpy(phs->opst, buf, MAX_STRING_SIZE);
				phs->opst[MAX_STRING_SIZE - 1] = '\0';
				if (MONITORMASK)
					db_post_events(phs, phs->opst, MONITORMASK);
			}

/* fkraemer - testing
			if ((sev == HS_ERROR || hsDebug > 0) && *buf) {*/
				fprintf(stderr, "%s: %s\n", phs->name, buf);
/* fkraemer - testing
			}
		} else {
			if (hsDebug >= sev)
				vprintf(fmt, ap);
		}*/
	}
	va_end(ap);
}

/*
 * Read the limit switches.  This doesn't deal with the pathological
 * case where a limit switch is stuck.  This can be detected using
 * the diagnose command.
 */

long
hsCheckLimits(HallStepRecord *phs, int *pInLimit)
{
	long high;
	long low;
	long status;
	
	status = 0;

	/*
	 * See if a limit switch is set.
	 */

	if (!status) {
		status = hsGetLinkValueLong(phs, &phs->ihls, &high);
		if (!status && high)
			*pInLimit = HS_LIMIT_HIGH;
	}

	if (!status) {
		status = hsGetLinkValueLong(phs, &phs->ills, &low);
		if (!status && low)
			*pInLimit = HS_LIMIT_LOW;
	}

	return status;
}

/*
 * Initialize the data set.  Xzero should be an approximation of the
 * average value of the x coordinates of the data set, and xscale
 * should be an estimate of the range of x coordinates.  It is
 * always acceptable to set xscale to 1.0 and xzero to zero, but
 * results will be much less accurate if x values are large.
 */

static void
qf_init(QuadFit *pQf, double xzero, double xscale)
{
	pQf->xzero = xzero;
	pQf->xscale = xscale;

	pQf->syx2 = 0.0;
	pQf->syx1 = 0.0;
	pQf->syx0 = 0.0;
	pQf->sx4 = 0.0;
	pQf->sx3 = 0.0;
	pQf->sx2 = 0.0;
	pQf->sx1 = 0.0;
	pQf->sx0 = 0.0;
}

static void
qf_addpoint(QuadFit *pQf, double xi, double yi)
{
	double x1, x2, x3, x4;

	x1 = (xi - pQf->xzero) / pQf->xscale;
	x2 = x1 * x1;
	x3 = x2 * x1;
	x4 = x3 * x1;

	pQf->sx0++;
	pQf->sx1 += x1;
	pQf->sx2 += x2;
	pQf->sx3 += x3;
	pQf->sx4 += x4;
	pQf->syx0 += yi;
	pQf->syx1 += yi * x1;
	pQf->syx2 += yi * x2;
}

static double
qf_getpeak(QuadFit *pQf)
{
	double a, b;

	a = pQf->syx2 * (pQf->sx2 * pQf->sx0 - pQf->sx1 * pQf->sx1)
		+ pQf->syx1 * (pQf->sx2 * pQf->sx1 - pQf->sx3 * pQf->sx0)
		+ pQf->syx0 * (pQf->sx3 * pQf->sx1 - pQf->sx2 * pQf->sx2);

	b = pQf->syx2 * (pQf->sx2 * pQf->sx1 - pQf->sx3 * pQf->sx0)
		+ pQf->syx1 * (pQf->sx4 * pQf->sx0 - pQf->sx2 * pQf->sx2)
		+ pQf->syx0 * (pQf->sx3 * pQf->sx2 - pQf->sx4 * pQf->sx1);

	return -b / a / 2.0 * pQf->xscale + pQf->xzero;
}
