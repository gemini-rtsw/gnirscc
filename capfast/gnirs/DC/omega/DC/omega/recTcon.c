static char rcsid[] = "$Id: recTcon.c,v 1.2 2009/05/27 19:32:35 fkraemer Exp $";

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
 *     recTcon.c
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

#include <tconRecord.h>
#include <choiceTcon.h>
#include "recTcon.h"

/*
 * Create RSET - Record Support Entry Table
 */

#define report NULL
#define initialize NULL
static long init_record(struct tconRecord *, int);
static long process(struct tconRecord *);

#define special NULL
static long get_value(tconRecord *, struct valueDes *);

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

struct rset tconRSET = {
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

struct tcondset {			/* tcon input dset */
	long number;
	DEVSUPFUN dev_report;
	DEVSUPFUN init;
	DEVSUPFUN init_record;		/* returns: (-1,0)=>(failure,success) */
	DEVSUPFUN get_ioint_info;
	DEVSUPFUN read_tcon;		/* returns: (-1,0)=>(failure,success) */
	DEVSUPFUN set_units;
};

static void alarm(struct tconRecord *);
static void monitor(struct tconRecord *);
static long readValue(struct tconRecord *);

/*
 *+
 * FUNCTION NAME: init_record
 *
 * INVOCATION: init_record(pTcon, pass)
 *
 * PARAMETERS:
 *
 *     (!) pTcon  (struct tconRecord *)  EPICS record
 *     (>) pass   (int)                  Initialization pass
 *
 * FUNCTION VALUE: (long) Error status (nonzero indicates an error)
 *
 * SCOPE: static (recTcon.c)
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
 *     Standard EPICS pre-initialization of pTcon.
 *
 * DEFICIENCIES:
 *-
 */

static long
init_record(struct tconRecord *pTcon, int pass)
{
	struct tcondset *pdset;
	long status;

	if (pass == 0)
		return 0;

	/*
	 * tcon.siml must be a CONSTANT or a PV_LINK or a DB_LINK
	 */

	if (pTcon->siml.type == CONSTANT) {
		pTcon->simm = pTcon->siml.value.value;
	} else {
		status = recGblInitFastInLink(&(pTcon->siml), (void *)pTcon,
			DBR_ENUM, "SIMM");
		if (status)
			return status;
	}

	/*
	 * tcon.siol must be a CONSTANT or a PV_LINK or a DB_LINK
	 */

	if (pTcon->siol.type == CONSTANT) {
		pTcon->sval = pTcon->siol.value.value;
	} else {
		status = recGblInitFastInLink(&(pTcon->siol), (void *)pTcon,
			DBR_LONG, "SVAL");
		if (status)
			return status;
	}

	if (!(pdset = (struct tcondset *)(pTcon->dset))) {
		recGblRecordError(S_dev_noDSET, (void *)pTcon,
			__FILE__ "::init_record");
		return S_dev_noDSET;
	}

	/*
	 * must have read_tcon function defined
	 */

	if ((pdset->number < 5) || (pdset->read_tcon == NULL)) {
		recGblRecordError(S_dev_missingSup, (void *)pTcon,
			__FILE__ "::init_record");
		return S_dev_missingSup;
	}
	if (pdset->init_record) {
		if ((status = (*pdset->init_record)(pTcon)))
			return status;
	}

	return 0;
}



/*
 *+
 * FUNCTION NAME: process
 *
 * INVOCATION: process(pTcon)
 *
 * PARAMETERS:
 *     (!) pTcon  (struct tconRecord *)  EPICS record
 *
 * FUNCTION VALUE: (long) Error status (nonzero indicates an error)
 *
 * SCOPE: static (recTcon.c)
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
 *     Standard EPICS initialization of pTcon.
 *
 * DEFICIENCIES:
 *
 *     This is intended to be called by the EPICS processing 
 *     procedure.  This should not be called directly.
 *-
 */

static long
process(struct tconRecord *pTcon)
{
	struct tcondset *pdset = (struct tcondset *)(pTcon->dset);
	long status;
	unsigned char pact = pTcon->pact;

	if (pdset && pdset->set_units)
		(*pdset->set_units)(pTcon);

	if ((pdset == NULL) || (pdset->read_tcon == NULL)) {
		pTcon->pact = TRUE;
		recGblRecordError(S_dev_missingSup, (void *)pTcon, 
			__FILE__ "::read_tcon");
		return S_dev_missingSup;
	}
	if (!status)
		status = readValue(pTcon);	/* read the new value */

	/*
	 * check if device support set pact
	 */

	if (!pact && pTcon->pact)
		return 0;
	pTcon->pact = TRUE;

	recGblGetTimeStamp(pTcon);

	/*
	 * check for alarms
	 */

	alarm(pTcon);

	/*
	 * check event list
	 */

	monitor(pTcon);

	/*
	 * process the forward scan link record
	 */
	 
	recGblFwdLink(pTcon);

	pTcon->pact = FALSE;
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
 * SCOPE: static (recTcon.c)
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
	struct tconRecord *pTcon = (struct tconRecord *)pAddr->precord;

	*precision = pTcon->prec;
	recGblGetPrec(pAddr, precision);
	return 0;
}



/*
 *+
 * FUNCTION NAME: get_value
 *
 * INVOCATION: get_value(phs, pvdes)
 *
 * PARAMETERS:
 *     (!) phs   (HallStepRecord *)    EPICS Record
 *     (!) pvdes (struct valueDes *)
 *
 * FUNCTION VALUE: (long) Error status (nonzero indicates an error)
 *
 * SCOPE: static (recTcon.c)
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
get_value(tconRecord *pTcon, struct valueDes *pvdes)
{
	pvdes->field_type = DBF_LONG;
	pvdes->no_elements = 1;
	pvdes->pvalue = (void *)&pTcon->val;

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
 * SCOPE: static (recTcon.c)
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
get_units(struct dbAddr *pAddr, char *units)
{
	struct tconRecord *pTcon = (struct tconRecord *)pAddr->precord;

	switch (pTcon->egu) {
	case TCON_UNITS_F:
		strncpy(units, "F", DB_UNITS_SIZE);
		break;

	case TCON_UNITS_K:
		strncpy(units, "K", DB_UNITS_SIZE);
		break;

	default: /* TCON_UNITS_K */
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
 * SCOPE: static (recTcon.c)
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
	struct tconRecord *pTcon = (struct tconRecord *)pAddr->precord;

