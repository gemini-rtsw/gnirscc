static struct {
    void *v;
    char *c;
} rcsid = {
    &rcsid,
    "$Id: xdispCad.c,v 1.2 2013/06/06 01:54:29 gemvx Exp $"
};

/*
 * Copyright 1997 Association of Universities for Research in 
 *	Astronomy, Inc.
 * See the file COPYRIGHT for more details.
 *
 * FILENAME
 * 	xdispCad.c
 *
 * DESCRIPTION 
 * 	xdispCad  support routines (prism motor EPICS support)
 *
 * 
 * FUNCTION NAME(S)
 *	xdispDatmCad
 *	xdispParkCad
 *	xdispStepsCad
 *	xdispPosCad
 *	xdispCtrl
 *   
 * DEPENDENCIES
 * 	EPICS support libraries
 *
 *
 *INDENT-OFF*
 * $Log: xdispCad.c,v $
 * Revision 1.2  2013/06/06 01:54:29  gemvx
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
 * Revision 1.1  2009/06/10 15:05:15  gemvx
 * Added Files:
 * epicsControl diectory populated.
 *
 *INDENT-ON* 
 */


/* VxWorks include files */
#include <vxWorks.h>
#include <taskLib.h>

/* EPICS specific include files */
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
int xdispId;

/* static variables*/
MSG_Q_ID xdispMotorQ;
/* this value will have the current state of the mechanism car*/
static carVal = CAR_IDLE;

/* Function prototypes of the functions in this file */
long xdispDatmCad( struct cadRecord* pCad);
long xdispParkCad( struct cadRecord* pCad);
long xdispStepsCad( struct cadRecord* pCad);
long xdispPosCad( struct cadRecord* pCad);
int xdispCtrl( int n1, int n2, int n3, int n4, int n5, int n6, 
			 int n7, int n8, int n9, int n10 );
#ifndef FINDMECH
int findMechName(int mech, char *name, char *pos);
#endif

