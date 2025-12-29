static char rcsid[] = "$Id: recTsen.c,v 1.2 2009/05/27 19:34:46 fkraemer Exp $";

/*
 * Copyright 1998 University of Hawaii, Institute for Astronomy.  All
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
 *     recTsen.c
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

#include <choiceTsen.h>
#include <tsenRecord.h>

/*
 * Create RSET - Record Support Entry Table
 */

#define report NULL
#define initialize NULL
static long init_record(struct tsenRecord *, int);
static long process(struct tsenRecord *);

#define special NULL
static long get_value(tsenRecord *, struct valueDes *);

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

struct rset tsenRSET = {
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

struct tsendset {			/* tsen input dset */
	long number;
	DEVSUPFUN dev_report;
	DEVSUPFUN init;
	DEVSUPFUN init_record;		/* returns: (-1,0)=>(failure,success) */
	DEVSUPFUN get_ioint_info;
	DEVSUPFUN read_tsen;		/* returns: (-1,0)=>(failure,success) */
	DEVSUPFUN set_units;
};

static void alarm(struct tsenRecord *);
static void monitor(struct tsenRecord *);
static long readValue(struct tsenRecord *);

/*
 *+
 * FUNCTION NAME: init_record
 *
 * INVOCATION: init_record(pTsen, pass)
 *
 * PARAMETERS:
 *
 *     (!) pTsen  (struct tsenRecord *)  EPICS record
 *     (>) pass   (int)                  Initialization pass
 *
 * FUNCTION VALUE: (long) Error status (nonzero indicates an error)
 *
 * SCOPE: static (recTsen.c)
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
 * EXTERNAL VARIABLES: 
 *
 * PRIOR REQUIREMENTS:
 *
 *     Standard EPICS pre-initialization of pTsen.
 *
 * DEFICIENCIES:
 *-
 */

static long
init_record(struct tsenRecord *pTsen, int pass)
{
	struct tsendset *pdset;
	long status;

	if (pass == 0)
		return 0;

	/*
	 * tsen.siml must be a CONSTANT or a PV_LINK or a DB_LINK
	 */

	if (pTsen->siml.type == CONSTANT) {
		pTsen->simm = pTsen->siml.value.value;
	} else {
		status = recGblInitFastInLink(&(pTsen->siml), (void *)pTsen,
			DBR_ENUM, "SIMM");
		if (status)
			return status;
	}

	/*
	 * tsen.siol must be a CONSTANT or a PV_LINK or a DB_LINK
	 */

	if (pTsen->siol.type == CONSTANT) {
		pTsen->sval = pTsen->siol.value.value;
	} else {
		status = recGblInitFastInLink(&(pTsen->siol), (void *)pTsen,
			DBR_LONG, "SVAL");
		if (status)
			return status;
	}

	if (!(pdset = (struct tsendset *)(pTsen->dset))) {
		recGblRecordError(S_dev_noDSET, (void *)pTsen,
			__FILE__ "::init_record");
		return S_dev_noDSET;
	}

	/*
	 * must have read_tsen function defined
	 */

	if ((pdset->number < 5) || (pdset->read_tsen == NULL)) {
		recGblRecordError(S_dev_missingSup, (void *)pTsen,
			__FILE__ "::init_record");
		return S_dev_missingSup;
	}
	if (pdset->init_record) {
		if ((status = (*pdset->init_record)(pTsen)))
			return status;
	}

	return 0;
}



/*
 *+
 * FUNCTION NAME: process
 *
 * INVOCATION: process(pTsen)
 *
 * PARAMETERS:
 *     (!) pTsen  (struct tsenRecord *)  EPICS record
 *
 * FUNCTION VALUE: (long) Error status (nonzero indicates an error)
 *
 * SCOPE: static (recTsen.c)
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
 * EXTERNAL VARIABLES:
 *
 * PRIOR REQUIREMENTS:
 *
 *     Standard EPICS initialization of pTsen.
 *
 * DEFICIENCIES:
 *
 *     This is intended to be called by the EPICS processing 
 *     procedure.  This should not be called directly.
 *-
 */

static long
process(struct tsenRecord *pTsen)
{
	struct tsendset *pdset = (struct tsendset *)(pTsen->dset);
	long status;
	unsigned char pact = pTsen->pact;

	if (pdset && pdset->set_units)
		(*pdset->set_units)(pTsen);

	if ((pdset == NULL) || (pdset->read_tsen == NULL)) {
		pTsen->pact = TRUE;
		recGblRecordError(S_dev_missingSup, (void *)pTsen, 
			__FILE__ "::read_tsen");
		return S_dev_missingSup;
	}
	status = readValue(pTsen);	/* read the new value */

	/*
	 * check if device support set pact
	 */

	if (!pact && pTsen->pact)
		return 0;
	pTsen->pact = TRUE;

	recGblGetTimeStamp(pTsen);

	/*
	 * check for alarms
	 */

	alarm(pTsen);

	/*
	 * check event list
	 */

	monitor(pTsen);

	/*
	 * process the forward scan link record
	 */
	 
	recGblFwdLink(pTsen);

	pTsen->pact = FALSE;
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
 * SCOPE: static (recTsen.c)
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
get_precision(struct dbAddr *pAddr, long *precision)
{
	struct tsenRecord *pTsen = (struct tsenRecord *)pAddr->precord;

	*precision = pTsen->prec;
	recGblGetPrec(pAddr, precision);
	return 0;
}



/*
 *+
 * FUNCTION NAME: get_value
 *
 * INVOCATION: get_value(pTsen, pvdes)
 *
 * PARAMETERS:
 *     (!) pTsen (HallStepRecord *)    EPICS Record
 *     (!) pvdes (struct valueDes *)
 *
 * FUNCTION VALUE: (long) Error status (nonzero indicates an error)
 *
 * SCOPE: static (recTsen.c)
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
get_value(tsenRecord *pTsen, struct valueDes *pvdes)
{
	pvdes->field_type = DBF_LONG;
	pvdes->no_elements = 1;
	pvdes->pvalue = (void *)&pTsen->val;

	return 0;
}



/*
 *+
 * FUNCTION NAME: get_units
 *
 * INVOCATION: get_units(pAddr, units)
 *
 * PARAMETERS:
 *     (!) pAddr (struct dbAddr *) EPICS Record
 *     (>) units (char [])
 *
 * FUNCTION VALUE: (long) Error status (nonzero indicates an error)
 *
 * SCOPE: static (recTsen.c)
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
get_units(struct dbAddr *pAddr, char units[])
{
	struct tsenRecord *pTsen = (struct tsenRecord *)pAddr->precord;

	switch (pTsen->egu) {
	case TSEN_UNITS_K:
		strncpy(units, "K", DB_UNITS_SIZE);
		break;

	case TSEN_UNITS_F:
		strncpy(units, "F", DB_UNITS_SIZE);
		break;

	default: /* TSEN_UNITS_C */
		strncpy(units, "C", DB_UNITS_SIZE);
		break;
	}

