static char rcsid[] = "$Id: devHsSoft.c,v 1.2 2009/05/27 19:34:42 fkraemer Exp $";

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
 *     devHallStepWheel.c
 *
 * FUNCTION NAME(S)
 */

#include <dbCommon.h>
#include <dbEvent.h>
#include <recSup.h>
#include <devSup.h>
#include <time.h>
#include <string.h>
#include <limits.h>
#include <sys/types.h>

#include <hallStepRecord.h>
#include "recHallStep.h"

static long move(HallStepRecord *);
static long datum(HallStepRecord *);
static long redatum(HallStepRecord *);
static long cycleN(HallStepRecord *, int);
#define init_record    ((DEVSUPFUN)0)
#define init           ((DEVSUPFUN)0)
#define report         ((DEVSUPFUN)0)
#define get_ioint_info ((DEVSUPFUN)0)
#define update         ((DEVSUPFUN)0)
#define verify         ((DEVSUPFUN)0)
#define directMove     ((DEVSUPFUN)0)

DevHallStep devHallStepSoft = {
	HS_DEVNUM,
	report,
	init,
	init_record,
	get_ioint_info,
	move,
	datum,
	redatum,
	cycleN,
	update,
	verify,
	directMove,
};



/*
 *+
 * FUNCTION NAME: datum
 *
 * INVOCATION: datum(phs)
 *
 * PARAMETERS:
 *
 *     (!) phs (HallStepRecord *)
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
datum(HallStepRecord *phs)
{
	hsMSPause(phs, 1000);

	return 0;
}



/*
 *+
 * FUNCTION NAME: move
 *
 * INVOCATION: move(phs)
 *
 * PARAMETERS:
 *
 *     (!) phs (HallStepRecord *)
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
 *
 *     static function
 *-
 */

static long
move(HallStepRecord *phs)
{
	hsMoveTo(phs, 100, 1);

	hsMSPause(phs, 1000);

	return 0;
}



/*
 *+
 * FUNCTION NAME: redatum
 *
 * INVOCATION: redatum(phs)
 *
 * PARAMETERS:
 *     phs (HallStepRecord *)
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
 *
 *     static function
 *-
 */

static long
redatum(HallStepRecord *phs)
{
	hsMSPause(phs, 1000);
	phs->rdtm = 0;

	return 0;
}



/*
 *+
 * FUNCTION NAME: cycleN
 *
 * INVOCATION: cycleN(phs, nPass)
 *
 * PARAMETERS:
 *     (!) phs (HallStepRecord)
 *     (>) nPass (int)
 *
 * FUNCTION VALUE:
 *     (long) status value (non-zero indicates an error)
 *
 * PURPOSE:  
 *     Exercise a piece of hardware
 *
 *     This is a diagnostic routine which is used to verify that a
 *     piece of hardware is fully operational and for lifetime testing.  
 *
 * DESCRIPTION:
 *
 *     This routine monitors the Hall effect sensors and will shut 
 *     itself off automatically if any values have changed.  This 
 *     makes it useful for detecting infrequent, lost stepper-motor
 *     counts.  It is safe for unattended operation, because it 
 *     should stop within a short time if the hardware hits a 
 *     limit or is otherwise unable to move.
 *
 *     This subroutine moves the hardware to three pre-defined positions
 *     (normally, two limits and the home position, or two filters and 
 *     the home position).  It records the three sensor values.  It then
 *     repeatedly moves the hardware to each position, and verifies that
 *     the sensor values have not changed.
 *
 * EXTERNAL VARIABLES: none
 *
 * PRIOR REQUIREMENTS:
 *
 *     Must always be called with the database locked (use dbScanLock).
 *
 * DEFICIENCIES:
 *-
 */

static long
cycleN(HallStepRecord *phs, int nRep)
{
	hsMSPause(phs, 1000);

	return 0;
}