/*
 *+
 * FUNCTION NAME:    xdispDatmCadInit
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
 *    create message queue and spawn task to deal with xdisp cad records
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

long xdispDatmCadInit( struct cadRecord* pCad )
{
    long status = CAD_ACCEPT;
    static int done = 0;

    LOG_MSG(DEBUG2_MSG,"xdispDatmCadInit:***********");
    /* park, datum, pos, and steps cads will call this, only do once*/
    if (!done)
    {
		/*create msg q*/
		LOG_MSG(DEBUG2_MSG,"create Msg Queue\n");
		xdispMotorQ = msgQCreate(4,sizeof(motorMsg),MSG_Q_FIFO);
		if(xdispMotorQ == NULL)
		{
			status = CAD_REJECT;
			strncpy(MESSAGE,"Init: Error creating messageQ",MAX_STRING_SIZE - 1);
		}
		/*spawn task*/
		LOG_MSG(DEBUG2_MSG,"spawn task\n");
		if(status == CAD_ACCEPT)
			xdispId = taskSpawn("tXdispCtrl",50,VX_FP_TASK,4000,xdispCtrl, 0,0,0,0,0,0,0,0,0,0);
		if(xdispId == ERROR)
		{
			status = CAD_REJECT;
			strncpy(MESSAGE,"Init: Error Spawning task",MAX_STRING_SIZE - 1);
		}
		if(status = CAD_ACCEPT)
			done = 1;
    }
    
	LOG_MSG(DEBUG2_MSG,"***********xdispDatmCadInit\n");
    return status;
}
long xdispParkCadInit( struct cadRecord* pCad )
{
    
    long status = CAD_ACCEPT;
	LOG_MSG(DEBUG2_MSG,"xdispParkCadInit:***********");
    
	LOG_MSG(DEBUG2_MSG,"***********xdispParkCadInit\n");
    return status;
}
long xdispPosCadInit( struct cadRecord* pCad )
{
    long status = CAD_ACCEPT;

  
	LOG_MSG(DEBUG2_MSG,"***********xdispPosCadInit\n");
    return status;
}
long xdispStepsCadInit( struct cadRecord* pCad )
{
   
    long status = CAD_ACCEPT;
	LOG_MSG(DEBUG2_MSG,"xdispStepsCadInit:***********");
  
	LOG_MSG(DEBUG2_MSG,"***********xdispStepsCadInit\n");
    return status;
}
/*
 *+
 * FUNCTION NAME:
 *	xdispCtrl
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
 *	Cad routine  releases semaphore which allows xdispCtrl to run.  
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

int xdispCtrl( int n1, int n2, int n3, int n4, int n5, int n6, 
			 int n7, int n8, int n9, int n10 )
{
    struct cadRecord* pCad;
    long status = VME_OK;
    char rMsg[MAX_STRING_SIZE];
    char dummy[MAX_STRING_SIZE];
    motorMsg msg;
  
  

  
    strncpy (rMsg,"Error in xdispCtrl\n",MAX_STRING_SIZE -1);
    /* Repeat as inifinite loop				*/
    while(1)
    {
		LOG_MSG(DEBUG2_MSG, "Task txdispCtrl sleeping...\n");
	
		/*wait for message from cad routines*/
		if( msgQReceive( xdispMotorQ, (char *)&msg, sizeof(motorMsg ), 
						 WAIT_FOREVER ) == ERROR ) 
		{
			sprintf(rMsg,"error in xdispCtrl msgReceive\n");
			LOG_MSG(ERROR_MSG,rMsg);
			carVal = CAR_ERROR;
			if(setCar(XDISP_CAR,carVal,ERROR,rMsg,dummy) != OK)
				LOG_MSG(ERROR_MSG,dummy);
			continue;
		}

		LOG_MSG(DEBUG2_MSG,"xdispCtrl:***********");
		pCad = msg.pCad;
	     
	
			/* send command to lower level to start	motion, 
			   this command blocks until completion*/
		status = moveMech(XDISP,rMsg);
		

		if(status == VME_OK)
		{
			/*set car back to idle*/
			carVal = CAR_IDLE;
			if(setCar(XDISP_CAR,carVal,OK,"",dummy) != OK)
				LOG_MSG(ERROR_MSG,dummy);
		}
		else
		{
			/*set car to error*/
			carVal = CAR_ERROR;
			if(setCar(XDISP_CAR,carVal,status,rMsg,dummy) != OK)
				LOG_MSG(ERROR_MSG,dummy);
			
		}

		LOG_MSG(DEBUG2_MSG,"***********xdispCtrl\n");

    }
    
    return status;
}
/*
 *+
 * FUNCTION NAME:
 *	xdispDatmCad
 *
 * INVOCATION:
 *	struct cadRecord* pCad;
 *	long status = 0;
 *	status = xdispDatmCad( pCad );
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
 *	Supports Gemini "xdispDatm" command, via CAD record
 *
 * DESCRIPTION:
 *	Inovked when xdispDatmCad record is processed by EPICS.  It 
 *	implements the "datum" command, which places the system in a 
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

long xdispDatmCad( struct cadRecord* pCad )
{
    motorMsg msg;
    long status = CAD_ACCEPT;

	LOG_MSG(DEBUG2_MSG,"xdispDatmCad:***********");

    /* Take appropriate action, based on directive type*/
    switch(DIRECTIVE )
    {
      case CAD_MARK:
		LOG_MSG(DEBUG2_MSG, 
				"xdispDatmCad - MARK directive");
	    strcpy(MESSAGE,"");
		break;

      case CAD_PRESET:	
		LOG_MSG(DEBUG2_MSG, 
				"xdispDatmCad - PRESET directive");

		/* send datum command to lower level code,
		 mechanism doesn't move until a start directive*/
		if(datumMech(XDISP)!= VME_OK)
		{
			sprintf(MESSAGE,"xdispDatmCad: ILLEGAL motor\n");
			status = CAD_REJECT;
		}
		break;

      case CAD_CLEAR:	
		LOG_MSG(DEBUG2_MSG, 
				"xdispDatmCad - CLEAR directive");
		/*clear cad msg field*/
		strcpy(MESSAGE,"");
		break;

      case CAD_START:	
		LOG_MSG(DEBUG2_MSG, 
				"xdispDatmCad - START directive");
		
		if (strcmp(pCad->h,DISABLED) == 0)
			{
				status = CAD_REJECT;
				sprintf(MESSAGE,"xdispPosCad - Xdisp Observe in progress");
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
					if(msgQSend(xdispMotorQ, (char *)&msg, sizeof(motorMsg),NO_WAIT, 
								MSG_PRI_NORMAL) != OK)
					{
						sprintf(MESSAGE,"xdispDatmCad: msgQSend error in \n");
						LOG_MSG(ERROR_MSG,MESSAGE);
						status = CAD_REJECT;
					}
				}
				else
				{
					sprintf(MESSAGE,"xdispDatmCad:  Xdisp is busy\n");
					LOG_MSG(ERROR_MSG,MESSAGE);
					status = CAD_REJECT;
				}
			}
		break;

      case CAD_STOP:   
		LOG_MSG(DEBUG2_MSG,
				"xdispDatmCad - STOP directive");
		if(carVal == CAR_BUSY)
		{
			abortMotor(XDISP);
		}
		else
		{
			sprintf(MESSAGE,"xdispDatmCad:  Xdisp is not busy\n");
			LOG_MSG(ERROR_MSG,MESSAGE);
			status = CAD_REJECT;	
		}
		
	
		break;

      default:	/* Unknown directive	*/
		strncpy(MESSAGE, "xdispDatmCad: Unrecognized directive",MAX_STRING_SIZE - 1);
		LOG_MSG(ERROR_MSG,MESSAGE);
		status = CAD_REJECT;
		break;
    } 
   
	LOG_MSG(DEBUG2_MSG,"***********xdispDatmCad\n");
    return status;
}

