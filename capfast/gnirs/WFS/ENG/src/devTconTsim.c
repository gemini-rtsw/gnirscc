static char rcsid[] = "$Id: devTconTsim.c,v 1.2 2009/05/27 19:34:43 fkraemer Exp $";

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
 *     devTconTsim.c
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

#include <tconRecord.h>
#include <choiceTcon.h>
#include "recTcon.h"
#include "drvTsim.h"

#if !defined(DEBUG)
#define DEBUG (0)
#endif

typedef struct {
	long number;
	DEVSUPFUN report;
	DEVSUPFUN init;
	DEVSUPFUN tcon_init_record;
	DEVSUPFUN get_ioint_info;
	DEVSUPFUN read_tcon;
	DEVSUPFUN set_units;
} DevTconTsim;

static long read_tcon(struct tconRecord *);
static long tcon_init_record(struct tconRecord *);
#define init ((DEVSUPFUN)0)
#define report ((DEVSUPFUN)0)
#define get_ioint_info ((DEVSUPFUN)0)
#define set_units ((DEVSUPFUN)0)

DevTconTsim devTconTsim = {
	7,
	report,
	init,
	tcon_init_record,
	get_ioint_info,
	read_tcon,
	set_units,
};



/*
 *+
 * FUNCTION NAME: read_tcon
 *
 * INVOCATION: read_tcon(pTcon)
 *
 * PARAMETERS:
 *
 *     (!) pTcon (struct tconRecord *)
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
read_tcon(struct tconRecord *pTcon)
{
	/*long status = 0;*/

	if (pTcon && pTcon->dpvt) {
		TsimTcon *const pTsimTcon = (TsimTcon *)pTcon->dpvt;

		pTcon->val = 0;

		if (pTcon->udf) {
			pTcon->udf = FALSE;

			pTcon->mon |= TCON_MONITOR_SETP;
			pTcon->setp = pTcon->lstp = pTsimTcon->setPoint;
		}

		/*
		 * Update the gain, rate, reset, and set point.  The values can
		 * be changed via channel access, in which case the last value
		 * will not agree with the current value.
		 */

		if (pTcon->setp != pTcon->lstp)
			pTsimTcon->setPoint = pTcon->lstp = pTcon->setp;
	}

	return 0;
}

static long
tcon_init_record(struct tconRecord *pTcon)
{
	long status = 0;
	int ctrl;
	TsimValues *pValues;

#if DEBUG > 1
	printf("This is " __FILE__ "::tcon_init_record(%p)\n", pTcon);
#endif

	pTcon->udf = TRUE;

	/**************************************************************
	 * Find the data field
	 **************************************************************/

	/*
	 * ai.out must be an INST_IO
	 */

	if (!status && pTcon->out.type != INST_IO) {
		status = S_dev_badInpType;
		recGblRecordError(S_dev_badInpType, (void *)pTcon,
			__FILE__ "::tcon_init_record Illegal INP field type.");
	}

	if (!status) {
		if (*pTcon->out.value.instio.string == 'C') {
			ctrl = atoi(pTcon->out.value.instio.string + 1);

			if (ctrl < 0 || ctrl > TSIM_NTC) {
				status = S_dev_badSignal;
				recGblRecordError(S_dev_badSignal, (void *)pTcon,
					"ai_init_record Controller out of range.");
			}
		} else {
			status = S_dev_badSignal;
			recGblRecordError(S_dev_badSignal, (void *)pTcon,
				"ai_init_record Bad channel specification.");
		}
	}

	if (!status)
		status = tsimRecInit(pTcon, (void *)&pValues);
	
	if (!status)
		pTcon->dpvt = &pValues->tc[ctrl];

	return status;
}