	if (pAddr->pfield == (void *)&pTcon->val
			|| pAddr->pfield == (void *)&pTcon->hihi
			|| pAddr->pfield == (void *)&pTcon->high
			|| pAddr->pfield == (void *)&pTcon->low
			|| pAddr->pfield == (void *)&pTcon->lolo) {
		pgd->upper_disp_limit = pTcon->hopr;
		pgd->lower_disp_limit = pTcon->lopr;
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
 * SCOPE: static (recTcon.c)
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
	struct tconRecord *pTcon = (struct tconRecord *)pAddr->precord;

	if (pAddr->pfield == (void *)&pTcon->val
			|| pAddr->pfield == (void *)&pTcon->hihi
			|| pAddr->pfield == (void *)&pTcon->high
			|| pAddr->pfield == (void *)&pTcon->low
			|| pAddr->pfield == (void *)&pTcon->lolo) {
		pcd->upper_ctrl_limit = pTcon->hopr;
		pcd->lower_ctrl_limit = pTcon->lopr;
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
 * SCOPE: static (recTcon.c)
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
	struct tconRecord *pTcon = (struct tconRecord *)pAddr->precord;

	if (pAddr->pfield == (void *)&pTcon->val) {
		pAd->upper_alarm_limit = pTcon->hihi;
		pAd->upper_warning_limit = pTcon->high;
		pAd->lower_warning_limit = pTcon->low;
		pAd->lower_alarm_limit = pTcon->lolo;
	} else
		recGblGetAlarmDouble(pAddr, pAd);
	return 0;
}



/*
 *+
 * FUNCTION NAME: alarm
 *
 * INVOCATION: alarm(pTcon)
 *
 * PARAMETERS:
 *
 *     (!) pTcon (struct tconRecord *) EPICS Record
 *     (>) pad   (struct dbr_alDouble *)
 *
 * FUNCTION VALUE: (long) Error status (nonzero indicates an error)
 *
 * SCOPE: static (recTcon.c)
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
alarm(struct tconRecord *pTcon)
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

	if (pTcon->udf == TRUE) {
		recGblSetSevr(pTcon, UDF_ALARM, INVALID_ALARM);
		return;
	}
	hihi = pTcon->hihi;
	lolo = pTcon->lolo;
	high = pTcon->high;
	low = pTcon->low;
	hhsv = pTcon->hhsv;
	llsv = pTcon->llsv;
	hsv = pTcon->hsv;
	lsv = pTcon->lsv;
	val = pTcon->val;
	hyst = pTcon->hyst;
	lalm = pTcon->lalm;

	/*
	 * alarm condition hihi
	 */

	if (hhsv && (val >= hihi || ((lalm == hihi) && (val >= hihi - hyst)))) {
		if (recGblSetSevr(pTcon, HIHI_ALARM, pTcon->hhsv))
			pTcon->lalm = hihi;
		return;
	}

	/*
	 * alarm condition lolo
	 */

	if (llsv && (val <= lolo || ((lalm == lolo) && (val <= lolo + hyst)))) {
		if (recGblSetSevr(pTcon, LOLO_ALARM, pTcon->llsv))
			pTcon->lalm = lolo;
		return;
	}

	/*
	 * alarm condition high
	 */

	if (hsv && (val >= high || ((lalm == high) && (val >= high - hyst)))) {
		if (recGblSetSevr(pTcon, HIGH_ALARM, pTcon->hsv))
			pTcon->lalm = high;
		return;
	}

	/*
	 * alarm condition low
	 */

	if (lsv && (val <= low || ((lalm == low) && (val <= low + hyst)))) {
		if (recGblSetSevr(pTcon, LOW_ALARM, pTcon->lsv))
			pTcon->lalm = low;
		return;
	}

	/*
	 * we get here only if val is out of alarm by at least hyst
	 */

	pTcon->lalm = val;
	return;
}



/*
 *+
 * FUNCTION NAME: monitor
 *
 * INVOCATION: monitor(pTcon)
 *
 * PARAMETERS:
 *
 *     (!) pTcon   (struct tconRecord) EPICS Record
 *
 * FUNCTION VALUE: none
 *
 * SCOPE: static (recTcon.c)
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
monitor(struct tconRecord *pTcon)
{
	unsigned short monitor_mask;
	long delta;

	/*
	 * get previous stat and sevr  and new stat and sevr
	 */

