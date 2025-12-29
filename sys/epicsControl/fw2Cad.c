static struct {
    void *v;
    char *c;
} rcsid = {
    &rcsid,
    "$Id: fw2Cad.c,v 1.4 2017/09/07 02:33:09 gemvx Exp $"
};
/*
 * Copyright 1997 Association of Universities for Research in 
 *	Astronomy, Inc.
 * See the file COPYRIGHT for more details.
 *
 * FILENAME
 * 	fw2Cad.c
 *
 * DESCRIPTION 
 * 	fw2Cad  support routines
 *
 * 
 * FUNCTION NAME(S)
 *	fw2DatmCad
 *	fw2ParkCad
 *	fw2StepsCad
 *	fw2PosCad
 *	fw2Ctrl
 *   
 * DEPENDENCIES
 * 	EPICS support libraries
 *
 *
 *INDENT-OFF*
 * $Log: fw2Cad.c,v $
 * Revision 1.4  2017/09/07 02:33:09  gemvx
 * Revert to using FW1 for anti-squiggle code (V1-13).
 *
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
 * Revision 1.1  2009/06/10 15:05:12  gemvx
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
MSG_Q_ID fw2MotorQ;
int fw2Id;

/* this value will have the current state of the car*/
static carVal = CAR_IDLE;

/* Forward declarations of the functions in this file */
long fw2DatmCad( struct cadRecord* pCad);

long fw2ParkCad( struct cadRecord* pCad);

long fw2StepsCad( struct cadRecord* pCad);

long fw2PosCad( struct cadRecord* pCad);

int fw2Ctrl( int n1, int n2, int n3, int n4, int n5, int n6, 
			 int n7, int n8, int n9, int n10 );
