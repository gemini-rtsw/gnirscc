static struct {
    void *v;
    char *c;
} rcsid = {
    &rcsid,
    "$Id: motor.c,v 1.1 2009/06/10 15:05:13 gemvx Exp $"
};
typedef struct testStruct
{
    long l;
    double d;
    char s[80];
}testStruct;
/*
 * Copyright 1997 Association of Universities for Research in 
 *	Astronomy, Inc.
 * See the file COPYRIGHT for more details.
 *
 * FILENAME
 * 	motor.c
 *
 * DESCRIPTION 
 * 	motor  support routines
 *
 * 
 * FUNCTION NAME(S)
 *	stopMech
 *   
 * DEPENDENCIES
 *
 *
 *INDENT-OFF*
 * $Log: motor.c,v $
 * Revision 1.1  2009/06/10 15:05:13  gemvx
 * Added Files:
 * epicsControl diectory populated.
 *
 *INDENT-ON* 
 */


/* VxWorks include files */
#include <vxWorks.h>
#include <taskLib.h>

/* EPICS specific include files */
/* #include "gnirsCcDefs.h" */
#include "gnirsTasks.h"
#include "epCommon.h"
#include "epicsNames.h"
#include "gnirsCC.h"

/* Include file needed for control tasks */
#include <sysLib.h>
#include <car.h>
/*
 *+
 * FUNCTION NAME:
 *	stopMech
 *
 * INVOCATION:
 *      status = stopMech(mechNum);
 *
 * PARAMETERS: (">" input, "!" modified, "<" output)
 *     >  mechNum - mechanism number
 *
 * FUNCTION VALUE:
 *    status value
 *
 * PURPOSE:
 *
 * DESCRIPTION:  
 *     stops any motion of the given motor.
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

void stopMech (int mechNum)
{

/*     abortMotor(mechMotor(mechNum)); */

}
