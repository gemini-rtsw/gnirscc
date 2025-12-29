static struct {
    void *v;
    char *c;
} rcsid = {
    &rcsid,
    "$Id: coverCad.c,v 1.2 2013/06/06 01:54:27 gemvx Exp $"
};
/*
 * Copyright 1997 Association of Universities for Research in 
 *	Astronomy, Inc.
 * See the file COPYRIGHT for more details.
 *
 * FILENAME
 * 	coverCad.c
 *
 * DESCRIPTION 
 * 	coverCad  support routines
 *
 * 
 * FUNCTION NAME(S)
 *	coverDatmCad
 *	coverParkCad
 *	coverStepsCad
 *	coverPosCad
 *	coverCtrl
 *   
 * DEPENDENCIES
 * 	EPICS support libraries
 *
 *
 *INDENT-OFF*
 * $Log: coverCad.c,v $
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
MSG_Q_ID coverMotorQ;
int coverId;

/* this value will have the current state of the car*/
static carVal = CAR_IDLE;

/* Forward declarations of the functions in this file */
long coverDatmCad( struct cadRecord* pCad);

long coverParkCad( struct cadRecord* pCad);

long coverStepsCad( struct cadRecord* pCad);

long coverPosCad( struct cadRecord* pCad);

int coverCtrl( int n1, int n2, int n3, int n4, int n5, int n6, 
			 int n7, int n8, int n9, int n10 );

