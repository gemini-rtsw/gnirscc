static struct {
    void *v;
    char *c;
} rcsid = {
    &rcsid,
    "$Id: gratingCad.c,v 1.2 2013/06/06 01:54:28 gemvx Exp $"
};
/*
 * Copyright 1997 Association of Universities for Research in 
 *	Astronomy, Inc.
 * See the file COPYRIGHT for more details.
 *
 * FILENAME
 * 	gratingCad.c
 *
 * DESCRIPTION 
 * 	gratingCad  support routines
 *
 * 
 * FUNCTION NAME(S)
 *	gratingDatmCad
 *	gratingParkCad
 *	gratingStepsCad
 *	gratingPosCad
 *	gratingCtrl
 *   
 * DEPENDENCIES
 * 	EPICS support libraries
 *
 *
 *INDENT-OFF*
 * $Log: gratingCad.c,v $
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
#include "gnirsCCDefines.h"

/* Include file needed for control tasks */
#include <sysLib.h>
#include <car.h>
#include "mechNames.h"

/*Global variables*/
MSG_Q_ID gratingMotorQ;
int gratingId;

/* this value will have the current state of the car*/
static carVal = CAR_IDLE;

/* Forward declarations of the functions in this file */
long gratingDatmCad( struct cadRecord* pCad);

long gratingParkCad( struct cadRecord* pCad);

long gratingStepsCad( struct cadRecord* pCad);

long gratingPosCad( struct cadRecord* pCad);

int gratingCtrl( int n1, int n2, int n3, int n4, int n5, int n6, 
			 int n7, int n8, int n9, int n10 );

