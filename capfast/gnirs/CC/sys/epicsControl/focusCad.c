static struct {
    void *v;
    char *c;
} rcsid = {
    &rcsid,
    "$Id: focusCad.c,v 1.2 2013/06/06 01:54:28 gemvx Exp $"
};
/*
 * Copyright 1997 Association of Universities for Research in 
 *	Astronomy, Inc.
 * See the file COPYRIGHT for more details.
 *
 * FILENAME
 * 	focusCad.c
 *
 * DESCRIPTION 
 * 	focusCad  support routines
 *
 * 
 * FUNCTION NAME(S)
 *	focusDatmCad
 *	focusParkCad
 *	focusStepsCad
 *	focusPosCad
 *	focusCtrl
 *   
 * DEPENDENCIES
 * 	EPICS support libraries
 *
 *
 *INDENT-OFF*
 * $Log: focusCad.c,v $
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
 * Revision 1.1  2009/06/10 15:05:11  gemvx
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
#include "mechNames.h"

/*Global variables*/
MSG_Q_ID focusMotorQ;
int focusId;

/* this value will have the current state of the car*/
static carVal = CAR_IDLE;

/* Forward declarations of the functions in this file */
long focusDatmCad( struct cadRecord* pCad);

long focusParkCad( struct cadRecord* pCad);

long focusStepsCad( struct cadRecord* pCad);

long focusPosCad( struct cadRecord* pCad);

int focusCtrl( int n1, int n2, int n3, int n4, int n5, int n6, 
			 int n7, int n8, int n9, int n10 );
#ifndef FINDMECH
int findMechName(int mech, char *name, char *pos);
#endif

