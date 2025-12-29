static struct {
	void *v;
	char *c;
} rcsid = {
	&rcsid,
	"$Id: parkCad.c,v 1.2 2013/06/06 01:54:28 gemvx Exp $"
};

/*
 * Copyright 1997 Association of Universities for Research in Astronomy, Inc.
 * See the file COPYRIGHT for more details.
 *
 * FILENAME
 * 	park.c
 *
 * DESCRIPTION 
 * 	Implements Gemini "park" command.
 *
 * 
 * FUNCTION NAME(S)
 *	parkCad
 *
 *   
 * DEPENDENCIES
 * 	EPICS support libraries
 *
 *
 *INDENT-OFF*
 * $Log: parkCad.c,v $
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
#include "epCommon.h"
#include "epicsNames.h"
#include "gnirsTasks.h"
#include "gnirsCC.h"

/* Include file needed for control tasks */
#include <sysLib.h>
#include <car.h>

/* this value will have the current state of the car*/
static carVal = CAR_IDLE;

extern MSG_Q_ID cameraMotorQ;
extern MSG_Q_ID coverMotorQ;
extern MSG_Q_ID deckerMotorQ;
extern MSG_Q_ID focusMotorQ;
extern MSG_Q_ID fw1MotorQ;
extern MSG_Q_ID fw2MotorQ;
extern MSG_Q_ID gratingMotorQ;
extern MSG_Q_ID slitMotorQ;
extern MSG_Q_ID spare1MotorQ;
extern MSG_Q_ID spare2MotorQ;
extern MSG_Q_ID xdispMotorQ;

long getacqCar();
long getcoverCar();
long getcameraCar();
long getdeckerCar();
long getfocusCar();
long getfw1Car();
long getfw2Car();
long getgratingCar();
long getslitCar();
long getxdispCar();
long setacqCarBusy();
long setcoverCarBusy();
long setcameraCarBusy();
long setdeckerCarBusy();
long setfocusCarBusy();
long setfw1CarBusy();
long setfw2CarBusy();
long setgratingCarBusy();
long setslitCarBusy();
long setxdispCarBusy();
extern long sendAcqPark();

/*
 *+
 * FUNCTION NAME:
 *	parkCad
 *
 * INVOCATION:
 *	struct cadRecord *pCad;
 * 	status = parkCad( pCad );
 *
 * PARAMETERS: (">" input, "!" modified, "<" output)
 * 	> pCad   (struct cadRecord *)   pointer to CAD data structure
 *
 * FUNCTION VALUE:
 * 	long  Status value written to CAD VAL field
 *
 * PURPOSE:
 * 	User defined function for "park" CAD record
 *
 *
 *
 * DESCRIPTION:
 *	This function is triggered by the park cad record.  When start
 * 	is received, it releases a semaphore to allow the parkCtrl routine
 *	to run.
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


int parkCad(struct cadRecord *pCad)

{
    long status = CAD_ACCEPT;


    /* Action to be taken depends on directive received		*/
    switch (DIRECTIVE)
    {
      case CAD_MARK:	       	/* No action required		*/
		LOG_MSG(DEBUG2_MSG, "parkCad - MARK directive");
		break;

      case CAD_PRESET:
		LOG_MSG(DEBUG2_MSG, "parkCad - PRESET directive");
	if(parkMech(ACQ)!= VME_OK)
		{
			sprintf(MESSAGE,"parkCad: acqILLEGAL motor\n");
			status = CAD_REJECT;
		}
		if(parkMech(COVER)!= VME_OK)
		{
			sprintf(MESSAGE,"parkCad: coverILLEGAL motor\n");
			status = CAD_REJECT;
		}
		if(parkMech(FW1)!= VME_OK)
		{
			sprintf(MESSAGE,"parkCad: fw1ILLEGAL motor\n");
			status = CAD_REJECT;
		}
		if(parkMech(FW2)!= VME_OK)
		{
			sprintf(MESSAGE,"parkCad: fw2 ILLEGAL motor\n");
			status = CAD_REJECT;
		}
		if(parkMech(SLIT)!= VME_OK)
		{
			sprintf(MESSAGE,"parkCad: slit ILLEGAL motor\n");
			status = CAD_REJECT;
		}
		if(parkMech(DECKER)!= VME_OK)
		{
			sprintf(MESSAGE,"parkCad: decker ILLEGAL motor\n");
			status = CAD_REJECT;
		}
		if(parkMech(GRATING)!= VME_OK)
		{
			sprintf(MESSAGE,"parkCad: grating ILLEGAL motor\n");
			status = CAD_REJECT;
		}
		if(parkMech(CAMERA)!= VME_OK)
		{
			sprintf(MESSAGE,"parkCad: camera ILLEGAL motor\n");
			status = CAD_REJECT;
		}
		if(parkMech(FOCUS)!= VME_OK)
		{
			sprintf(MESSAGE,"datmCad: focus ILLEGAL motor\n");
			status = CAD_REJECT;
		}
		if(parkMech(XDISP)!= VME_OK)
		{
			sprintf(MESSAGE,"parkCad: xdisp ILLEGAL motor\n");
			status = CAD_REJECT;
		}
		break;
      case CAD_CLEAR:		/* No action required		*/
		LOG_MSG(DEBUG2_MSG, "parkCad - CLEAR directive");
		break;

      case CAD_START:		/* Execute park command	*/
		LOG_MSG(DEBUG2_MSG, "parkCad - START directive");
	
		if (strcmp(pCad->d,DISABLED) == 0)
		{
			status = CAD_REJECT;
			sprintf(MESSAGE,"parkCad - Observe in progress");
			LOG_MSG(ERROR_MSG, MESSAGE);	
		}
		else
		{
			carVal = CAR_BUSY;
			/*valh is connected to car record*/
			*(long *)pCad->valh = carVal;
			
			/* release semaphore to parkCtrl function*/
			semGive (semPark);
		}
		break;

      case CAD_STOP:		   /* Really can't stop	       */
		LOG_MSG(DEBUG2_MSG, "parkCad - STOP directive");
		/* 	strcpy(MESSAGE, "Cannot stop"); */
		status = CAD_ACCEPT;
		break;

      default:		   /* Unknown directive		*/
		strcpy(MESSAGE, "Unrecongized directive");
		status = CAD_REJECT;
		break;
    }
   
    return status;
 
}