/*
 *+
 * FUNCTION NAME:    grating????CadInit
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
 *    create message queue and spawn task to deal with grating cad records
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

long gratingDatmCadInit( struct cadRecord* pCad )
{
    long status = CAD_ACCEPT;
    static int done = 0;

    LOG_MSG(DEBUG2_MSG,"gratingDatmCadInit:***********");
    /* park, datum, pos, and steps cads will call this, only do once*/
    if (!done)
    {
		/*create msg q*/
		LOG_MSG(DEBUG2_MSG,"create Msg Queue\n");
		gratingMotorQ = msgQCreate(4,sizeof(motorMsg),MSG_Q_FIFO);
		if(gratingMotorQ == NULL)
		{
			status = CAD_REJECT;
			strncpy(MESSAGE,"Init: Error creating messageQ",MAX_STRING_SIZE - 1);
		}
		/*spawn task*/
		LOG_MSG(DEBUG2_MSG,"spawn task\n");
		if(status == CAD_ACCEPT)
			gratingId = taskSpawn("tGratingCtrl",50,VX_FP_TASK,4000,gratingCtrl, 0,0,0,0,0,0,0,0,0,0);
		if(gratingId == ERROR)
		{
			status = CAD_REJECT;
			strncpy(MESSAGE,"Init: Error Spawning task",MAX_STRING_SIZE - 1);
		}
		if(status = CAD_ACCEPT)
			done = 1;
    }
    
	LOG_MSG(DEBUG2_MSG,"***********gratingDatmCadInit\n");
    return status;
}
long gratingParkCadInit( struct cadRecord* pCad )
{
    
    long status = CAD_ACCEPT;
	LOG_MSG(DEBUG2_MSG,"gratingParkCadInit:***********");
    
	LOG_MSG(DEBUG2_MSG,"***********gratingParkCadInit\n");
    return status;
}
long gratingPosCadInit( struct cadRecord* pCad )
{
    long status = CAD_ACCEPT;

	LOG_MSG(DEBUG2_MSG,"***********gratingPosCadInit\n");
    return status;
}
long gratingStepsCadInit( struct cadRecord* pCad )
{
   
    long status = CAD_ACCEPT;
	LOG_MSG(DEBUG2_MSG,"gratingStepsCadInit:***********");
	LOG_MSG(DEBUG2_MSG,"***********gratingStepsCadInit\n");
    return status;
}
/*
 *+
 * FUNCTION NAME:
 *	gratingCtrl
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
 *	Cad record releases semaphore which allows gratingCtrl to run.  
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

int gratingCtrl( int n1, int n2, int n3, int n4, int n5, int n6, 
			 int n7, int n8, int n9, int n10 )
{
    struct cadRecord* pCad;
    long status = VME_OK;
    char rMsg[MAX_STRING_SIZE];
    char dummy[MAX_STRING_SIZE];
    motorMsg msg;
 
    strncpy (rMsg,"Error in gratingCtrl\n",MAX_STRING_SIZE -1);
    /* Repeat as inifinite loop				*/
    while(1)
    {
		LOG_MSG(DEBUG2_MSG, "Task tgratingCtrl sleeping...\n");
	
		if( msgQReceive( gratingMotorQ, (char *)&msg, sizeof(motorMsg ), 
						 WAIT_FOREVER ) == ERROR ) 
		{
			sprintf(rMsg,"error in gratingCtrl msgReceive\n");
			LOG_MSG(ERROR_MSG,rMsg);
			carVal = CAR_ERROR;
			if(setCar(GRATING_CAR,carVal,ERROR,rMsg,dummy) != OK)
				LOG_MSG(ERROR_MSG,dummy);
			continue;
		}

		LOG_MSG(DEBUG2_MSG,"gratingCtrl:***********");
		pCad = msg.pCad;
	     
	
			status = moveMech(GRATING,rMsg);
		
		if(status == VME_OK)
		{
			carVal = CAR_IDLE;
			if(setCar(GRATING_CAR,carVal,OK,"",dummy) != OK)
				LOG_MSG(ERROR_MSG,dummy);
		}
		else
		{
			carVal = CAR_ERROR;
			if(setCar(GRATING_CAR,carVal,status,rMsg,dummy) != OK)
				LOG_MSG(ERROR_MSG,dummy);
			
		}

		LOG_MSG(DEBUG2_MSG,"***********gratingCtrl\n");

    }
    
    return status;
}
/*
 *+
 * FUNCTION NAME:
 *	gratingDatmCad
 *
 * INVOCATION:
 *	struct cadRecord* pCad;
 *	long status = 0;
 *	status = gratingDatmCad( pCad );
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
 *	Supports Gemini "gratingDatm" command, via CAD record
 *
 * DESCRIPTION:
 *	Inovked when gratingDatmCad record is processed by EPICS.  It 
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

long gratingDatmCad( struct cadRecord* pCad )
{
    motorMsg msg;
    long status = CAD_ACCEPT;

	LOG_MSG(DEBUG2_MSG,"gratingDatmCad:***********");
    /* Take appropriate action, based on directive type*/
    switch(DIRECTIVE )
    {
      case CAD_MARK:
		LOG_MSG(DEBUG2_MSG, 
				"gratingDatmCad - MARK directive");
	    strcpy(MESSAGE,"");
		break;

      case CAD_PRESET:	
		LOG_MSG(DEBUG2_MSG, 
				"gratingDatmCad - PRESET directive");

		if(datumMech(GRATING)!= VME_OK)
		{
			sprintf(MESSAGE,"gratingDatmCad: ILLEGAL motor\n");
			status = CAD_REJECT;
		}
		break;

      case CAD_CLEAR:	
		LOG_MSG(DEBUG2_MSG, 
				"gratingDatmCad - CLEAR directive");
		/*clear cad msg field*/
		strcpy(MESSAGE,"");
		break;

      case CAD_START:	
		LOG_MSG(DEBUG2_MSG, 
				"gratingDatmCad - START directive");

		if (strcmp(pCad->h,DISABLED) == 0)
		{
			status = CAD_REJECT;
			sprintf(MESSAGE,"gratingPosCad - Grating Observe in progress");
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
		}
		break;

      case CAD_STOP:   
		LOG_MSG(DEBUG2_MSG,
				"gratingDatmCad - STOP directive");
		if(carVal == CAR_BUSY)
		{
			abortMotor(GRATING);
		}
		else
		{
			sprintf(MESSAGE,"gratingDatmCad:  Grating is not busy\n");
			LOG_MSG(ERROR_MSG,MESSAGE);
			status = CAD_REJECT;	
		}
		/*check if car is busy and then call  abortMotor*/
	
		break;

      default:	/* Unknown directive	*/
		strncpy(MESSAGE, "gratingDatmCad: Unrecognized directive",MAX_STRING_SIZE - 1);
		LOG_MSG(ERROR_MSG,MESSAGE);
		status = CAD_REJECT;
		break;
    } 
   
	LOG_MSG(DEBUG2_MSG,"***********gratingDatmCad\n");
    return status;
}