/*
 *+
 * FUNCTION NAME:    focus????CadInit
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
 *    create message queue and spawn task to deal with focus cad records
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
long focusDatmCadInit( struct cadRecord* pCad )
{
    long status = CAD_ACCEPT;
    static int done = 0;

    LOG_MSG(DEBUG2_MSG,"focusDatmCadInit:***********");
    /* park, datum, pos, and steps cads will call this, only do once*/
    if (!done)
    {
		/*create msg q*/
		LOG_MSG(DEBUG2_MSG,"create Msg Queue\n");
		focusMotorQ = msgQCreate(4,sizeof(motorMsg),MSG_Q_FIFO);
		if(focusMotorQ == NULL)
		{
			status = CAD_REJECT;
			strncpy(MESSAGE,"Init: Error creating messageQ",MAX_STRING_SIZE - 1);
		}
		/*spawn task*/
		LOG_MSG(DEBUG2_MSG,"spawn task\n");
		if(status == CAD_ACCEPT)
			focusId = taskSpawn("tFocusCtrl",50,VX_FP_TASK,4000,focusCtrl, 0,0,0,0,0,0,0,0,0,0);
		if(focusId == ERROR)
		{
			status = CAD_REJECT;
			strncpy(MESSAGE,"Init: Error Spawning task",MAX_STRING_SIZE - 1);
		}
		if(status = CAD_ACCEPT)
			done = 1;
    }
    
	LOG_MSG(DEBUG2_MSG,"***********focusDatmCadInit\n");
    return status;
}
long focusParkCadInit( struct cadRecord* pCad )
{
    
    long status = CAD_ACCEPT;
	LOG_MSG(DEBUG2_MSG,"focusParkCadInit:***********");
    
	LOG_MSG(DEBUG2_MSG,"***********focusParkCadInit\n");
    return status;
}
long focusPosCadInit( struct cadRecord* pCad )
{
    long status = CAD_ACCEPT;

	LOG_MSG(DEBUG2_MSG,"***********focusPosCadInit\n");
    return status;
}
long focusStepsCadInit( struct cadRecord* pCad )
{
   
    long status = CAD_ACCEPT;
	LOG_MSG(DEBUG2_MSG,"focusStepsCadInit:***********");
	LOG_MSG(DEBUG2_MSG,"***********focusStepsCadInit\n");
    return status;
}
/*
 *+
 * FUNCTION NAME:
 *	focusCtrl
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
 *	Cad record releases semaphore which allows focusCtrl to run.  
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

int focusCtrl( int n1, int n2, int n3, int n4, int n5, int n6, 
			 int n7, int n8, int n9, int n10 )
{
    struct cadRecord* pCad;
    long status = VME_OK;
    char rMsg[MAX_STRING_SIZE];
    char dummy[MAX_STRING_SIZE];
    motorMsg msg;
   
   

  
    strncpy (rMsg,"Error in focusCtrl\n",MAX_STRING_SIZE -1);
    /* Repeat as inifinite loop				*/
    while(1)
    {
		LOG_MSG(DEBUG2_MSG, "Task tfocusCtrl sleeping...\n");
	
		if( msgQReceive( focusMotorQ, (char *)&msg, sizeof(motorMsg ), 
						 WAIT_FOREVER ) == ERROR ) 
		{
			sprintf(rMsg,"error in focusCtrl msgReceive\n");
			LOG_MSG(ERROR_MSG,rMsg);
			carVal = CAR_ERROR;
			if(setCar(FOCUS_CAR,carVal,ERROR,rMsg,dummy) != OK)
				LOG_MSG(ERROR_MSG,dummy);
			continue;
		}

		LOG_MSG(DEBUG2_MSG,"focusCtrl:***********");
		pCad = msg.pCad;
	     
		LOG_MSG(DEBUG2_MSG,"focusCtrl: SIM_NONE\n");

		status = moveMech(FOCUS,rMsg);
		
		if(status == VME_OK)
		{
			carVal = CAR_IDLE;
			if(setCar(FOCUS_CAR,carVal,OK,"",dummy) != OK)
				LOG_MSG(ERROR_MSG,dummy);
		}
		else
		{
			carVal = CAR_ERROR;
			if(setCar(FOCUS_CAR,carVal,status,rMsg,dummy) != OK)
				LOG_MSG(ERROR_MSG,dummy);
			
		}

		LOG_MSG(DEBUG2_MSG,"***********focusCtrl\n");

    }
    
    return status;
}
/*
 *+
 * FUNCTION NAME:
 *	focusDatmCad
 *
 * INVOCATION:
 *	struct cadRecord* pCad;
 *	long status = 0;
 *	status = focusDatmCad( pCad );
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
 *	Supports Gemini "focusDatm" command, via CAD record
 *
 * DESCRIPTION:
 *	Inovked when focusDatmCad record is processed by EPICS.  It 
 *	implements the "datm" command, which initializes the mechanism.  
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

long focusDatmCad( struct cadRecord* pCad )
{
    motorMsg msg;
    long status = CAD_ACCEPT;

	LOG_MSG(DEBUG2_MSG,"focusDatmCad:***********");
    /* Take appropriate action, based on directive type*/
    switch(DIRECTIVE )
    {
      case CAD_MARK:
		LOG_MSG(DEBUG2_MSG, 
				"focusDatmCad - MARK directive");
	    strcpy(MESSAGE,"");
		break;

      case CAD_PRESET:	
		LOG_MSG(DEBUG2_MSG, 
				"focusDatmCad - PRESET directive");

		if(datumMech(FOCUS)!= VME_OK)
		{
			sprintf(MESSAGE,"focusDatumCad: ILLEGAL motor\n");
			status = CAD_REJECT;
		}
		break;

      case CAD_CLEAR:	
		LOG_MSG(DEBUG2_MSG, 
				"focusDatmCad - CLEAR directive");
		/*clear cad msg field*/
		strcpy(MESSAGE,"");
		break;

      case CAD_START:	
		LOG_MSG(DEBUG2_MSG, 
				"focusDatmCad - START directive");

		if (strcmp(pCad->h,DISABLED) == 0)
		{
			status = CAD_REJECT;
			sprintf(MESSAGE,"focusPosCad - Focus Observe in progress");
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
				if(msgQSend(focusMotorQ, (char *)&msg, sizeof(motorMsg),NO_WAIT, 
							MSG_PRI_NORMAL) != OK)
				{
					sprintf(MESSAGE,"msgQSend error in focusDatmCad\n");
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
		}
		break;

      case CAD_STOP:   
		LOG_MSG(DEBUG2_MSG,
				"focusDatmCad - STOP directive");
		if(carVal == CAR_BUSY)
		{
			abortMotor(FOCUS);
		}
		else
		{
			sprintf(MESSAGE,"focusDatmCad:  Focus is not busy\n");
			LOG_MSG(ERROR_MSG,MESSAGE);
			status = CAD_REJECT;	
		}
		/*check if car is busy and then call  abortMotor*/
	
		break;

      default:	/* Unknown directive	*/
		strncpy(MESSAGE, "focusDatmCad: Unrecognized directive",MAX_STRING_SIZE - 1);
		LOG_MSG(ERROR_MSG,MESSAGE);
		status = CAD_REJECT;
		break;
    } 
   
	LOG_MSG(DEBUG2_MSG,"***********focusDatmCad\n");
    return status;
}

