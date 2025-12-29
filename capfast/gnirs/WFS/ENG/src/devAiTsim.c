static char rcsid[] = "$Id: devAiTsim.c,v 1.2 2009/05/27 19:34:41 fkraemer Exp $";

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
 * wheel-like component.
 *
 * FILENAME
 *     devAiTsim.c
 *
 * FUNCTION NAME(S)
 */

#include <sys/types.h>
#include <dbCommon.h>
#include <dbEvent.h>
#include <recSup.h>
#include <devSup.h>
#include <time.h>
#include <string.h>
#include <limits.h>
#include <stdlib.h>

#include <aiRecord.h>
#include "drvTsim.h"

#if !defined(DEBUG)
#	define DEBUG (0)
#endif

typedef struct {
	long number;
	DEVSUPFUN report;
	DEVSUPFUN init;
	DEVSUPFUN ai_init_record;
	DEVSUPFUN get_ioint_info;
	DEVSUPFUN read_ai;
	DEVSUPFUN special_linconv;
} DevAiTsim;

static long read_ai(struct aiRecord *);
static long ai_init_record(struct aiRecord *);
#define init ((DEVSUPFUN)0)
#define report ((DEVSUPFUN)0)
#define get_ioint_info ((DEVSUPFUN)0)
#define special_linconv ((DEVSUPFUN)0)

DevAiTsim devAiTsim = {
	7,
	report,
	init,
	ai_init_record,
	get_ioint_info,
	read_ai,
	special_linconv,
};

typedef struct {
	long id;
	long field;
	void *drv;
} Driver;

#define READ_SEN_TEMPERATURE (0x0001)
#define READ_CON_TEMPERATURE (0x0002)
#define READ_CON_HEATING     (0x0004)



/*
 *+
 * FUNCTION NAME: read_ai
 *
 * INVOCATION: read_ai(pai)
 *
 * PARAMETERS:
 *
 *     (!) pai (struct aiRecord *)
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
read_ai(struct aiRecord *pai)
{
	if (pai && pai->dpvt) {
		const Driver *const pDriver = (Driver *)pai->dpvt;
		const TsimValues *const pValues = (TsimValues *)pDriver->drv;

		switch (pDriver->field) {
		case READ_SEN_TEMPERATURE:
			pai->udf = FALSE;
			pai->val = pValues->ts.temperature;
			break;

		case READ_CON_TEMPERATURE:
			pai->udf = FALSE;
			pai->val = pValues->tc[pDriver->id].temperature;
			break;

		case READ_CON_HEATING:
			pai->udf = FALSE;
			pai->val = pValues->tc[pDriver->id].heating;
			break;
		}
	}

	return 2; /* Don't convert */
}

static long
ai_init_record(struct aiRecord *pai)
{
	Driver *pDriver = NULL;
	long status = 0;
	char temp[MAX_STRING_SIZE + 1];
	char *tty;
	char *field;
	char *channel;

#if DEBUG > 0
	printf("This is " __FILE__ "::ai_init_record(%p)\n", pai);
#endif

	pai->udf = TRUE;

	if (!status) {
		if ((pDriver = malloc(sizeof(Driver))) == NULL) {
			status = S_rec_outMem;
		} else {
			pai->dpvt = pDriver;
			pDriver->drv = NULL;
			pDriver->field = 0;
		}
	}

	/**************************************************************
	 * Find the data field
	 **************************************************************/

	/*
	 * ai.inp must be an INST_IO
	 */

	if (!status && pai->inp.type != INST_IO) {
		status = S_dev_badInpType;
		recGblRecordError(S_dev_badInpType, (void *)pai,
			__FILE__ "::ai_init_record Illegal INP field type.");
	}

	if (!status) {
		strncpy(temp, pai->inp.value.instio.string, MAX_STRING_SIZE);
		temp[MAX_STRING_SIZE] = '\0';

		tty = strtok(temp, " \t");
		field = strtok(NULL, " \t");
		channel = strtok(NULL, " \t");

		if (!field) {
			status = S_dev_badSignal;
			recGblRecordError(S_dev_badSignal, (void *)pai,
				"ai_init_record Missing field specification.");
		}
	}	

	if (!status) {
		if (!tty) {
			status = S_dev_badSignal;
			recGblRecordError(S_dev_badSignal, (void *)pai,
				"ai_init_record Missing field specification.");
		} else if (*tty == 'C') { /* Temperature controller */
			pDriver->id = atoi(tty + 1);

			if (pDriver->id < 0 || pDriver->id > TSIM_NTC) {
				status = S_dev_badSignal;
				recGblRecordError(S_dev_badSignal, (void *)pai,
					"ai_init_record Controller out of range.");
			} else if (field && strcmp(field, "tmp") == 0) {
				pDriver->field = READ_CON_TEMPERATURE;
			} else if (field && strcmp(field, "heat") == 0) {
				pDriver->field = READ_CON_HEATING;
			} else {
				status = S_dev_badSignal;
				recGblRecordError(S_dev_badSignal, (void *)pai,
					"ai_init_record Bad channel specification.");
			}
		} else if (*tty == 'M') { /* Cooling motor */
			pDriver->id = atoi(tty + 1);

			if (pDriver->id < 0 || pDriver->id > TSIM_NTC) {
				status = S_dev_badSignal;
				recGblRecordError(S_dev_badSignal, (void *)pai,
					"ai_init_record Controller out of range.");
			} else if (field && strcmp(field, "tmp") == 0) {
				pDriver->field = READ_CON_TEMPERATURE;
			} else if (field && strcmp(field, "heat") == 0) {
				pai->udf = FALSE; /* Field is not used */
			} else {
				status = S_dev_badSignal;
				recGblRecordError(S_dev_badSignal, (void *)pai,
					"ai_init_record Bad channel specification.");
			}
		} else if (*tty == 'S') { /* Temperature sensor */
			if (strcmp(field, "tmp") == 0) {
				pDriver->field = READ_SEN_TEMPERATURE;
			} else {
				status = S_dev_badSignal;
				recGblRecordError(S_dev_badSignal, (void *)pai,
					"ai_init_record Bad channel specification.");
			}
		} else {
			status = S_dev_badSignal;
			recGblRecordError(S_dev_badSignal, (void *)pai,
				"ai_init_record Bad field specification.");
		}
	}

	if (!status)
		status = tsimRecInit(pai, &pDriver->drv);

	if (!status)
		pai->dpvt = pDriver;
	else if (pDriver)
		free(pDriver);

	return status;
}
