static char rcsid[] = "$Id: devHmotorTsim.c,v 1.2 2009/05/27 19:34:41 fkraemer Exp $";

/*
 * Copyright 1997 University of Hawaii, Institute for Astronomy.  All
 * rights reserved.
 *
 * The University of Hawaii grants AURA a non-exclusive license to
 * use this software, as stated in [contract].
 *
 * Author: Hubert Yamada
 *
 * Record Support Routines for Hall-Effect/Stepper-motor control record
 *
 * FILENAME
 *
 *     devHmotorTsim.c
 *
 * FUNCTION NAME(S)
 *
 *     tsim_init
 *     tsim_init_record
 *     tsim_update_values
 *     tsim_start_trans
 *     tsim_end_trans
 *     tsim_build_trans
 *     tsim_callback
 */

#include <vxWorks.h>
#include <types.h>
#include <string.h>
#include <taskLib.h>
#include <time.h>
#include <stdioLib.h>

#include <alarm.h>
#include <cvtTable.h>
#include <dbAccess.h>
#include <dbDefs.h>
#include <module_types.h>
#include <hmotorRecord.h>
#include <recSup.h>
#include <link.h>
#include <devSup.h>

#include "drvTsim.h"
#include "recHmotor.h"

typedef struct {
    long number;
    DEVSUPFUN report;
    DEVSUPFUN init;
    DEVSUPFUN init_record;
    DEVSUPFUN get_ioint_info;
    DEVSUPFUN update_values;
    DEVSUPFUN start_trans;
    DEVSUPFUN build_trans;
    DEVSUPFUN end_trans;
} DevHmotorTsim;

#define report ((DEVSUPFUN)0)
static long tsim_init(int after);
static long tsim_init_record();
#define get_ioint_info ((DEVSUPFUN)0)
static long tsim_start_trans(struct hmotorRecord *);
static long tsim_update_values(struct hmotorRecord *);
static long tsim_build_trans(int, double *, struct hmotorRecord *);
static long tsim_end_trans(struct hmotorRecord *, int);
static long tsim_callback(void *);

DevHmotorTsim devHmotorTsim = {
	9,
    report,
    tsim_init,
    tsim_init_record,
	get_ioint_info,
    tsim_update_values,
    tsim_start_trans,
    tsim_build_trans,
    tsim_end_trans
};

/*
 * Types of motion commands.  It is not at all clear to me why these
 * are defined in each header file for each device type, and not
 * defined in the motor.h file instead.
 */

#define UNDEFINED (unsigned char)0 /* garbage type */
#define IMMEDIATE (unsigned char)1 /* 'i' an execute immediate, no reply */
#define MOVE_TERM (unsigned char)2 /* 't' terminate a previous active motion */
#define MOTION    (unsigned char)3 /* 'm' will produce motion updates */
#define VELOCITY  (unsigned char)4 /* 'v' make motion updates till MOVE_TERM */
#define INFO      (unsigned char)5 /* 'f' get curr motor/encoder pos and stat */
#define QUERY     (unsigned char)6 /* 'q' specialty type, not needed */

/*
 * The order of these commands is defined in motor.h.  Don't rearrange
 * or extend this table, unless you want to change the device support
 * for all of the devices that are supported by the motor record.
 *
 * It's not clear to me why num_parms, which should be the same for
 * all device support for the motor record, is initialized separately
 * in the code for each device.
 */

static struct hmotor_table cmd_table[] = {
	/* type     cmd    num_parms */
	{MOTION,    " MA", 1}, /* MOVE_ABS */
	{MOTION,    " MR", 1}, /* MOVE_REL */
	{MOTION,    " HM", 1}, /* HOME_FOR */
	{MOTION,    " HR", 1}, /* HOME_REV */
	{MOTION,    " HE", 1}, /* HOME_ENC */
	{IMMEDIATE, " LP", 1}, /* LOAD_POS */
	{IMMEDIATE, " VB", 1}, /* SET_VELO_BASE */
	{IMMEDIATE, " VL", 1}, /* SET_VELO */
	{IMMEDIATE, " AC", 1}, /* SET_ACCEL */
	{IMMEDIATE, " GD", 0}, /* GO */
	{IMMEDIATE, " ER", 2}, /* SET_ENC_RATIO */
	{INFO,      " ",   0}, /* GET_MOTOR_POS */
	{INFO,      " ",   0}, /* GET_ENCODER_POS */
	{INFO,      " ",   0}, /* GET_INFO */
	{MOVE_TERM, " ST", 0}, /* STOP_AXIS */
	{VELOCITY,  " JG", 1}, /* JOG */
};