/*
 *+
 * FUNCTION NAME:
 *	gratingParkCad
 *
 * INVOCATION:
 *	struct cadRecord* pCad;
 *	long status = 0;
 *	status = gratingParkCad( pCad );
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
 *	Supports Gemini "gratingPark" command, via CAD record
 *
 * DESCRIPTION:
 *	Inovked when gratingParkCad record is processed by EPICS.  It 
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
long gratingParkCad( struct cadRecord* pCad)
{
   
    motorMsg msg;
    long status = CAD_ACCEPT;

    LOG_MSG(DEBUG2_MSG,"gratingParkCad:***********");
    /* Take appropriate action, based on directive type*/
    switch(DIRECTIVE )
    {
      case CAD_MARK:   
		LOG_MSG(DEBUG2_MSG,"gratingParkCad - MARK directive");

		break;

      case CAD_PRESET:	
		LOG_MSG(DEBUG2_MSG,"gratingParkCad - PRESET directive");
		/* check datumed before trying to move motors*/
		if(!atoi(pCad->f))
		{
			status = CAD_REJECT;
			sprintf(MESSAGE,"gratingParkCad: grating mechanism not datumed\n");
			LOG_MSG(ERROR_MSG, MESSAGE);
		}
		else
			if(parkMech(GRATING)!= VME_OK)
			{
				sprintf(MESSAGE,"gratingParkCad: Illegal motor\n");
				LOG_MSG(ERROR_MSG, MESSAGE);
				status = CAD_REJECT;
			}
		break;

      case CAD_CLEAR:	
		LOG_MSG(DEBUG2_MSG, "gratingParkCad - CLEAR directive");
		/*clear cad msg field*/
		strcpy(MESSAGE,"");
		break;

      case CAD_START:	
		LOG_MSG(DEBUG2_MSG,"gratingParkCad - START directive");

		if (strcmp(pCad->h,DISABLED) == 0)
		{
			status = CAD_REJECT;
			sprintf(MESSAGE,"gratingPosCad - Grating Observe in progress");
			LOG_MSG(ERROR_MSG, MESSAGE);    
		}
		else
		{
			/*set car to busy (ctrl task will set back to idle)*/
			carVal = CAR_BUSY;
			
			LOG_MSG(DEBUG2_MSG,"Waking park\n");
			msg.op = PARK;
			msg.pCad = pCad;
			if(msgQSend(gratingMotorQ, (char *)&msg, sizeof(motorMsg),NO_WAIT, 
						MSG_PRI_NORMAL) != OK)
			{
				status = CAD_REJECT;
				sprintf(MESSAGE,"msgQSend error in gratingParkCad\n");
				LOG_MSG(ERROR_MSG,MESSAGE);
			}	
		}
	
		break;

      case CAD_STOP:	
		LOG_MSG(DEBUG2_MSG,"gratingParkCad - STOP directive");
		/*call stop command*/
	if(carVal == CAR_BUSY)
		{
			abortMotor(GRATING);
		}
		else
		{
			sprintf(MESSAGE,"gratingParkCad:  Grating is not busy\n");
			LOG_MSG(ERROR_MSG,MESSAGE);
			status = CAD_REJECT;	
		}
		status = CAD_ACCEPT;
		break;

      default:	/* Unknown directive	*/
		strncpy(MESSAGE, "gratingParkCad: Unrecognized directive",MAX_STRING_SIZE - 1);
		LOG_MSG(ERROR_MSG,MESSAGE);
		status = CAD_REJECT;
		break;
    } 
   
	LOG_MSG(DEBUG2_MSG,"***********gratingParkCad\n");
    return status;
}

