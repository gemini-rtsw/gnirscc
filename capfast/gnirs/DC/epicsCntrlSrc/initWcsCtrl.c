static struct {
	void *v;
	char *c;
} rcsid = {
	&rcsid,
	"$Id: initWcsCtrl.c,v 1.3 2010/11/17 00:56:27 mrippa Exp $"
};
/* #define DEBUG */
/*
 * Copyright 1997 Association of Universities for Research in Astronomy, Inc.
 * See the file COPYRIGHT for more details.
 *
 * FILENAME
 * initWcsCtrl.c
 *
 * DESCRIPTION 
 * Set World Coordinate System values for data records.
 *
 * 
 * FUNCTION NAME(S)
 *	initWcsCtrl - initWcsCad control function
 *	getWcsParams - get tcs values and send to controller
 *	printCtx - print tcs context ( used for testing and debugging)
 *	printTel - print telescope parameters ( used for testing and debugging)
 *   
 * DEPENDENCIES
 * EPICS support libraries.
 *	TCS
 *	astLib
 *
 *INDENT-OFF*
 * $Log: initWcsCtrl.c,v $
 * Revision 1.3  2010/11/17 00:56:27  mrippa
 * Process CalcWcs(0,0) when semInitWcs semaphore
 * is released. This favors getWcsParams() since CalcWcs
 * is called there as well and the WCS data gets processed
 * in CalcWcs().
 *
 * Revision 1.2  2009/05/27 19:32:21  fkraemer
 * fkraemer - copied my complete working dir over trunk
 *
 *INDENT-ON* 
 */


/* VxWorks include files */
#include <vxWorks.h>
#include <taskLib.h>
#include <semLib.h>

/* EPICS specific include files */
/* #define NODBACCESS   */
#include <epCommon.h>

/* Include file needed for control tasks */
#include <sysLib.h>
#include <car.h>
#include "timeLib.h"
#include "slalib.h"
#include "astLib.h"
#include "localWcs.h"

/* Function declarations  */
long printctx(struct WCS_CTX ctx);
long printTel(struct TELP tel);
int mytimeOffline ( double tai,
                  double elong, double phi, double hm,
                  double dleap, double dat, double dut );
int InitWcs(int, char *, char *, char *);

/* global declarations*/

extern long TCS;
extern SEM_ID semInitWcs;
extern char *dbTop;
char ErrorMessage[120];

/*
 *+
 * FUNCTION NAME:
 * initWcsCtrl
 *
 * INVOCATION:
 *	spawned from initTasks.c
 *
 * PARAMETERS: (">" input, "!" modified, "<" output)
 *
 *
 * FUNCTION VALUE:
 *	infinite loop
 *
 * PURPOSE:
 *	do time consuming tasks related to wcs cad record
 *
 * DESCRIPTION:
 *
 *
 *
 * EXTERNAL VARIABLES:
 *
 *
 * PRIOR REQUIREMENTS:
 *
 *
 * DEFICIENCIES:
 *
 *
 * HISTORY (optional):
 *
 *
 *
 *-
 */
int initWcsCtrl( int n1, int n2, int n3, int n4, int n5, int n6, int n7, int n8,
		 int n9, int n10 )
{
#ifdef DEBUG
    int t1=1;
#endif
    char errMess[80];
    char err[80];
    struct cadRecord* pCad;
    long status = CAD_ACCEPT;
    unsigned short lVal;

    strcpy(err,"");
    /* Convert first parameter to CAD address	*/
    pCad = (struct cadRecord *) n1;

    /* Repeat as inifinite loop	*/
    while(1)
    {
	DPRINT(t1, "Task tinitWcsCtrl sleeping...\n");

	/* Wait for our semaphore   */
	semTake(semInitWcs, WAIT_FOREVER);
	status = OK;
	DPRINT(t1, "Task tinitWcsCtrl awake...");

	/* send file names to controller*/
	if(InitWcs(NUM_CHIPS,pCad->vala ,pCad->valb ,pCad->valc ) == OK)
		 
	{   /* set car back to idle*/

	    CalcWcs(0, 0);
	    /*getWcsParams();*/
	    status = OK; 
		   
	    lVal = GNAAC_DONE;
		    
	    /*putDbInfoT(dbTop, INIT_WCS_DONE ".VAL", MSG, DBF_ENUM, &lVal);*/
	    if(setCar(WCS_CAR,CAR_IDLE,OK,"",errMess)!= OK)
	    {
		status = ERROR;
		DPRINT(1,errMess);
	    }
	}
	else
	{
	    status = ERROR;
	    ErrorMessage[39] = 0; 
	    lVal = GNAAC_NOT_DONE;
	    /*putDbInfoT(dbTop, INIT_WCS_DONE ".VAL", MSG, DBF_ENUM, &lVal); */
	    /* set car 	to error if InitWcs returned an error*/
	    if(setCar(WCS_CAR,CAR_ERROR,ERROR_WCS,ErrorMessage,errMess)!= OK)
	    {
		status = ERROR;
		DPRINT(1,errMess);
	    }
	}
		
	
    }
	

    return status;
}