#ifndef FINDMECH
int findMechName(int mech, char *name, char *pos);
#endif
/*
 *+
 * FUNCTION NAME:    fw2????CadInit
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
 *    create message queue and spawn task to deal with fw2 cad records
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
long fw2DatmCadInit( struct cadRecord* pCad )
{
    long status = CAD_ACCEPT;
    static int done = 0;

    LOG_MSG(DEBUG2_MSG,"fw2DatmCadInit:***********");
    /* park, datum, pos, and steps cads will call this, only do once*/
    if (!done)
    {
		/*create msg q*/
		LOG_MSG(DEBUG2_MSG,"create Msg Queue\n");
		fw2MotorQ = msgQCreate(4,sizeof(motorMsg),MSG_Q_FIFO);
		if(fw2MotorQ == NULL)
		{
			status = CAD_REJECT;
			strncpy(MESSAGE,"Init: Error creating messageQ",MAX_STRING_SIZE - 1);
		}
		/*spawn task*/
		LOG_MSG(DEBUG2_MSG,"spawn task\n");
		if(status == CAD_ACCEPT)
			fw2Id = taskSpawn("tFw2Ctrl",50,VX_FP_TASK,4000,fw2Ctrl, 0,0,0,0,0,0,0,0,0,0);
		if(fw2Id == ERROR)
		{
			status = CAD_REJECT;
			strncpy(MESSAGE,"Init: Error Spawning task",MAX_STRING_SIZE - 1);
		}
		if(status = CAD_ACCEPT)
			done = 1;
    }
    
	LOG_MSG(DEBUG2_MSG,"***********fw2DatmCadInit\n");
    return status;
}
long fw2ParkCadInit( struct cadRecord* pCad )
{
    
    long status = CAD_ACCEPT;
	LOG_MSG(DEBUG2_MSG,"fw2ParkCadInit:***********");
    
	LOG_MSG(DEBUG2_MSG,"***********fw2ParkCadInit\n");
    return status;
}
long fw2PosCadInit( struct cadRecord* pCad )
{
    long status = CAD_ACCEPT;

	LOG_MSG(DEBUG2_MSG,"***********fw2PosCadInit\n");
    return status;
}
long fw2StepsCadInit( struct cadRecord* pCad )
{
   
    long status = CAD_ACCEPT;
	LOG_MSG(DEBUG2_MSG,"fw2StepsCadInit:***********");
	LOG_MSG(DEBUG2_MSG,"***********fw2StepsCadInit\n");
    return status;
}
/*
 *+
 * FUNCTION NAME:
 *	fw2Ctrl
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
 *	Cad record releases semaphore which allows fw2Ctrl to run.  
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

int fw2Ctrl( int n1, int n2, int n3, int n4, int n5, int n6, 
			 int n7, int n8, int n9, int n10 )
{
    struct cadRecord* pCad;
    long status = VME_OK;
    char rMsg[MAX_STRING_SIZE];
    char dummy[MAX_STRING_SIZE];
    motorMsg msg;
   
  

  
    strncpy (rMsg,"Error in fw2Ctrl\n",MAX_STRING_SIZE -1);
    /* Repeat as inifinite loop				*/
    while(1)
    {
		LOG_MSG(DEBUG2_MSG, "Task tfw2Ctrl sleeping...\n");
	
		if( msgQReceive( fw2MotorQ, (char *)&msg, sizeof(motorMsg ), 
						 WAIT_FOREVER ) == ERROR ) 
		{
			sprintf(rMsg,"error in fw2Ctrl msgReceive\n");
			LOG_MSG(ERROR_MSG,rMsg);
			carVal = CAR_ERROR;
			if(setCar(FW2_CAR,carVal,ERROR,rMsg,dummy) != OK)
				LOG_MSG(ERROR_MSG,dummy);
			continue;
		}

		LOG_MSG(DEBUG2_MSG,"fw2Ctrl:***********");
		pCad = msg.pCad;
	     

			status = moveMech(FW2,rMsg);
		

		if(status == VME_OK)
		{
			carVal = CAR_IDLE;
			if(setCar(FW2_CAR,carVal,OK,"",dummy) != OK)
				LOG_MSG(ERROR_MSG,dummy);
		}
		else
		{
			carVal = CAR_ERROR;
			if(setCar(FW2_CAR,carVal,status,rMsg,dummy) != OK)
				LOG_MSG(ERROR_MSG,dummy);
			
		}

	LOG_MSG(DEBUG2_MSG,"***********fw2Ctrl\n");

    }
    
    return status;
}
/*
 *+
 * FUNCTION NAME:
 *	fw2DatmCad
 *
 * INVOCATION:
 *	struct cadRecord* pCad;
 *	long status = 0;
 *	status = fw2DatmCad( pCad );
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
 *	Supports Gemini "fw2Datm" command, via CAD record
 *
 * DESCRIPTION:
 *	Inovked when fw2DatmCad record is processed by EPICS.  It 
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

long fw2DatmCad( struct cadRecord* pCad )
{
    motorMsg msg;
    long status = CAD_ACCEPT;

	LOG_MSG(DEBUG2_MSG,"fw2DatmCad:***********");
    /* Take appropriate action, based on directive type*/
    switch(DIRECTIVE )
    {
      case CAD_MARK:
		LOG_MSG(DEBUG2_MSG, 
				"fw2DatmCad - MARK directive");
	    strcpy(MESSAGE,"");
		break;

      case CAD_PRESET:	
		LOG_MSG(DEBUG2_MSG, 
				"fw2DatmCad - PRESET directive");

		if(datumMech(FW2)!= VME_OK)
		{
			sprintf(MESSAGE,"fw2DatmCad: ILLEGAL motor\n");
			status = CAD_REJECT;
		}
		break;

      case CAD_CLEAR:	
		LOG_MSG(DEBUG2_MSG, 
				"fw2DatmCad - CLEAR directive");
		/*clear cad msg field*/
		strcpy(MESSAGE,"");
		break;

      case CAD_START:	
		LOG_MSG(DEBUG2_MSG, 
				"fw2DatmCad - START directive");

		if (strcmp(pCad->h,DISABLED) == 0)
		{
			status = CAD_REJECT;
			sprintf(MESSAGE,"fw2PosCad - Fw2 Observe in progress");
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
				sprintf(MESSAGE,"fw2DatmCad:  Fw2 is busy\n");
				LOG_MSG(ERROR_MSG,MESSAGE);
				status = CAD_REJECT;
			}
		}
		break;

      case CAD_STOP:   
		LOG_MSG(DEBUG2_MSG,
				"fw2DatmCad - STOP directive");
		if(carVal == CAR_BUSY)
		{
			abortMotor(FW2);
		}
		else
		{
			sprintf(MESSAGE,"fw2DatmCad:  Fw2 is not busy\n");
			LOG_MSG(ERROR_MSG,MESSAGE);
			status = CAD_REJECT;	
		}
		/*check if car is busy and then call  abortMotor*/
	
		break;

      default:	/* Unknown directive	*/
		strncpy(MESSAGE, "fw2DatmCad: Unrecognized directive",MAX_STRING_SIZE - 1);
		LOG_MSG(ERROR_MSG,MESSAGE);
		status = CAD_REJECT;
		break;
    } 
   
	LOG_MSG(DEBUG2_MSG,"***********fw2DatmCad\n");
    return status;
}

