static char rcsid[] = "$Id: recCool.c,v 1.2 2009/05/27 19:34:45 fkraemer Exp $";

/*
 * Copyright 1998 University of Hawaii, Institute for Astronomy.  All
 * rights reserved.
 *
 * The University of Hawaii grants AURA a non-exclusive license to 
 * use this software, as stated in [contract].
 *
 * Author: Hubert Yamada, University of Hawaii, Institute for Astronomy
 *
 * Device Support Routines for cooling process-control record
 * wheel-like component.
 *
 * FILENAME
 *     recCool.c
 *
 * FUNCTION NAME(S)
 */

/*
 * This module borrows heavily from the longin record:
 *
 *      Author:  Janet Anderson
 *      Date:    9/23/91
 *
 *      Experimental Physics and Industrial Control System (EPICS)
 *
 *      Copyright 1991, the Regents of the University of California,
 *      and the University of Chicago Board of Governors.
 *
 *      This software was produced under  U.S. Government contracts:
 *      (W-7405-ENG-36) at the Los Alamos National Laboratory,
 *      and (W-31-109-ENG-38) at Argonne National Laboratory.
 *
 *      Initial development by:
 *              The Controls and Automation Group (AT-8)
 *              Ground Test Accelerator
 *              Accelerator Technology Division
 *              Los Alamos National Laboratory
 *
 *      Co-developed with
 *              The Controls and Computing Group
 *              Accelerator Systems Division
 *              Advanced Photon Source
 *              Argonne National Laboratory
 */

#include <vxWorks.h>
#include <types.h>
#include <stdioLib.h>
#include <lstLib.h>
#include <string.h>

#include <alarm.h>
#include <dbDefs.h>
#include <dbEvent.h>
#include <dbAccess.h>
#include <dbFldTypes.h>
#include <devSup.h>
#include <errMdef.h>
#include <recSup.h>
#include <time.h>
#include <stdlib.h>
#include <taskLib.h>

#include <coolRecord.h>
#include "recCool.h"

#define PWR_ON (1)
#define PWR_OFF (2)

/*
 * Create RSET - Record Support Entry Table
 */

#define report NULL
#define initialize NULL
static long init_record(struct coolRecord *, int);
static long process(struct coolRecord *);

#define special NULL
static long get_value(coolRecord *, struct valueDes *);

#define cvt_dbaddr NULL
#define get_array_info NULL
#define put_array_info NULL
static long get_units(struct dbAddr *, char *);
static long get_precision(struct dbAddr *, long *);

#define get_enum_str NULL
#define get_enum_strs NULL
#define put_enum_str NULL
static long get_graphic_double(struct dbAddr *, struct dbr_grDouble *);
static long get_control_double(struct dbAddr *, struct dbr_ctrlDouble *);
static long get_alarm_double(struct dbAddr *, struct dbr_alDouble *);