/*
 *+
 * FUNCTION NAME:
 * 	parkCtrl
 *
 * INVOCATION:
 *	Spawned from initTasks routine.
 *
 * PARAMETERS: (">" input, "!" modified, "<" output)
 *	 Set of 10 integer variables, most of which mean nothing, some of which
 *      	are interpreted in various ways.
 *
 * FUNCTION VALUE:
 *	int             Result of function...should never be seen.
 *
 * PURPOSE:
 *	perform a park.
 *
 * DESCRIPTION:
 *	Started by initTasks, when system boots up.  Waits on a semaphore
 *	which is released by the parkCad routine.  Tells low level software
 *	to perform a park.
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
int parkCtrl( int n1, int n2, int n3, int n4, int n5, int n6, int n7, int n8,
			int n9, int n10 )
{
	char err[80];
    struct cadRecord* pCad;
    long status = OK;

    motorMsg msg;

    /* Convert first parameter to CAD address	*/
    pCad = (struct cadRecord *) n1;

    /* Repeat as inifinite loop	*/
    while(1)
    {
		status = OK;
		LOG_MSG(DEBUG2_MSG, "Task tparkCtrl sleeping...\n");

		/* Wait for our semaphore   */
		semTake(semPark, WAIT_FOREVER);
		LOG_MSG(DEBUG2_MSG, "Task tparkCtrl awake..."); 
			msg.op = PARK;
		msg.pCad = pCad;
		/*send message to ctrl function*/
		if(getcoverCar() != CAR_BUSY)
		{
			setcoverCarBusy();
			if(msgQSend(coverMotorQ, (char *)&msg, sizeof(motorMsg),NO_WAIT, 
						MSG_PRI_NORMAL) != OK)
			{
				sprintf(MESSAGE,"coverDatmCad: msgQSend error in \n");
				LOG_MSG(ERROR_MSG,MESSAGE);
				status = CAD_REJECT;
			}
		}
		else
		{
			sprintf(MESSAGE,"coverDatmCad:  Cover is busy\n");
			LOG_MSG(ERROR_MSG,MESSAGE);
			status = CAD_REJECT;
		}
		
		
		/*send message to ctrl function*/
		if(getacqCar() != CAR_BUSY)
		{
			
			setacqCarBusy();
                        status = sendAcqPark();
		}
		else
		{
			sprintf(MESSAGE,"acqDatmCad:  Acq is busy\n");
			LOG_MSG(ERROR_MSG,MESSAGE);
			status = CAD_REJECT;
		}
		
		
		/*send message to ctrl function*/
		if(getcameraCar() != CAR_BUSY)
		{
			setcameraCarBusy();
			if(msgQSend(cameraMotorQ, (char *)&msg, sizeof(motorMsg),NO_WAIT, 
						MSG_PRI_NORMAL) != OK)
			{
				sprintf(MESSAGE,"cameraDatmCad: msgQSend error in \n");
				LOG_MSG(ERROR_MSG,MESSAGE);
				status = CAD_REJECT;
			}
		}
		else
		{
			sprintf(MESSAGE,"cameraDatmCad:  Camera is busy\n");
			LOG_MSG(ERROR_MSG,MESSAGE);
			status = CAD_REJECT;
		}
		
		/*send message to ctrl function*/
		if(getdeckerCar() != CAR_BUSY)
		{
			setdeckerCarBusy();
			if(msgQSend(deckerMotorQ, (char *)&msg, sizeof(motorMsg),NO_WAIT, 
						MSG_PRI_NORMAL) != OK)
			{
				sprintf(MESSAGE,"deckerDatmCad: msgQSend error in \n");
				LOG_MSG(ERROR_MSG,MESSAGE);
				status = CAD_REJECT;
			}
		}
		else
		{
			sprintf(MESSAGE,"deckerDatmCad:  Decker is busy\n");
			LOG_MSG(ERROR_MSG,MESSAGE);
			status = CAD_REJECT;
		}
		
		/*send message to ctrl function*/
		if(getfocusCar() != CAR_BUSY)
		{
			setfocusCarBusy();
			if(msgQSend(focusMotorQ, (char *)&msg, sizeof(motorMsg),NO_WAIT, 
						MSG_PRI_NORMAL) != OK)
			{
				sprintf(MESSAGE,"focusDatmCad: msgQSend error in \n");
				LOG_MSG(ERROR_MSG,MESSAGE);
				status = CAD_REJECT;
			}
		}
		else
		{
			sprintf(MESSAGE,"focusDatmCad:  Focus is busy\n");
			LOG_MSG(ERROR_MSG,MESSAGE);
			status = CAD_REJECT;
		}
		
		/*send message to ctrl function*/
		if(getfw1Car() != CAR_BUSY)
		{
			setfw1CarBusy();
			if(msgQSend(fw1MotorQ, (char *)&msg, sizeof(motorMsg),NO_WAIT, 
						MSG_PRI_NORMAL) != OK)
			{
				sprintf(MESSAGE,"fw1DatmCad: msgQSend error in \n");
				LOG_MSG(ERROR_MSG,MESSAGE);
				status = CAD_REJECT;
			}
		}
		else
		{
			sprintf(MESSAGE,"fw1DatmCad:  FW1 is busy\n");
			LOG_MSG(ERROR_MSG,MESSAGE);
			status = CAD_REJECT;
		}
		/*send message to ctrl function*/
		if(getfw2Car() != CAR_BUSY)
		{
			
			setfw2CarBusy();
			if(msgQSend(fw2MotorQ, (char *)&msg, sizeof(motorMsg),NO_WAIT, 
						MSG_PRI_NORMAL) != OK)
			{
				sprintf(MESSAGE,"fw2DatmCad: msgQSend error in \n");
				LOG_MSG(ERROR_MSG,MESSAGE);
				status = CAD_REJECT;
			}
		}
		else
		{
			sprintf(MESSAGE,"fw2DatmCad:  FW2 is busy\n");
			LOG_MSG(ERROR_MSG,MESSAGE);
			status = CAD_REJECT;
		}
		
		/*send message to ctrl function*/
		if(getgratingCar() != CAR_BUSY)
		{
			setgratingCarBusy();
			if(msgQSend(gratingMotorQ, (char *)&msg, sizeof(motorMsg),NO_WAIT, 
						MSG_PRI_NORMAL) != OK)
			{
				sprintf(MESSAGE,"gratingDatmCad: msgQSend error in \n");
				LOG_MSG(ERROR_MSG,MESSAGE);
				status = CAD_REJECT;
			}
		}
		else
		{
			sprintf(MESSAGE,"gratingDatmCad:  Grating is busy\n");
			LOG_MSG(ERROR_MSG,MESSAGE);
			status = CAD_REJECT;
		}
		
		/*send message to ctrl function*/
		if(getslitCar() != CAR_BUSY)
		{
			setslitCarBusy();
			if(msgQSend(slitMotorQ, (char *)&msg, sizeof(motorMsg),NO_WAIT, 
						MSG_PRI_NORMAL) != OK)
			{
				sprintf(MESSAGE,"slitDatmCad: msgQSend error in \n");
				LOG_MSG(ERROR_MSG,MESSAGE);
				status = CAD_REJECT;
			}
		}
		else
		{
			sprintf(MESSAGE,"slitDatmCad:  Slit is busy\n");
			LOG_MSG(ERROR_MSG,MESSAGE);
			status = CAD_REJECT;
		}
		
		/*send message to ctrl function*/
		if(getxdispCar() != CAR_BUSY)
		{
			setxdispCarBusy();
			if(msgQSend(xdispMotorQ, (char *)&msg, sizeof(motorMsg),NO_WAIT, 
						MSG_PRI_NORMAL) != OK)
			{
				sprintf(MESSAGE,"xdispDatmCad: msgQSend error in \n");
				LOG_MSG(ERROR_MSG,MESSAGE);
				
			}
		}
		else
		{
			sprintf(MESSAGE,"xdispDatmCad:  Xdisp is busy\n");
			LOG_MSG(ERROR_MSG,MESSAGE);
			status = CAD_REJECT;
		}
		/*set car back to idle*/
		carVal = CAR_IDLE;
		if(setCar(PARK_CAR,carVal,OK,"",err) != OK)
		{
		 	LOG_MSG(ERROR_MSG,err);
		} 
		
	}
    
    return status;
}
