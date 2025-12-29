static char rcsid[] = "$Id: devAiOcyd.c,v 1.2 2009/05/27 19:34:41 fkraemer Exp $";

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
 *     devAiOcyd.c
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
#include "drvOcyd.h"

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
} DevAiOcyd;

static long read_ai(struct aiRecord *);
static long ai_init_record(struct aiRecord *);
#define init ((DEVSUPFUN)0)
#define report ((DEVSUPFUN)0)
#define get_ioint_info ((DEVSUPFUN)0)
#define special_linconv ((DEVSUPFUN)0)

#define READ_TMP    (0x0001)

DevAiOcyd devAiOcyd = {
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
	int channel;
	void *drv;
} Driver;



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
		Driver *pDriver = (Driver *)pai->dpvt;
		const OcydValues *pValues;

		pValues = ocydGet(pDriver->drv);

		switch (pDriver->field) {
		case READ_TMP:
			pai->val = pValues->tmp[pDriver->channel];
			pai->udf = FALSE;
			break;
		}
	}

	return 2; /* Don't convert */
}

static long
ai_init_record(struct aiRecord *pai)
{
	Driver *pDriver;
	long status = 0;
	char temp[MAX_STRING_SIZE];
	char *tty;
	char *field;
	char *channel;

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
		temp[MAX_STRING_SIZE - 1] = '\0';

		tty = strtok(temp, " ");
		field = strtok(NULL, " ");
		channel = strtok(NULL, " ");
	}

	if (!status && !tty) {
		status = S_dev_badSignal;
		recGblRecordError(status, (void *)pai,
			__FILE__ "::ai_init_record No serial port specified.");
	}

	if (!status && !field) {
		status = S_dev_badSignal;
		recGblRecordError(status, (void *)pai,
			__FILE__ "::ai_init_record No field specified.");
	}

	if (!status) {
		if (strcmp(field, "tmp") == 0) {
			if (!channel) {
				status = S_dev_badSignal;
				recGblRecordError(status, (void *)pai,
					__FILE__ "::ai_init_record No channel specified.");
			} else {
				pDriver->channel = atoi(channel);
				pDriver->field = READ_TMP;
			}
		} else {
			status = S_dev_badSignal;
			recGblRecordError(status, (void *)pai,
				__FILE__ "::ai_init_record Illegal field type.");
		}
	}

	if (!status)
		status = ocydRecInit(pai, tty, &pDriver->drv);

	return status;
}
