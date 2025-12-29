static struct {
    void *v;
    char *c;
} rcsid = {
    &rcsid,
    "$Id: genSub.c,v 1.1 2009/06/10 15:05:12 gemvx Exp $"
};
/*
 * Copyright 1997 Association of Universities for Research in 
 *	Astronomy, Inc.
 * See the file COPYRIGHT for more details.
 *
 * FILENAME
 * 	xdispCad.c
 *
 * DESCRIPTION 
 * 	xdispCad  support routines
 *
 * 
 * FUNCTION NAME(S)
 *    combStatus - 
 *   
 * DEPENDENCIES
 * 	EPICS support libraries
 *
 *
 *INDENT-OFF*
 * $Log: genSub.c,v $
 * Revision 1.1  2009/06/10 15:05:12  gemvx
 * Added Files:
 * epicsControl diectory populated.
 *
 *INDENT-ON* 
 */


/* VxWorks include files */
#include <vxWorks.h>
#include <taskLib.h>

/* EPICS specific include files */
#include "gnirsTasks.h"
#include "epCommon.h"
#include "epicsNames.h"
#include "gnirsCC.h"

/* Include file needed for control tasks */
#include <sysLib.h>
/*
 *+
 * FUNCTION NAME:
 *   combStatus
 *
 * INVOCATION:
 *   run from datumcombStatus and parkcombStatus genSub records
 *      status = combStatus( *pGenSub )
 *
 * PARAMETERS: (">" input, "!" modified, "<" output)
 *	! pGenSub   (struct genSubRecord *)   pointer to genSub data structure
 *
 * FUNCTION VALUE:
 *	long  Status value, 0 for success
 *
 * PURPOSE:
 *
 * DESCRIPTION:
 *	Cad record releases semaphore which allows xdispCtrl to run.  
 *
 * EXTERNAL VARIABLES:
 *	None
 *
 * PRIOR REQUIREMENTS:
 *	None
 *
 * DEFICIENCIES:
 *	
 *
 * HISTORY (optional):
 *
 *-
 */
long combStatus( struct genSubRecord *pGenSub )
{
	static long zero = 0;
	static long one = 1;
    long status = OK;
/* 	printf("combStatus,%d, %d, %d, %d \n",atoi(pGenSub->a),pGenSub->a,atoi(pGenSub->b),pGenSub->b); */
	if((atoi(pGenSub->a) == 0)||(atoi(pGenSub->b) == 0)||(atoi(pGenSub->c) == 0)||(atoi(pGenSub->d) == 0)||(atoi(pGenSub->e) == 0)||(atoi(pGenSub->f) == 0)||(atoi(pGenSub->g) == 0)||(atoi(pGenSub->h) == 0)||(atoi(pGenSub->i) == 0)||(atoi(pGenSub->j) == 0))
		*(long*)pGenSub->vala = zero;
	else
		*(long *)pGenSub->vala = one;

	return status;
}