/*
 *+
 * FUNCTION NAME:
 *	focusParkCad
 *
 * INVOCATION:
 *	struct cadRecord* pCad;
 *	long status = 0;
 *	status = focusParkCad( pCad );
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
 *	Supports Gemini "focusPark" command, via CAD record
 *
 * DESCRIPTION:
 *	Inovked when focusParkCad record is processed by EPICS.  It 
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
long focusParkCad( struct cadRecord* pCad)
{
   
    motorMsg msg;
    long status = CAD_ACCEPT;

    LOG_MSG(DEBUG2_MSG,"focusParkCad:***********");
    /* Take appropriate action, based on directive type*/
    switch(DIRECTIVE )
    {
      case CAD_MARK:   
		LOG_MSG(DEBUG2_MSG,"focusParkCad - MARK directive");

		break;

      case CAD_PRESET:	
		LOG_MSG(DEBUG2_MSG,"focusParkCad - PRESET directive");
		/* check datumed before trying to move motors*/
		if(!atoi(pCad->f))
		{
			status = CAD_REJECT;
			sprintf(MESSAGE,"focus mechanism not datumed\n");
			LOG_MSG(ERROR_MSG, MESSAGE);
		}
		else
			if(parkMech(FOCUS)!= VME_OK)
			{
				sprintf(MESSAGE,"Illegal motor\n");
				LOG_MSG(ERROR_MSG, MESSAGE);
				status = CAD_REJECT;
			}
		break;

      case CAD_CLEAR:	
		LOG_MSG(DEBUG2_MSG, "focusParkCad - CLEAR directive");
		/*clear cad msg field*/
		strcpy(MESSAGE,"");
		break;

      case CAD_START:	
		LOG_MSG(DEBUG2_MSG,"focusParkCad - START directive");

		if (strcmp(pCad->h,DISABLED) == 0)
		{
			status = CAD_REJECT;
			sprintf(MESSAGE,"focusPosCad - Focus Observe in progress");
			LOG_MSG(ERROR_MSG, MESSAGE);    
		}
		else
		{
			/*set car to busy (ctrl task will set back to idle)*/
			carVal = CAR_BUSY;	
			
			LOG_MSG(DEBUG2_MSG,"Waking park\n");
			msg.op = PARK;
			msg.pCad = pCad;
			if(msgQSend(focusMotorQ, (char *)&msg, sizeof(motorMsg),NO_WAIT, 
						MSG_PRI_NORMAL) != OK)
			{
				status = CAD_REJECT;
				sprintf(MESSAGE,"msgQSend error in focusDatmCad\n");
				LOG_MSG(ERROR_MSG,MESSAGE);
			}	
		}
		break;

      case CAD_STOP:	
		LOG_MSG(DEBUG2_MSG,"focusParkCad - STOP directive");
		/*call stop command*/
	if(carVal == CAR_BUSY)
		{
			abortMotor(FOCUS);
		}
		else
		{
			sprintf(MESSAGE,"focusParkCad:  Focus is not busy\n");
			LOG_MSG(ERROR_MSG,MESSAGE);
			status = CAD_REJECT;	
		}
		status = CAD_ACCEPT;
		break;

      default:	/* Unknown directive	*/
		strncpy(MESSAGE, "focusParkCad: Unrecognized directive",MAX_STRING_SIZE - 1);
		LOG_MSG(ERROR_MSG,MESSAGE);
		status = CAD_REJECT;
		break;
    } 
   
	LOG_MSG(DEBUG2_MSG,"***********focusParkCad\n");
    return status;
}
long markFocus()
{
	long status;
	char mess[80];
	char name[80];
	short mark=1;
	sprintf (name, "%s%s.MARK",dbTop,FOCUS_POS);
	status = putDbInfo(name,mess,DBF_SHORT,&mark);
	return OK;
}
/*
 *+
 * FUNCTION NAME:
 *	focusStepsCad
 *
 * INVOCATION:
 *	struct cadRecord* pCad;
 *	long status = 0;
 *	status = focusStepsCad( pCad );
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
 *	Supports Gemini "focusSteps" command, via CAD record
 *
 * DESCRIPTION:
 *	Inovked when focusStepsCad record is processed by EPICS.  It 
 *	implements the "steps" command, which moves the mechanism to an
 *  absolute step position.  
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
long focusStepsCad( struct cadRecord* pCad )
{
   
    motorMsg msg;
    long status = CAD_ACCEPT;
  

	LOG_MSG(DEBUG2_MSG,"focusStepsCad:***********");
    /* Take appropriate action, based on directive type*/
    switch(DIRECTIVE )
    {
      case CAD_MARK:
		LOG_MSG(DEBUG2_MSG,"focusStepsCad - MARK directive");
		break;

      case CAD_PRESET:	
		LOG_MSG(DEBUG2_MSG,"focusStepsCad - PRESET directive");

		/* check datumed before*/
		if(!atoi(pCad->f))
		{
			status = CAD_REJECT;
      	    strncpy(MESSAGE,"focusStepsCad - Focus not Datumed",39);
			LOG_MSG(ERROR_MSG, MESSAGE);
		}
		else
		{
			if(setEngPos(FOCUS,atoi(cadInput(STEPSCAD)))!= VME_OK)
			{
				strcpy(MESSAGE,gnirsErrorMessage);
				status = CAD_REJECT;
			}
		}
		break;
      case CAD_CLEAR:
		LOG_MSG(DEBUG2_MSG,"focusStepsCad - CLEAR directive");
		/*clear cad msg field*/
	    strcpy(MESSAGE,"");
		break;

      case CAD_START:	
		LOG_MSG(DEBUG2_MSG,"focusStepsCad - START directive");
	
		if (strcmp(pCad->h,DISABLED) == 0)
		{
			status = CAD_REJECT;
			sprintf(MESSAGE,"focusPosCad - Focus Observe in progress");
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
			
			if(msgQSend(focusMotorQ, (char *)&msg, sizeof(motorMsg),NO_WAIT, 
						MSG_PRI_NORMAL) != OK)
			{
				sprintf(MESSAGE,"msgQSend error in focusStepsCad\n");
				LOG_MSG(ERROR_MSG,MESSAGE);
				status = CAD_REJECT;
			}
		}
	
		break;

      case CAD_STOP:
		LOG_MSG(DEBUG2_MSG,"focusStepsCad - STOP directive");
			if(carVal == CAR_BUSY)
		{
			abortMotor(FOCUS);
		}
		else
		{
			sprintf(MESSAGE,"focusStepsCad:  Focus is not busy\n");
			LOG_MSG(ERROR_MSG,MESSAGE);
			status = CAD_REJECT;	
		}
		status = CAD_ACCEPT;
		break;

      default:	/* Unknown directive	*/
		strncpy(MESSAGE, "focusStepsCad: Unrecognized directive",MAX_STRING_SIZE - 1);
		status = CAD_REJECT;
		break;
    } 
   
	LOG_MSG(DEBUG2_MSG,"***********focusStepsCad\n");
    return status;
}