#ifndef FINDMECH
int findMechName(int mech, char *name, char *pos);
#endif
int val[10];
/*
 *+
 * FUNCTION NAME:    cover????CadInit
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
 *    create message queue and spawn task to deal with cover cad records
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
long coverDatmCadInit( struct cadRecord* pCad )
{
    long status = CAD_ACCEPT;
    static int done = 0;

    LOG_MSG(DEBUG2_MSG,"coverDatmCadInit:***********");
    /* park, datum, pos, and steps cads will call this, only do once*/
    if (!done)
    {
		/*create msg q*/
		LOG_MSG(DEBUG2_MSG,"create Msg Queue\n");
		coverMotorQ = msgQCreate(4,sizeof(motorMsg),MSG_Q_FIFO);
		if(coverMotorQ == NULL)
		{
			status = CAD_REJECT;
			strncpy(MESSAGE,"Init: Error creating messageQ",MAX_STRING_SIZE - 1);
		}
		/*spawn task*/
		LOG_MSG(DEBUG2_MSG,"spawn task\n");
		if(status == CAD_ACCEPT)
			coverId = taskSpawn("tCoverCtrl",50,VX_FP_TASK,4000,coverCtrl, 0,0,0,0,0,0,0,0,0,0);
		if(coverId == ERROR)
		{
			status = CAD_REJECT;
			strncpy(MESSAGE,"Init: Error Spawning task",MAX_STRING_SIZE - 1);
		}
		if(status = CAD_ACCEPT)
			done = 1;
    }
    
	LOG_MSG(DEBUG2_MSG,"***********coverDatmCadInit\n");
    return status;
}
long coverParkCadInit( struct cadRecord* pCad )
{
    
    long status = CAD_ACCEPT;
	LOG_MSG(DEBUG2_MSG,"coverParkCadInit:***********");
    
	LOG_MSG(DEBUG2_MSG,"***********coverParkCadInit\n");
    return status;
}
long coverPosCadInit( struct cadRecord* pCad )
{
    long status = CAD_ACCEPT;

	LOG_MSG(DEBUG2_MSG,"***********coverPosCadInit\n");
    return status;
}
long coverStepsCadInit( struct cadRecord* pCad )
{
   
    long status = CAD_ACCEPT;
	LOG_MSG(DEBUG2_MSG,"coverStepsCadInit:***********");
	LOG_MSG(DEBUG2_MSG,"***********coverStepsCadInit\n");
    return status;
}
/*
 *+
 * FUNCTION NAME:
 *	coverCtrl
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
 *	Cad record releases semaphore which allows coverCtrl to run.  
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

int coverCtrl( int n1, int n2, int n3, int n4, int n5, int n6, 
			 int n7, int n8, int n9, int n10 )
{
    struct cadRecord* pCad;
    long status = VME_OK;
    char rMsg[MAX_STRING_SIZE];
    char dummy[MAX_STRING_SIZE];
    motorMsg msg;
   
    

  
    strncpy (rMsg,"Error in coverCtrl\n",MAX_STRING_SIZE -1);
    /* Repeat as inifinite loop				*/
    while(1)
    {
		LOG_MSG(DEBUG2_MSG, "Task tcoverCtrl sleeping...\n");
	
		if( msgQReceive( coverMotorQ, (char *)&msg, sizeof(motorMsg ), 
						 WAIT_FOREVER ) == ERROR ) 
		{
			sprintf(rMsg,"error in coverCtrl msgReceive\n");
			LOG_MSG(ERROR_MSG,rMsg);
			carVal = CAR_ERROR;
			if(setCar(COVER_CAR,carVal,ERROR,rMsg,dummy) != OK)
				LOG_MSG(ERROR_MSG,dummy);

			continue;
		}

		LOG_MSG(DEBUG2_MSG,"coverCtrl:***********");
		pCad = msg.pCad;
	
		LOG_MSG(DEBUG2_MSG,"coverCtrl: SIM_NONE\n");
	    
		status = moveMech(COVER,rMsg);
	
		if(status == VME_OK)
		{
			carVal = CAR_IDLE;
			if(setCar(COVER_CAR,carVal,OK,"",dummy) != OK)
				LOG_MSG(ERROR_MSG,dummy);
		}
		else
		{
			carVal = CAR_ERROR;
			if(setCar(COVER_CAR,carVal,status,rMsg,dummy) != OK)
				LOG_MSG(ERROR_MSG,dummy);
			
		}

		LOG_MSG(DEBUG2_MSG,"***********coverCtrl\n");

    }
    
    return status;
}
/*
 *+
 * FUNCTION NAME:
 *	coverDatmCad
 *
 * INVOCATION:
 *	struct cadRecord* pCad;
 *	long status = 0;
 *	status = coverDatmCad( pCad );
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
 *	Supports Gemini "coverDatm" command, via CAD record
 *
 * DESCRIPTION:
 *	Inovked when coverDatmCad record is processed by EPICS.  It 
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

long coverDatmCad( struct cadRecord* pCad )
{
    motorMsg msg;
    long status = CAD_ACCEPT;

	LOG_MSG(DEBUG2_MSG,"coverDatmCad:***********");
    /* Take appropriate action, based on directive type*/
    switch(DIRECTIVE )
    {
      case CAD_MARK:
		LOG_MSG(DEBUG2_MSG, 
				"coverDatmCad - MARK directive");
	    strcpy(MESSAGE,"");
		break;

      case CAD_PRESET:	
		LOG_MSG(DEBUG2_MSG, 
				"coverDatmCad - PRESET directive");

		if(datumMech(COVER)!= VME_OK)
		{
			sprintf(MESSAGE,"coverDatmCad: ILLEGAL motor\n");
			status = CAD_REJECT;
		}
		break;

      case CAD_CLEAR:	
		LOG_MSG(DEBUG2_MSG, 
				"coverDatmCad - CLEAR directive");
		/*clear cad msg field*/
		strcpy(MESSAGE,"");
		break;

      case CAD_START:	
		LOG_MSG(DEBUG2_MSG, 
				"coverDatmCad - START directive");

		if (strcmp(pCad->h,DISABLED) == 0)
		{
			status = CAD_REJECT;
			sprintf(MESSAGE,"coverPosCad - Cover Observe in progress");
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
				if(msgQSend(coverMotorQ, (char *)&msg, sizeof(motorMsg),NO_WAIT, 
							MSG_PRI_NORMAL) != OK)
				{
					sprintf(MESSAGE,"msgQSend error in coverDatmCad\n");
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
		}
		break;

      case CAD_STOP:   
		LOG_MSG(DEBUG2_MSG,
				"coverDatmCad - STOP directive");
		if(carVal == CAR_BUSY)
		{
			abortMotor(COVER);
		}
		else
		{
			sprintf(MESSAGE,"coverDatmCad:  Cover is not busy\n");
			LOG_MSG(ERROR_MSG,MESSAGE);
			status = CAD_REJECT;	
		}
		/*check if car is busy and then call  abortMotor*/
	
		break;

      default:	/* Unknown directive	*/
		strncpy(MESSAGE, "coverDatmCad: Unrecognized directive",MAX_STRING_SIZE - 1);
		LOG_MSG(ERROR_MSG,MESSAGE);
		status = CAD_REJECT;
		break;
    } 
   
	LOG_MSG(DEBUG2_MSG,"***********coverDatmCad\n");
    return status;
}