/*
 *+
 * FUNCTION NAME:
 *	fw2ParkCad
 *
 * INVOCATION:
 *	struct cadRecord* pCad;
 *	long status = 0;
 *	status = fw2ParkCad( pCad );
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
 *	Supports Gemini "fw2Park" command, via CAD record
 *
 * DESCRIPTION:
 *	Inovked when fw2ParkCad record is processed by EPICS.  It 
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
long fw2ParkCad( struct cadRecord* pCad)
{
   
    motorMsg msg;
    long status = CAD_ACCEPT;

    LOG_MSG(DEBUG2_MSG,"fw2ParkCad:***********");
    /* Take appropriate action, based on directive type*/
    switch(DIRECTIVE )
    {
      case CAD_MARK:   
		LOG_MSG(DEBUG2_MSG,"fw2ParkCad - MARK directive");

		break;

      case CAD_PRESET:	
		LOG_MSG(DEBUG2_MSG,"fw2ParkCad - PRESET directive");
		/* check datumed before trying to move motors*/
		if(!atoi(pCad->f))
		{
			status = CAD_REJECT;
			sprintf(MESSAGE,"fw2 mechanism not datumed\n");
			LOG_MSG(ERROR_MSG, MESSAGE);
		}
		else
			if(parkMech(FW2)!= VME_OK)
			{
				sprintf(MESSAGE,"fw2ParkCad: Illegal motor\n");
				LOG_MSG(ERROR_MSG, MESSAGE);
				status = CAD_REJECT;
			}
		break;

      case CAD_CLEAR:	
		LOG_MSG(DEBUG2_MSG, "fw2ParkCad - CLEAR directive");
		/*clear cad msg field*/
		strcpy(MESSAGE,"");
		break;

      case CAD_START:	
		LOG_MSG(DEBUG2_MSG,"fw2ParkCad - START directive");

		if (strcmp(pCad->h,DISABLED) == 0)
		{
			status = CAD_REJECT;
			sprintf(MESSAGE,"fw2PosCad - Fw2 Observe in progress");
			LOG_MSG(ERROR_MSG, MESSAGE);    
		}
		else
		{
			/*set car to busy (ctrl task will set back to idle)*/
			carVal = CAR_BUSY;
			
			LOG_MSG(DEBUG2_MSG,"Waking park\n");
			msg.op = PARK;
			msg.pCad = pCad;
			if(msgQSend(fw2MotorQ, (char *)&msg, sizeof(motorMsg),NO_WAIT, 
						MSG_PRI_NORMAL) != OK)
			{
				status = CAD_REJECT;
				sprintf(MESSAGE,"msgQSend error in fw2ParkCad\n");
				LOG_MSG(ERROR_MSG,MESSAGE);
			}	
		}
		break;

      case CAD_STOP:	
		LOG_MSG(DEBUG2_MSG,"fw2ParkCad - STOP directive");
		/*call stop command*/
		if(carVal == CAR_BUSY)
		{
			abortMotor(FW2);
		}
		else
		{
			sprintf(MESSAGE,"fw2ParkCad:  Fw2 is not busy\n");
			LOG_MSG(ERROR_MSG,MESSAGE);
			status = CAD_REJECT;	
		}
		status = CAD_ACCEPT;
		break;

      default:	/* unknown directive	*/
		strncpy(MESSAGE, "fw2ParkCad: Unrecognized directive",MAX_STRING_SIZE - 1);
		LOG_MSG(ERROR_MSG,MESSAGE);
		status = CAD_REJECT;
		break;
    } 
   
	LOG_MSG(DEBUG2_MSG,"***********fw2ParkCad\n");
    return status;
}

