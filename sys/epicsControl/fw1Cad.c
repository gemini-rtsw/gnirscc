static struct {
    void *v;
    char *c;
} rcsid = {
    &rcsid,
    "$Id: fw1Cad.c,v 1.4 2017/09/07 02:33:09 gemvx Exp $"
};
/*
 * Copyright 1997 Association of Universities for Research in 
 *	Astronomy, Inc.
 * See the file COPYRIGHT for more details.
 *
 * FILENAME
 * 	fw1Cad.c
 *
 * DESCRIPTION 
 * 	fw1Cad  support routines
 *
 * 
 * FUNCTION NAME(S)
 *	fw1DatmCad
 *	fw1ParkCad
 *	fw1StepsCad
 *	fw1PosCad
 *	fw1Ctrl
 *   
 * DEPENDENCIES
 * 	EPICS support libraries
 *
 *
 *INDENT-OFF*
 * $Log: fw1Cad.c,v $
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
#include <unistd.h>


/*Global variables*/
MSG_Q_ID fw1MotorQ;
int fw1Id;

/* this value will have the current state of the car*/
static carVal = CAR_IDLE;

/* Forward declarations of the functions in this file */
long fw1DatmCad( struct cadRecord* pCad);

long fw1ParkCad( struct cadRecord* pCad);

long fw1StepsCad( struct cadRecord* pCad);

long fw1PosCad( struct cadRecord* pCad);

int fw1Ctrl( int n1, int n2, int n3, int n4, int n5, int n6, 
			 int n7, int n8, int n9, int n10 );
#ifndef FINDMECH
int findMechName(int mech, char *name, char *pos);
#endif

/*
 *+
 * FUNCTION NAME:    fw1????CadInit
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
 *    create message queue and spawn task to deal with fw1 cad records
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


long fw1DatmCadInit( struct cadRecord* pCad )
{
    long status = CAD_ACCEPT;
    static int done = 0;

    LOG_MSG(DEBUG2_MSG,"fw1DatmCadInit:***********");
    /* park, datum, pos, and steps cads will call this, only do once*/
    if (!done)
    {
		/*create msg q*/
		LOG_MSG(DEBUG2_MSG,"create Msg Queue\n");
		fw1MotorQ = msgQCreate(4,sizeof(motorMsg),MSG_Q_FIFO);
		if(fw1MotorQ == NULL) {
			status = CAD_REJECT;
			strncpy(MESSAGE,"Init: Error creating messageQ",MAX_STRING_SIZE - 1);
		}
		/*spawn task*/
		LOG_MSG(DEBUG2_MSG,"spawn task\n");
		if(status == CAD_ACCEPT)
			fw1Id = taskSpawn("tFw1Ctrl",50,VX_FP_TASK,4000,fw1Ctrl, 0,0,0,0,0,0,0,0,0,0);
		if(fw1Id == ERROR) {
			status = CAD_REJECT;
			strncpy(MESSAGE,"Init: Error Spawning task",MAX_STRING_SIZE - 1);
		}
		if(status = CAD_ACCEPT)
			done = 1;
    }
    
    LOG_MSG(DEBUG2_MSG,"***********fw1DatmCadInit\n");
    return status;
}



long fw1ParkCadInit( struct cadRecord* pCad )
{
    
    long status = CAD_ACCEPT;
    LOG_MSG(DEBUG2_MSG,"fw1ParkCadInit:***********");
    
    LOG_MSG(DEBUG2_MSG,"***********fw1ParkCadInit\n");
    return status;
}



long fw1PosCadInit( struct cadRecord* pCad )
{
    long status = CAD_ACCEPT;

    LOG_MSG(DEBUG2_MSG,"***********fw1PosCadInit\n");
    return status;
}



long fw1StepsCadInit( struct cadRecord* pCad )
{
   
    long status = CAD_ACCEPT;
    LOG_MSG(DEBUG2_MSG,"fw1StepsCadInit:***********");
    LOG_MSG(DEBUG2_MSG,"***********fw1StepsCadInit\n");
    return status;
}



