static struct {
    void *v;
    char *c;
} rcsid = {
    &rcsid,
    "$Id: slitCad.c,v 1.2 2013/06/06 01:54:28 gemvx Exp $"
};
/*
 * Copyright 1997 Association of Universities for Research in 
 *	Astronomy, Inc.
 * See the file COPYRIGHT for more details.
 *
 * FILENAME
 * 	slitCad.c
 *
 * DESCRIPTION 
 * 	slitCad  support routines
 *
 * 
 * FUNCTION NAME(S)
 *	slitDatmCad
 *	slitParkCad
 *	slitStepsCad
 *	slitPosCad
 *	slitCtrl
 *   
 * DEPENDENCIES
 * 	EPICS support libraries
 *
 *
 *INDENT-OFF*
 * $Log: slitCad.c,v $
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
MSG_Q_ID slitMotorQ;
int slitId;

/* this value will have the current state of the car*/
static carVal = CAR_IDLE;

/* Forward declarations of the functions in this file */
long slitDatmCad( struct cadRecord* pCad);

long slitParkCad( struct cadRecord* pCad);

long slitStepsCad( struct cadRecord* pCad);

long slitPosCad( struct cadRecord* pCad);

int slitCtrl( int n1, int n2, int n3, int n4, int n5, int n6, 
			 int n7, int n8, int n9, int n10 );
