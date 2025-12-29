static struct {
	void *v;
	char *c;
} rcsid = {
	&rcsid,
	"$Id: datumCad.c,v 1.2 2013/06/06 01:54:27 gemvx Exp $"
};

/*
 * Copyright 1997 Association of Universities for Research in Astronomy, Inc.
 * See the file COPYRIGHT for more details.
 *
 * FILENAME
 * 	datum.c
 *
 * DESCRIPTION 
 * 	Implements Gemini "datum" command.  Connections to all the mechanisms are in
 *  the capfast file where the individual mechanism datum commands do the work.
 *
 * 
 * FUNCTION NAME(S)
 *	datumCad
 *
 *   
 * DEPENDENCIES
 * 	EPICS support libraries
 *
 *
 *INDENT-OFF*
 * $Log: datumCad.c,v $
 * Revision 1.2  2013/06/06 01:54:27  gemvx
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
 * Revision 1.1  2009/06/10 15:05:10  gemvx
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
extern long sendAcqDatum();

/*
 *+
 * FUNCTION NAME:
 *	datumCad
 *
 * INVOCATION:
 *	struct cadRecord *pCad;
 * 	status = datumCad( pCad );
 *
 * PARAMETERS: (">" input, "!" modified, "<" output)
 * 	> pCad   (struct cadRecord *)   pointer to CAD data structure
 *
 * FUNCTION VALUE:
 * 	long  Status value written to CAD VAL field
 *
 * PURPOSE:
 * 	User defined function for "datum" CAD record
 *
 *
 *
 * DESCRIPTION:
 *	This function is triggered by the datum cad record.  When start
 * 	is received, it releases a semaphore to allow the datumCtrl routine
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


int datumCad(struct cadRecord *pCad)

{
    long status = CAD_ACCEPT;

    /* Action to be taken depends on directive received		*/
    switch (DIRECTIVE)
    {
      case CAD_MARK:	       	/* No action required		*/
		LOG_MSG(DEBUG2_MSG, "datumCad - MARK directive");
		break;

      case CAD_PRESET:
		LOG_MSG(DEBUG2_MSG, "datumCad - PRESET directive");
	/* send datum command to lower level code,
		 mechanism doesn't move until a start directive*/
		if(datumMech(ACQ)!= VME_OK)
		{
			sprintf(MESSAGE,"datumCad: acqILLEGAL motor\n");
			status = CAD_REJECT;
		}
		if(datumMech(COVER)!= VME_OK)
		{
			sprintf(MESSAGE,"datumCad: coverILLEGAL motor\n");
			status = CAD_REJECT;
		}
		if(datumMech(FW1)!= VME_OK)
		{
			sprintf(MESSAGE,"datumCad: fw1ILLEGAL motor\n");
			status = CAD_REJECT;
		}
		if(datumMech(FW2)!= VME_OK)
		{
			sprintf(MESSAGE,"datumCad: fw2 ILLEGAL motor\n");
			status = CAD_REJECT;
		}
		if(datumMech(SLIT)!= VME_OK)
		{
			sprintf(MESSAGE,"datumCad: slit ILLEGAL motor\n");
			status = CAD_REJECT;
		}
		if(datumMech(DECKER)!= VME_OK)
		{
			sprintf(MESSAGE,"datumCad: decker ILLEGAL motor\n");
			status = CAD_REJECT;
		}
		if(datumMech(GRATING)!= VME_OK)
		{
			sprintf(MESSAGE,"datumCad: grating ILLEGAL motor\n");
			status = CAD_REJECT;
		}
		if(datumMech(CAMERA)!= VME_OK)
		{
			sprintf(MESSAGE,"datumCad: camera ILLEGAL motor\n");
			status = CAD_REJECT;
		}
		if(datumMech(FOCUS)!= VME_OK)
		{
			sprintf(MESSAGE,"datmCad: focus ILLEGAL motor\n");
			status = CAD_REJECT;
		}
		if(datumMech(XDISP)!= VME_OK)
		{
			sprintf(MESSAGE,"datumCad: xdisp ILLEGAL motor\n");
			status = CAD_REJECT;
		}
		break;
      case CAD_CLEAR:		/* No action required		*/
		LOG_MSG(DEBUG2_MSG, "datumCad - CLEAR directive");
		break;

      case CAD_START:		/* Execute datum command	*/
		LOG_MSG(DEBUG2_MSG, "datumCad - START directive");
		printf("datumCad disabled = %s\n",pCad->d);
		if (strcmp(pCad->d,DISABLED) == 0)
		{
			status = CAD_REJECT;
			sprintf(MESSAGE,"datumCad -  Observe in progress");
			LOG_MSG(ERROR_MSG, MESSAGE);    
		}
		else
		{
			carVal = CAR_BUSY;
			/*valh is connected to car record*/
			*(long *)pCad->valh = carVal;
			/* set structure values for ctrl function*/
			
			
			
			/* release semaphore to datumCtrl function*/
			semGive (semDatm);
		}
		break;

      case CAD_STOP:		   /* Really can't stop	       */
		LOG_MSG(DEBUG2_MSG, "datumCad - STOP directive");
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
 * 	datumCtrl
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
 *	perform a datum.
 *
 * DESCRIPTION:
 *	Started by initTasks, when system boots up.  Waits on a semaphore
 *	which is released by the datumCad routine.  Tells low level software
 *	to perform a datum.
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
int datumCtrl( int n1, int n2, int n3, int n4, int n5, int n6, int n7, int n8,
			   int n9, int n10 )
{
    char err[80];
    motorMsg msg;
    struct cadRecord* pCad;
    long status = OK;

    /* Convert first parameter to CAD address	*/
    pCad = (struct cadRecord *) n1;

    /* Repeat as inifinite loop	*/
    while(1)
    {
		status = OK;
		LOG_MSG(DEBUG2_MSG, "Task tdatumCtrl sleeping...\n");

		/* Wait for our semaphore   */
		semTake(semDatm, WAIT_FOREVER);
		LOG_MSG(DEBUG2_MSG, "Task tdatumCtrl awake...");
 
		msg.op = DATUM;
		msg.pCad = pCad;

		/*send message to ctrl function*/
		if(getcoverCar() != CAR_BUSY)
		{
			LOG_MSG(DEBUG2_MSG, "datumCad: cover Car is not busy");
			setcoverCarBusy();
			printf("set cover to busy\n");
			if(msgQSend(coverMotorQ, (char *)&msg, sizeof(motorMsg),NO_WAIT, MSG_PRI_NORMAL) != OK)
			{
				sprintf(MESSAGE,"coverDatmCad: msgQSend error in \n");
				LOG_MSG(ERROR_MSG,MESSAGE);
				status = CAD_REJECT;
			}
			printf("cover task started\n");
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
			status = sendAcqDatum();
		}
		else
		{
			sprintf(MESSAGE,"AcqDatmCad:  Acq is busy\n");
			LOG_MSG(ERROR_MSG,MESSAGE);
			status = CAD_REJECT;
		}
		
		
		/*send message to ctrl function*/
		if(getcameraCar() != CAR_BUSY)
		{
			setcameraCarBusy();
			if(msgQSend(cameraMotorQ, (char *)&msg, sizeof(motorMsg),NO_WAIT, MSG_PRI_NORMAL) != OK)
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
				sprintf(MESSAGE,"fw2atmCad: msgQSend error in \n");
				LOG_MSG(ERROR_MSG,MESSAGE);
				status = CAD_REJECT;
			}
		}
		else
		{
			sprintf(MESSAGE,"fw2atmCad:  FW2 is busy\n");
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
	
		/* set car back to idle*/
		printf("\n\nset car back to idle\n\n");
		carVal = CAR_IDLE;
		if(setCar(DATUM_CAR,carVal,OK,"",err) != OK)
		{
			LOG_MSG(ERROR_MSG,err);
		}

    }
    return status;
}