/*
 *+
 * FUNCTION NAME:
 *	fw1Ctrl
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
 *	Cad record releases semaphore which allows fw1Ctrl to run.  
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

int fw1Ctrl( int n1, int n2, int n3, int n4, int n5, int n6, int n7, int n8, int n9, int n10 )
{
    struct cadRecord* pCad;
    long status = VME_OK;
    char rMsg[MAX_STRING_SIZE];
    char dummy[MAX_STRING_SIZE];
    motorMsg msg;
  
   
  
    strncpy (rMsg,"Error in fw1Ctrl\n",MAX_STRING_SIZE -1);
    /* Repeat as inifinite loop				*/
    while(1)
    {
		LOG_MSG(DEBUG2_MSG, "Task tfw1Ctrl sleeping...\n");
	
		if( msgQReceive( fw1MotorQ, (char *)&msg, sizeof(motorMsg ), WAIT_FOREVER ) == ERROR ) {
			sprintf(rMsg,"error in fw1Ctrl msgReceive\n");
			LOG_MSG(ERROR_MSG,rMsg);
			carVal = CAR_ERROR;
			if(setCar(FW1_CAR,carVal,ERROR,rMsg,dummy) != OK)
				LOG_MSG(ERROR_MSG,dummy);
			continue;
		}

		LOG_MSG(DEBUG2_MSG,"fw1Ctrl:***********");
		pCad = msg.pCad;

		carVal = CAR_BUSY;
		if(setCar(FW1_CAR,carVal,OK,"",dummy) != OK)
			LOG_MSG(ERROR_MSG,dummy);
		
		status = moveMech(FW1,rMsg);
		
		if(status == VME_OK) {
			carVal = CAR_IDLE;
			if(setCar(FW1_CAR,carVal,OK,"",dummy) != OK)
				LOG_MSG(ERROR_MSG,dummy);
		}
		else {
			carVal = CAR_ERROR;
			if(setCar(FW1_CAR,carVal,status,rMsg,dummy) != OK)
				LOG_MSG(ERROR_MSG,dummy);
			
		}

		LOG_MSG(DEBUG2_MSG,"***********fw1Ctrl\n");

    }
    
    return status;
}




/*
 *+
 * FUNCTION NAME:
 *	fw1DatmCad
 *
 * INVOCATION:
 *	struct cadRecord* pCad;
 *	long status = 0;
 *	status = fw1DatmCad( pCad );
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
 *	Supports Gemini "fw1Datm" command, via CAD record
 *
 * DESCRIPTION:
 *	Inovked when fw1DatmCad record is processed by EPICS.  It 
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

long fw1DatmCad( struct cadRecord* pCad )
{
    motorMsg msg;
    long status = CAD_ACCEPT;

	LOG_MSG(DEBUG2_MSG,"fw1DatmCad:***********");
    /* Take appropriate action, based on directive type*/
    switch(DIRECTIVE )
    {
      case CAD_MARK:
		LOG_MSG(DEBUG2_MSG, "fw1DatmCad - MARK directive");
	    strcpy(MESSAGE,"");
		break;

      case CAD_PRESET:	
		LOG_MSG(DEBUG2_MSG, "fw1DatmCad - PRESET directive");

		if(datumMech(FW1)!= VME_OK) {
			sprintf(MESSAGE,"fw1DatmCad: ILLEGAL motor\n");
			status = CAD_REJECT;
		}
	
			
		break;

      case CAD_CLEAR:	
		LOG_MSG(DEBUG2_MSG, "fw1DatmCad - CLEAR directive");
		/*clear cad msg field*/
		strcpy(MESSAGE,"");
		break;

      case CAD_START:	
		LOG_MSG(DEBUG2_MSG, "fw1DatmCad - START directive");
	
		if (strcmp(pCad->h,DISABLED) == 0)
		{
			status = CAD_REJECT;
			sprintf(MESSAGE,"fw1DatmCad - Fw1 Observe in progress");
			LOG_MSG(ERROR_MSG, MESSAGE);	
			break;
		}

		if(carVal != CAR_BUSY) {
			/*set car to busy (ctrl task will set back to idle)*/
			carVal = CAR_BUSY;	
				
			/* set structure values for ctrl function*/
			msg.op = DATUM;
			msg.pCad = pCad;
				
			/*send message to ctrl function*/
			if(msgQSend(fw1MotorQ, (char *)&msg, sizeof(motorMsg),NO_WAIT, MSG_PRI_NORMAL) != OK) {
				sprintf(MESSAGE,"fw1DatmCad: msgQSend error in \n");
				LOG_MSG(ERROR_MSG,MESSAGE);
				status = CAD_REJECT;
			}
		}
		else {
			sprintf(MESSAGE,"fw1DatmCad:  Fw1 is busy\n");
			LOG_MSG(ERROR_MSG,MESSAGE);
			status = CAD_REJECT;
		}
		break;

      case CAD_STOP:   
		LOG_MSG(DEBUG2_MSG, "fw1DatmCad - STOP directive");
		if(carVal == CAR_BUSY) {
			abortMotor(FW1);
		}
		else {
			sprintf(MESSAGE,"fw1DatmCad:  Fw1 is not busy\n");
			LOG_MSG(ERROR_MSG,MESSAGE);
			status = CAD_REJECT;	
		}
		/*check if car is busy and then call  abortMotor*/
	
		break;

      default:	/* Unknown directive	*/
		strncpy(MESSAGE, "fw1DatmCad: Unrecognized directive",MAX_STRING_SIZE - 1);
		LOG_MSG(ERROR_MSG,MESSAGE);
		status = CAD_REJECT;
		break;
    } 
   
    LOG_MSG(DEBUG2_MSG,"***********fw1DatmCad\n");
    return status;
}





