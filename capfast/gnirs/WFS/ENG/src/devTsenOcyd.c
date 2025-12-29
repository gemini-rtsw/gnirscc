static char rcsid[] = "$Id: devTsenOcyd.c,v 1.2 2009/05/27 19:34:43 fkraemer Exp $";

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
 *     devTsenOcyd.c
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

#include <tsenRecord.h>
#include <choiceTsen.h>
#include "drvOcyd.h"

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
} DevTsenOcyd;

static long read_tsen(struct tsenRecord *);
static long tsen_init_record(struct tsenRecord *);
static long set_units(struct tsenRecord *);
#define init ((DEVSUPFUN)0)
#define report ((DEVSUPFUN)0)
#define get_ioint_info ((DEVSUPFUN)0)

DevTsenOcyd devTsenOcyd = {
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
#if DEBUG > 0
	fprintf(stderr, __FILE__ "(%d): read_tsen(%p)\n", __LINE__, pTsen);
#endif

	if (pTsen && pTsen->dpvt) {
		const OcydValues *pValues;

		pValues = ocydGet(pTsen->dpvt);
		pTsen->val = !!pValues->status;
		pTsen->udf = FALSE;
	}

	return 2; /* Don't convert */
}

static long
tsen_init_record(struct tsenRecord *pTsen)
{
	long status = 0;

#if DEBUG > 0
	fprintf(stderr, __FILE__ "(%d): tsen_init_record(%p)\n", __LINE__, pTsen);
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

	if (!status) {
		status = ocydRecInit(pTsen, pTsen->out.value.instio.string, 
			&pTsen->dpvt);
	}

	/**************************************************************
	 * Activate all necessary channels
	 **************************************************************/

	if (!status) {
		OcydUpdate update;

		update.flags = 0;

		if (pTsen->tmp1) {
			update.flags |= OCYD_SET_ACTIVE(1);
			update.active[1] = 1;
		}
		if (pTsen->tmp2) {
			update.flags |= OCYD_SET_ACTIVE(2);
			update.active[2] = 1;
		}
		if (pTsen->tmp3) {
			update.flags |= OCYD_SET_ACTIVE(3);
			update.active[3] = 1;
		}
		if (pTsen->tmp4) {
			update.flags |= OCYD_SET_ACTIVE(4);
			update.active[4] = 1;
		}
		if (pTsen->tmp5) {
			update.flags |= OCYD_SET_ACTIVE(5);
			update.active[5] = 1;
		}
		if (pTsen->tmp6) {
			update.flags |= OCYD_SET_ACTIVE(6);
			update.active[6] = 1;
		}
		if (pTsen->tmp7) {
			update.flags |= OCYD_SET_ACTIVE(7);
			update.active[7] = 1;
		}
		if (pTsen->tmp8) {
			update.flags |= OCYD_SET_ACTIVE(8);
			update.active[8] = 1;
		}

		ocydSet(pTsen->dpvt, &update);
	}

	return status;
}

static long
set_units(struct tsenRecord *pTsen)
{
	switch (pTsen->egu) {
	case TSEN_UNITS_K:
		ocydSetUnits(pTsen->dpvt, OCYD_UNITS_K);
		break;

	case TSEN_UNITS_F:
		ocydSetUnits(pTsen->dpvt, OCYD_UNITS_F);
		break;

	default:
		ocydSetUnits(pTsen->dpvt, OCYD_UNITS_C);
	}
	return 0;
}
