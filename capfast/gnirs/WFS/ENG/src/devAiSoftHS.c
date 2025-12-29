static char rcsid[] = "$Id: devAiSoftHS.c,v 1.2 2009/05/27 19:34:41 fkraemer Exp $";

/*
 * Copyright 1997 University of Hawaii, Institute for Astronomy.  All
 * rights reserved.
 *
 * The University of Hawaii grants AURA a non-exclusive license to
 * use this software, as stated in [contract].
 *
 * Author: Hubert Yamada, University of Hawaii, Institute for Astronomy
 *
 * EPICS Device support software simulation system for NIRI mechanism. 
 * (This is derived from the standard devAiSoft support.)
 *
 * FILENAME
 *     drvSoftHS.c
 *
 * FUNCTION NAME(S)
 */

#include <vxWorks.h>
#include <types.h>
#include <stdioLib.h>
#include <string.h>

#include <alarm.h>
#include <cvtTable.h>
#include <dbDefs.h>
#include <dbAccess.h>
#include <recSup.h>
#include <devSup.h>
#include <link.h>
#include <aiRecord.h>

#include "recHallStep.h"
#include "drvSoftHS.h"

typedef struct {
	long		number;
	DEVSUPFUN	report;
	DEVSUPFUN	init;
	DEVSUPFUN	ai_init_record;
	DEVSUPFUN	get_ioint_info;
	DEVSUPFUN	read_ai;
	DEVSUPFUN	special_linconv;
} DevAiSoftHS;

#define report ((DEVSUPFUN)0)
#define init ((DEVSUPFUN)0)
static long ai_init_record();
static long read_ai();
#define special_linconv ((DEVSUPFUN)0)
#define get_ioint_info ((DEVSUPFUN)0)
DevAiSoftHS devAiSoftHS = {
	7,
	report,
	init,
	ai_init_record,
	get_ioint_info,
	read_ai,
	special_linconv
};

#define CONVERT 0 /* compute VAL from RVAL */ 
#define NOCONVERT 2 /* use VAL directly */



/*
 *+
 * FUNCTION NAME: ai_init_record
 *
 * INVOCATION: ai_init_record(pai)
 *
 * PARAMETERS:
 *
 *     (>) pai (struct aiRecord *)
 *
 * FUNCTION VALUE: (long) Status value (non-zero indicates an error)
 *
 * SCOPE: global
 *
 * PURPOSE: EPICS device-support initialization
 *
 *     Do all required EPICS device support for the ai record used
 *     with the SoftHS device.
 *
 * DESCRIPTION:
 *
 *     This routine will read the input field of an ai record, and
 *     translate it into a pointer to the appropriate driver-support
 *     structure.
 *
 * EXTERNAL VARIABLES: 
 *
 * PRIOR REQUIREMENTS:
 *
 * DEFICIENCIES:
 *-
 */

static long
ai_init_record(struct aiRecord *pai)
{
	long status = 0;

	/* ai.inp must be an INST_IO */

	if (pai->inp.type == INST_IO) {
		status = shsSensorInit(pai, pai->inp.value.instio.string, &pai->dpvt);
		pai->udf = TRUE;
	} else {
		recGblRecordError(S_dev_badInpType, (void *)pai,
			"devAiSoftHS (ai_init_record) Illegal INP field Type");
		status = S_dev_badInpType;
	}

	return status;
}



/*
 *+
 * FUNCTION NAME: read_ai
 *
 * INVOCATION: read_ai(pai)
 *
 * PARAMETERS:
 *
 *     (>) pai (struct aiRecord *)
 *
 * FUNCTION VALUE: (long) always returns the constant value NOCONVERT
 *
 * SCOPE: global
 *
 * PURPOSE: Update EPICS record VAL (value) field.
 *
 *     Read the value from the driver support for the SoftHS driver,
 *     and use it to update the VAL field of an ai record.
 *
 * DESCRIPTION:
 *
 *     This routine is called whenever the corresponding record needs
 *     to be updated.  It calls the simulated-hardware driver and
 *     reads the current, simulated, input value and uses the new
 *     value to update the ai VAL field.
 *
 * EXTERNAL VARIABLES: 
 *
 * PRIOR REQUIREMENTS:
 *
 * DEFICIENCIES:
 *
 *     None known.
 *-
 */

static long
read_ai(struct aiRecord *pai)
{
	if (pai && pai->dpvt) {
		shsSensorRead(pai->dpvt, &pai->val);
		pai->udf = FALSE;
	}

    return NOCONVERT;
}