/*
 *+
 * FUNCTION NAME:
 *	xdispParkCad
 *
 * INVOCATION:
 *	struct cadRecord* pCad;
 *	long status = 0;
 *	status = xdispParkCad( pCad );
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
 *	Supports Gemini "xdispPark" command, via CAD record
 *
 * DESCRIPTION:
 *	Inovked when xdispParkCad record is processed by EPICS.  It 
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
long xdispParkCad( struct cadRecord* pCad)
{
   
    motorMsg msg;
    long status = CAD_ACCEPT;

    LOG_MSG(DEBUG2_MSG,"xdispParkCad:***********");

    /* Take appropriate action, based on directive type*/
    switch(DIRECTIVE )
    {
      case CAD_MARK:   
		LOG_MSG(DEBUG2_MSG,"xdispParkCad - MARK directive");

		break;

      case CAD_PRESET:	
		LOG_MSG(DEBUG2_MSG,"xdispParkCad - PRESET directive");
		/* check datumed before trying to move motors*/
		if(!atoi(pCad->f))
		{
			status = CAD_REJECT;
			sprintf(MESSAGE,"xdispParkCad: xdisp mechanism not datumed\n");
			LOG_MSG(ERROR_MSG, MESSAGE);
		}
		else
			if(parkMech(XDISP)!= VME_OK)
			{
				sprintf(MESSAGE,"xdispParkCad: Illegal motor\n");
				LOG_MSG(ERROR_MSG, MESSAGE);
				status = CAD_REJECT;
			}
		break;

      case CAD_CLEAR:	
		LOG_MSG(DEBUG2_MSG, "xdispParkCad - CLEAR directive");
		/*clear cad msg field*/
		strcpy(MESSAGE,"");
		break;

      case CAD_START:	
		LOG_MSG(DEBUG2_MSG,"xdispParkCad - START directive");

		if (strcmp(pCad->h,DISABLED) == 0)
		{
			status = CAD_REJECT;
			sprintf(MESSAGE,"xdispPosCad - Xdisp Observe in progress");
			LOG_MSG(ERROR_MSG, MESSAGE);	
		}
		else
		{
			/*set car to busy (ctrl task will set back to idle)*/
			carVal = CAR_BUSY;	
			
			LOG_MSG(DEBUG2_MSG,"Waking park\n");
			msg.op = PARK;
			msg.pCad = pCad;
			if(msgQSend(xdispMotorQ, (char *)&msg, sizeof(motorMsg),NO_WAIT, 
						MSG_PRI_NORMAL) != OK)
			{
				status = CAD_REJECT;
				sprintf(MESSAGE,"msgQSend error in xdispDatmCad\n");
				LOG_MSG(ERROR_MSG,MESSAGE);
			}	
		}
		break;

      case CAD_STOP:	
		LOG_MSG(DEBUG2_MSG,"xdispParkCad - STOP directive");
		/*call stop command*/
	if(carVal == CAR_BUSY)
		{
			abortMotor(XDISP);
		}
		else
		{
			sprintf(MESSAGE,"xdispParkCad:  Xdisp is not busy\n");
			LOG_MSG(ERROR_MSG,MESSAGE);
			status = CAD_REJECT;	
		}
		 
		status = CAD_ACCEPT;
		break;

      default:	/* Unknown directive	*/
		strncpy(MESSAGE, "xdispParkCad: Unrecognized directive",MAX_STRING_SIZE - 1);
		LOG_MSG(ERROR_MSG,MESSAGE);
		status = CAD_REJECT;
		break;
    } 
   
	LOG_MSG(DEBUG2_MSG,"***********xdispParkCad\n");
    return status;
}

