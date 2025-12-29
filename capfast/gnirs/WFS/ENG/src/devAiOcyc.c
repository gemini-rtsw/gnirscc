static char rcsid[] = "$Id: devAiOcyc.c,v 1.2 2009/05/27 19:34:40 fkraemer Exp $";

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
 *     devAiOcyc.c
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
#include <logLib.h>

#include <aiRecord.h>
#include "drvOcyc.h"

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
} DevAiOcyc;

static long read_ai(struct aiRecord *);
static long ai_init_record(struct aiRecord *);
#define init ((DEVSUPFUN)0)
#define report ((DEVSUPFUN)0)
#define get_ioint_info ((DEVSUPFUN)0)
#define special_linconv ((DEVSUPFUN)0)

DevAiOcyc devAiOcyc = {
	7,
	report,
	init,
	ai_init_record,
	get_ioint_info,
	read_ai,
	special_linconv,
};

typedef struct {
	long field;
	void *drv;
} Driver;

#define READ_TEMPERATURE  (0x0001)
#define READ_HEATER_POWER (0x0002)



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
#if DEBUG > 0 
	fprintf(stderr, __FILE__ "(%d): read_ai(%p)\n", __LINE__, pai);
#endif

	if (pai && pai->dpvt) {
		const Driver *const pDriver = (Driver *)pai->dpvt;
		const OcycValues *pValues;

		pValues = ocycGet(pDriver->drv);

		switch (pDriver->field) {
		case READ_TEMPERATURE:
			pai->udf = FALSE;
			pai->val = pValues->temperature;
			break;
		
		case READ_HEATER_POWER:
			pai->udf = FALSE;
			pai->val = pValues->heat;
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

#if DEBUG > 0
	fprintf(stderr, __FILE__ "(%d): ai_init_record(%p)\n", __LINE__, pai);
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

		if (!tty) {
			status = S_dev_badSignal;
			recGblRecordError(S_dev_badSignal, (void *)pai,
				"ai_init_record Missing device specification.");
		} else if (!field) {
			status = S_dev_badSignal;
			recGblRecordError(S_dev_badSignal, (void *)pai,
				"ai_init_record Missing field specification.");
		}
	}	

	if (!status) {
		if (strcmp(field, "tmp") == 0) {
			pDriver->field = READ_TEMPERATURE;
		} else if (strcmp(field, "heat") == 0) {
			pDriver->field = READ_HEATER_POWER;
		} else {
			status = S_dev_badSignal;
			recGblRecordError(S_dev_badSignal, (void *)pai,
				"ai_init_record Bad field specification.");
		}
	}

	if (!status)
		status = ocycRecInit(pai, tty, &pDriver->drv);

	if (!status)
		pai->dpvt = pDriver;
	else if (pDriver)
		free(pDriver);

	return status;
}