/*
 *+
 * FUNCTION NAME:
 *	gratingStepsCad
 *
 * INVOCATION:
 *	struct cadRecord* pCad;
 *	long status = 0;
 *	status = gratingStepsCad( pCad );
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
 *	Supports Gemini "gratingSteps" command, via CAD record
 *
 * DESCRIPTION:
 *	Inovked when gratingStepsCad record is processed by EPICS.  It 
 *	implements the "steps" command, which moves the mechanism to the
 *  given absolute position.  
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
long gratingStepsCad( struct cadRecord* pCad )
{
   
    motorMsg msg;
    long status = CAD_ACCEPT;
 
	LOG_MSG(DEBUG2_MSG,"gratingStepsCad:***********");
    /* Take appropriate action, based on directive type*/
    switch(DIRECTIVE )
    {
      case CAD_MARK:
		LOG_MSG(DEBUG2_MSG,"gratingStepsCad - MARK directive");
		break;

      case CAD_PRESET:	
		LOG_MSG(DEBUG2_MSG,"gratingStepsCad - PRESET directive");

		/* check datumed before*/
		if(!atoi(pCad->f))
		{
			status = CAD_REJECT;
      	    strncpy(MESSAGE,"gratingStepsCad - Grating not Datumed",39);
			LOG_MSG(ERROR_MSG, MESSAGE);
		}
		else
		{
			if(setEngPos(GRATING,atoi(cadInput(STEPSCAD)))!= VME_OK)
			{
				strcpy(MESSAGE,gnirsErrorMessage);
				status = CAD_REJECT;
			}
		}
		break;
      case CAD_CLEAR:
		LOG_MSG(DEBUG2_MSG,"gratingStepsCad - CLEAR directive");

		/*clear cad msg field*/
	    strcpy(MESSAGE,"");
		break;

      case CAD_START:	
		LOG_MSG(DEBUG2_MSG,"gratingStepsCad - START directive");
	
		if (strcmp(pCad->h,DISABLED) == 0)
		{
			status = CAD_REJECT;
			sprintf(MESSAGE,"gratingPosCad - Grating Observe in progress");
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
			
			if(msgQSend(gratingMotorQ, (char *)&msg, sizeof(motorMsg),NO_WAIT, 
						MSG_PRI_NORMAL) != OK)
			{
				sprintf(MESSAGE,"msgQSend error in gratingStepsCad\n");
				LOG_MSG(ERROR_MSG,MESSAGE);
				status = CAD_REJECT;
			}
			
		}
		break;

      case CAD_STOP:
		LOG_MSG(DEBUG2_MSG,"gratingStepsCad - STOP directive");
			if(carVal == CAR_BUSY)
		{
			abortMotor(GRATING);
		}
		else
		{
			sprintf(MESSAGE,"gratingStepsCad:  Grating is not busy\n");
			LOG_MSG(ERROR_MSG,MESSAGE);
			status = CAD_REJECT;	
		}
		status = CAD_ACCEPT;
		break;

      default:	/* Unknown directive	*/
		strncpy(MESSAGE, "gratingStepsCad: Unrecognized directive",MAX_STRING_SIZE - 1);
		status = CAD_REJECT;
		break;
    } 
   
	LOG_MSG(DEBUG2_MSG,"***********gratingStepsCad\n");
    return status;
}