#ifndef FINDMECH
int findMechName(int mech, char *name, char *pos);
#endif
int val[10];
/*
 *+
 * FUNCTION NAME:    slit????CadInit
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
 *    create message queue and spawn task to deal with slit cad records
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
long slitDatmCadInit( struct cadRecord* pCad )
{
    long status = CAD_ACCEPT;
    static int done = 0;

    LOG_MSG(DEBUG2_MSG,"slitDatmCadInit:***********");
    /* park, datum, pos, and steps cads will call this, only do once*/
    if (!done)
    {
		/*create msg q*/
		LOG_MSG(DEBUG2_MSG,"create Msg Queue\n");
		slitMotorQ = msgQCreate(4,sizeof(motorMsg),MSG_Q_FIFO);
		if(slitMotorQ == NULL)
		{
			status = CAD_REJECT;
			strncpy(MESSAGE,"Init: Error creating messageQ",MAX_STRING_SIZE - 1);
		}
		/*spawn task*/
		LOG_MSG(DEBUG2_MSG,"spawn task\n");
		if(status == CAD_ACCEPT)
			slitId = taskSpawn("tSlitCtrl",50,VX_FP_TASK,4000,slitCtrl, 0,0,0,0,0,0,0,0,0,0);
		if(slitId == ERROR)
		{
			status = CAD_REJECT;
			strncpy(MESSAGE,"Init: Error slitCtrl Spawning task",MAX_STRING_SIZE - 1);
		}
		if(status = CAD_ACCEPT)
			done = 1;
    }
    
	LOG_MSG(DEBUG2_MSG,"***********slitDatmCadInit\n");
    return status;
}
long slitParkCadInit( struct cadRecord* pCad )
{
    
    long status = CAD_ACCEPT;
	LOG_MSG(DEBUG2_MSG,"slitParkCadInit:***********");
    
	LOG_MSG(DEBUG2_MSG,"***********slitParkCadInit\n");
    return status;
}
long slitPosCadInit( struct cadRecord* pCad )
{
    long status = CAD_ACCEPT;

	LOG_MSG(DEBUG2_MSG,"***********slitPosCadInit\n");
    return status;
}
long slitStepsCadInit( struct cadRecord* pCad )
{
   
    long status = CAD_ACCEPT;
	LOG_MSG(DEBUG2_MSG,"slitStepsCadInit:***********");
	LOG_MSG(DEBUG2_MSG,"***********slitStepsCadInit\n");
    return status;
}
/*
 *+
 * FUNCTION NAME:
 *	slitCtrl
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
 *	Cad record releases semaphore which allows slitCtrl to run.  
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

int slitCtrl( int n1, int n2, int n3, int n4, int n5, int n6, 
			 int n7, int n8, int n9, int n10 )
{
    struct cadRecord* pCad;
    long status = VME_OK;
    char rMsg[MAX_STRING_SIZE];
    char dummy[MAX_STRING_SIZE];
    motorMsg msg;
   
    strncpy (rMsg,"Error in slitCtrl\n",MAX_STRING_SIZE -1);
    /* Repeat as inifinite loop				*/
    while(1)
    {
		LOG_MSG(DEBUG2_MSG, "Task tslitCtrl sleeping...\n");
	
		if( msgQReceive( slitMotorQ, (char *)&msg, sizeof(motorMsg ), 
						 WAIT_FOREVER ) == ERROR ) 
		{
			sprintf(rMsg,"error in slitCtrl msgReceive\n");
			LOG_MSG(ERROR_MSG,rMsg);
			carVal = CAR_ERROR;
			if(setCar(SLIT_CAR,carVal,ERROR,rMsg,dummy) != OK)
				LOG_MSG(ERROR_MSG,dummy);
			continue;
		}

		LOG_MSG(DEBUG2_MSG,"slitCtrl:***********");
		pCad = msg.pCad;
	     
	
			status = moveMech(SLIT,rMsg);
		
		if(status == VME_OK)
		{
			carVal = CAR_IDLE;
			if(setCar(SLIT_CAR,carVal,OK,"",dummy) != OK)
				LOG_MSG(ERROR_MSG,dummy);
		}
		else
		{
			carVal = CAR_ERROR;
			if(setCar(SLIT_CAR,carVal,status,rMsg,dummy) != OK)
				LOG_MSG(ERROR_MSG,dummy);
			
		}

		LOG_MSG(DEBUG2_MSG,"***********slitCtrl\n");

    }
    
    return status;
}
/*
 *+
 * FUNCTION NAME:
 *	slitDatmCad
 *
 * INVOCATION:
 *	struct cadRecord* pCad;
 *	long status = 0;
 *	status = slitDatmCad( pCad );
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
 *	Supports Gemini "slitDatm" command, via CAD record
 *
 * DESCRIPTION:
 *	Inovked when slitDatmCad record is processed by EPICS.  It 
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

long slitDatmCad( struct cadRecord* pCad )
{
    motorMsg msg;
    long status = CAD_ACCEPT;

	LOG_MSG(DEBUG2_MSG,"slitDatmCad:***********");
    /* Take appropriate action, based on directive type*/
    switch(DIRECTIVE )
    {
      case CAD_MARK:
		LOG_MSG(DEBUG2_MSG, 
				"slitDatmCad - MARK directive");
	    strcpy(MESSAGE,"");
		break;

      case CAD_PRESET:	
		LOG_MSG(DEBUG2_MSG, 
				"slitDatmCad - PRESET directive");

		if(datumMech(SLIT)!= VME_OK)
		{
			sprintf(MESSAGE,"slitDatmCad: ILLEGAL motor\n");
			status = CAD_REJECT;
		}
		break;

      case CAD_CLEAR:	
		LOG_MSG(DEBUG2_MSG, 
				"slitDatmCad - CLEAR directive");
		/*clear cad msg field*/
		strcpy(MESSAGE,"");
		break;

      case CAD_START:	
		LOG_MSG(DEBUG2_MSG, 
				"slitDatmCad - START directive");

		if (strcmp(pCad->h,DISABLED) == 0)
		{
			status = CAD_REJECT;
			sprintf(MESSAGE,"slitPosCad - Slit Observe in progress");
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
				if(msgQSend(slitMotorQ, (char *)&msg, sizeof(motorMsg),NO_WAIT, 
							MSG_PRI_NORMAL) != OK)
				{
					sprintf(MESSAGE,"slitDatmCad: msgQSend error  \n");
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
		}
		break;

      case CAD_STOP:   
		LOG_MSG(DEBUG2_MSG,
				"slitDatmCad - STOP directive");
		if(carVal == CAR_BUSY)
		{
			abortMotor(SLIT);
		}
		else
		{
			sprintf(MESSAGE,"slitDatmCad:  Slit is not busy\n");
			LOG_MSG(ERROR_MSG,MESSAGE);
			status = CAD_REJECT;	
		}
		/*check if car is busy and then call  abortMotor*/
	
		break;

      default:	/* Unknown directive	*/
		strncpy(MESSAGE, "slitDatmCad: Unrecognized directive",MAX_STRING_SIZE - 1);
		LOG_MSG(ERROR_MSG,MESSAGE);
		status = CAD_REJECT;
		break;
    } 
   
	LOG_MSG(DEBUG2_MSG,"***********slitDatmCad\n");
    return status;
}