	return 0;
}



/*
 *+
 * FUNCTION NAME: get_graphic_double
 *
 * INVOCATION: get_graphic_double(pAddr, struct dbr_grDouble *)
 *
 * PARAMETERS:
 *
 *     (!) pAddr (struct dbAddr *) EPICS Record
 *     (>) pgd   (struct dbr_grDouble *)
 *
 * FUNCTION VALUE: (long) Error status (nonzero indicates an error)
 *
 * SCOPE: static (recTsen.c)
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
get_graphic_double(struct dbAddr *pAddr, struct dbr_grDouble *pgd)
{
	struct tsenRecord *pTsen = (struct tsenRecord *)pAddr->precord;

	if (pAddr->pfield == (void *)&pTsen->val
			|| pAddr->pfield == (void *)&pTsen->hihi
			|| pAddr->pfield == (void *)&pTsen->high
			|| pAddr->pfield == (void *)&pTsen->low
			|| pAddr->pfield == (void *)&pTsen->lolo) {
		pgd->upper_disp_limit = pTsen->hopr;
		pgd->lower_disp_limit = pTsen->lopr;
	} else
		recGblGetGraphicDouble(pAddr, pgd);
	return 0;
}



/*
 *+
 * FUNCTION NAME: get_control_double
 *
 * INVOCATION: get_control_double(pAddr, struct dbr_ctrlDouble *)
 *
 * PARAMETERS:
 *
 *     (!) pAddr (struct dbAddr *) EPICS Record
 *     (>) pcd   (struct dbr_ctrlDouble *)
 *
 * FUNCTION VALUE: (long) Error status (nonzero indicates an error)
 *
 * SCOPE: static (recTsen.c)
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
get_control_double(struct dbAddr *pAddr, struct dbr_ctrlDouble *pcd)
{
	struct tsenRecord *pTsen = (struct tsenRecord *)pAddr->precord;

	if (pAddr->pfield == (void *)&pTsen->val
			|| pAddr->pfield == (void *)&pTsen->hihi
			|| pAddr->pfield == (void *)&pTsen->high
			|| pAddr->pfield == (void *)&pTsen->low
			|| pAddr->pfield == (void *)&pTsen->lolo) {
		pcd->upper_ctrl_limit = pTsen->hopr;
		pcd->lower_ctrl_limit = pTsen->lopr;
	} else
		recGblGetControlDouble(pAddr, pcd);
	return 0;
}



/*
 *+
 * FUNCTION NAME: get_alarm_double
 *
 * INVOCATION: get_alarm_double(pAddr, struct dbr_alDouble *)
 *
 * PARAMETERS:
 *
 *     (!) pAddr (struct dbAddr *) EPICS Record
 *     (>) pAd   (struct dbr_alDouble *)
 *
 * FUNCTION VALUE: (long) Error status (nonzero indicates an error)
 *
 * SCOPE: static (recTsen.c)
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
get_alarm_double(struct dbAddr *pAddr, struct dbr_alDouble *pAd)
{
	struct tsenRecord *pTsen = (struct tsenRecord *)pAddr->precord;

	if (pAddr->pfield == (void *)&pTsen->val) {
		pAd->upper_alarm_limit = pTsen->hihi;
		pAd->upper_warning_limit = pTsen->high;
		pAd->lower_warning_limit = pTsen->low;
		pAd->lower_alarm_limit = pTsen->lolo;
	} else
		recGblGetAlarmDouble(pAddr, pAd);
	return 0;
}



/*
 *+
 * FUNCTION NAME: alarm
 *
 * INVOCATION: alarm(pTsen)
 *
 * PARAMETERS:
 *
 *     (!) pTsen (struct tsenRecord *) EPICS Record
 *     (>) pad   (struct dbr_alDouble *)
 *
 * FUNCTION VALUE: (long) Error status (nonzero indicates an error)
 *
 * SCOPE: static (recTsen.c)
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
alarm(struct tsenRecord *pTsen)
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

	if (pTsen->udf == TRUE) {
		recGblSetSevr(pTsen, UDF_ALARM, INVALID_ALARM);
		return;
	}
	hihi = pTsen->hihi;
	lolo = pTsen->lolo;
	high = pTsen->high;
	low = pTsen->low;
	hhsv = pTsen->hhsv;
	llsv = pTsen->llsv;
	hsv = pTsen->hsv;
	lsv = pTsen->lsv;
	val = pTsen->val;
	hyst = pTsen->hyst;
	lalm = pTsen->lalm;

	/*
	 * alarm condition hihi
	 */

	if (hhsv && (val >= hihi || ((lalm == hihi) && (val >= hihi - hyst)))) {
		if (recGblSetSevr(pTsen, HIHI_ALARM, pTsen->hhsv))
			pTsen->lalm = hihi;
		return;
	}

	/*
	 * alarm condition lolo
	 */

	if (llsv && (val <= lolo || ((lalm == lolo) && (val <= lolo + hyst)))) {
		if (recGblSetSevr(pTsen, LOLO_ALARM, pTsen->llsv))
			pTsen->lalm = lolo;
		return;
	}

	/*
	 * alarm condition high
	 */

	if (hsv && (val >= high || ((lalm == high) && (val >= high - hyst)))) {
		if (recGblSetSevr(pTsen, HIGH_ALARM, pTsen->hsv))
			pTsen->lalm = high;
		return;
	}

	/*
	 * alarm condition low
	 */

	if (lsv && (val <= low || ((lalm == low) && (val <= low + hyst)))) {
		if (recGblSetSevr(pTsen, LOW_ALARM, pTsen->lsv))
			pTsen->lalm = low;
		return;
	}

	/*
	 * we get here only if val is out of alarm by at least hyst
	 */

	pTsen->lalm = val;
	return;
}