/*
 *+
 * FUNCTION NAME:
 *	xdispStepsCad
 *
 * INVOCATION:
 *	struct cadRecord* pCad;
 *	long status = 0;
 *	status = xdispStepsCad( pCad );
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
 *	Supports Gemini "xdispSteps" command, via CAD record
 *
 * DESCRIPTION:
 *	Inovked when xdispStepsCad record is processed by EPICS.  It 
 *	implements the "steps" command, which places the system in a 
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
long xdispStepsCad( struct cadRecord* pCad )
{
   
    motorMsg msg;
    long status = CAD_ACCEPT;
  

	LOG_MSG(DEBUG2_MSG,"xdispStepsCad:***********");
    /* Take appropriate action, based on directive type*/
    switch(DIRECTIVE )
    {
      case CAD_MARK:
		LOG_MSG(DEBUG2_MSG,"xdispStepsCad - MARK directive");
		break;

      case CAD_PRESET:	
		LOG_MSG(DEBUG2_MSG,"xdispStepsCad - PRESET directive");

		/* check datumed before*/
		if(!atoi(pCad->f))
		{
			status = CAD_REJECT;
      	    strncpy(MESSAGE,"xdispStepsCad - Xdisp not Datumed",39);
			LOG_MSG(ERROR_MSG, MESSAGE);
		}
		else
		{
			if(setEngPos(XDISP,atoi(cadInput(STEPSCAD)))!= VME_OK)
			{
				strcpy(MESSAGE,gnirsErrorMessage);
				status = CAD_REJECT;
			}
		}
		break;
      case CAD_CLEAR:
		LOG_MSG(DEBUG2_MSG,"xdispStepsCad - CLEAR directive");
		/*clear cad msg field*/
	    strcpy(MESSAGE,"");
		break;

      case CAD_START:	
		LOG_MSG(DEBUG2_MSG,"xdispStepsCad - START directive");
	
		if (strcmp(pCad->h,DISABLED) == 0)
		{
			status = CAD_REJECT;
			sprintf(MESSAGE,"xdispPosCad - Xdisp Observe in progress");
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
			
			if(msgQSend(xdispMotorQ, (char *)&msg, sizeof(motorMsg),NO_WAIT, 
						MSG_PRI_NORMAL) != OK)
			{
				sprintf(MESSAGE,"msgQSend error in xdispStepsCad\n");
				LOG_MSG(ERROR_MSG,MESSAGE);
				status = CAD_REJECT;
			}
		}
	
		break;

      case CAD_STOP:
		LOG_MSG(DEBUG2_MSG,"xdispStepsCad - STOP directive");
			if(carVal == CAR_BUSY)
		{
			abortMotor(XDISP);
		}
		else
		{
			sprintf(MESSAGE,"xdispStepsCad:  Xdisp is not busy\n");
			LOG_MSG(ERROR_MSG,MESSAGE);
			status = CAD_REJECT;	
		}
		   
		status = CAD_ACCEPT;
		break;

      default:	/* Unknown directive	*/
		strncpy(MESSAGE, "xdispStepsCad: Unrecognized directive",MAX_STRING_SIZE - 1);
		status = CAD_REJECT;
		break;
    } 
   
	LOG_MSG(DEBUG2_MSG,"***********xdispStepsCad\n");
    return status;
}