/*
 *+
 * FUNCTION NAME:
 *	slitParkCad
 *
 * INVOCATION:
 *	struct cadRecord* pCad;
 *	long status = 0;
 *	status = slitParkCad( pCad );
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
 *	Supports Gemini "slitPark" command, via CAD record
 *
 * DESCRIPTION:
 *	Inovked when slitParkCad record is processed by EPICS.  It 
 *	implements the "park" command, which places the system in a 
 *	safe shutdown status.  
 *
 * EXTERNAL VARIABLES:
 *	None
 *
 * PRIOR REQUIREMENTS:
 *	CAD record should be initialized, with no parameters.
 *
 * DEFICIENCIES:
 *	
 *
 * HISTORY (optional):
 *	  
 *
 *-
 */
long slitParkCad( struct cadRecord* pCad)
{
   
    motorMsg msg;
    long status = CAD_ACCEPT;

    LOG_MSG(DEBUG2_MSG,"slitParkCad:***********");
    /* Take appropriate action, based on directive type*/
    switch(DIRECTIVE )
    {
      case CAD_MARK:   
		LOG_MSG(DEBUG2_MSG,"slitParkCad - MARK directive");

		break;

      case CAD_PRESET:	
		LOG_MSG(DEBUG2_MSG,"slitParkCad - PRESET directive");
		/* check datumed before trying to move motors*/
		if(!atoi(pCad->f))
		{
			status = CAD_REJECT;
			sprintf(MESSAGE,"slitParkCad: slit mechanism not datumed\n");
			LOG_MSG(ERROR_MSG, MESSAGE);
		}
		else
			if(parkMech(SLIT)!= VME_OK)
			{
				sprintf(MESSAGE,"slitParkCad: Illegal motor\n");
				LOG_MSG(ERROR_MSG, MESSAGE);
				status = CAD_REJECT;
			}
		break;

      case CAD_CLEAR:	
		LOG_MSG(DEBUG2_MSG, "slitParkCad - CLEAR directive");
		/*clear cad msg field*/
		strcpy(MESSAGE,"");
		break;

      case CAD_START:	
		LOG_MSG(DEBUG2_MSG,"slitParkCad - START directive");

		if (strcmp(pCad->h,DISABLED) == 0)
		{
			status = CAD_REJECT;
			sprintf(MESSAGE,"slitPosCad - Slit Observe in progress");
			LOG_MSG(ERROR_MSG, MESSAGE);	
		}
		else
		{
			/*set car to busy (ctrl task will set back to idle)*/
			carVal = CAR_BUSY;	
			
			LOG_MSG(DEBUG2_MSG,"Waking park\n");
			msg.op = PARK;
			msg.pCad = pCad;
			if(msgQSend(slitMotorQ, (char *)&msg, sizeof(motorMsg),NO_WAIT, 
						MSG_PRI_NORMAL) != OK)
			{
				status = CAD_REJECT;
				sprintf(MESSAGE,"msgQSend error in slitDatmCad\n");
				LOG_MSG(ERROR_MSG,MESSAGE);
			}	
		}
		break;

      case CAD_STOP:	
		LOG_MSG(DEBUG2_MSG,"slitParkCad - STOP directive");
		/*call stop command*/
	if(carVal == CAR_BUSY)
		{
			abortMotor(SLIT);
		}
		else
		{
			sprintf(MESSAGE,"slitParkCad:  Slit is not busy\n");
			LOG_MSG(ERROR_MSG,MESSAGE);
			status = CAD_REJECT;	
		}
		status = CAD_ACCEPT;
		break;

      default:	/* Unknown directive	*/
		strncpy(MESSAGE, "slitParkCad: Unrecognized directive",MAX_STRING_SIZE - 1);
		LOG_MSG(ERROR_MSG,MESSAGE);
		status = CAD_REJECT;
		break;
    } 
   
	LOG_MSG(DEBUG2_MSG,"***********slitParkCad\n");
    return status;
}

