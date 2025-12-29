static struct {
    void *v;
    char *c;
} rcsid = {
    &rcsid,
    "$Id: cameraCad.c,v 1.2 2013/06/06 01:54:27 gemvx Exp $"
};
/*
 * Copyright 1997 Association of Universities for Research in 
 *	Astronomy, Inc.
 * See the file COPYRIGHT for more details.
 *
 * FILENAME
 * 	cameraCad.c
 *
 * DESCRIPTION 
 * 	cameraCad  support routines
 *
 * 
 * FUNCTION NAME(S)
 *	cameraDatmCad
 *	cameraParkCad
 *	cameraStepsCad
 *	cameraPosCad
 *	cameraCtrl
 *   
 * DEPENDENCIES
 * 	EPICS support libraries
 *
 *
 *INDENT-OFF*
 * $Log: cameraCad.c,v $
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
 * Revision 1.1  2009/06/10 15:05:09  gemvx
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
#define POS_LEN 5

/* Include file needed for control tasks */
#include <sysLib.h>
#include <car.h>

/*Global variables*/
MSG_Q_ID cameraMotorQ;
int cameraId;

/* this value will have the current state of the car*/
static carVal = CAR_IDLE;

/* Forward declarations of the functions in this file */
long cameraDatmCad( struct cadRecord* pCad);

long cameraParkCad( struct cadRecord* pCad);

long cameraStepsCad( struct cadRecord* pCad);

long cameraPosCad( struct cadRecord* pCad);

int cameraCtrl( int n1, int n2, int n3, int n4, int n5, int n6, 
			 int n7, int n8, int n9, int n10 );

#ifndef FINDMECH
int findMechName(int mech, char *name, char *pos);
#endif
/*
 *+
 * FUNCTION NAME:    camera????CadInit
 *
 * INVOCATION:
 *      cad init routine
 *
 * PARAMETERS: (">" input, "!" modified, "<" output)
 *	   ! struct cadRecord *pCad
 *
 * FUNCTION VALUE:
 *    status val
 *
 * PURPOSE:
 *    create message queue and spawn task to deal with camera cad records
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
 *
 *-
 */
