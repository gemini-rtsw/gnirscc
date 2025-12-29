static char rcsid[] = "$Id: devCoolSoft.c,v 1.2 2009/05/27 19:34:41 fkraemer Exp $";

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
 *     devCoolSoft.c
 *
 * FUNCTION NAME(S)
 */

/*
 * Note: there are a large number of unused inputs and outputs,
 * t0, ..., t7, tmp0, etc.
 *
 * These were intended to be used as 'hooks' for a more sophisticated
 * temperature control system which was never implemented.  They remain
 * just in case such a system may someday be added.
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

#include <coolRecord.h>
#include "recCool.h"

#if !defined(DEBUG)
#	define DEBUG (0)
#endif

typedef struct {
	long number;
	DEVSUPFUN report;
	DEVSUPFUN init;
	DEVSUPFUN cool_init_record;
	DEVSUPFUN get_ioint_info;
} DevCoolSoft;

static long cool_init_record(struct coolRecord *);
#define init ((DEVSUPFUN)0)
#define report ((DEVSUPFUN)0)
#define get_ioint_info ((DEVSUPFUN)0)

DevCoolSoft devCoolSoft = {
	5,
	report,
	init,
	cool_init_record,
	get_ioint_info,
};



static long
cool_init_record(struct coolRecord *pCool)
{
	long status = 0;

#if DEBUG > 0
	printf("This is " __FILE__ "::cool_init_record(%p)\n", pCool);
#endif

	pCool->udf = FALSE;

	return status;
}