/*
 *+
 * FUNCTION NAME:
 *	slitStepsCad
 *
 * INVOCATION:
 *	struct cadRecord* pCad;
 *	long status = 0;
 *	status = slitStepsCad( pCad );
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
 *	Supports Gemini "slitSteps" command, via CAD record
 *
 * DESCRIPTION:
 *	Inovked when slitStepsCad record is processed by EPICS.  It 
 *	implements the "steps" command,which moves the mechanism to the
 *  absolute position given in the input.  
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
long slitStepsCad( struct cadRecord* pCad )
{
   
    motorMsg msg;
    long status = CAD_ACCEPT;
   

	LOG_MSG(DEBUG2_MSG,"slitStepsCad:***********");
    /* Take appropriate action, based on directive type*/
    switch(DIRECTIVE )
    {
      case CAD_MARK:
		LOG_MSG(DEBUG2_MSG,"slitStepsCad - MARK directive");
		break;

      case CAD_PRESET:	
		LOG_MSG(DEBUG2_MSG,"slitStepsCad - PRESET directive");

		/* check datumed before*/
		if(!atoi(pCad->f))
		{
			status = CAD_REJECT;
      	    strncpy(MESSAGE,"slitStepsCad - Slit not Datumed",39);
			LOG_MSG(ERROR_MSG, MESSAGE);
		}
		else
		{
			if(setEngPos(SLIT,atoi(cadInput(STEPSCAD)))!= VME_OK)
			{
				strcpy(MESSAGE,gnirsErrorMessage);
				status = CAD_REJECT;
			}
		}
		break;
      case CAD_CLEAR:
		LOG_MSG(DEBUG2_MSG,"slitStepsCad - CLEAR directive");
		/*clear cad msg field*/
	    strcpy(MESSAGE,"");
		break;

      case CAD_START:	
		LOG_MSG(DEBUG2_MSG,"slitStepsCad - START directive");
	
		if (strcmp(pCad->h,DISABLED) == 0)
		{
			status = CAD_REJECT;
			sprintf(MESSAGE,"slitPosCad - Slit Observe in progress");
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
			
			if(msgQSend(slitMotorQ, (char *)&msg, sizeof(motorMsg),NO_WAIT, 
						MSG_PRI_NORMAL) != OK)
			{
				sprintf(MESSAGE,"msgQSend error in slitStepsCad\n");
				LOG_MSG(ERROR_MSG,MESSAGE);
				status = CAD_REJECT;
			}
		}
	
		break;

      case CAD_STOP:
		LOG_MSG(DEBUG2_MSG,"slitStepsCad - STOP directive");
			if(carVal == CAR_BUSY)
		{
			abortMotor(SLIT);
		}
		else
		{
			sprintf(MESSAGE,"slitStepsCad:  Slit is not busy\n");
			LOG_MSG(ERROR_MSG,MESSAGE);
			status = CAD_REJECT;	
		}
		status = CAD_ACCEPT;
		break;

      default:	/* Unknown directive	*/
		strncpy(MESSAGE, "slitStepsCad: Unrecognized directive",MAX_STRING_SIZE - 1);
		status = CAD_REJECT;
		break;
    } 
   
	LOG_MSG(DEBUG2_MSG,"***********slitStepsCad\n");
    return status;
}