/*
 *+
 * FUNCTION NAME:
 *	coverParkCad
 *
 * INVOCATION:
 *	struct cadRecord* pCad;
 *	long status = 0;
 *	status = coverParkCad( pCad );
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
 *	Supports Gemini "coverPark" command, via CAD record
 *
 * DESCRIPTION:
 *	Inovked when coverParkCad record is processed by EPICS.  It 
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
long coverParkCad( struct cadRecord* pCad)
{
   
    motorMsg msg;
    long status = CAD_ACCEPT;

    LOG_MSG(DEBUG2_MSG,"coverParkCad:***********");
    /* Take appropriate action, based on directive type*/
    switch(DIRECTIVE )
    {
      case CAD_MARK:   
		LOG_MSG(DEBUG2_MSG,"coverParkCad - MARK directive");

		break;

      case CAD_PRESET:	
		LOG_MSG(DEBUG2_MSG,"coverParkCad - PRESET directive");
		/* check datumed before trying to move motors*/
		if(!atoi(pCad->f))
		{
			status = CAD_REJECT;
			sprintf(MESSAGE,"cover mechanism not datumed\n");
			LOG_MSG(ERROR_MSG, MESSAGE);
		}
		else
			if(parkMech(COVER)!= VME_OK)
			{
				sprintf(MESSAGE,"Illegal motor\n");
				LOG_MSG(ERROR_MSG, MESSAGE);
				status = CAD_REJECT;
			}
		break;

      case CAD_CLEAR:	
		LOG_MSG(DEBUG2_MSG, "coverParkCad - CLEAR directive");
		/*clear cad msg field*/
		strcpy(MESSAGE,"");
		break;

      case CAD_START:	
		LOG_MSG(DEBUG2_MSG,"coverParkCad - START directive");

		if (strcmp(pCad->h,DISABLED) == 0)
		{
			status = CAD_REJECT;
			sprintf(MESSAGE,"coverPosCad - Cover Observe in progress");
			LOG_MSG(ERROR_MSG, MESSAGE);    
		}
		else
		{
			/*set car to busy (ctrl task will set back to idle)*/
			carVal = CAR_BUSY;
			
			LOG_MSG(DEBUG2_MSG,"Waking park\n");
			msg.op = PARK;
			msg.pCad = pCad;
			if(msgQSend(coverMotorQ, (char *)&msg, sizeof(motorMsg),NO_WAIT, 
						MSG_PRI_NORMAL) != OK)
			{
				status = CAD_REJECT;
				sprintf(MESSAGE,"msgQSend error in coverDatmCad\n");
				LOG_MSG(ERROR_MSG,MESSAGE);
			}	
		}
		break;

      case CAD_STOP:	
		LOG_MSG(DEBUG2_MSG,"coverParkCad - STOP directive");
		/*call stop command*/
	if(carVal == CAR_BUSY)
		{
			abortMotor(COVER);
		}
		else
		{
			sprintf(MESSAGE,"coverParkCad:  Cover is not busy\n");
			LOG_MSG(ERROR_MSG,MESSAGE);
			status = CAD_REJECT;	
		}
		status = CAD_ACCEPT;
		break;

      default:	/* Unknown directive	*/
		strncpy(MESSAGE, "coverParkCad: Unrecognized directive",MAX_STRING_SIZE - 1);
		LOG_MSG(ERROR_MSG,MESSAGE);
		status = CAD_REJECT;
		break;
    } 
   
	LOG_MSG(DEBUG2_MSG,"***********coverParkCad\n");
    return status;
}

