static char rcsid[] = "$Id: devTsenTsim.c,v 1.2 2009/05/27 19:34:43 fkraemer Exp $";

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
 *     devTsenTsim.c
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

#include <tsenRecord.h>
#include "drvTsim.h"

#if !defined(DEBUG)
#	define DEBUG (0)
#endif

typedef struct {
	long number;
	DEVSUPFUN report;
	DEVSUPFUN init;
	DEVSUPFUN tsen_init_record;
	DEVSUPFUN get_ioint_info;
	DEVSUPFUN read_tsen;
	DEVSUPFUN set_units;
} DevTsenTsim;

static long read_tsen(struct tsenRecord *);
static long tsen_init_record(struct tsenRecord *);
#define init ((DEVSUPFUN)0)
#define report ((DEVSUPFUN)0)
#define get_ioint_info ((DEVSUPFUN)0)
#define set_units ((DEVSUPFUN)0)

DevTsenTsim devTsenTsim = {
	7,
	report,
	init,
	tsen_init_record,
	get_ioint_info,
	read_tsen,
	set_units,
};



/*
 *+
 * FUNCTION NAME: read_tsen
 *
 * INVOCATION: read_tsen(pTsen)
 *
 * PARAMETERS:
 *
 *     (!) pTsen (struct tsenRecord *)
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
read_tsen(struct tsenRecord *pTsen)
{
	/*printf(__FILE__ "::read_tsen(%p)\n", pTsen);*/

	if (pTsen && pTsen->dpvt) {
		/*const TsimValues *pValues = (TsimValues *)pTsen->dpvt;*/

		pTsen->val = 0;
		pTsen->udf = FALSE;
	}

	return 2; /* Don't convert */
}

static long
tsen_init_record(struct tsenRecord *pTsen)
{
	long status = 0;

#if DEBUG > 0
	printf("This is " __FILE__ "::tsen_init_record(%p)\n", pTsen);
#endif

	pTsen->udf = TRUE;

	/**************************************************************
	 * Find the data field
	 **************************************************************/

	/*
	 * ai.out must be an INST_IO
	 */

	if (!status && pTsen->out.type != INST_IO) {
		status = S_dev_badInpType;
		recGblRecordError(S_dev_badInpType, (void *)pTsen,
			__FILE__ "::tsen_init_record Illegal INP field type.");
	}

	if (!status)
		status = tsimRecInit(pTsen, &pTsen->dpvt);

	return status;
}