/*
 *+
 * FUNCTION NAME:
 *	slitPosCad
 *
 * INVOCATION:
 *	struct cadRecord* pCad;
 *	long status = 0;
 *	status = slitPosCad( pCad );
 *
 * PARAMETERS: (">" input, "!" modified, "<" output)
 *	> pCad		(struct cadRecord*)	pointer to CAD record
 *        pCad->b  = position
 *      > pCad->valf       datumed sad record
 *      > pCad->valg       initCad.VALA  (epics sim mode)
 *
 * FUNCTION VALUE:
 *	long		status value written to CAD VAL field
 *
 * PURPOSE:
 *	Supports Gemini "slitPos" command, via CAD record
 *
 * DESCRIPTION:
 *	Inovked when slitPosCad record is processed by EPICS.  It 
 *	implements the "pos" command, which moves the mechanism to 
 *  the named position.  
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
long slitPosCad( struct cadRecord* pCad )
{
   
	char pos[POS_LEN];
    motorMsg msg;
    long status = CAD_ACCEPT;
   

	LOG_MSG(DEBUG2_MSG,"slitPosCad:***********");
    /* Take appropriate action, based on directive type*/
    switch(DIRECTIVE )
    {
      case CAD_MARK:
		LOG_MSG(DEBUG2_MSG,"slitPosCad - MARK directive");
		break;

      case CAD_PRESET:	
		LOG_MSG(DEBUG2_MSG,"slitPosCad - PRESET directive");
		if(!atoi(pCad->f))
		{
			status = CAD_REJECT;
      	    sprintf(MESSAGE,"slitPosCad - Slit not Datumed");
			LOG_MSG(ERROR_MSG, MESSAGE);
		}
		else
		{
			printf("CC slit name = %s\n",pCad->b);
#ifndef FINDMECH
			if(findMechName(SLIT,pCad->b,pos))
			{
#endif
				if(valid(SLIT,pCad->b)!= VME_OK)
				{
					sprintf(MESSAGE,gnirsErrorMessage);
					status = CAD_REJECT;
				}
#ifndef FINDMECH
			}
			else
			{
				sprintf(MESSAGE,"%s not found in database for slit\n",pCad->b);
				status = CAD_REJECT;
			}
#endif
		}
		break;

      case CAD_CLEAR:
		LOG_MSG(DEBUG2_MSG, 
				"slitPosCad - CLEAR directive");
	    strcpy(MESSAGE,"");
		break;

      case CAD_START:
		LOG_MSG(DEBUG2_MSG,"slitPosCad - START directive");
		if (strcmp(pCad->h,DISABLED) == 0)
		{
			status = CAD_REJECT;
			sprintf(MESSAGE,"slitPosCad - Slit Observe in progress");
			LOG_MSG(ERROR_MSG, MESSAGE);	
		}
		else
		{
			/*copy inputs to outputs*/
			status = assignVal(type(POSCAD), cadInput(POSCAD), cadOutput(POSCAD), MESSAGE);
			
			/*set car to busy (ctrl task will set back to idle)*/	  
			carVal = CAR_BUSY; 
			
			/*send message to ctrl task*/
			DPRINT(DPdebug,DEBUG2_MSG,"Waking slitCtrl\n");
			msg.op = POS;
			msg.pCad = pCad;	
			if(msgQSend(slitMotorQ, (char *)&msg, sizeof(motorMsg),NO_WAIT, 
						MSG_PRI_NORMAL) != OK)
			{
				sprintf(MESSAGE,"slitPosCad: Error in msgQSend ");
				status = CAD_REJECT;
			}	
		}
		break;

      case CAD_STOP:	
		LOG_MSG(DEBUG2_MSG,
				"slitPosCad - STOP directive");
			if(carVal == CAR_BUSY)
		{
			abortMotor(SLIT);
		}
		else
		{
			sprintf(MESSAGE,"slitPosCad:  Slit is not busy\n");
			LOG_MSG(ERROR_MSG,MESSAGE);
			status = CAD_REJECT;	
		}
		status = CAD_ACCEPT;
		break;

      default:	/* Unknown directive	*/
		strncpy(MESSAGE, "slitPosCad: Unrecognized directive",
				MAX_STRING_SIZE - 1);
		status = CAD_REJECT;
		break;
    } 
   
	LOG_MSG(DEBUG2_MSG,"***********slitPosCad\n");
    return status;
}

long setslitCarBusy()
{
	carVal = CAR_BUSY;
	if(setCar(SLIT_CAR,carVal,OK,"",dummy) != OK)
	{
		LOG_MSG(ERROR_MSG,dummy);
		return ERROR;
	}
	return OK;
}
long setslitCar()
{
	carVal = CAR_BUSY;
	return OK;
}
long getslitCar()
{
	return carVal;
}