/*
 *+
 * FUNCTION NAME:
 *	coverStepsCad
 *
 * INVOCATION:
 *	struct cadRecord* pCad;
 *	long status = 0;
 *	status = coverStepsCad( pCad );
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
 *	Supports Gemini "coverSteps" command, via CAD record
 *
 * DESCRIPTION:
 *	Inovked when coverStepsCad record is processed by EPICS.  It 
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
long coverStepsCad( struct cadRecord* pCad )
{
   
    motorMsg msg;
    long status = CAD_ACCEPT;
  

	LOG_MSG(DEBUG2_MSG,"coverStepsCad:***********");
    /* Take appropriate action, based on directive type*/
    switch(DIRECTIVE )
    {
      case CAD_MARK:
		LOG_MSG(DEBUG2_MSG,"coverStepsCad - MARK directive");
		break;

      case CAD_PRESET:	
		LOG_MSG(DEBUG2_MSG,"coverStepsCad - PRESET directive");

		/* check datumed before*/
		if(!atoi(pCad->f))
		{
			status = CAD_REJECT;
      	    strncpy(MESSAGE,"coverStepsCad - Cover not Datumed",39);
			LOG_MSG(ERROR_MSG, MESSAGE);
		}
		else
		{
			if(setEngPos(COVER,atoi(cadInput(STEPSCAD)))!= VME_OK)
			{
				strcpy(MESSAGE,gnirsErrorMessage);
				status = CAD_REJECT;
			}
		}
		break;
      case CAD_CLEAR:
		LOG_MSG(DEBUG2_MSG,"coverStepsCad - CLEAR directive");
		/*clear cad msg field*/
	    strcpy(MESSAGE,"");
		break;

      case CAD_START:	
		LOG_MSG(DEBUG2_MSG,"coverStepsCad - START directive");
	
		if (strcmp(pCad->h,DISABLED) == 0)
		{
			status = CAD_REJECT;
			sprintf(MESSAGE,"coverPosCad - Cover Observe in progress");
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
			
			if(msgQSend(coverMotorQ, (char *)&msg, sizeof(motorMsg),NO_WAIT, 
						MSG_PRI_NORMAL) != OK)
			{
				sprintf(MESSAGE,"msgQSend error in coverStepsCad\n");
				LOG_MSG(ERROR_MSG,MESSAGE);
				status = CAD_REJECT;
			}
		}
	
		break;

      case CAD_STOP:
		LOG_MSG(DEBUG2_MSG,"coverStepsCad - STOP directive");
			if(carVal == CAR_BUSY)
		{
			abortMotor(COVER);
		}
		else
		{
			sprintf(MESSAGE,"coverStepsCad:  Cover is not busy\n");
			LOG_MSG(ERROR_MSG,MESSAGE);
			status = CAD_REJECT;	
		}
		status = CAD_ACCEPT;
		break;

      default:	/* Unknown directive	*/
		strncpy(MESSAGE, "coverStepsCad: Unrecognized directive",MAX_STRING_SIZE - 1);
		status = CAD_REJECT;
		break;
    } 
   
	LOG_MSG(DEBUG2_MSG,"***********coverStepsCad\n");
    return status;
}

