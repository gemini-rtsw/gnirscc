static struct {
	void *v;
	char *c;
} rcsid = {
	&rcsid,
	"$Id: rebootCad.c,v 1.2 2013/06/06 01:54:28 gemvx Exp $"
};
/*
 * Copyright 1997 Association of Universities for Research in Astronomy, Inc.
 * See the file COPYRIGHT for more details.
 *
 * FILENAME
 * 	rebootCad.c
 *
 * DESCRIPTION 
 *	Reboot GNIRS controller and WFS.
 *
 * 
 * FUNCTION NAME(S)
 *	rebootCad
 *
 *   
 * DEPENDENCIES
 * 	 EPICS support functions.
 *
 *
 *INDENT-OFF*
 * $Log: rebootCad.c,v $
 * Revision 1.2  2013/06/06 01:54:28  gemvx
 * Checking in for fixes related to REL-1149.
 *
 * REL-1149 requires moving FW1 into a blocking position when the aquisition
 * mirror is moving out. This will inhibit, "squiggles" on the detector caused
 * by a bright star reflecting off of the aquisition mirror onto the detector.
 *
 * See: http://swgserv01.cl.gemini.edu:8080/browse/REL-1149
 *
 * tom.c
 *
 * Revision 1.1  2009/06/10 15:05:13  gemvx
 * Added Files:
 * epicsControl diectory populated.
 *
 *INDENT-ON* 
 */


/* VxWorks include files */
#include <vxWorks.h>
#include <taskLib.h>
#include <semLib.h>

/* EPICS specific include files */
#include <epCommon.h>
#include "epicsNames.h"
#include "gnirsTasks.h"
#include "gnirsCC.h"


/* Include file needed for control tasks */
#include <sysLib.h>
#include <car.h>

/* this value will have the current state of the car*/
static carVal = CAR_IDLE;

#define HW_REG32 volatile unsigned long
#define BUS_RESET_REG_MV167   0xfff40060   /* Bus reset register for MVME167 */
#define BUS_RESET_BIT_MV167   0x01800000   /* Reset-Switch-Enable and*/
#define BIT_SET(p, d)      { __typeof__ (* (p)) __temp = (* (p));        \
                             * (p) = __temp | (d); }


/* function prototypes*/
int rebootCPU();
void wfsBusReset(void);

/*
 *+
 * FUNCTION NAME:
 *	rebootCad
 *
 * INVOCATION:
 * 	 struct cadRecord* pCad;
 *  	status = rebootCad(pCad);
 *
 * PARAMETERS: (">" input, "!" modified, "<" output)
 *	> pCad  (struct cadRecord*)             pointer to CAD record
 *
 * FUNCTION VALUE:
 *      long            status value written to CAD val field
 *
 * PURPOSE:
 *      User-defined function to support "rebootCad" record
 *
 * DESCRIPTION:
 *	Called whenever rebootCad command is processed.  Prepares for
 *		a reboot and then boots the system.
 *
 *
 * EXTERNAL VARIABLES:
 *	none
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
int rebootInitCad(struct cadRecord *pCad)
{
	
   long status = CAD_ACCEPT; 
   return status;
}

int rebootCad(struct cadRecord *pCad)
{
    int status = CAD_ACCEPT;

	LOG_MSG(DEBUG2_MSG,"\n\nRoutine:rebootCad \n");
	switch (DIRECTIVE)
    {
      case CAD_MARK:			/* No action required	*/
		LOG_MSG(DEBUG2_MSG, "rebootCad - MARK directive");
		break;

      case CAD_PRESET:		
		LOG_MSG(DEBUG2_MSG, "rebootCad - PRESET directive");

		break;

      case CAD_CLEAR:		/* No action required		*/
		LOG_MSG(DEBUG2_MSG, "rebootCad - CLEAR directive");
		break;

      case CAD_START:		/* Execute reboot command	*/
		LOG_MSG(DEBUG2_MSG, "rebootCad - START directive");
		/*set car to busy (ctrl task will set back to idle)*/

	    carVal = CAR_BUSY;
	    /*valh is connected to car record*/
	    *(long *)pCad->valh = carVal;/*   ???  */

	    DPRINT(DPdebug,DEBUG2_MSG,"Waking reboot\n");
	    /*allow control program to run*/
	    semGive(semReboot);


		break;

      case CAD_STOP:			/* Really can't	*/
		LOG_MSG(DEBUG2_MSG, "rebootCad - STOP directive");
		strncpy(MESSAGE, "rebootCad: Cannot stop",MAX_STRING_SIZE - 1);

		status = CAD_REJECT;
		break;

      default:				/* Unknown directive*/
		strncpy(MESSAGE, "rebootCad: Unrecognized directive",MAX_STRING_SIZE - 1);
		status = CAD_REJECT;
		break;
    } 
	return status;
}