/*
 *+
 * FUNCTION NAME:
 *	fw1ParkCad
 *
 * INVOCATION:
 *	struct cadRecord* pCad;
 *	long status = 0;
 *	status = fw1ParkCad( pCad );
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
 *	Supports Gemini "fw1Park" command, via CAD record
 *
 * DESCRIPTION:
 *	Inovked when fw1ParkCad record is processed by EPICS.  It 
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





long fw1ParkCad( struct cadRecord* pCad)
{
   
    motorMsg msg;
    long status = CAD_ACCEPT;

    LOG_MSG(DEBUG2_MSG,"fw1ParkCad:***********");
    /* Take appropriate action, based on directive type*/
    switch(DIRECTIVE )
    {
      case CAD_MARK:   
		LOG_MSG(DEBUG2_MSG,"fw1ParkCad - MARK directive");

		break;

      case CAD_PRESET:	
		LOG_MSG(DEBUG2_MSG,"fw1ParkCad - PRESET directive");
		/* check datumed before trying to move motors*/
		if(!atoi(pCad->f)) {
			status = CAD_REJECT;
			sprintf(MESSAGE,"fw1 mechanism not datumed\n");
			LOG_MSG(ERROR_MSG, MESSAGE);
 			break;
		}
			
		if(parkMech(FW1)!= VME_OK) {
			sprintf(MESSAGE,"fw1ParkCad: Illegal motor\n");
			LOG_MSG(ERROR_MSG, MESSAGE);
			status = CAD_REJECT;
		}
			
		break;

      case CAD_CLEAR:	
		LOG_MSG(DEBUG2_MSG, "fw1ParkCad - CLEAR directive");
		/*clear cad msg field*/
		strcpy(MESSAGE,"");
		break;

      case CAD_START:	
		LOG_MSG(DEBUG2_MSG,"fw1ParkCad - START directive");
	
		if (strcmp(pCad->h,DISABLED) == 0)
		{
			status = CAD_REJECT;
			sprintf(MESSAGE,"fw1ParkCad - Fw1 Observe in progress");
			LOG_MSG(ERROR_MSG, MESSAGE);	
			break;
		}
			/*set car to busy (ctrl task will set back to idle)*/
		carVal = CAR_BUSY;
			
		LOG_MSG(DEBUG2_MSG,"Waking park\n");
		msg.op = PARK;
		msg.pCad = pCad;
		if(msgQSend(fw1MotorQ, (char *)&msg, sizeof(motorMsg),NO_WAIT, MSG_PRI_NORMAL) != OK) {
			status = CAD_REJECT;
			sprintf(MESSAGE,"msgQSend error in fw1DatmCad\n");
			LOG_MSG(ERROR_MSG,MESSAGE);
		}
		break;

      case CAD_STOP:	
		LOG_MSG(DEBUG2_MSG,"fw1ParkCad - STOP directive");
		/*call stop command*/
		if(carVal == CAR_BUSY) {
			abortMotor(FW1);
		}
		else {
			sprintf(MESSAGE,"fw1ParkCad:  Fw1 is not busy\n");
			LOG_MSG(ERROR_MSG,MESSAGE);
			status = CAD_REJECT;	
		}
		status = CAD_ACCEPT;
		break;
		
      default:	/* Unknown directive	*/
		strncpy(MESSAGE, "fw1ParkCad: Unrecognized directive",MAX_STRING_SIZE - 1);
		LOG_MSG(ERROR_MSG,MESSAGE);
		status = CAD_REJECT;
		break;
    } 
	
    LOG_MSG(DEBUG2_MSG,"***********fw1ParkCad\n");
    return status;
}