/*
 *+
 * FUNCTION NAME:
 *	focusPosCad
 *
 * INVOCATION:
 *	struct cadRecord* pCad;
 *	long status = 0;
 *	status = focusPosCad( pCad );
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
 *	Supports Gemini "focusPos" command, via CAD record
 *
 * DESCRIPTION:
 *	Inovked when focusPosCad record is processed by EPICS.  It 
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
long focusPosCad( struct cadRecord* pCad )
{
   
    motorMsg msg;
    long status = CAD_ACCEPT;
  

	LOG_MSG(DEBUG2_MSG,"focusPosCad:***********");
    /* Take appropriate action, based on directive type*/
    switch(DIRECTIVE )
    {
      case CAD_MARK:
		LOG_MSG(DEBUG2_MSG,"focusPosCad - MARK directive");
		break;

      case CAD_PRESET:	
		LOG_MSG(DEBUG2_MSG,"focusPosCad - PRESET directive");
		if(!atoi(pCad->f))
		{
			status = CAD_REJECT;
      	    sprintf(MESSAGE,"focusPosCad - Focus not Datumed");
			LOG_MSG(ERROR_MSG, MESSAGE);
		}

		break;

      case CAD_CLEAR:
		LOG_MSG(DEBUG2_MSG, 
				"focusPosCad - CLEAR directive");
	    strcpy(MESSAGE,"");
		break;

      case CAD_START:
		LOG_MSG(DEBUG2_MSG,"focusPosCad - START directive");
		if (strcmp(pCad->h,DISABLED) == 0)
		{
			status = CAD_REJECT;
			sprintf(MESSAGE,"focusPosCad - Focus Observe in progress");
			LOG_MSG(ERROR_MSG, MESSAGE);    
		}
		else
		{
			/*copy inputs to outputs*/
			status = assignVal(type(POSCAD), cadInput(POSCAD), cadOutput(POSCAD), MESSAGE);
			
			/*set car to busy (ctrl task will set back to idle)*/	  
			carVal = CAR_BUSY; 
			
			/*send message to ctrl task*/
			DPRINT(DPdebug,DEBUG2_MSG,"Waking focusCtrl\n");
			msg.op = POS;
			msg.pCad = pCad;	
			if(msgQSend(focusMotorQ, (char *)&msg, sizeof(motorMsg),NO_WAIT, 
						MSG_PRI_NORMAL) != OK)
			{
				sprintf(MESSAGE,"focusPosCad: Error in msgQSend ");
				status = CAD_REJECT;
			}	
		}
		break;

      case CAD_STOP:	
		LOG_MSG(DEBUG2_MSG,
				"focusPosCad - STOP directive");
			if(carVal == CAR_BUSY)
		{
			abortMotor(FOCUS);
		}
		else
		{
			sprintf(MESSAGE,"focusPosCad:  Focus is not busy\n");
			LOG_MSG(ERROR_MSG,MESSAGE);
			status = CAD_REJECT;	
		}
		status = CAD_ACCEPT;
		break;

      default:	/* Unknown directive	*/
		strncpy(MESSAGE, "focusPosCad: Unrecognized directive",
				MAX_STRING_SIZE - 1);
		status = CAD_REJECT;
		break;
    } 
   
	LOG_MSG(DEBUG2_MSG,"***********focusPosCad\n");
    return status;
}

long setfocusCarBusy()
{
	carVal = CAR_BUSY;
	if(setCar(FOCUS_CAR,carVal,OK,"",dummy) != OK)
	{
		LOG_MSG(ERROR_MSG,dummy);
		return ERROR;
	}
	return OK;
}
long setfocusCar()
{
	carVal = CAR_BUSY;
	return OK;
}
long getfocusCar()
{
	return carVal;
}