/*
 *+
 * FUNCTION NAME:
 *	rebootCtrl
 *
 * INVOCATION:
 *	Started as seperate task in initTasks.
 *
 * PARAMETERS: (">" input, "!" modified, "<" output)
 *	Set of 10 integer variables, interpreted as variety of 
 *	parameters.
 *
 * FUNCTION VALUE:
 *	int		Result of "reboot" command
 *
 * PURPOSE:
 *	Reboot the boards in the vme chassis.
 *
 * DESCRIPTION:
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
 *	09 Mar 98		Initial version - kjr
 *	
 *      4  Feb 99               Changed task name to {cmd}Ctrl. 
 *						Janet Tvedt
 *
 *-
 */

int rebootCtrl( int n1, int n2, int n3, int n4, int n5, int n6, 
	      int n7, int n8, int n9, int n10 )
{
    struct cadRecord* pCad;
    long status = OK;
    long simMode;

    char dummy[MAX_STRING_SIZE];

    /* Convert first parameter to CAD address		*/
    pCad = (struct cadRecord *) n1;

    /* Repeat as inifinite loop				*/
    while(1)
    {
		status = OK;
		LOG_MSG(DEBUG2_MSG, "Task trebootCtrl sleeping...\n");
	
		/* Wait for semaphore			*/
		semTake(semReboot, WAIT_FOREVER);
		LOG_MSG(DEBUG2_MSG, "Task trebootCtrl awake...");
		status = setCar(REBOOT_CAR,CAR_BUSY,OK,"",MESSAGE);
		/* Find out what execution mode we are in	*/
		getDbInfoT(dbTop, INIT_CAD ".VALA",dummy,DBF_LONG,&simMode);

		switch (simMode)
		{
		  case SIM_NONE: 
			sleep(2,0); 
			if(rebootCPU() != VME_OK)/* this will probably never return*/
			{
				if(setCar(REBOOT_CAR,CAR_ERROR,ERROR_REBOOT,"rebootError",
						  dummy) != OK)
					DPRINT(DPdebug,ERROR_MSG,dummy);
			}
			else
				if(setCar(REBOOT_CAR,CAR_IDLE,OK,"",dummy) != OK)
					DPRINT(DPdebug,ERROR_MSG,dummy);
			/*reboot system*/
			break;

		  case SIM_VSM:
		  case SIM_FULL:
		  case SIM_FAST:
			if(setCar(REBOOT_CAR,CAR_IDLE,OK,"",dummy) != OK)
				DPRINT(DPdebug,ERROR_MSG,dummy);
		  default:		/* Don't see any action	*/
			LOG_MSG(DEBUG2_MSG, "Simulation is irrelevant!");
			break;
		}

    }
    return status;
}/*
  *+
  * FUNCTION NAME:
  * rebootCPU
 *
 * INVOCATION:
 * rebootCPU()
 *
 * PARAMETERS: (">" input, "!" modified, "<" output)
 * None
 *
 * FUNCTION VALUE:
 * (int) Returns VME_OK (0) or VME_ERROR (1) as status
 *
 * PURPOSE:
 * Reboots the system
 *
 * DESCRIPTION:
 * Spawns a task which should reboot the system.
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
 * HISTORY:
 * Nov 2, 1999      Initial Version                 (rwolff@noao.edu)
 *
 *-
 */
int rebootCPU()
{

/*     dropDHS(); */
    taskSpawn("suicide", 20, VX_NO_STACK_FILL, 2000, (FUNCPTR) wfsBusReset, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);

    return VME_OK;
}
/*+
 * FUNCTION NAME:
 * wfsBusReset
 *
 * INVOCATION:
 * wfsBusReset (void)
 *
 * PARAMETERS: (">" input, "!" modified, "<" output)
 * None
 *
 * FUNCTION VALUE:
 * None
 *
 * PURPOSE:
 * Resets the VME bus
 *
 * DESCRIPTION:
 * This routine resets the VME bus. It may be used to free up the VME bus if it
 * has hung up after a software or hardware problem. The reset will cause the
 * IOC to reboot.  A pause of 4 seconds has been inserted so that the message
 * that the system is about to reboot will be logged.
 *
 * NOTE:
 * This code has been copied from sysextLib, since sysextLib is no longer going
 * to be used. It only works on an mv167.
 *
 * EXTERNAL VARIABLES:
 * None
 *
 * PRIOR REQUIREMENTS:
 * It is assumed that an EPICS record daemon is running or will soon
 * be spawned
 *
 * INCLUDE FILES:
 * gemTypes.h
 * wfsLib.h
 *
 * DEFICIENCIES:
 * None known
 *
 *-
 */
void wfsBusReset(void)
{
   
    LOG_MSG (CICS_DB_ERROR,
            "wfsBusReset: BUS RESET - SYSTEM WILL REBOOT.\n");

    /* Brief pause to allow message to flush..   */
    taskDelay(4 * sysClkRateGet());

    /* ..then waggle the hardware bits            */
    BIT_SET((HW_REG32 *) BUS_RESET_REG_MV167, BUS_RESET_BIT_MV167);
}