struct rset coolRSET = {
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

typedef struct cool_dset_ {			/* cool input dset */
	long number;
	DEVSUPFUN dev_report;
	DEVSUPFUN init;
	DEVSUPFUN init_record;		/* returns: (-1,0)=>(failure,success) */
	DEVSUPFUN get_ioint_info;
} CoolDSet;

typedef struct parms_ {
	SEM_ID semOp;
} Parms;

static void alarm(struct coolRecord *);
static void monitor(struct coolRecord *);
static void helperTask(int);
static long putLinkValueLong(struct coolRecord *, struct link *, long);
static long getLinkValueLong(struct coolRecord *, struct link *, long *);
static void msPause(struct coolRecord *, int);

static long
init_record(struct coolRecord *pCool, int pass)
{
	CoolDSet *pdset;
	long status = OK;
	static int recno;
	char buf[1024];

	/*
	 * Set initial state.
	 */

	strncpy(pCool->busy, "Off", sizeof(pCool->busy));
	pCool->busy[sizeof(pCool->busy) - 1] = '\0';

	/*
	 * Check for device support (not really needed)
	 */

	if (status == OK) {
		if (!(pdset = (CoolDSet *)(pCool->dset))) {
			recGblRecordError(S_dev_noDSET, (void *)pCool,
				__FILE__ "::init_record");
			status = S_dev_noDSET;
		}
	}

	if (pass == 0) {

		/*
		 * Initialize record
		 */

		if (status == OK && pdset->init_record)
			status = (*pdset->init_record)(pCool, pass);

		/*
		 * Allocate and initialize private data structure
		 */

		if (status == OK) {
			if ((pCool->dpvt = malloc(sizeof(Parms))) == NULL)
				status = S_rec_outMem;
		}

		if (status == OK) {
			Parms *const pParms = (Parms *)pCool->dpvt;

			pParms->semOp = semBCreate(SEM_Q_FIFO, SEM_EMPTY);
		}

		/*
		 * Start helper function
		 */

		if (status == OK) {
			sprintf(buf, "cool%d", recno++);
			if (taskSpawn(buf, 42, VX_FP_TASK, 8000, (FUNCPTR)helperTask,
					(int)pCool, 0, 0, 0, 0, 0, 0, 0, 0, 0) == ERROR)
				status = ERROR;
		}

	} else {

		/*
		 * cool.siml must be a CONSTANT or a PV_LINK or a DB_LINK
		 */

		if (status == OK) {
			if (pCool->siml.type == CONSTANT) {
				pCool->simm = pCool->siml.value.value;
			} else {
				status = recGblInitFastInLink(&(pCool->siml), (void *)pCool,
					DBR_ENUM, "SIMM");
			}
		}

		/*
		 * cool.siol must be a CONSTANT or a PV_LINK or a DB_LINK
		 */

		if (status == OK) {
			if (pCool->siol.type == CONSTANT) {
				pCool->sval = pCool->siol.value.value;
			} else {
				status = recGblInitFastInLink(&(pCool->siol), (void *)pCool,
					DBR_LONG, "SVAL");
			}
		}

		/*
		 * Initialize record
		 */

		if (status == OK && pdset->init_record)
			status = (*pdset->init_record)(pCool, pass);
	}

	return status;
}



static long
process(struct coolRecord *pCool)
{
	long status;

	if (pCool->pact == TRUE) {
		/*
		 * Already processing a request -- return an error
		 */

		status = S_ifa_cool_Busy;
	} else {
		/*
		 * Set processing active
		 */

		pCool->pact = TRUE;
		recGblGetTimeStamp(pCool);

		/*
		 * Wake the helper task
		 */

		semGive(((Parms *)pCool->dpvt)->semOp);
	}

	/*
	 * process the forward scan link record
	 */
	 
	recGblFwdLink(pCool);

	return status;
}

static long
get_precision(struct dbAddr *paddr, long *precision)
{
	struct coolRecord *pCool = (struct coolRecord *)paddr->precord;

	*precision = pCool->prec;
	recGblGetPrec(paddr, precision);
	return 0;
}



static long
get_value(coolRecord *pCool, struct valueDes *pvdes)
{
	pvdes->field_type = DBF_LONG;
	pvdes->no_elements = 1;
	pvdes->pvalue = (void *)&pCool->val;

	return 0;
}



static long
get_units(struct dbAddr *paddr, char *units)
{
	struct coolRecord *pCool = (struct coolRecord *)paddr->precord;

	strncpy(units, pCool->egu, DB_UNITS_SIZE);

	return 0;
}



static long
get_graphic_double(struct dbAddr *paddr, struct dbr_grDouble *pgd)
{
	struct coolRecord *pCool = (struct coolRecord *)paddr->precord;

	if (paddr->pfield == (void *)&pCool->val
			|| paddr->pfield == (void *)&pCool->hihi
			|| paddr->pfield == (void *)&pCool->high
			|| paddr->pfield == (void *)&pCool->low
			|| paddr->pfield == (void *)&pCool->lolo) {
		pgd->upper_disp_limit = pCool->hopr;
		pgd->lower_disp_limit = pCool->lopr;
	} else
		recGblGetGraphicDouble(paddr, pgd);
	return 0;
}



static long
get_control_double(struct dbAddr *paddr, struct dbr_ctrlDouble *pcd)
{
	struct coolRecord *pCool = (struct coolRecord *)paddr->precord;

	if (paddr->pfield == (void *)&pCool->val
			|| paddr->pfield == (void *)&pCool->hihi
			|| paddr->pfield == (void *)&pCool->high
			|| paddr->pfield == (void *)&pCool->low
			|| paddr->pfield == (void *)&pCool->lolo) {
		pcd->upper_ctrl_limit = pCool->hopr;
		pcd->lower_ctrl_limit = pCool->lopr;
	} else
		recGblGetControlDouble(paddr, pcd);
	return 0;
}



static long
get_alarm_double(struct dbAddr *paddr, struct dbr_alDouble *pad)
{
	struct coolRecord *pCool = (struct coolRecord *)paddr->precord;

	if (paddr->pfield == (void *)&pCool->val) {
		pad->upper_alarm_limit = pCool->hihi;
		pad->upper_warning_limit = pCool->high;
		pad->lower_warning_limit = pCool->low;
		pad->lower_alarm_limit = pCool->lolo;
	} else
		recGblGetAlarmDouble(paddr, pad);
	return 0;
}



static void
alarm(struct coolRecord *pCool)
{
	double val;
	float hyst;
	float lalm;
	float hihi;
	float high;
	float low;
	float lolo;
	unsigned short hhsv;
	unsigned short llsv;
	unsigned short hsv;
	unsigned short lsv;

	if (pCool->udf == TRUE) {
		recGblSetSevr(pCool, UDF_ALARM, INVALID_ALARM);
		return;
	}
	hihi = pCool->hihi;
	lolo = pCool->lolo;
	high = pCool->high;
	low = pCool->low;
	hhsv = pCool->hhsv;
	llsv = pCool->llsv;
	hsv = pCool->hsv;
	lsv = pCool->lsv;
	val = pCool->val;
	hyst = pCool->hyst;
	lalm = pCool->lalm;

	/*
	 * alarm condition hihi
	 */

	if (hhsv && (val >= hihi || ((lalm == hihi) && (val >= hihi - hyst)))) {
		if (recGblSetSevr(pCool, HIHI_ALARM, pCool->hhsv))
			pCool->lalm = hihi;
		return;
	}

	/*
	 * alarm condition lolo
	 */

	if (llsv && (val <= lolo || ((lalm == lolo) && (val <= lolo + hyst)))) {
		if (recGblSetSevr(pCool, LOLO_ALARM, pCool->llsv))
			pCool->lalm = lolo;
		return;
	}

	/*
	 * alarm condition high
	 */

	if (hsv && (val >= high || ((lalm == high) && (val >= high - hyst)))) {
		if (recGblSetSevr(pCool, HIGH_ALARM, pCool->hsv))
			pCool->lalm = high;
		return;
	}

	/*
	 * alarm condition low
	 */

	if (lsv && (val <= low || ((lalm == low) && (val <= low + hyst)))) {
		if (recGblSetSevr(pCool, LOW_ALARM, pCool->lsv))
			pCool->lalm = low;
		return;
	}

	/*
	 * we get here only if val is out of alarm by at least hyst
	 */

	pCool->lalm = val;
	return;
}



static void
monitor(struct coolRecord *pCool)
{
	unsigned short monitor_mask;
	long delta;

	/*
	 * get previous stat and sevr  and new stat and sevr
	 */

	monitor_mask = recGblResetAlarms(pCool);

	/*
	 * Check for changes in field values
	 */

	if (pCool->mon & COOL_MONITOR_RATE) {
		monitor_mask |= DBE_VALUE;
		db_post_events(pCool, &pCool->rate, monitor_mask);
		db_post_events(pCool, &pCool->lrte, monitor_mask);
	}

	if (pCool->mon & COOL_MONITOR_INC) {
		monitor_mask |= DBE_VALUE;
		db_post_events(pCool, &pCool->inc, monitor_mask);
	}

	if (pCool->mon & COOL_MONITOR_BUSY) {
		monitor_mask |= DBE_VALUE;
		db_post_events(pCool, &pCool->busy, monitor_mask);
	}

	pCool->mon = 0;

	/*
	 * check for value change
	 */

	delta = pCool->mlst - pCool->val;
	if (delta < 0)
		delta = -delta;
	if (delta > pCool->mdel) {
		/*
		 * post events for value change
		 */

		monitor_mask |= DBE_VALUE;

		/*
		 * update last value monitored
		 */

		pCool->mlst = pCool->val;
	}

	/*
	 * check for archive change
	 */

	delta = pCool->alst - pCool->val;
	if (delta < 0)
		delta = -delta;
	if (delta > pCool->adel) {
		/*
		 * post events on value field for archive change
		 */

		monitor_mask |= DBE_LOG;

		/*
		 * update last archive value monitored
		 */

		pCool->alst = pCool->val;
	}

	/*
	 * send out monitors connected to the value field
	 */

	if (monitor_mask)
		db_post_events(pCool, &pCool->val, monitor_mask);

	return;
}

static long
putLinkValueLong(struct coolRecord *pCool, struct link *pLink, long val)
{
	long nRequest;
	long status = 0;

	nRequest = 1; /* Only getting 1 element */

	status = recGblPutLinkValue(pLink, pCool, DBF_LONG, &val, &nRequest);
	if (status) {
		fprintf(stderr, 
			"wfsPutLinkValueLong(%p,%p,%ld): recGblPutLinkValue returned %d\n",
			pLink, pCool, val, status);
	}

	return status;
}



/*
 *+
 * FUNCTION NAME: getLinkValueLong
 *
 * INVOCATION: getLinkValueLong(pCool, pLink, pVal)
 *
 * PARAMETERS:
 *
 *     (!) pCool (struct coolRecord *) struct coolRecord
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

static long 
getLinkValueLong(struct coolRecord *pCool, struct link *pLink, long *pVal)
{
	long options;
	long nRequest;
	long status = 0;

	options = 0;
	nRequest = 1; /* Only getting 1 element */