/*
 *+
 * FUNCTION NAME: monitor
 *
 * INVOCATION: monitor(pTsen)
 *
 * PARAMETERS:
 *
 *     (!) pTsen   (struct tsenRecord) EPICS Record
 *
 * FUNCTION VALUE: none
 *
 * SCOPE: static (recTsen.c)
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
monitor(struct tsenRecord *pTsen)
{
	unsigned short monitor_mask;
	long delta;

	/*
	 * get previous stat and sevr  and new stat and sevr
	 */

	monitor_mask = recGblResetAlarms(pTsen);

	/*
	 * Check for changes in field values
	 */

#if 0
	if (pTsen->rate != pTsen->orte) {
		monitor_mask |= DBE_VALUE;
		pTsen->orte = pTsen->rate;
		db_post_events(pTsen, &pTsen->rate, monitor_mask);
	}
#endif

	/*
	 * check for value change
	 */

	delta = pTsen->mlst - pTsen->val;
	if (delta < 0)
		delta = -delta;
	if (delta > pTsen->mdel) {
		/*
		 * post events for value change
		 */

		monitor_mask |= DBE_VALUE;

		/*
		 * update last value monitored
		 */

		pTsen->mlst = pTsen->val;
	}

	/*
	 * check for archive change
	 */

	delta = pTsen->alst - pTsen->val;
	if (delta < 0)
		delta = -delta;
	if (delta > pTsen->adel) {
		/*
		 * post events on value field for archive change
		 */

		monitor_mask |= DBE_LOG;

		/*
		 * update last archive value monitored
		 */

		pTsen->alst = pTsen->val;
	}

	/*
	 * send out monitors connected to the value field
	 */

	if (monitor_mask) {
		db_post_events(pTsen, &pTsen->val, monitor_mask);
	}
	return;
}