/*
 *+
 * FUNCTION NAME:
 *	fw1StepsCad
 *
 * INVOCATION:
 *	struct cadRecord* pCad;
 *	long status = 0;
 *	status = fw1StepsCad( pCad );
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
 *	Supports Gemini "fw1Steps" command, via CAD record
 *
 * DESCRIPTION:
 *	Inovked when fw1StepsCad record is processed by EPICS.  It 
 *	implements the "steps" command, which moves the mechanism to the
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
long fw1StepsCad( struct cadRecord* pCad )
{
   
    motorMsg msg;
    long status = CAD_ACCEPT;
   

    LOG_MSG(DEBUG2_MSG,"fw1StepsCad:***********");

    /* Take appropriate action, based on directive type*/
    switch(DIRECTIVE )
    {
      case CAD_MARK:
		LOG_MSG(DEBUG2_MSG,"fw1StepsCad - MARK directive");
		break;

      case CAD_PRESET:	
		LOG_MSG(DEBUG2_MSG,"fw1StepsCad - PRESET directive");

		/* check datumed before*/
		if(!atoi(pCad->f)) {
			status = CAD_REJECT;
      	    		strncpy(MESSAGE,"fw1StepsCad - Fw1 not Datumed",39);
			LOG_MSG(ERROR_MSG, MESSAGE);
			break;
		}
		if(setEngPos(FW1,atoi(cadInput(STEPSCAD))) != VME_OK) {
			strcpy(MESSAGE,gnirsErrorMessage);
			status = CAD_REJECT;
		}
		break;
      case CAD_CLEAR:
		LOG_MSG(DEBUG2_MSG,"fw1StepsCad - CLEAR directive");
		/*clear cad msg field*/
	    	strcpy(MESSAGE,"");
		break;

      case CAD_START:	
		LOG_MSG(DEBUG2_MSG,"fw1StepsCad - START directive");
	
		/*copy inputs to outputs*/
	
		if (strcmp(pCad->h,DISABLED) == 0) {
			status = CAD_REJECT;
			sprintf(MESSAGE,"fw1StepsCad - Fw1 Observe in progress");
			LOG_MSG(ERROR_MSG, MESSAGE);	
			break;
		}

		*(long *)cadOutput(STEPSCAD) = atoi(cadInput(STEPSCAD));
			
		/*set car to busy (ctrl task will set back to idle)*/
		carVal = CAR_BUSY;
			
		DPRINT(DPdebug,DEBUG2_MSG,"Waking steps\n");
		msg.op = STEPS;
		msg.pCad = pCad;
			
		if(msgQSend(fw1MotorQ, (char *)&msg, sizeof(motorMsg),NO_WAIT, MSG_PRI_NORMAL) != OK) {
			sprintf(MESSAGE,"msgQSend error in fw1StepsCad\n");
			LOG_MSG(ERROR_MSG,MESSAGE);
			status = CAD_REJECT;
		}
	
		break;

      case CAD_STOP:
		LOG_MSG(DEBUG2_MSG,"fw1StepsCad - STOP directive");
		if(carVal == CAR_BUSY) {
			abortMotor(FW1);
		}
		else {
			sprintf(MESSAGE,"fw1StepsCad:  Fw1 is not busy\n");
			LOG_MSG(ERROR_MSG,MESSAGE);
			status = CAD_REJECT;	
		}
		status = CAD_ACCEPT;
		break;

      default:	/* Unknown directive	*/
		strncpy(MESSAGE, "fw1StepsCad: Unrecognized directive",MAX_STRING_SIZE - 1);
		status = CAD_REJECT;
		break;
    } 
   
    LOG_MSG(DEBUG2_MSG,"***********fw1StepsCad\n");
    return status;
}