/*
 *+
 * FUNCTION NAME:
 *	fw2StepsCad
 *
 * INVOCATION:
 *	struct cadRecord* pCad;
 *	long status = 0;
 *	status = fw2StepsCad( pCad );
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
 *	Supports Gemini "fw2Steps" command, via CAD record
 *
 * DESCRIPTION:
 *	Inovked when fw2StepsCad record is processed by EPICS.  It 
 *	implements the "steps" command, which moves the mechanism to an
 *  absolute position.  
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
long fw2StepsCad( struct cadRecord* pCad )
{
   
    motorMsg msg;
    long status = CAD_ACCEPT;
   

	LOG_MSG(DEBUG2_MSG,"fw2StepsCad:***********");
    /* Take appropriate action, based on directive type*/
    switch(DIRECTIVE )
    {
      case CAD_MARK:
		LOG_MSG(DEBUG2_MSG,"fw2StepsCad - MARK directive");
		break;

      case CAD_PRESET:	
		LOG_MSG(DEBUG2_MSG,"fw2StepsCad - PRESET directive");

		/* check datumed before*/
		if(!atoi(pCad->f))
		{
			status = CAD_REJECT;
      	    strncpy(MESSAGE,"fw2StepsCad - Fw2 not Datumed",39);
			LOG_MSG(ERROR_MSG, MESSAGE);
		}
		else
		{
			if(setEngPos(FW2,atoi(cadInput(STEPSCAD)))!= VME_OK)
			{
				strcpy(MESSAGE,gnirsErrorMessage);
				status = CAD_REJECT;
			}
		}
		break;
      case CAD_CLEAR:
		LOG_MSG(DEBUG2_MSG,"fw2StepsCad - CLEAR directive");
		/*clear cad msg field*/
	    strcpy(MESSAGE,"");
		break;

      case CAD_START:	
		LOG_MSG(DEBUG2_MSG,"fw2StepsCad - START directive");
	
		if (strcmp(pCad->h,DISABLED) == 0)
		{
			status = CAD_REJECT;
			sprintf(MESSAGE,"fw2PosCad - Fw2 Observe in progress");
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
			
			if(msgQSend(fw2MotorQ, (char *)&msg, sizeof(motorMsg),NO_WAIT, 
						MSG_PRI_NORMAL) != OK)
			{
				sprintf(MESSAGE,"msgQSend error in fw2StepsCad\n");
				LOG_MSG(ERROR_MSG,MESSAGE);
				status = CAD_REJECT;
			}
		}
	
		break;

      case CAD_STOP:
		LOG_MSG(DEBUG2_MSG,"fw2StepsCad - STOP directive");
			if(carVal == CAR_BUSY)
		{
			abortMotor(FW2);
		}
		else
		{
			sprintf(MESSAGE,"fw2StepsCad:  Fw2 is not busy\n");
			LOG_MSG(ERROR_MSG,MESSAGE);
			status = CAD_REJECT;	
		}
		status = CAD_ACCEPT;
		break;

      default:	/* Unknown directive	*/
		strncpy(MESSAGE, "fw2StepsCad: Unrecognized directive",MAX_STRING_SIZE - 1);
		status = CAD_REJECT;
		break;
    } 
   
	LOG_MSG(DEBUG2_MSG,"***********fw2StepsCad\n");
    return status;
}