	monitor_mask = recGblResetAlarms(pTcon);

	/*
	 * Check for changes in field values
	 */

	if (pTcon->mon & TCON_MONITOR_GAIN) {
		monitor_mask |= DBE_VALUE;
		db_post_events(pTcon, &pTcon->gain, monitor_mask);
	}

	if (pTcon->mon & TCON_MONITOR_SETP) {
		monitor_mask |= DBE_VALUE;
		db_post_events(pTcon, &pTcon->setp, monitor_mask);
	}

	if (pTcon->mon & TCON_MONITOR_RATE) {
		monitor_mask |= DBE_VALUE;
		db_post_events(pTcon, &pTcon->rate, monitor_mask);
	}

	if (pTcon->mon & TCON_MONITOR_RST) {
		monitor_mask |= DBE_VALUE;
		db_post_events(pTcon, &pTcon->rst, monitor_mask);
	}

	if (pTcon->mon & TCON_MONITOR_RANG) {
		monitor_mask |= DBE_VALUE;
		db_post_events(pTcon, &pTcon->rang, monitor_mask);
	}

	if (pTcon->mon & TCON_MONITOR_TUNE) {
		monitor_mask |= DBE_VALUE;
		db_post_events(pTcon, &pTcon->tune, monitor_mask);
	}

	pTcon->mon = 0;

	/*
	 * check for value change
	 */

	delta = pTcon->mlst - pTcon->val;
	if (delta < 0)
		delta = -delta;
	if (delta > pTcon->mdel) {
		/*
		 * post events for value change
		 */

		monitor_mask |= DBE_VALUE;

		/*
		 * update last value monitored
		 */

		pTcon->mlst = pTcon->val;
	}

	/*
	 * check for archive change
	 */

	delta = pTcon->alst - pTcon->val;
	if (delta < 0)
		delta = -delta;
	if (delta > pTcon->adel) {
		/*
		 * post events on value field for archive change
		 */

		monitor_mask |= DBE_LOG;

		/*
		 * update last archive value monitored
		 */

		pTcon->alst = pTcon->val;
	}

	/*
	 * send out monitors connected to the value field
	 */

	if (monitor_mask) {
		db_post_events(pTcon, &pTcon->val, monitor_mask);
	}
	return;
}



/*
 *+
 * FUNCTION NAME: readValue
 *
 * INVOCATION: readValue(struct tconRecord *)
 *
 * PARAMETERS:
 *
 *     (!) pTcon   (struct tconRecord *) EPICS Record
 *
 * FUNCTION VALUE: none
 *
 * SCOPE: static (recTcon.c)
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
readValue(struct tconRecord *pTcon)
{
	long status;
	struct tcondset *pdset = (struct tcondset *)(pTcon->dset);

	if (pTcon->pact == TRUE) {
		status = (*pdset->read_tcon)(pTcon);
		return status;
	}
	status = recGblGetFastLink(&(pTcon->siml), (void *)pTcon,
		&(pTcon->simm));
	if (status)
		return status;

	if (pTcon->simm == NO) {
		status = (*pdset->read_tcon)(pTcon);
		return status;
	}
	if (pTcon->simm == YES) {
		status = recGblGetFastLink(&(pTcon->siol), (void *) pTcon,
			&(pTcon->sval));

		if (status == 0) {
			pTcon->val = pTcon->sval;
			pTcon->udf = FALSE;
		}
	} else {
		status = -1;
		recGblSetSevr(pTcon, SOFT_ALARM, INVALID_ALARM);
		return status;
	}
	recGblSetSevr(pTcon, SIMM_ALARM, pTcon->sims);

	return status;
}