/*
 *+
 * FUNCTION NAME:
 *	fw1PosCad
 *
 * INVOCATION:
 *	struct cadRecord* pCad;
 *	long status = 0;
 *	status = fw1PosCad( pCad );
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
 *	Supports Gemini "fw1Pos" command, via CAD record
 *
 * DESCRIPTION:
 *	Inovked when fw1PosCad record is processed by EPICS.  It 
 *	implements the "pos" command, which moves the mechanism to the named
 *  position.  
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
long fw1PosCad( struct cadRecord* pCad )
{

    char pos[POS_LEN];
    motorMsg msg;
    long status = CAD_ACCEPT;
   

	LOG_MSG(DEBUG2_MSG,"fw1PosCad:***********");
    /* Take appropriate action, based on directive type*/
    switch(DIRECTIVE )
    {
      case CAD_MARK:
		LOG_MSG(DEBUG2_MSG,"fw1PosCad - MARK directive");
		break;

      case CAD_PRESET:	
		LOG_MSG(DEBUG2_MSG,"fw1PosCad - PRESET directive");

		if(!atoi(pCad->f)) {
			status = CAD_REJECT;
			sprintf(MESSAGE,"fw1PossCad - Fw1 Observe in progress");
			LOG_MSG(ERROR_MSG, MESSAGE);
			break;
		}
		
		printf("name = %s\n",pCad->b);
		/* check pCad->b against values from structure and send pos from structure */
#ifndef FINDMECH
		if(! findMechName(FW1,pCad->b,pos)) {
			sprintf(MESSAGE,"%s not found in database for fw1\n",pCad->b);
			status = CAD_REJECT;
			break;
		}
#endif
		if(valid(FW1,pos) != VME_OK) {
			sprintf(MESSAGE,gnirsErrorMessage);
			status = CAD_REJECT;
		}

                printf("fw1PosCad PRESET OK name = %s %d, %d\n",pCad->b, status, carVal);
		
		break;

      case CAD_CLEAR:
		LOG_MSG(DEBUG2_MSG, "fw1PosCad - CLEAR directive");
	        strcpy(MESSAGE,"");
		break;

      case CAD_START:
		LOG_MSG(DEBUG2_MSG,"fw1PosCad - START directive");
	
		if (strcmp(pCad->h,DISABLED) == 0) {
			status = CAD_REJECT;
			sprintf(MESSAGE,"fw1PosCad - Fw1 not Datumed");
			LOG_MSG(ERROR_MSG, MESSAGE);	
			break;
		}

		/*copy inputs to outputs*/	
		status = assignVal(type(POSCAD), cadInput(POSCAD), cadOutput(POSCAD), MESSAGE);
			
		/*set car to busy (ctrl task will set back to idle)*/	  
		carVal = CAR_BUSY; 
			
		/*send message to ctrl task*/
		DPRINT(DPdebug,DEBUG2_MSG,"Waking fw1Ctrl\n");
		msg.op = POS;
		msg.pCad = pCad;	
		if(msgQSend(fw1MotorQ, (char *)&msg, sizeof(motorMsg),NO_WAIT, MSG_PRI_NORMAL) != OK) {
			sprintf(MESSAGE,"fw1PosCad: Error in msgQSend ");
			status = CAD_REJECT;
		}
                printf("fw1PosCad - START directive done. %d %d", status, carVal);
		break;

      case CAD_STOP:	
		LOG_MSG(DEBUG2_MSG, "fw1PosCad - STOP directive");
		if(carVal == CAR_BUSY) {
			abortMotor(FW1);
		}
		else
		{
			sprintf(MESSAGE,"fw1PosCad:  Fw1 is not busy\n");
			LOG_MSG(ERROR_MSG,MESSAGE);
			status = CAD_REJECT;	
		}
		status = CAD_ACCEPT;
		break;

      default:	/* Unknown directive	*/
		strncpy(MESSAGE, "fw1PosCad: Unrecognized directive", MAX_STRING_SIZE - 1);
		status = CAD_REJECT;
		break;
    } 
   
    LOG_MSG(DEBUG2_MSG,"***********fw1PosCad\n");
    return status;
}

long setfw1CarBusy()
{
	carVal = CAR_BUSY;
	if(setCar(FW1_CAR,carVal,OK,"",dummy) != OK) {
		LOG_MSG(ERROR_MSG,dummy);
		return ERROR;
	}
	return OK;
}


long setfw1Car()
{
	carVal = CAR_BUSY;
	return OK;
}


long getfw1Car()
{
	return carVal;
}