long cameraDatmCadInit( struct cadRecord* pCad )
{
    long status = CAD_ACCEPT;
    static int done = 0;

    LOG_MSG(DEBUG2_MSG,"cameraDatmCadInit:***********");
    /* park, datum, pos, and steps cads will call this, only do once*/
    if (!done)
    {
		/*create msg q*/
		LOG_MSG(DEBUG2_MSG,"create Msg Queue\n");
		cameraMotorQ = msgQCreate(4,sizeof(motorMsg),MSG_Q_FIFO);
		if(cameraMotorQ == NULL)
		{
			status = CAD_REJECT;
			strncpy(MESSAGE,"Init: Error creating messageQ",MAX_STRING_SIZE - 1);
		}
		/*spawn task*/
		LOG_MSG(DEBUG2_MSG,"spawn task\n");
		if(status == CAD_ACCEPT)
			cameraId = taskSpawn("tCameraCtrl",50,VX_FP_TASK,4000,cameraCtrl, 0,0,0,0,0,0,0,0,0,0);
		if(cameraId == ERROR)
		{
			status = CAD_REJECT;
			strncpy(MESSAGE,"Init: Error Spawning task",MAX_STRING_SIZE - 1);
		}
		if(status = CAD_ACCEPT)
			done = 1;
    }
    
    /*valh is connected to car record*/
	LOG_MSG(DEBUG2_MSG,"***********cameraDatmCadInit\n");
    return status;
}
long cameraParkCadInit( struct cadRecord* pCad )
{
    
    long status = CAD_ACCEPT;
	LOG_MSG(DEBUG2_MSG,"cameraParkCadInit:***********");
    
	LOG_MSG(DEBUG2_MSG,"***********cameraParkCadInit\n");
    return status;
}
long cameraPosCadInit( struct cadRecord* pCad )
{
    long status = CAD_ACCEPT;

	LOG_MSG(DEBUG2_MSG,"***********cameraPosCadInit\n");
    return status;
}
long cameraStepsCadInit( struct cadRecord* pCad )
{
   
    long status = CAD_ACCEPT;
	LOG_MSG(DEBUG2_MSG,"cameraStepsCadInit:***********");
	LOG_MSG(DEBUG2_MSG,"***********cameraStepsCadInit\n");
    return status;
}
/*
 *+
 * FUNCTION NAME:
 *	cameraCtrl
 *
 * INVOCATION:
 *	Started as seperate task in initTasks.
 *
 * PARAMETERS: (">" input, "!" modified, "<" output)
 *	Set of 10 integer variables, interpreted as variety of 
 *	parameters.
 *
 * FUNCTION VALUE:
 *	never returns
 *
 * PURPOSE:
 *
 * DESCRIPTION:
 *	Cad record releases semaphore which allows cameraCtrl to run.  
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

int cameraCtrl( int n1, int n2, int n3, int n4, int n5, int n6, 
			 int n7, int n8, int n9, int n10 )
{
    struct cadRecord* pCad;
    long status = VME_OK;
    char rMsg[MAX_STRING_SIZE];
    char dummy[MAX_STRING_SIZE];
    motorMsg msg;
   

  
    strncpy (rMsg,"Error in cameraCtrl\n",MAX_STRING_SIZE -1);
    /* Repeat as inifinite loop				*/
    while(1)
    {
		LOG_MSG(DEBUG2_MSG, "Task tcameraCtrl sleeping...\n");
	
		if( msgQReceive( cameraMotorQ, (char *)&msg, sizeof(motorMsg ), 
						 WAIT_FOREVER ) == ERROR ) 
		{
			sprintf(rMsg,"error in cameraCtrl msgReceive\n");
			LOG_MSG(ERROR_MSG,rMsg);
			carVal = CAR_ERROR;
			if(setCar(CAMERA_CAR,carVal,ERROR,rMsg,dummy) != OK)
				LOG_MSG(ERROR_MSG,dummy);
			continue;
		}

		LOG_MSG(DEBUG2_MSG,"cameraCtrl:***********");
		pCad = msg.pCad;
	   
		LOG_MSG(DEBUG2_MSG,"cameraCtrl: SIM_NONE\n");
		  
		status = moveMech(CAMERA,rMsg);
		
		if(status == VME_OK)
		{
			carVal = CAR_IDLE;
			if(setCar(CAMERA_CAR,carVal,OK,"",dummy) != OK)
				LOG_MSG(ERROR_MSG,dummy);
		}
		else
		{
			carVal = CAR_ERROR;
			if(setCar(CAMERA_CAR,carVal,status,rMsg,dummy) != OK)
				LOG_MSG(ERROR_MSG,dummy);
			
		}

		LOG_MSG(DEBUG2_MSG,"***********cameraCtrl\n");

    }
    
    return status;
}
/*
 *+
 * FUNCTION NAME:
 *	cameraDatmCad
 *
 * INVOCATION:
 *	struct cadRecord* pCad;
 *	long status = 0;
 *	status = cameraDatmCad( pCad );
 *
 * PARAMETERS: (">" input, "!" modified, "<" output)
 *	> pCad		(struct cadRecord*)	pointer to CAD record
 *      > pCad->valf       datumed sad record
 *      > pCad->valg       initCad.VALA  (epics sim mode)
 *
 * FUNCTION VALUE:
 *	long		status value written to CAD VAL field
 *
 * PURPOSE:
 *	Supports Gemini "cameraDatm" command, via CAD record
 *
 * DESCRIPTION:
 *	Inovked when cameraDatmCad record is processed by EPICS.  It 
 *	implements the "datm" command, which initializes the mechanism before use.
 *
 * EXTERNAL VARIABLES:
 *	None
 *
 * PRIOR REQUIREMENTS:
 *	CAD record should be initialized.
 *
 * DEFICIENCIES:
 *	
 *
 * HISTORY (optional):
 *	  
 *
 *-
 */