/*
 *+
 * FUNCTION NAME: readValue
 *
 * INVOCATION: readValue(struct tsenRecord *)
 *
 * PARAMETERS:
 *
 *     (!) pTsen   (struct tsenRecord *) EPICS Record
 *
 * FUNCTION VALUE: none
 *
 * SCOPE: static (recTsen.c)
 *
 * PURPOSE: Read temperature sensor value.
 *
 * DESCRIPTION:
 *
 *     Calls the driver function to read the temperature-sensor
 *     value, sets an alarm if there is an error.
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
readValue(struct tsenRecord *pTsen)
{
	long status;
	struct tsendset *pdset = (struct tsendset *)(pTsen->dset);

	if (pTsen->pact == TRUE) {
		status = (*pdset->read_tsen)(pTsen);
		return status;
	}
	status = recGblGetFastLink(&(pTsen->siml), (void *)pTsen,
		&(pTsen->simm));
	if (status)
		return status;

	if (pTsen->simm == NO) {
		status = (*pdset->read_tsen)(pTsen);
		return status;
	}
	if (pTsen->simm == YES) {
		status = recGblGetFastLink(&(pTsen->siol), (void *) pTsen,
			&(pTsen->sval));

		if (status == 0) {
			pTsen->val = pTsen->sval;
			pTsen->udf = FALSE;
		}
	} else {
		status = -1;
		recGblSetSevr(pTsen, SOFT_ALARM, INVALID_ALARM);
		return status;
	}
	recGblSetSevr(pTsen, SIMM_ALARM, pTsen->sims);

	return status;
}