/*
 *+
 * FUNCTION NAME:
 *	fw2PosCad
 *
 * INVOCATION:
 *	struct cadRecord* pCad;
 *	long status = 0;
 *	status = fw2PosCad( pCad );
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
 *	Supports Gemini "fw2Pos" command, via CAD record
 *
 * DESCRIPTION:
 *	Inovked when fw2PosCad record is processed by EPICS.  It 
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
long fw2PosCad( struct cadRecord* pCad )
{
   
	char pos[POS_LEN];
    motorMsg msg;
    long status = CAD_ACCEPT;
   

	LOG_MSG(DEBUG2_MSG,"fw2PosCad:***********");
    /* Take appropriate action, based on directive type*/
    switch(DIRECTIVE )
    {
      case CAD_MARK:
		LOG_MSG(DEBUG2_MSG,"fw2PosCad - MARK directive");
		break;

      case CAD_PRESET:	
		LOG_MSG(DEBUG2_MSG,"fw2PosCad - PRESET directive");
		if(!atoi(pCad->f))
		{
			status = CAD_REJECT;
      	    sprintf(MESSAGE,"fw2PosCad - Fw2 not Datumed");
			LOG_MSG(ERROR_MSG, MESSAGE);
		}
		else
		{
			printf("name = %s\n",pCad->b);
#ifndef FINDMECH
			if(findMechName(FW2,pCad->b,pos))
			{
#endif
				if(valid(FW2,pos)!= VME_OK)
				{
					sprintf(MESSAGE,gnirsErrorMessage);
					status = CAD_REJECT;
				}
#ifndef FINDMECH
			}
			else
			{
				sprintf(MESSAGE,"%s not found in database for fw2\n",pCad->b);
				status = CAD_REJECT;
			}
#endif
		}
		break;

      case CAD_CLEAR:
		LOG_MSG(DEBUG2_MSG, 
				"fw2PosCad - CLEAR directive");
	    strcpy(MESSAGE,"");
		break;

      case CAD_START:
		LOG_MSG(DEBUG2_MSG,"fw2PosCad - START directive");
		if (strcmp(pCad->h,DISABLED) == 0)
		{
			status = CAD_REJECT;
			sprintf(MESSAGE,"fw2PosCad - Fw2 Observe in progress");
			LOG_MSG(ERROR_MSG, MESSAGE);    
		}
		else
		{
			/*copy inputs to outputs*/
			status = assignVal(type(POSCAD), cadInput(POSCAD), cadOutput(POSCAD), MESSAGE);
			
			/*set car to busy (ctrl task will set back to idle)*/	  
			carVal = CAR_BUSY; 
			
			/*send message to ctrl task*/
			DPRINT(DPdebug,DEBUG2_MSG,"Waking fw2Ctrl\n");
			msg.op = POS;
			msg.pCad = pCad;	
			if(msgQSend(fw2MotorQ, (char *)&msg, sizeof(motorMsg),NO_WAIT, 
						MSG_PRI_NORMAL) != OK)
			{
				sprintf(MESSAGE,"fw2PosCad: Error in msgQSend ");
				status = CAD_REJECT;
			}	
		}
		break;
		

      case CAD_STOP:	
		LOG_MSG(DEBUG2_MSG,
				"fw2PosCad - STOP directive");
			if(carVal == CAR_BUSY)
		{
			abortMotor(FW2);
		}
		else
		{
			sprintf(MESSAGE,"fw2PosCad:  Fw2 is not busy\n");
			LOG_MSG(ERROR_MSG,MESSAGE);
			status = CAD_REJECT;	
		}
		status = CAD_ACCEPT;
		break;

      default:	/* Unknown directive	*/
		strncpy(MESSAGE, "fw2PosCad: Unrecognized directive",
				MAX_STRING_SIZE - 1);
		status = CAD_REJECT;
		break;
    } 
   
	LOG_MSG(DEBUG2_MSG,"***********fw2PosCad\n");
    return status;
}

long setfw2CarBusy()
{
	carVal = CAR_BUSY;
	if(setCar(FW2_CAR,carVal,OK,"",dummy) != OK)
	{
		LOG_MSG(ERROR_MSG,dummy);
		return ERROR;
	}
	return OK;
}
long setfw2Car()
{
	carVal = CAR_BUSY;
	return OK;
}
long getfw2Car()
{
	return carVal;
}
