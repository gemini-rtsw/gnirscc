
static struct {
        void *v;
        char *c;
} rcsid = {
        &rcsid,
        "$Id: gnDCASetup.c,v 1.2 2009/05/27 19:32:44 fkraemer Exp $"
};



/*****************************************************************************
 * Copyright 1999 Association of Universities for Research in Astronomy, Inc. 
 * See the file COPYRIGHT for more details.
 *
 * Filename: 	
 * 	gnDCASetup.c
 *
 * Description:
 * 	This file contains the tasks in the Data Handler IOC which setup the NOAO 
 *		Coadd board. These operations are in seperate tasks because 
 *		I wanted to keep the structure of the code for the new system as
 *		close as possible to the structure of the dataCube code.  These may be
 *		noops in the final code
 *
 * Function name(s)
 * 	gnTestSetup() - uses observation parameters from the EPICS IOC to 
 *		setup the NOAO CoAdd Board to run the test suite (NYI)
 * 	gnSepSetup() - uses observation parameters from the EPICS IOC to
 *		setup the NOAO CoAdd Board to run in the Sepped data 
 *		processing mode mode (NYI)
 * 	gnRDDStareSetup() - uses observation parameters from the EPICS IOC to
 *		setup the NOAO CoAdd Board to run in the normal
 *		subtracted data mode.  Implements LNRs and Coadding.
 * 	gnRRDStareSetup() - uses observation parameters from the EPICS IOC to
 *		setup the NOAO CoAdd Board to run in the normal
 *		Row Reset High background Co-adding data mode.
 *
 * Dependencies
 * 	The Data Coadder interupt event driver for vxWorks should be loaded and running.
 *
 * Orginial Author:
 *	Nick C. Buchholz
 *
 * History:
 *	20-Oct-1999: Created original version - ncb
 *
 ***************************************************************************/
#include <stdio.h>
#include <usrLib.h>
#include <sysLib.h>
#include <rpcLib.h>
#include <rpc/rpc.h>	/* always need this here */

#include <carRecord.h>
#include <dataHandling.h>

#include <noaoDCA.h>
#include <gnerrno.h>
#include <gnDCADefs.h>

/* #define TASKINIT */
#undef MAIN
#include <gnDCAVars.h>

#include <epCommon.h>

#include "gnDQSocket.h"

#define SDEBUG
static char retString[80];
extern int dcadebug;

int gnTestSetup(int n1, int n2, int n3, int n4, int n5, int n6, int n7,
		 int n8, int n9, int n10)
{
    int status =(OK);
    char errString[MAX_STRING_SIZE];
 
  /* run Test setup routines then do a SemGive on the appropriate semaphore
     so the waiting RPC routine can complete
  */

    /* Sleep until there is something to do */
    while( 1 )
    {
	semTake(semSetupTEST,WAIT_FOREVER); 

	/* Initialize the Capture Buffer system for data taking */
	status =  initCaptBufs( arSizeIdx );
	if (status != (OK))
	{
	    sprintf(errString, "gnRddStS error: DCA Capture buf Setup Failed");
	    setCar(OBSERVE_CAR,menuCarstatesERROR,CAPT_BUF_NOT_SET,errString,retString);
	   
	    continue;
	}


	testSetupDone = TRUE;    
	semGive(semSetup); 
	continue;
    }
}

int gnSepSetup(int n1, int n2, int n3, int n4, int n5, int n6, int n7,
		 int n8, int n9, int n10)
{
    int status =(OK);
    char errString[MAX_STRING_SIZE];
   
    /* Sleep until there is something to do */
    while( 1 )
    {
	semTake(semSetupSEP, WAIT_FOREVER); 
	/* Initialize the Capture Buffer system for data taking */
	status =  initCaptBufs( arSizeIdx );
	if (status != (OK))
	{
	    sprintf(errString, "gnRddStS error: DCA Capture buf Setup Failed");
	    setCar(OBSERVE_CAR,menuCarstatesERROR,CAPT_BUF_NOT_SET,errString,retString);
	   
	    continue;
	}


	sepSetupDone = TRUE;    
	semGive(semSetup); 
	continue;
    }
}



int gnRDDStareSetup(int n1, int n2, int n3, int n4, int n5, int n6, int n7,
		 int n8, int n9, int n10)
{
    int status =(OK);
    char errString[MAX_STRING_SIZE];
    
    
  /* run RDD setup routines then do a semGive on the appropriate semaphore
   * so the waiting RPC routine can complete
   */

    /* Sleep until there is something to do */
    while( 1 )
    {
	semTake(semSetupRDD,WAIT_FOREVER); 

	/* Initialize the Capture Buffer system for data taking */
/* 	printf("setting capt bufs\n"); */
	status =  initCaptBufs( arSizeIdx );
/* 	printf("done setting capt bufs\n"); */
	if (status != (OK))
	{
	    sprintf(errString, "gnRddStS error: DCA Capture buf Setup Failed");
	    setCar(OBSERVE_CAR,menuCarstatesERROR,CAPT_BUF_NOT_SET,errString,retString);
	   
	    continue;
	}


	rddSetupDone = TRUE;    
	semGive(semSetup); 
	continue;
    }

}

int gnRRDStareSetup(int n1, int n2, int n3, int n4, int n5, int n6, int n7,
		 int n8, int n9, int n10)
{
    int status =(OK);
    char errString[MAX_STRING_SIZE];
    
    /* run RRD setup routines then do a SemGive on the appropriate semaphore
     * so the waiting RPC routine can complete
     */
    /* Sleep until there is something to do */
    while( 1 )
    {
	semTake(semSetupRRD,WAIT_FOREVER); 

	/* Initialize the Capture Buffer system for data taking */
	status =  initCaptBufs( arSizeIdx );
	if (status != (OK))
	{
	    sprintf(errString, "gnRRdStS error: DCA Capture buf Setup Failed");
	    setCar(OBSERVE_CAR,menuCarstatesERROR,CAPT_BUF_NOT_SET,errString,retString);
	   
	    continue;
	}



	rrdSetupDone = TRUE;
	semGive(semSetup); 
	continue;
    }

}