/*
 *+
 * FUNCTION NAME:
 *	coverPosCad
 *
 * INVOCATION:
 *	struct cadRecord* pCad;
 *	long status = 0;
 *	status = coverPosCad( pCad );
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
 *	Supports Gemini "coverPos" command, via CAD record
 *
 * DESCRIPTION:
 *	Inovked when coverPosCad record is processed by EPICS.  It 
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
long coverPosCad( struct cadRecord* pCad )
{
   
	char pos[POS_LEN];
    motorMsg msg;
    long status = CAD_ACCEPT;
  

	LOG_MSG(DEBUG2_MSG,"coverPosCad:***********");
    /* Take appropriate action, based on directive type*/
    switch(DIRECTIVE )
    {
      case CAD_MARK:
		LOG_MSG(DEBUG2_MSG,"coverPosCad - MARK directive");
		break;

      case CAD_PRESET:	
		LOG_MSG(DEBUG2_MSG,"coverPosCad - PRESET directive");
		if(!atoi(pCad->f))
		{
			status = CAD_REJECT;
      	    sprintf(MESSAGE,"coverPosCad - Cover not Datumed");
			LOG_MSG(ERROR_MSG, MESSAGE);
		}
		else
		{
			printf("name = %s\n",pCad->b);
#ifndef FINDMECH
			if(findMechName(COVER,pCad->b,pos))
			{
#endif
				if(valid(COVER,pCad->b)!= VME_OK)
				{
					sprintf(MESSAGE,gnirsErrorMessage);
					status = CAD_REJECT;
				}
#ifndef FINDMECH
			}
			else
			{
				sprintf(MESSAGE,"%s not found in database for cover\n",pCad->b);
				status = CAD_REJECT;
			}
#endif
		}
		break;

      case CAD_CLEAR:
		LOG_MSG(DEBUG2_MSG, 
				"coverPosCad - CLEAR directive");
	    strcpy(MESSAGE,"");
		break;

      case CAD_START:
		LOG_MSG(DEBUG2_MSG,"coverPosCad - START directive");
		if (strcmp(pCad->h,DISABLED) == 0)
		{
			status = CAD_REJECT;
			sprintf(MESSAGE,"coverPosCad - Cover Observe in progress");
			LOG_MSG(ERROR_MSG, MESSAGE);    
		}
		else
		{
			/*copy inputs to outputs*/
			status = assignVal(type(POSCAD), cadInput(POSCAD), cadOutput(POSCAD), MESSAGE);
			
			/*set car to busy (ctrl task will set back to idle)*/	  
			carVal = CAR_BUSY; 
			
			/*send message to ctrl task*/
			DPRINT(DPdebug,DEBUG2_MSG,"Waking coverCtrl\n");
			msg.op = POS;
			msg.pCad = pCad;	
			if(msgQSend(coverMotorQ, (char *)&msg, sizeof(motorMsg),NO_WAIT, 
						MSG_PRI_NORMAL) != OK)
			{
				sprintf(MESSAGE,"coverPosCad: Error in msgQSend ");
				status = CAD_REJECT;
			}	
		}
		break;

      case CAD_STOP:	
		LOG_MSG(DEBUG2_MSG,
				"coverPosCad - STOP directive");
			if(carVal == CAR_BUSY)
		{
			abortMotor(COVER);
		}
		else
		{
			sprintf(MESSAGE,"coverPosCad:  Cover is not busy\n");
			LOG_MSG(ERROR_MSG,MESSAGE);
			status = CAD_REJECT;	
		}
		status = CAD_ACCEPT;
		break;

      default:	/* Unknown directive	*/
		strncpy(MESSAGE, "coverPosCad: Unrecognized directive",
				MAX_STRING_SIZE - 1);
		status = CAD_REJECT;
		break;
    } 
   
	LOG_MSG(DEBUG2_MSG,"***********coverPosCad\n");
    return status;
}

long setcoverCarBusy()
{
	carVal = CAR_BUSY;
	if(setCar(COVER_CAR,carVal,OK,"",dummy) != OK)
	{
		LOG_MSG(ERROR_MSG,dummy);
		return ERROR;
	}
	return OK;
}
long getcoverCar()
{
	return carVal;
}
