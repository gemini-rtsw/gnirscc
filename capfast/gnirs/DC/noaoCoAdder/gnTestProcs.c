static struct {
        void *v;
        char *c;
} rcsid = {
        &rcsid,
        "$Id: gnTestProcs.c,v 1.2 2009/05/27 19:32:31 fkraemer Exp $"
};


/*****************************************************************************
 * Copyright 1997 Association of Universities for Research in Astronomy, Inc. 
 * See the file COPYRIGHT for more details.
 *
 * Filename:
 *	gnTestProcs.c
 *
 * Description:
 *	Initializes connects. creates arms and fires the gnaac TestProcuistion pipe
 *
 * Function Names:
 *   	int gnTProcPatSetup(void) - Connects, Creates, arms and fires the 
 *		test program processing pipe
 *
 *
 * Dependencies:
 * 	
 *
 * Author:      
 *	Nick C Buchholz copied from J. Heim naacdq.c
 *
 * History:
 *	03-June-1997 - Original version copied from J. Heim's naacdq.c - ncb
 *
 ***************************************************************************/
#include <sys/types.h>
#include <sys/times.h>

#include <stdio.h>
/* #include <datacube.h> */
#include <vxWorks.h>  
#include <ioLib.h>    
#include <pipeDrv.h>  

#include "gnDCADefs.h"
#undef	MAIN
#include "gnDCAVars.h"

/*****************************************************************************
 * Function name:
 * 	gnTProcPatSetup
 *
 * Invocation:
 * 	status = gnTProcPatSetup( );
 *
 * PARAMETERS:
 *	None
 *
 * FUNCTION VALUE:
 * 	int - Status value returned by function 
 *
 * PURPOSE:
 * 	sets up the pipe and PAT to move data from the Fnl Data surface through
 *
 * 	the AP LUT into The display source surface.
 *
 * DESCRIPTION:
 * 	Uses DataCube library calls to do the connections and configurations
 *	needed to unscramble the four qudrants of the array in the test (i.e.
 *	display only) version of the gndq program. does the hardware
 *	initialization to attach surfaces to the pipe ends.
 *
 * EXTERNAL VARIABLES:
 * 	Lots - see gnaaDQVars.h
 *
 * PRIOR REQUIREMENTS:
 *	Uses the new Acquire Pipe so It Needs to Be Complied in
 *	Assumes two Datacube boards with six, 4Mb Am memory modules each
 *		board. Assumes the ImageFlow Libraries are loaded.  
 *
 * DEFICIENCIES:
 * 	None known
 *
 * HISTORY:
 * 	13-June-1997  Original version  N Buchholz
 *
 *****************************************************************************/
#if defined DATACUBE
int gnTProcPatSetup( void )
{
    int status = (OK);

    SendSurf20Mhz(oFnlDataUwHb);
    SendSurf20Mhz(oFnlDataUwLb);

#if 0
    tPRect.lXMin = 0;
    tPRect.lXMax = 1023;
    tPRect.lYMin = 0;
    tPRect.lYMax = 1023;
     dqSpecSurfAlignPoint (oFnlDataUwHb, 0, 0); 
    dqSpecSurfAlignPoint (oFnlDataUwLb, 0, 0);
    dqSetSurfProcRect (oFnlDataUwHb, pRect);
    dqSetSurfProcRect (oFnlDataUwLb, pRect);
#endif

    dqConnect(oAb0, DQ_IMX3, AB_OP16);   /* AM03 (Hb) XMT gate to AP DQ_CP02 */
    dqConnect(oAb0, DQ_IMX2, AB_OP15);   /* AM02 (Lb) XMT gate to AP DQ_CP01 */
    dqConnect(oAp_B0, AP_ZEROS, AP_MOSC1_SRC);/* Make sure MOSC is disabled */
    dqConnect(oAp_B0, AP_DLUT, AP_CFG);   /* Configure AP for LUT output  */
    dqConnect(oAb0, DQ_CP11, AB_OP00);  /* AP output to Display Surface */

    RcvSurf20Mhz(oTProcDstSurf);

    /* Create Pipe */
    oDispMvPipe=dqCreatePipe(oTProcDstSurf, DQ_TRG_ONESHOT);

emBegPatDef();
{
	setTP(9);
	dqArmPipe(oDispMvPipe, DQ_DSM_PIPE);
	iDispMvPipeEvnt = emFindPipeEvent(oDispMvPipe);
	dqFirePipe(oDispMvPipe);
	emWaitEvent(iDispMvPipeEvnt, 1);
	clrTP(9);
	dqFreePipeTimingBus(oDispMvPipe); /* Now free the pipe timing bus to allow
				      other pipes to run */
}
oDispMvPat = emEndPatDef();
    /* Find Handles for PAT events */
    iDispMvPatDone = emFindPatEvent (oDispMvPat);

    return (status);
}

#endif