/*
 *+
 * FUNCTION NAME:
 *	gratingPosCad
 *
 * INVOCATION:
 *	struct cadRecord* pCad;
 *	long status = 0;
 *	status = gratingPosCad( pCad );
 *
 * PARAMETERS: (">" input, "!" modified, "<" output)
 *	> pCad		(struct cadRecord*)	pointer to CAD record
 *      >  pCad->a  = mode
 *      > pCad->b = grating selection
 *      > pCad->c = desired wavelength
 *      > pCad->d = desired order
 *      > pCad->e = incident angle on grating
 *      > pCad->valf =      datumed sad record
 *      > pCad->valg =      initCad.VALA  (epics sim mode)
 *
 * FUNCTION VALUE:
 *	long		status value written to CAD VAL field
 *
 * PURPOSE:
 *	Supports Gemini "gratingPos" command, via CAD record
 *
 * DESCRIPTION:
 *	Inovked when gratingPosCad record is processed by EPICS.  It 
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
#define ORDER d
#define ANGLE e
#define WAVELENGTH c
#define NAME b
#define MODE a
long gratingPosCad( struct cadRecord* pCad )
{
   
    motorMsg msg;
    long status = CAD_ACCEPT;
   

	LOG_MSG(DEBUG2_MSG,"gratingPosCad:***********");
    /* Take appropriate action, based on directive type*/
    switch(DIRECTIVE )
    {
      case CAD_MARK:
		LOG_MSG(DEBUG2_MSG,"gratingPosCad - MARK directive");
		break;

      case CAD_PRESET:	
		LOG_MSG(DEBUG2_MSG,"gratingPosCad - PRESET directive");
		if(!atoi(pCad->f))
		{
			status = CAD_REJECT;
      	    sprintf(MESSAGE,"gratingPosCad - Grating not Datumed");
			LOG_MSG(ERROR_MSG, MESSAGE);
		}
		else
		{
		
			/*check  grating name*/
			if(valid(GRATING,cadInput(NAME)) != VME_OK)
			{
				printf("grating name = %s\n",cadInput(NAME));
				sprintf(MESSAGE,gnirsErrorMessage);
				status = CAD_REJECT;	
			}
			else
			{
				/*best fit, or user supplied order*/
				printf("mode = %s\n",cadInput(MODE));
				if(strcmp(cadInput(MODE),"WAVELENGTH")==0)
				{
				 
					printf ("set wavelength\n");
					if(validPreferredWavelength(atof(cadInput(WAVELENGTH)))!= VME_OK)
					{
						sprintf(MESSAGE,gnirsErrorMessage);
						status = CAD_REJECT;
					}
				}
				else if(strcmp(cadInput(MODE),"WAVELENGTH/ORDE")==0)
				{
					printf ("set wavelength and order\n");
					if(validGratingWavelength(atof(cadInput(WAVELENGTH)),atoi(cadInput(ORDER)))!= VME_OK)
					{
						sprintf(MESSAGE,gnirsErrorMessage);
						status = CAD_REJECT;
					}
				}
				else if(strcmp(cadInput(MODE),"TILT")==0)
				{
					printf("set tilt\n");
					/*use tilt angle*/
					if(setGratingTilt(atof(cadInput(ANGLE))) != VME_OK)	
					{
						sprintf(MESSAGE,gnirsErrorMessage);
						status = CAD_REJECT;
					}
				}
				  
			}
		
		}
		break;

      case CAD_CLEAR:
		LOG_MSG(DEBUG2_MSG, 
				"gratingPosCad - CLEAR directive");
	    strcpy(MESSAGE,"");
		break;

      case CAD_START:
		LOG_MSG(DEBUG2_MSG,"gratingPosCad - START directive");
		/*copy inputs to outputs*/
		if (strcmp(pCad->h,DISABLED) == 0)
		{
			status = CAD_REJECT;
			sprintf(MESSAGE,"gratingPosCad - Grating Observe in progress");
			LOG_MSG(ERROR_MSG, MESSAGE);    
		}
		else
		{
			status = assignVal(type(MODE), cadInput(MODE), cadOutput(MODE), MESSAGE);
			status = assignVal(type(NAME), cadInput(NAME), cadOutput(NAME), MESSAGE);
			status = assignVal(type(WAVELENGTH), cadInput(WAVELENGTH), cadOutput(WAVELENGTH), MESSAGE);
			status = assignVal(type(ORDER), cadInput(ORDER), cadOutput(ORDER), MESSAGE);
			status = assignVal(type(ANGLE), cadInput(ANGLE), cadOutput(ANGLE), MESSAGE);
			
			/*set car to busy (ctrl task will set back to idle)*/	  
			carVal = CAR_BUSY; 
			/*send message to ctrl task*/
			DPRINT(DPdebug,DEBUG2_MSG,"Waking gratingCtrl\n");
			msg.op = POS;
			msg.pCad = pCad;	
			if(msgQSend(gratingMotorQ, (char *)&msg, sizeof(motorMsg),NO_WAIT, 
						MSG_PRI_NORMAL) != OK)
			{
				sprintf(MESSAGE,"gratingPosCad: Error in msgQSend ");
				status = CAD_REJECT;
			}	
		}
		break;

      case CAD_STOP:	
		LOG_MSG(DEBUG2_MSG,
				"gratingPosCad - STOP directive");
			if(carVal == CAR_BUSY)
		{
			abortMotor(GRATING);
		}
		else
		{
			sprintf(MESSAGE,"gratingPosCad:  Grating is not busy\n");
			LOG_MSG(ERROR_MSG,MESSAGE);
			status = CAD_REJECT;	
		}
		status = CAD_ACCEPT;
		break;

      default:	/* Unknown directive	*/
		strncpy(MESSAGE, "gratingPosCad: Unrecognized directive",
				MAX_STRING_SIZE - 1);
		status = CAD_REJECT;
		break;
    } 
/* 	printf("status = %d\n",status); */
	LOG_MSG(DEBUG2_MSG,"**********gratingPosCad\n");
    return status;
}

long setgratingCarBusy()
{
	carVal = CAR_BUSY;
	if(setCar(GRATING_CAR,carVal,OK,"",dummy) != OK)
	{
		LOG_MSG(ERROR_MSG,dummy);
		return ERROR;
	}
	return OK;
}
long setgratingCar()
{
	carVal = CAR_BUSY;
	return OK;
}
long getgratingCar()
{
	return carVal;
}