/*
 * The order of these commands is defined in motor.h.  Don't rearrange
 * or extend this table, unless you want to change the device support
 * for all of the devices that are supported by the motor record.
 */

static const char *cmd_name[] = {
	"MOVE_ABS",
	"MOVE_REL",
	"HOME_FOR",
	"HOME_REV",
	"HOME_ENC",
	"LOAD_POS",
	"SET_VEL_BASE",
	"SET_VELOCITY",
	"SET_ACCEL",
	"GO",
	"SET_ENC_RATIO",
	"GET_MOTOR_POS",
	"GET_ENCODER_POS",
	"GET_INFO",
	"STOP_AXIS",
	"JOG",
};



/*
 *+
 * FUNCTION NAME: tsim_init
 *
 * INVOCATION: 
 *
 * PARAMETERS:
 *
 * FUNCTION VALUE: 
 *
 * SCOPE:
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
tsim_init(int after)
{
	return 0;
}



/*
 *+
 * FUNCTION NAME: tsim_init_record
 *
 * INVOCATION: 
 *
 * PARAMETERS:
 *
 * FUNCTION VALUE: 
 *
 * SCOPE:
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
tsim_init_record(struct hmotorRecord *pmr)
{
	static long status = 0;
	int ctrl;

	if (!status) {
		if (*pmr->out.value.instio.string == 'M') {
			ctrl = atoi(pmr->out.value.instio.string + 1);

			if (ctrl < 0 || ctrl > TSIM_NMOTOR) {
				status = S_dev_badSignal;
				recGblRecordError(S_dev_badSignal, (void *)pmr,
					"ai_init_record Controller out of range.");
			}
		} else {
			status = S_dev_badSignal;
			recGblRecordError(S_dev_badSignal, (void *)pmr,
				"ai_init_record Bad channel specification.");
		}
	}

	if (!status)
		status = tsimMotorInit(pmr, ctrl, &pmr->dpvt);

	if (!status)
		tsimMotorSetCallback(pmr->dpvt, tsim_callback, pmr);

	(void)tsim_update_values(pmr);

	return status;
}



/*
 *+
 * FUNCTION NAME: tsim_update_values
 *
 * INVOCATION: 
 *
 * PARAMETERS:
 *
 * FUNCTION VALUE: 
 *
 * SCOPE:
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
tsim_update_values(struct hmotorRecord *pmr)
{
	int info = HMOTOR_NOTHING_DONE;
	TsimMotor *const pMotor = pmr->dpvt;

	if (pmr->rep != 0) {
		pmr->rep = 0;
		info = HMOTOR_CALLBACK_DATA;
	}

	if (pmr->mres != 1) {
		pmr->mres = 1;
		info = HMOTOR_CALLBACK_DATA;
	}

	if (pmr->res != 1) {
		pmr->res = 1;
		info = HMOTOR_CALLBACK_DATA;
	}

	if (pmr->rvel != 1000) {
		pmr->rvel = 1000;
		info = HMOTOR_CALLBACK_DATA;
	}

	if (pmr->rmp != pMotor->pos) {
		pmr->rmp = pMotor->pos;
		info = HMOTOR_CALLBACK_DATA;
	}

	if (pMotor->moving && pmr->msta & HMOTOR_RA_DONE) {
		pmr->msta &= ~HMOTOR_RA_DONE;
		info = HMOTOR_CALLBACK_DATA;
	} else if (!pMotor->moving && !(pmr->msta & HMOTOR_RA_DONE)) {
		pmr->msta |= HMOTOR_RA_DONE;
		info = HMOTOR_CALLBACK_DATA;
	}

	if (pMotor->dir && pmr->msta & HMOTOR_RA_DIRECTION) {
		pmr->msta &= ~HMOTOR_RA_DIRECTION;
		info = HMOTOR_CALLBACK_DATA;
	} else if (!pMotor->dir && !(pmr->msta & HMOTOR_RA_DIRECTION)) {
		pmr->msta |= HMOTOR_RA_DIRECTION;
		info = HMOTOR_CALLBACK_DATA;
	}

	return info;
}



/*
 *+
 * FUNCTION NAME: tsim_start_trans
 *
 * INVOCATION: 
 *
 * PARAMETERS:
 *
 * FUNCTION VALUE: 
 *
 * SCOPE:
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
tsim_start_trans(struct hmotorRecord *pmr)
{
    return 0;
}



/*
 *+
 * FUNCTION NAME: tsim_end_trans
 *
 * INVOCATION: 
 *
 * PARAMETERS:
 *
 * FUNCTION VALUE: 
 *
 * SCOPE:
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
tsim_end_trans(struct hmotorRecord *pmr, int term_method)
{
    return 0;
}



/*
 *+
 * FUNCTION NAME: tsim_build_trans
 *
 * INVOCATION: 
 *
 * PARAMETERS:
 *
 * FUNCTION VALUE: 
 *
 * SCOPE:
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
tsim_build_trans(int command, double *parms, struct hmotorRecord *pmr)
{
	long status;
	int i;
	TsimMotor *const pMotor = pmr->dpvt;

	switch (command) {
	case HMOTOR_GET_INFO:
		status = tsimMotorCallback(pMotor);
		break;

	case HMOTOR_SET_VEL_BASE:
	case HMOTOR_SET_ACCEL:
	case HMOTOR_POWER_ON:
	case HMOTOR_POWER_OFF:
		break;

	case HMOTOR_MOVE_ABS:
		status = tsimMotorSetDest(pMotor, (int)*parms, 0);
		break;

	case HMOTOR_MOVE_REL:
		status = tsimMotorSetDest(pMotor, (int)*parms, 1);
		break;

	case HMOTOR_SET_VELOCITY:
		status = tsimMotorSetSpeed(pMotor, (int)*parms);
		break;

	case HMOTOR_STOP_AXIS:
		status = tsimMotorStop(pMotor);
		break;

	case HMOTOR_GO:
		status = tsimMotorStart(pMotor);
		break;

	case HMOTOR_LOAD_POS:
		status = tsimMotorSetPos(pMotor, (int)*parms);
		break;

	case HMOTOR_HOME_FOR:
	case HMOTOR_HOME_REV:
		status = tsimMotorSetDest(pMotor, 0, 0);
		if (!status)
			status = tsimMotorStart(pMotor);
		break;

	case HMOTOR_JOG:
		if (*parms > 0) {
			status = tsimMotorSetSpeed(pMotor, (int)*parms);
			if (!status)
				status = tsimMotorSetDir(pMotor, 1);
		} else {
			status = tsimMotorSetSpeed(pMotor, (int)-*parms);
			if (!status)
				status = tsimMotorSetDir(pMotor, -1);
		}

		if (!status)
			status = tsimMotorJog(pMotor);

		if (!status)
			status = tsimMotorStart(pMotor);

		break;

	default:
		printf("Unknown command (%s", cmd_name[command]);
		for (i = 0; i < cmd_table[command].num_parms; i++)
			printf(" %f", parms[i]);
		puts(")");
	}

    return status;
}



/*
 *+
 * FUNCTION NAME: tsim_callback
 *
 * INVOCATION: 
 *
 * PARAMETERS:
 *
 * FUNCTION VALUE: 
 *
 * SCOPE:
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
tsim_callback(void *pArg)
{
	struct hmotorRecord *const pmr = (struct hmotorRecord *)pArg;

   /*
    * Unfortunately, record processing could request an update, 
	* which would cause this routine to be processed recursively.
	* We take the easy approach, and just ignore the callback
	* request if the record is already being processed.  Not
	* the best possible solution, but probably ok.
    */

	if (!pmr->pact) {
		dbScanLock((struct dbCommon *)pmr);
		dbProcess((struct dbCommon *)pmr);
		dbScanUnlock((struct dbCommon *)pmr);
	}

	return 0;
}