	status = recGblGetLinkValue(pLink, pCool, DBF_LONG, pVal, &options, 
		&nRequest);
	if (status) {
		fprintf(stderr, 
			"hsGetLinkValueLong(%p,%p,%p): recGblGetLinkValue returned %d\n",
			pLink, pCool, pVal, status);
	}

	return status;
}

/*
 * Processing an operation can take a long time, so I use a separate
 * task to do the actual processing.
 */

static void
helperTask(int ptr)
{
	struct coolRecord *const pCool = (struct coolRecord *)ptr;
	long dmov0;
	long dmov1;
	int didOffset;

	for (;;) {
		/*
		 * Wait for an operation
		 */

		if (pCool->rate == pCool->lrte || pCool->inc == 0)
			semTake(((Parms *)(pCool->dpvt))->semOp, WAIT_FOREVER); 

		/*
		 * Initialize
		 */

		didOffset = 0;

		/*
		 * Lock the record
		 */

		dbScanLock((struct dbCommon *)pCool);

		/*
		 * Offset the motor.
		 */

		if (pCool->inc != 0) {
			long pos;

			/*
			 * Set state.
			 */

			strncpy(pCool->busy, "Offset", sizeof(pCool->busy));
			pCool->busy[sizeof(pCool->busy) - 1] = '\0';
			db_post_events(pCool, &pCool->busy, DBE_VALUE);

			/*
			 * Cancel the previous motion, if any.
			 *
			 * Wait for the motion to complete.  If this record
			 * is called too quickly, a request to stop motion
			 * might get lost, so resend it, regularly.
			 */

			do {
				putLinkValueLong(pCool, &pCool->ojg0, 0);
				putLinkValueLong(pCool, &pCool->ojg1, 0);
				msPause(pCool, 200);
				getLinkValueLong(pCool, &pCool->idm0, &dmov0);
				getLinkValueLong(pCool, &pCool->idm1, &dmov1);
			} while (!dmov0 && !dmov1);

			/*
			 * Disable the second motor, enable the first, read the
			 * current position, and set the appropriate speed.
			 */

			putLinkValueLong(pCool, &pCool->opw0, PWR_ON);
			putLinkValueLong(pCool, &pCool->opw1, PWR_OFF);
			getLinkValueLong(pCool, &pCool->ips0, &pos);
			putLinkValueLong(pCool, &pCool->ovl0, pCool->hisp);
			msPause(pCool, 200);

			pos += pCool->inc * pCool->step;
			pCool->inc = 0; /* No need to process again */
			pCool->mon |= COOL_MONITOR_INC;

			/*
			 * Advance the motor
			 */

			putLinkValueLong(pCool, &pCool->ops0, pos);

			/*
			 * Give the motion time to complete.
			 */

			do {
				msPause(pCool, 100);
				getLinkValueLong(pCool, &pCool->idm0, &dmov0);
			} while (!dmov0);

			/*
			 * Turn off the power
			 */

			putLinkValueLong(pCool, &pCool->opw0, PWR_OFF);

			/*
			 * We need to turn the motor back on.
			 */

			didOffset = 1;
		}

		/* 
		 * Start the motion
		 */

		if (pCool->rate != pCool->lrte || didOffset) {
			if (pCool->rate != pCool->lrte) {
				pCool->lrte = pCool->rate;
				pCool->mon |= COOL_MONITOR_RATE;
			}

			switch (pCool->rate) {
			case COOL_RATE_OFF:
				putLinkValueLong(pCool, &pCool->ojg0, 0);
				putLinkValueLong(pCool, &pCool->ojg1, 0);

				/*
				 * Cancel the current motion, if any.
				 *
				 * Wait for the motion to complete.  If this record
				 * is called too quickly, a request to stop motion
				 * might get lost, so resend it, regularly.
				 */

				do {
					putLinkValueLong(pCool, &pCool->ojg0, 0);
					putLinkValueLong(pCool, &pCool->ojg1, 0);
					msPause(pCool, 200);
					getLinkValueLong(pCool, &pCool->idm0, &dmov0);
					getLinkValueLong(pCool, &pCool->idm1, &dmov1);
				} while (!dmov0 || !dmov1);

				putLinkValueLong(pCool, &pCool->opw0, PWR_OFF);
				putLinkValueLong(pCool, &pCool->opw1, PWR_OFF);

				/*
				 * Set state.
				 */

				strncpy(pCool->busy, "Off", sizeof(pCool->busy));
				pCool->busy[sizeof(pCool->busy) - 1] = '\0';
				pCool->mon |= COOL_MONITOR_BUSY;

				break;

			case COOL_RATE_HIGH:
				/*
				 * Cancel the previous motion, if any.
				 *
				 * Wait for the motion to complete.  If this record
				 * is called too quickly, a request to stop motion
				 * might get lost, so resend it periodically.
				 */
				
				do {
					putLinkValueLong(pCool, &pCool->ojg0, 0);
					putLinkValueLong(pCool, &pCool->ojg1, 0);
					msPause(pCool, 200);
					getLinkValueLong(pCool, &pCool->idm0, &dmov0);
					getLinkValueLong(pCool, &pCool->idm1, &dmov1);
				} while (!dmov0 && !dmov1);

				/*
				 * Turn on both motors and set the velocity.
				 */

				putLinkValueLong(pCool, &pCool->ovl0, pCool->hisp);
				putLinkValueLong(pCool, &pCool->ovl1, pCool->hisp);
				putLinkValueLong(pCool, &pCool->opw0, PWR_ON);
				putLinkValueLong(pCool, &pCool->opw1, PWR_ON);
				msPause(pCool, 200);

				/*
				 * Start moving
				 */

				putLinkValueLong(pCool, &pCool->ojg0, 1);
				putLinkValueLong(pCool, &pCool->ojg1, 1);

				/*
				 * Set state.
				 */

				strncpy(pCool->busy, "High", sizeof(pCool->busy));
				pCool->busy[sizeof(pCool->busy) - 1] = '\0';
				pCool->mon |= COOL_MONITOR_BUSY;

				break;

			case COOL_RATE_LOW:
				/*
				 * Cancel the previous motion, if any.
				 *
				 * Wait for the motion to complete.  If this record
				 * is called too quickly, a request to stop motion
				 * might get lost, so resend it, regularly.
				 */

				do {
					putLinkValueLong(pCool, &pCool->ojg0, 0);
					putLinkValueLong(pCool, &pCool->ojg1, 0);
					msPause(pCool, 200);
					getLinkValueLong(pCool, &pCool->idm0, &dmov0);
					getLinkValueLong(pCool, &pCool->idm1, &dmov1);
				} while (!dmov0 && !dmov1);

				/*
				 * Turn on both motors and set the velocity.
				 */

				putLinkValueLong(pCool, &pCool->ovl0, pCool->losp);
				putLinkValueLong(pCool, &pCool->ovl1, pCool->losp);
				putLinkValueLong(pCool, &pCool->opw0, PWR_ON);
				putLinkValueLong(pCool, &pCool->opw1, PWR_ON);
				msPause(pCool, 200);

				/*
				 * Start moving
				 */

				putLinkValueLong(pCool, &pCool->ojg0, 1);
				putLinkValueLong(pCool, &pCool->ojg1, 1);

				/*
				 * Set state.
				 */

				strncpy(pCool->busy, "Low", sizeof(pCool->busy));
				pCool->busy[sizeof(pCool->busy) - 1] = '\0';
				pCool->mon |= COOL_MONITOR_BUSY;

				break;

			default:
				recGblRecordError(S_dev_missingSup, (void *)pCool,
					"Illegal rate value");
			}
		}

		/*
		 * check for alarms
		 */

		alarm(pCool);

		/*
		 * check event list
		 */

		monitor(pCool);

		/*
		 * We're done
		 */

		pCool->pact = FALSE;

		dbScanUnlock((struct dbCommon *)pCool);
	}
}

/*
 *+
 * FUNCTION NAME: msPause
 *
 * INVOCATION: msPause(pCool, msec)
 *
 * PARAMETERS:
 *
 *     (!) pCool (struct coolRecord *) Cool record
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
 *    to fields of the Cool record while this record is processing.
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

static void
msPause(struct coolRecord *pCool, int msec)
{
	struct timespec ts;

	dbScanUnlock((struct dbCommon *)pCool);
	ts.tv_sec = msec / 1000;
	ts.tv_nsec = (msec % 1000) * 1000000;
	nanosleep(&ts, NULL);
	dbScanLock((struct dbCommon *)pCool);
}