long cameraDatmCad( struct cadRecord* pCad )
{
    motorMsg msg;
    long status = CAD_ACCEPT;

	LOG_MSG(DEBUG2_MSG,"cameraDatmCad:***********");
    /* Take appropriate action, based on directive type*/
    switch(DIRECTIVE )
    {
      case CAD_MARK:
		LOG_MSG(DEBUG2_MSG, 
				"cameraDatmCad - MARK directive");
	    strcpy(MESSAGE,"");
		break;

      case CAD_PRESET:	
		LOG_MSG(DEBUG2_MSG, 
				"cameraDatmCad - PRESET directive");

		if(datumMech(CAMERA)!= VME_OK)
		{
			sprintf(MESSAGE,"cameraDatmCad: ILLEGAL motor\n");
			status = CAD_REJECT;
		}
		break;

      case CAD_CLEAR:	
		LOG_MSG(DEBUG2_MSG, 
				"cameraDatmCad - CLEAR directive");
		/*clear cad msg field*/
		strcpy(MESSAGE,"");
		break;

      case CAD_START:	
		LOG_MSG(DEBUG2_MSG, 
				"cameraDatmCad - START directive");

		if (strcmp(pCad->h,DISABLED) == 0)
		{
			status = CAD_REJECT;
			sprintf(MESSAGE,"cameraPosCad - Camera Observe in progress");
			LOG_MSG(ERROR_MSG, MESSAGE);    
		}
		else
		{
			if(carVal != CAR_BUSY)
			{
				/*set car to busy (ctrl task will set back to idle)*/
				carVal = CAR_BUSY;	
				
				/* set structure values for ctrl function*/
				msg.op = DATUM;
				msg.pCad = pCad;
				
				/*send message to ctrl function*/
				if(msgQSend(cameraMotorQ, (char *)&msg, sizeof(motorMsg),NO_WAIT, 
							MSG_PRI_NORMAL) != OK)
				{
					sprintf(MESSAGE,"msgQSend error in cameraDatmCad\n");
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
		}
		break;

      case CAD_STOP:   
		LOG_MSG(DEBUG2_MSG,
				"cameraDatmCad - STOP directive");
		/*check if car is busy and then call  abortMotor*/
		if(carVal == CAR_BUSY)
		{
			abortMotor(CAMERA);
		}
		else
		{
			sprintf(MESSAGE,"cameraDatmCad:  Camera is not busy\n");
			LOG_MSG(ERROR_MSG,MESSAGE);
			status = CAD_REJECT;	
		}
	
		break;

      default:	/* Unknown directive	*/
		strncpy(MESSAGE, "cameraDatmCad: Unrecognized directive",MAX_STRING_SIZE - 1);
		LOG_MSG(ERROR_MSG,MESSAGE);
		status = CAD_REJECT;
		break;
    } 
   
	LOG_MSG(DEBUG2_MSG,"***********cameraDatmCad\n");
    return status;
}

/*
 *+
 * FUNCTION NAME:
 *	cameraParkCad
 *
 * INVOCATION:
 *	struct cadRecord* pCad;
 *	long status = 0;
 *	status = cameraParkCad( pCad );
 *
 * PARAMETERS: (">" input, "!" modified, "<" output)
 *	> pCad		(struct cadRecord*)	pointer to CAD record
 *      > pCad->valf       datumed sad record
 *      > pCad->valg       initCad.VALA  (epics sim mode)
 *
 * FUNCTION VALUE:
 *	long		status value written to CAD VAL field
 *
 * PURPOSE:
 *	Supports Gemini "cameraPark" command, via CAD record
 *
 * DESCRIPTION:
 *	Inovked when cameraParkCad record is processed by EPICS.  It 
 *	implements the "park" command, which places the system in a 
 *	safe shutdown status.  
 *
 * EXTERNAL VARIABLES:
 *	None
 *
 * PRIOR REQUIREMENTS:
 *	CAD record should be initialized.
 *
 * DEFICIENCIES:
 *	
 *
 * HISTORY (optional):
 *	  
 *
 *-
 */
long cameraParkCad( struct cadRecord* pCad)
{
   
    motorMsg msg;
    long status = CAD_ACCEPT;

    LOG_MSG(DEBUG2_MSG,"cameraParkCad:***********");
    /* Take appropriate action, based on directive type*/
    switch(DIRECTIVE )
    {
      case CAD_MARK:   
		LOG_MSG(DEBUG2_MSG,"cameraParkCad - MARK directive");

		break;

      case CAD_PRESET:	
		LOG_MSG(DEBUG2_MSG,"cameraParkCad - PRESET directive");
		/* check datumed before trying to move motors*/
		if(!atoi(pCad->f))
		{
			status = CAD_REJECT;
			sprintf(MESSAGE,"camera mechanism not datumed\n");
			LOG_MSG(ERROR_MSG, MESSAGE);
		}
		else
			if(parkMech(CAMERA)!= VME_OK)
			{
				sprintf(MESSAGE,"Illegal motor\n");
				LOG_MSG(ERROR_MSG, MESSAGE);
				status = CAD_REJECT;
			}
		break;

      case CAD_CLEAR:	
		LOG_MSG(DEBUG2_MSG, "cameraParkCad - CLEAR directive");
		/*clear cad msg field*/
		strcpy(MESSAGE,"");
		break;

      case CAD_START:	
		LOG_MSG(DEBUG2_MSG,"cameraParkCad - START directive");

		if (strcmp(pCad->h,DISABLED) == 0)
		{
			status = CAD_REJECT;
			sprintf(MESSAGE,"cameraPosCad - Camera Observe in progress");
			LOG_MSG(ERROR_MSG, MESSAGE);    
		}
		else
		{
			/*set car to busy (ctrl task will set back to idle)*/
			carVal = CAR_BUSY;
			
			LOG_MSG(DEBUG2_MSG,"Waking park\n");
			msg.op = PARK;
			msg.pCad = pCad;
			if(msgQSend(cameraMotorQ, (char *)&msg, sizeof(motorMsg),NO_WAIT, 
						MSG_PRI_NORMAL) != OK)
			{
				status = CAD_REJECT;
				sprintf(MESSAGE,"msgQSend error in cameraDatmCad\n");
				LOG_MSG(ERROR_MSG,MESSAGE);
			}	
		}
		break;

      case CAD_STOP:	
		LOG_MSG(DEBUG2_MSG,"cameraParkCad - STOP directive");
		/*call stop command*/
	if(carVal == CAR_BUSY)
		{
			abortMotor(CAMERA);
		}
		else
		{
			sprintf(MESSAGE,"cameraParkCad:  Camera is not busy\n");
			LOG_MSG(ERROR_MSG,MESSAGE);
			status = CAD_REJECT;	
		}
		status = CAD_ACCEPT;
		break;

      default:	/* Unknown directive	*/
		strncpy(MESSAGE, "cameraParkCad: Unrecognized directive",MAX_STRING_SIZE - 1);
		LOG_MSG(ERROR_MSG,MESSAGE);
		status = CAD_REJECT;
		break;
    } 
   
	LOG_MSG(DEBUG2_MSG,"***********cameraParkCad\n");
    return status;
}

/*
 *+
 * FUNCTION NAME:
 *	cameraStepsCad
 *
 * INVOCATION:
 *	struct cadRecord* pCad;
 *	long status = 0;
 *	status = cameraStepsCad( pCad );
 *
 * PARAMETERS: (">" input, "!" modified, "<" output)
 *	> pCad		(struct cadRecord*)	pointer to CAD record
 *      > pCad->vala       desired position
 *      > pCad->valf       datumed sad record
 *      > pCad->valg       initCad.VALA  (epics sim mode)
 *
 * FUNCTION VALUE:
 *	long		status value written to CAD VAL field
 *
 * PURPOSE:
 *	Supports Gemini "cameraSteps" command, via CAD record
 *
 * DESCRIPTION:
 *	Inovked when cameraStepsCad record is processed by EPICS.  It 
 *	implements the "steps" command, which moves the mechanism to
 *  an absolute step position.  
 *
 * EXTERNAL VARIABLES:
 *	None
 *
 * PRIOR REQUIREMENTS:
 *	CAD record should be initialized.
 *
 * DEFICIENCIES:
 *	
 *
 * HISTORY (optional):
 *	  
 *
 *-
 */
long cameraStepsCad( struct cadRecord* pCad )
{
   
    motorMsg msg;
    long status = CAD_ACCEPT;
  

	LOG_MSG(DEBUG2_MSG,"cameraStepsCad:***********");
    /* Take appropriate action, based on directive type*/
    switch(DIRECTIVE )
    {
      case CAD_MARK:
		LOG_MSG(DEBUG2_MSG,"cameraStepsCad - MARK directive");
		break;

      case CAD_PRESET:	
		LOG_MSG(DEBUG2_MSG,"cameraStepsCad - PRESET directive");

		/* check datumed before*/
		if(!atoi(pCad->f))
		{
			status = CAD_REJECT;
      	    strncpy(MESSAGE,"cameraStepsCad - Camera not Datumed",39);
			LOG_MSG(ERROR_MSG, MESSAGE);
		}
		else
		{
			if(setEngPos(CAMERA,atoi(cadInput(STEPSCAD)))!= VME_OK)
			{
				strcpy(MESSAGE,gnirsErrorMessage);
				status = CAD_REJECT;
			}
		}
		break;
      case CAD_CLEAR:
		LOG_MSG(DEBUG2_MSG,"cameraStepsCad - CLEAR directive");
		/*clear cad msg field*/
	    strcpy(MESSAGE,"");
		break;

      case CAD_START:	
		LOG_MSG(DEBUG2_MSG,"cameraStepsCad - START directive");
	
		if (strcmp(pCad->h,DISABLED) == 0)
		{
			status = CAD_REJECT;
			sprintf(MESSAGE,"cameraPosCad - Camera Observe in progress");
			LOG_MSG(ERROR_MSG, MESSAGE);    
		}
		else
		{
			/*copy inputs to outputs*/
			
			*(long *)cadOutput(STEPSCAD) = atoi(cadInput(STEPSCAD));
			
			/*set car to busy (ctrl task will set back to idle)*/
			carVal = CAR_BUSY;
			
			
			DPRINT(DPdebug,DEBUG2_MSG,"Waking steps\n");
			msg.op = STEPS;
			msg.pCad = pCad;
			
			if(msgQSend(cameraMotorQ, (char *)&msg, sizeof(motorMsg),NO_WAIT, 
						MSG_PRI_NORMAL) != OK)
			{
				sprintf(MESSAGE,"msgQSend error in cameraStepsCad\n");
				LOG_MSG(ERROR_MSG,MESSAGE);
				status = CAD_REJECT;
			}
		}
	
		break;

      case CAD_STOP:
		LOG_MSG(DEBUG2_MSG,"cameraStepsCad - STOP directive");
			if(carVal == CAR_BUSY)
		{
			abortMotor(CAMERA);
		}
		else
		{
			sprintf(MESSAGE,"cameraStepsCad:  Camera is not busy\n");
			LOG_MSG(ERROR_MSG,MESSAGE);
			status = CAD_REJECT;	
		}
		status = CAD_ACCEPT;
		break;

      default:	/* Unknown directive	*/
		strncpy(MESSAGE, "cameraStepsCad: Unrecognized directive",MAX_STRING_SIZE - 1);
		status = CAD_REJECT;
		break;
    } 
   
	LOG_MSG(DEBUG2_MSG,"***********cameraStepsCad\n");
    return status;
}

/*
 *+
 * FUNCTION NAME:
 *	cameraPosCad
 *
 * INVOCATION:
 *	struct cadRecord* pCad;
 *	long status = 0;
 *	status = cameraPosCad( pCad );
 *
 * PARAMETERS: (">" input, "!" modified, "<" output)
 *	> pCad		(struct cadRecord*)	pointer to CAD record
 *        pCad->a  = position
 *      > pCad->valf       datumed sad record
 *      > pCad->valg       initCad.VALA  (epics sim mode)
 *
 * FUNCTION VALUE:
 *	long		status value written to CAD VAL field
 *
 * PURPOSE:
 *	Supports Gemini "cameraPos" command, via CAD record
 *
 * DESCRIPTION:
 *	Inovked when cameraPosCad record is processed by EPICS.  It 
 *	implements the "pos" command, which moves the mechanism to the
 *  named position.  
 *
 * EXTERNAL VARIABLES:
 *	None
 *
 * PRIOR REQUIREMENTS:
 *	CAD record should be initialized.
 *
 * DEFICIENCIES:
 *	
 *
 * HISTORY (optional):
 *	  
 *
 *-
 */
long cameraPosCad( struct cadRecord* pCad )
{
   
	char pos[POS_LEN];
    motorMsg msg;
    long status = CAD_ACCEPT;

	LOG_MSG(DEBUG2_MSG,"cameraPosCad:***********");
    /* Take appropriate action, based on directive type*/
    switch(DIRECTIVE )
    {
      case CAD_MARK:
		LOG_MSG(DEBUG2_MSG,"cameraPosCad - MARK directive");
		break;

      case CAD_PRESET:	
		LOG_MSG(DEBUG2_MSG,"cameraPosCad - PRESET directive");
		if(!atoi(pCad->f))
		{
			status = CAD_REJECT;
      	    sprintf(MESSAGE,"cameraPosCad - Camera not Datumed");
			LOG_MSG(ERROR_MSG, MESSAGE);
		}
		else
		{
			printf("name = %s\n",pCad->b);
#ifndef FINDMECH
			if(findMechName(CAMERA,pCad->b,pos))
			{
#endif
				if(valid(CAMERA,pCad->b)!= VME_OK)
				{
					sprintf(MESSAGE,gnirsErrorMessage);
					status = CAD_REJECT;
				}
#ifndef FINDMECH
			}
			else
			{
				sprintf(MESSAGE,"%s not found in database for camera\n",pCad->b);
				status = CAD_REJECT;
			}
#endif
		}
		break;

      case CAD_CLEAR:
		LOG_MSG(DEBUG2_MSG, 
				"cameraPosCad - CLEAR directive");
	    strcpy(MESSAGE,"");
		break;

      case CAD_START:
		LOG_MSG(DEBUG2_MSG,"cameraPosCad - START directive");
		if (strcmp(pCad->h,DISABLED) == 0)
		{
			status = CAD_REJECT;
			sprintf(MESSAGE,"cameraPosCad - Camera Observe in progress");
			LOG_MSG(ERROR_MSG, MESSAGE);    
		}
		else
		{
			/*copy inputs to outputs*/
			status = assignVal(type(POSCAD), cadInput(POSCAD), cadOutput(POSCAD), MESSAGE);
			
			/*set car to busy (ctrl task will set back to idle)*/	  
			carVal = CAR_BUSY; 
			
			/*send message to ctrl task*/
			DPRINT(DPdebug,DEBUG2_MSG,"Waking cameraCtrl\n");
			msg.op = POS;
			msg.pCad = pCad;	
			if(msgQSend(cameraMotorQ, (char *)&msg, sizeof(motorMsg),NO_WAIT, 
						MSG_PRI_NORMAL) != OK)
			{
				sprintf(MESSAGE,"cameraPosCad: Error in msgQSend ");
				status = CAD_REJECT;
			}	
		}
		break;

      case CAD_STOP:	
		LOG_MSG(DEBUG2_MSG,
				"cameraPosCad - STOP directive");
			if(carVal == CAR_BUSY)
		{
			abortMotor(CAMERA);
		}
		else
		{
			sprintf(MESSAGE,"cameraPosCad:  Camera is not busy\n");
			LOG_MSG(ERROR_MSG,MESSAGE);
			status = CAD_REJECT;	
		}
		status = CAD_ACCEPT;
		break;

      default:	/* Unknown directive	*/
		strncpy(MESSAGE, "cameraPosCad: Unrecognized directive",
				MAX_STRING_SIZE - 1);
		status = CAD_REJECT;
		break;
    } 
   
	LOG_MSG(DEBUG2_MSG,"***********cameraPosCad\n");
    return status;
}

long setcameraCarBusy()
{
	carVal = CAR_BUSY;
	if(setCar(CAMERA_CAR,carVal,OK,"",dummy) != OK)
	{
		LOG_MSG(ERROR_MSG,dummy);
		return ERROR;
	}
	return OK;
}
long getcameraCar()
{
	return carVal;
}
