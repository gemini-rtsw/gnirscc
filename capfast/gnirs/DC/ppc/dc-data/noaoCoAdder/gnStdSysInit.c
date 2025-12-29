
static struct {
        void *v;
        char *c;
} rcsid = {
        &rcsid,
        "$Id: gnStdSysInit.c,v 1.2 2009/05/27 19:32:44 fkraemer Exp $"
};


/*****************************************************************************
 * Copyright 1999 Association of Universities for Research in Astronomy, Inc. 
 * See the file COPYRIGHT for more details.
 *
 * Filename:
 * 	gnStdSysInit.c
 *
 * Description:
 *      Creates and Initializes the Data CoAdder Control Structure, sets up DCA Board 
 *	    This routine sets up a single CoAdder system with a Single Array.
 *	    It is designed use with the VxWorks operating system. 
 *
 * Routines:
 * 	int gnCreateStdSys(void) - Creates and Initializes the Data CoAdder Control
 *	        structure. Initializes Board registers as needed.
 *
 * 
 * 	int gnDisposeSys(void) - Disposes of the data CoAdder structure an shutdown.
 *	        Should never be used in normal operations
 *			
 * 	int gnDCASurfInit(void) - sets up buffer descriptions needed for Data processing in
 *		data CoAdder board.
 *
 *
 * Dependencies
 * 	The Data CoAdder interupt handler must be installed.  NOAO Coadder board must be 
 	    installed at address  xxx???
 *	
 * Original Author:      
 * 	Nick C Buchholz
 *
 * History:	
 *	02-Oct-1999 - Original version - Create routines to work under
 *		VXWorksOS - ncb
 *
 ***************************************************************************/
#include <sys/types.h>
#include <sys/times.h>

#include <stdio.h>
#include <stdlib.h>
#include <vme.h>
#include <sysLib.h>

#include "gnDCADefs.h"
#undef MAIN
#include "gnDCAVars.h"
#include <irstd.h>

/*****************************************************************************
 * Function name:
 * 	gnCreateStdSys
 *
 * Invocation:
 * 	status = gnCreateStdSys ( );
 *
 * PARAMETERS:
 *	None
 *
 * FUNCTION VALUE:
 * 	int - Status value returned by function 
 *
 * PURPOSE:
 * 	Sets up Standard system control structures
 *
 * DESCRIPTION:
 *	Creates the Data CoAdder oSystem variable and assigns message queues for interupt 
 *	    action handlers. Also does standard hardware initialization for tasks used by 
 *	    GNAAC programs.
 *
 * EXTERNAL VARIABLES:
 * 	Lots - see gnDCAVars.h
 *
 * PRIOR REQUIREMENTS:
 *	Assumes NOAO data CoAdder board intalled at address xxx???
 *
 * DEFICIENCIES:
 * 	None known
 *
 * HISTORY:
 * 	03-Oct-1999  Original version  N Buchholz
 *
 *****************************************************************************/
int gnCreateStdSys(void)
{
#ifdef DEBUG
    int t1 = 0;
#endif
    int status = (OK);
    
    DPRINT(t1,"gnCreateStdSys 1\n");

    /* handle system setup */
    

    
   


    return (status);
}


/*****************************************************************************
 * Function name:
 * 	gnDisposeSys
 *
 * Invocation:
 * 	status = gnDisposeSys ( );
 *
 * PARAMETERS:
 *	None
 *
 * FUNCTION VALUE:
 * 	int - Status value returned by function
 *
 * PURPOSE:
 * 	Disposes of Data CoAdd control structure.
 *
 * DESCRIPTION:
 *	Frees data CoAdd control structure memory.
 *
 * EXTERNAL VARIABLES:
 * 	Lots - see gnDCAVars.h
 *
 * PRIOR REQUIREMENTS:
 *	Assumes the system structure has been created and malloc'ed and assigned to the 
 *	    oSystem variables.
 *
 * DEFICIENCIES:
 * 	None known
 *
 * HISTORY:
 * 	03-Oct-1999  Original version  N Buchholz
 *
 *****************************************************************************/
int gnDisposeSys(void)
{
    int status = (OK);

    /* nothing to do here */

    return (status);

}