/*
 *+
 * FUNCTION NAME:
 *	xdispPosCad
 *
 * INVOCATION:
 *	struct cadRecord* pCad;
 *	long status = 0;
 *	status = xdispPosCad( pCad );
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
 *	Supports Gemini "xdispPos" command, via CAD record
 *
 * DESCRIPTION:
 *	Inovked when xdispPosCad record is processed by EPICS.  It 
 *	implements the "pos" command, which places the system in a 
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
long xdispPosCad( struct cadRecord* pCad )
{
   
	char pos[POS_LEN];
    motorMsg msg;
    long status = CAD_ACCEPT;
  

	LOG_MSG(DEBUG2_MSG,"xdispPosCad:***********");
    /* Take appropriate action, based on directive type*/
    switch(DIRECTIVE )
    {
      case CAD_MARK:
		LOG_MSG(DEBUG2_MSG,"xdispPosCad - MARK directive");
		break;

      case CAD_PRESET:	
		LOG_MSG(DEBUG2_MSG,"xdispPosCad - PRESET directive");
		if(!atoi(pCad->f))
		{
			status = CAD_REJECT;
      	    sprintf(MESSAGE,"xdispPosCad - Xdisp not Datumed");
			LOG_MSG(ERROR_MSG, MESSAGE);
		}
		else
		{
			printf("name = %s\n",pCad->b);
#ifndef FINDMECH
			if(findMechName(XDISP,pCad->b,pos))
			{
#endif
				if(valid(XDISP,pCad->b)!= VME_OK)
				{
					sprintf(MESSAGE,gnirsErrorMessage);
					status = CAD_REJECT;
				}

#ifndef FINDMECH
			}
			else
			{
				sprintf(MESSAGE,"%s not found in database for xdisp\n",pCad->b);
				status = CAD_REJECT;
			}
#endif
		}
		break;

      case CAD_CLEAR:
		LOG_MSG(DEBUG2_MSG, 
				"xdispPosCad - CLEAR directive");
	    strcpy(MESSAGE,"");
		break;

      case CAD_START:
		LOG_MSG(DEBUG2_MSG,"xdispPosCad - START directive");

		if (strcmp(pCad->h,DISABLED) == 0)
		{
			status = CAD_REJECT;
			sprintf(MESSAGE,"xdispPosCad - Xdisp Observe in progress");
			LOG_MSG(ERROR_MSG, MESSAGE);	
		}
		else
		{
			/*copy inputs to outputs*/	   
			status = assignVal(type(POSCAD), cadInput(POSCAD), cadOutput(POSCAD), MESSAGE);
			
			/*set car to busy (ctrl task will set back to idle)*/	  
			carVal = CAR_BUSY; 
			
			/*send message to ctrl task*/
			DPRINT(DPdebug,DEBUG2_MSG,"Waking xdispCtrl\n");
			msg.op = POS;
			msg.pCad = pCad;	
			if(msgQSend(xdispMotorQ, (char *)&msg, sizeof(motorMsg),NO_WAIT, 
						MSG_PRI_NORMAL) != OK)
			{
				sprintf(MESSAGE,"xdispPosCad: Error in msgQSend ");
				status = CAD_REJECT;
			}	
		}
		break;

      case CAD_STOP:	
		LOG_MSG(DEBUG2_MSG,
				"xdispPosCad - STOP directive");
		if(carVal == CAR_BUSY)
		{
			abortMotor(XDISP);
		}
		else
		{
			sprintf(MESSAGE,"xdispPosCad:  Xdisp is not busy\n");
			LOG_MSG(ERROR_MSG,MESSAGE);
			status = CAD_REJECT;	
		}
		 
		status = CAD_ACCEPT;
		break;

      default:	/* Unknown directive	*/
		strncpy(MESSAGE, "xdispPosCad: Unrecognized directive",
				MAX_STRING_SIZE - 1);
		status = CAD_REJECT;
		break;
    } 
   
	LOG_MSG(DEBUG2_MSG,"***********xdispPosCad\n");
    return status;
}

long setxdispCarBusy()
{
	carVal = CAR_BUSY;
	if(setCar(XDISP_CAR,carVal,OK,"",dummy) != OK)
		LOG_MSG(ERROR_MSG,dummy);
	return OK;
}
long setxdispCar()
{
	carVal = CAR_BUSY;
	return OK;
}
long getxdispCar()
{
	return carVal;
  
}
